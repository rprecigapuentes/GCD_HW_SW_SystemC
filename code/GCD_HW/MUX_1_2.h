#ifndef MUX_2
#define MUX_2

#include <systemc.h>

template <unsigned int N>
class MUX2: public sc_module {

    public: 
        // Entradas del MUX
        sc_in<sc_uint<N>> in0mux2,in1mux2;
        sc_in<bool> selector;
        sc_out<sc_uint<N>> outmux2;
    
    SC_CTOR(MUX2) {
        SC_METHOD(sensible_a_int);
        sensitive << in0mux2 << in1mux2 << selector;
    }
    void trace(sc_trace_file* tf) {
        sc_trace(tf, in0mux2, ".in0");
        sc_trace(tf, in1mux2, ".in1");
        sc_trace(tf, selector, ".selector");
        sc_trace(tf, outmux2, ".out");
    }

    private: 
    void sensible_a_int(){
        switch (selector.read()) {
            case 0: outmux2.write(in0mux2.read()); break;
            case 1: outmux2.write(in1mux2.read()); break;
            default: outmux2.write(in0mux2.read()); break; // Por seguridad
        } 
    }

};
#endif