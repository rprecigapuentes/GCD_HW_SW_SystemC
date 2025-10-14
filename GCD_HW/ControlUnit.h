#ifndef ControlUnit_H
#define ControlUnit_H

#include <systemc.h>

template <unsigned int N>
class ControlUnit : public sc_module {
public:
    // Entradas
    sc_in<sc_uint<N>> Ain, Bin, Y;
    sc_in<bool> start, clk, reset;

    // Salidas
    sc_out<sc_uint<N>> Aout, Bout, result;
    sc_out<sc_uint<3>> F;
    sc_out<bool> done;

    // Señales internas
    sc_signal<sc_uint<N>> A_reg, B_reg, reg_temp;
    sc_signal<bool> is_swap_done;

    enum StateValues {
        IDLE = 0,
        COMPARISON = 1,
        MSBAnalysis = 2,
        SUBSTRACT = 3,
        SWAP = 4,
        UPDATE_SWAP = 5,
        FINISH = 6
    };

    sc_signal<sc_uint<3>> current_state, next_state;

    SC_CTOR(ControlUnit) {
        // FSM
        SC_METHOD(state_register);
        sensitive << clk.pos() << reset.neg();

        SC_METHOD(next_state_logic);
        sensitive << current_state << start << A_reg << B_reg << Y;

        // Lógica combinacional de salidas
        SC_METHOD(output_logic);
        sensitive << current_state << A_reg << B_reg << is_swap_done;

        // Proceso que registra los valores en flanco de reloj
        SC_METHOD(register_logic);
        sensitive << clk.pos() << reset.neg();
    }

    void trace(sc_trace_file* tf) const {
        sc_trace(tf, Ain, "ControlUnit.Ain");
        sc_trace(tf, Bin, "ControlUnit.Bin");
        sc_trace(tf, Y, "ControlUnit.Y");
        sc_trace(tf, start, "ControlUnit.start");
        sc_trace(tf, clk, "ControlUnit.clk");
        sc_trace(tf, reset, "ControlUnit.reset");

        sc_trace(tf, Aout, "ControlUnit.Aout");
        sc_trace(tf, Bout, "ControlUnit.Bout");
        sc_trace(tf, result, "ControlUnit.result");
        sc_trace(tf, F, "ControlUnit.F");
        sc_trace(tf, done, "ControlUnit.done");

        sc_trace(tf, A_reg, "ControlUnit.A_reg");
        sc_trace(tf, B_reg, "ControlUnit.B_reg");
        sc_trace(tf,reg_temp,"ControlUnit.temp_reg");
        sc_trace(tf, is_swap_done, "ControlUnit.is_swap_done");
        sc_trace(tf, current_state, "ControlUnit.current_state");
        sc_trace(tf, next_state, "ControlUnit.next_state");
    }

private:
    // Registro de estado
    void state_register() {
        if (reset.read()) {
            current_state.write(IDLE);
        } else {
            current_state.write(next_state.read());
        }
    }

    // Transición de estados
    void next_state_logic() {
        auto state = current_state.read();
        sc_uint<3> next = state;

        switch (state) {
            case IDLE:
                if (start.read()) next = COMPARISON;
                break;
            case COMPARISON:
            next = (A_reg.read() == 0 || B_reg.read() == 0 || A_reg.read() == B_reg.read()) ? FINISH : MSBAnalysis;
            break;
            case MSBAnalysis:
                next = (Y.read() == 0) ? SUBSTRACT : SWAP;
                break;
            case SWAP:
                next = UPDATE_SWAP;
                break;
            case UPDATE_SWAP:
                next = SUBSTRACT;
                break;
            case SUBSTRACT:
                next = COMPARISON;
                break;
            case FINISH:
                next = IDLE;
                break;
        }

        next_state.write(next);
    }

    // Salidas combinacionales
    void output_logic() {
        Aout.write(0);
        Bout.write(0);
        F.write(0);
        done.write(false);

        switch (current_state.read()) {
            case COMPARISON:
                Aout.write(A_reg.read());
                Bout.write(B_reg.read());
                break;

            case MSBAnalysis:
                Aout.write(A_reg.read());
                Bout.write(B_reg.read());
                F.write(111); // MSB
                break;

            case SWAP:
                break;
        
            case SUBSTRACT:
                Aout.write(A_reg.read());
                Bout.write(B_reg.read());
                F.write(110); // resta
                break;

            case FINISH:
                result.write(A_reg.read());
                done.write(true);
                break;

            default:
                break;
        }
    }

    // Registro de datos internos sincronizado con reloj
    void register_logic() {
        if (reset.read()) {
            A_reg.write(0);
            B_reg.write(0);
            reg_temp.write(0);
        } else {
            switch (current_state.read()) {
                case IDLE:
                    A_reg.write(Ain.read());
                    B_reg.write(Bin.read());
                    break;
                case COMPARISON:
                    break;
                case SWAP:
                    reg_temp.write(A_reg.read());
                    break;
                case UPDATE_SWAP: //Permite un ciclo de reloj adicional para que se refleje el cambio
                    A_reg.write(B_reg.read());
                    B_reg.write(reg_temp.read());
                    break;
                case SUBSTRACT:
                    A_reg.write(Y.read());
                    break;

                default:
                    // No se modifican registros en los demás estados
                    break;
            }
        }
    }
};

#endif
