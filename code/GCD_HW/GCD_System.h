#ifndef GCD_SYSTEM_H
#define GCD_SYSTEM_H

#include <systemc.h>
#include "ALU.h"
#include "ControlUnit.h"

template <unsigned int DATA_WIDTH>
class GCD_System : public sc_module {
public:
    // Puertos de interfaz
    sc_in<sc_uint<DATA_WIDTH>> num1, num2;
    sc_in<bool> start, clk, reset;
    sc_out<sc_uint<DATA_WIDTH>> result;
    sc_out<bool> done;

    // Componentes
    ALU<DATA_WIDTH> alu;
    ControlUnit<DATA_WIDTH> control;

    // Señales de interconexión
    sc_signal<sc_uint<DATA_WIDTH>> alu_a, alu_b, alu_result;
    sc_signal<sc_uint<3>> alu_selector;

    SC_CTOR(GCD_System) : 
        alu("alu"),
        control("control") 
    {
        // Conexiones de la Unidad de Control
        control.Ain(num1);
        control.Bin(num2);
        control.start(start);
        control.clk(clk);
        control.reset(reset);
        control.Aout(alu_a);
        control.Bout(alu_b);
        control.F(alu_selector);
        control.Y(alu_result);
        control.done(done);
        control.result(result);

        // Conexiones de la ALU
        alu.AIn(alu_a);
        alu.BIn(alu_b);
        alu.FIn(alu_selector);
        alu.YOut(alu_result);
    }

    void trace(sc_trace_file* tf) {
        // Señales principales
        sc_trace(tf, num1, "num1");
        sc_trace(tf, num2, "num2");
        sc_trace(tf, start, "start");
        sc_trace(tf, clk, "clk");
        sc_trace(tf, reset, "reset");
        sc_trace(tf, result, "result");
        sc_trace(tf, done, "done");

        // Señales de interconexión
        sc_trace(tf, alu_a, "alu_a");
        sc_trace(tf, alu_b, "alu_b");
        sc_trace(tf, alu_selector, "alu_selector");
        sc_trace(tf, alu_result, "alu_result");

        // Trazado de componentes
        alu.trace(tf);
        control.trace(tf);
    }
};
#endif