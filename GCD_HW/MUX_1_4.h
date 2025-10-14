#ifndef MUX_4
#define MUX_4

#include <systemc.h>

template <unsigned int N>
class MUX4: public sc_module {    

    public:
    // Entradas del mux
    sc_in<sc_uint<N>> in0, in1, in2, in3;
    sc_in<sc_uint<2>> selector;  // 2 bits para 4 opciones
    sc_out<sc_uint<N>> outmux4;

    SC_CTOR(MUX4) {
        SC_METHOD(sensible_a_int);
        sensitive << in0 << in1 << in2 << in3 << selector;
    }
    void trace(sc_trace_file* tf) {
        sc_trace(tf, in0, ".in0");
        sc_trace(tf, in1, ".in1");
        sc_trace(tf, in2, ".in2");
        sc_trace(tf, in3, ".in3");
        sc_trace(tf, selector,".selector");
        sc_trace(tf, outmux4,".out");
    }

    private:
        void sensible_a_int() {
        switch (selector.read()) {
            case 0: outmux4.write(in0.read()); break;
            case 1: outmux4.write(in1.read()); break;
            case 2: outmux4.write(in2.read()); break;
            case 3: outmux4.write(in3.read()); break;
            default: outmux4.write(in0.read()); break; // Por seguridad
        }
    }   
};
    
#endif
