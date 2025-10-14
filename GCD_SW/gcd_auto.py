import os
import subprocess
import random
import csv

# Plantilla del archivo gcd.asm
gcd_template = """movi R0, 0
movi R1, {R1}
movi R2, {R2}
beq R1, R0, 16    
beq R2, R0, 14 
beq R1, R2, 12
blt R1, R2, 5
sub R1, R1, R2
j 14
sub R2, R2, R1
j 14
halt

"""

# Crear carpeta de resultados si no existe
os.makedirs("gcd_runs", exist_ok=True)

# Abrir CSV para escritura
with open("gcd_runs/resultados.csv", "w", newline="") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["Prueba", "R1_inicial", "R2_inicial", "GCD_final", "Ciclos_CSIM", "Iteraciones_Bucle"])

    for i in range(1, 31): 
        R1 = random.randint(0, 255) 
        R2 = random.randint(0, 255)
        asm_code = gcd_template.format(R1=R1, R2=R2)

        # Guardar el archivo asm
        with open("gcd.asm", "w") as f:
            f.write(asm_code)

        # Ejecutar pipeline de ensamblado y simulación
        subprocess.run("perl assembler.pl gcd.asm -code > icache.img", shell=True)
        subprocess.run("make clean && make", shell=True, stdout=subprocess.DEVNULL)
        result = subprocess.run("make run", shell=True, capture_output=True, text=True)

        # Procesar salida
        output = result.stdout
        last_csim = "N/A"
        final_r1 = "N/A"
        iteraciones = 0

        for line in reversed(output.splitlines()):
            if "at CSIM" in line and last_csim == "N/A":
                last_csim = line.split("at CSIM")[1].split("ns")[0].strip()
            if "R 1(" in line and final_r1 == "N/A":
                try:
                    final_r1 = int(line.split("R 1(")[1].split(")")[0], 16)
                except:
                    pass

        for line in output.splitlines():
            if "blt R1" in line:
                iteraciones += 1

        # Si uno de los valores era cero, forzar GCD = 0
        if R1 == 0 or R2 == 0:
            final_r1 = 0

        # Guardar resultados
        writer.writerow([i, R1, R2, final_r1, last_csim, iteraciones])
        print(f"Prueba {i}: R1={R1}, R2={R2}, GCD={final_r1}, Ciclos={last_csim}, Iteraciones={iteraciones}")
