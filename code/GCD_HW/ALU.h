#ifndef ALU_H
#define ALU_H

#include <systemc.h>
#include "MUX_1_2.h"
#include "MUX_1_4.h"

template <unsigned int N>

class ALU: public sc_module{
    
    public: 
        //Entradas de la ALU
        sc_in<sc_uint<N>> AIn,BIn;
        sc_in<sc_uint<3>> FIn;
        
        //Salidas
        sc_out<sc_uint<N>> YOut;

        //Cables entre modulos
        sc_signal<sc_uint<N>> BInNeg, BB, OR, AND, Sum, msb_extended;
        sc_signal<bool> MSBSum;
        //Selector Multiplexores
        sc_signal<sc_uint<2>> F01;
        sc_signal<bool> F2; 
    
        //Constructor
        SC_CTOR(ALU):
            mux2("MUX2"),
            mux4("MUX4")
        {
        SC_METHOD(sensible_a);
        sensitive << AIn << BIn << FIn << AND << OR << Sum << MSBSum << BB << F2 << F01 << BInNeg << msb_extended;
    
        //Conexiones entre modulos
        //Mux para B o !B
        mux2.in0mux2(BIn);
        mux2.in1mux2(BInNeg);
        mux2.selector(F2);
        mux2.outmux2(BB);
    
        mux4.in0(AND);
        mux4.in1(OR);
        mux4.in2(Sum); 
        //Para realizar el zero extended
        mux4.in3(msb_extended); 

        mux4.selector(F01);
        mux4.outmux4(YOut);
        }
        void trace(sc_trace_file* tf) {
            sc_trace(tf, AIn, "AIn");
            sc_trace(tf, BIn, "BIn");
            sc_trace(tf, FIn, "FIn");
            sc_trace(tf, YOut, "YOut");
            sc_trace(tf, BInNeg, "BInNeg");
            sc_trace(tf, BB, "BB");
            sc_trace(tf, OR, "OR");
            sc_trace(tf, AND, "AND");
            sc_trace(tf, Sum, "Sum");
            sc_trace(tf, MSBSum, "MSBSum");
            sc_trace(tf, F01, "F01");
            sc_trace(tf, F2, "F2");
            sc_trace(tf, mux2.in0mux2, "mux2.in0");
            sc_trace(tf, mux2.in1mux2, "mux2.in1");
            sc_trace(tf, mux2.selector, "mux2.selector");
            sc_trace(tf, mux2.outmux2, "mux2.out");
        
        // Trazado de señales internas de los multiplexores
            mux2.trace(tf);
            mux4.trace(tf);
        }

    private:
        //instancias del MUX

        MUX2<N> mux2;
        MUX4<N> mux4;

        void sensible_a() {
        // Separar selector

        F2.write(FIn.read()[2]);
        F01.write(FIn.read().range(1,0));
        

        // Operaciones básicas
        BInNeg.write(~BIn.read());
        OR.write(AIn.read() | BB.read());
        AND.write(AIn.read() & BB.read());
        Sum.write(AIn.read() + (BB.read() +  F2.read()));

        // Realizar Zero extended
        MSBSum.write(Sum.read()[N-1]);
        sc_uint<N> temp_extended = 0;
        temp_extended[0] = MSBSum.read();
        msb_extended.write(temp_extended);
    }
    
};
#endif