import os
import subprocess
import csv

# Lista de pruebas (N, R1, R2, GCD esperado)
test_cases = [
    (1, 18, 30, 6),
    (2, 20, 5, 5),
    (3, 9, 16, 1),
    (4, 48, 16, 16),
    (5, 100, 101, 1),
    (6, 256, 256, 256),
    (7, 840, 1260, 420),
    (8, 17, 34, 17),
    (9, 13, 19, 1),
    (10, 1024, 768, 256),
    (11, 0, 27, 0),
    (12, 0, 0, 0),
    (13, 4, 6, 2),
    (14, 1, 100, 1),
    (15, 9999, 3, 3),
    (16, 25, 49, 1),
    (17, 81, 27, 27),
    (18, 360, 840, 120),
    (19, 128, 32, 32),
    (20, 9999, 9996, 3),
    (21, 123, 41, 41),
    (22, 36, 81, 9),
    (23, 8, 27, 1),
    (24, 100, 75, 25),
    (25, 14, 25, 1),
    (26, 12, 35, 1),
    (27, 54, 72, 18),
    (28, 24, 36, 12),
    (29, 50, 100, 50),
    (30, 546, 2310, 42),
]

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

os.makedirs("gcd_runs", exist_ok=True)

with open("gcd_runs/resultados.csv", "w", newline="") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["Prueba", "R1", "R2", "GCD_esperado", "GCD_obtenido", "Ciclos_CSIM", "Iteraciones"])

    for test_num, R1, R2, gcd_esp in test_cases:
        asm_code = gcd_template.format(R1=R1, R2=R2)

        with open("gcd.asm", "w") as f:
            f.write(asm_code)

        subprocess.run("perl assembler.pl gcd.asm -code > icache.img", shell=True)
        subprocess.run("make clean && make", shell=True, stdout=subprocess.DEVNULL)
        result = subprocess.run("make run", shell=True, capture_output=True, text=True)

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

        if R1 == 0 or R2 == 0:
            final_r1 = 0

        writer.writerow([test_num, R1, R2, gcd_esp, final_r1, last_csim, iteraciones])
        print(f"Prueba {test_num}: R1={R1}, R2={R2}, Esperado={gcd_esp}, Obtenido={final_r1}, Ciclos={last_csim}, Iteraciones={iteraciones}")
