#include <systemc.h>
#include "GCD_System.h"

int sc_main(int argc, char* argv[]) {
    const unsigned int DATA_WIDTH = 32;
    sc_clock sys_clock("sys_clock", 10, SC_NS);
    sc_signal<bool> reset, start, done;
    sc_signal<sc_uint<DATA_WIDTH>> num1, num2, result;

    GCD_System<DATA_WIDTH> gcd("gcd");
    gcd.clk(sys_clock);
    gcd.reset(reset);
    gcd.start(start);
    gcd.num1(num1);
    gcd.num2(num2);
    gcd.result(result);
    gcd.done(done);

    sc_trace_file* tf = sc_create_vcd_trace_file("gcd_wave");
    gcd.trace(tf);
    sc_trace(tf, sys_clock, "sys_clock");

    cout << "Inicio de simulacion GCD" << endl;

    // Función auxiliar para ejecutar un caso de prueba
    auto run_test = [&](int test_num, sc_uint<DATA_WIDTH> a, sc_uint<DATA_WIDTH> b, sc_uint<DATA_WIDTH> expected) {
        unsigned int cycle_count = 0;  // Contador de ciclos de reloj
        
        reset.write(true);
        sc_start(1, SC_NS);
        reset.write(false);
        num1.write(a);
        num2.write(b);
        start.write(true);
        sc_start(10, SC_NS);
        cycle_count++;  // Contamos el ciclo de inicio
        cout << "Prueba #" << test_num << ": A = " << a << ", B = " << b;
        start.write(false);
        
        while(!done.read()) {
            sc_start(10, SC_NS);
            cycle_count++;  // Incrementamos el contador por cada ciclo de espera
        }
        
        cout << " | Resultado: " << result.read() 
             << " (Esperado: " << expected << ")"
             << " | Ciclos: " << cycle_count;
        
        if(result.read() == expected) {
            cout << " \033[1;32m✓\033[0m" << endl; // Verde
        } else {
            cout << " \033[1;31m✗\033[0m" << endl; // Rojo
        }
    };

    // Caso 1: Caso básico
    run_test(1, 18, 30, 6);
    
    // Caso 2: Un número es divisor del otro
    run_test(2, 20, 5, 5);
    
    // Caso 3: Números coprimos
    run_test(3, 9, 16, 1);
    
    // Caso 4: Múltiplos
    run_test(4, 48, 16, 16);
    
    // Caso 5: Números consecutivos
    run_test(5, 100, 101, 1);
    
    // Caso 6: Números iguales
    run_test(6, 256, 256, 256);
    
    // Caso 7: Múltiples factores comunes
    run_test(7, 840, 1260, 420);
    
    // Caso 8: Un número es primo
    run_test(8, 17, 34, 17);
    
    // Caso 9: Ambos números primos
    run_test(9, 13, 19, 1);
    
    // Caso 10: Potencias de 2
    run_test(10, 1024, 768, 256);
    
    // Caso 11: Uno es cero
    run_test(11, 0, 27, 0);
    
    // Caso 12: Ambos son cero
    run_test(12, 0, 0, 0);
    
    // Caso 13: Números pequeños
    run_test(13, 4, 6, 2);
    
    // Caso 14: Un número es 1
    run_test(14, 1, 100, 1);
    
    // Caso 15: Números con diferencia grande
    run_test(15, 9999, 3, 3);
    
    // Caso 16: Números con factores primos diferentes
    run_test(16, 25, 49, 1);
    
    // Caso 17: Múltiplo no trivial
    run_test(17, 81, 27, 27);
    
    // Caso 18: Números con muchos divisores
    run_test(18, 360, 840, 120);
    
    // Caso 19: Números con diferencia de potencia de 2
    run_test(19, 128, 32, 32);
    
    // Caso 20: Números con GCD pequeño pero valores grandes
    run_test(20, 9999, 9996, 3);
    
    // Caso 21: Números con GCD igual al número más pequeño
    run_test(21, 123, 41, 41);
    
    // Caso 22: Números con factores cuadrados
    run_test(22, 36, 81, 9);
    
    // Caso 23: Números con factores cúbicos
    run_test(23, 8, 27, 1);
    
    // Caso 24: Números con GCD igual a la diferencia
    run_test(24, 100, 75, 25);
    
    // Caso 25: Números con GCD 1 pero no consecutivos
    run_test(25, 14, 25, 1);
    
    // Caso 26: Números con factores primos alternados
    run_test(26, 12, 35, 1);
    
    // Caso 27: Números con GCD igual a la suma de dígitos
    run_test(27, 54, 72, 18);
    
    // Caso 28: Números con GCD igual a producto de dígitos
    run_test(28, 24, 36, 12);
    
    // Caso 29: Números con GCD igual a la mitad del mayor
    run_test(29, 50, 100, 50);
    
    // Caso 30: Caso aleatorio bien conocido
    run_test(30, 546, 2310, 42);

    sc_close_vcd_trace_file(tf);
    cout << "Simulacion completada. Archivo: gcd_wave.vcd" << endl;

    return 0;
}