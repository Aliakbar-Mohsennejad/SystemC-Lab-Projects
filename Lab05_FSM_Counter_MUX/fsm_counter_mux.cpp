// fsm_counter_mux.cpp
#include <systemc.h>
#include <iostream>
using namespace std;

// ==== ENUM  ====
enum fsm_state { IDLE, LOAD, COUNT_UP, COUNT_DOWN };

// ====  FSM ====
SC_MODULE(FSM) {
    sc_in<bool> clk, reset, start;
    sc_in<int> mode; // number between 0 and 3
    sc_out<fsm_state> state;

    void process() {
        if (reset.read()) {
            state.write(IDLE);
        }
        else if (clk.posedge()) {
            if (start.read()) {
                switch (mode.read()) {
                case 0: state.write(IDLE); break;
                case 1: state.write(LOAD); break;
                case 2: state.write(COUNT_UP); break;
                case 3: state.write(COUNT_DOWN); break;
                default: state.write(IDLE); break;
                }
                cout << "[FSM] Mode: " << mode.read() << ", State: " << state.read() << endl;
            }
        }
    }

    SC_CTOR(FSM) {
        SC_METHOD(process);
        sensitive << clk.pos();
        sensitive << reset;
    }
};

// ==== Counter ====
SC_MODULE(Counter) {
    sc_in<bool> clk;
    sc_in<fsm_state> state;
    sc_in<sc_uint<32>> data_in;
    sc_out<sc_uint<32>> count_out;

    sc_uint<32> count;

    void process() {
        if (clk.posedge()) {
            switch (state.read()) {
            case LOAD:
                count = data_in.read();
                break;
            case COUNT_UP:
                count++;
                break;
            case COUNT_DOWN:
                count--;
                break;
            default:
                break;
            }
            count_out.write(count);
            cout << "[Counter] Output: " << count << endl;
        }
    }

    SC_CTOR(Counter) : count(0) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

// ==== (MUX) ====
SC_MODULE(MUX) {
    sc_in<bool> sel; // 0: خروجی counter، 1: مقدار ثابت
    sc_in<sc_uint<32>> in1, in2;
    sc_out<sc_uint<32>> out;

    void process() {
        out.write(sel.read() ? in2.read() : in1.read());
        cout << "[MUX] Selected Output: " << out.read() << endl;
    }

    SC_CTOR(MUX) {
        SC_METHOD(process);
        sensitive << sel << in1 << in2;
    }
};

// ==== Test Bech ====
SC_MODULE(Testbench) {
    sc_signal<bool> clk, reset, start, mux_sel;
    sc_signal<int> mode;
    sc_signal<fsm_state> state;
    sc_signal<sc_uint<32>> data_in, counter_out, const_val, mux_out;

    FSM fsm_inst;
    Counter counter_inst;
    MUX mux_inst;

    void clk_gen() {
        while (true) {
            clk = false;
            wait(5, SC_NS);
            clk = true;
            wait(5, SC_NS);
        }
    }

    void test() {
        reset = true;
        start = false;
        mode = 0;
        data_in = 0;
        const_val = 1234;
        mux_sel = false;
        wait(10, SC_NS);

        reset = false;
        start = true;

        // Mode 1: LOAD
        mode = 1;
        data_in = 42;
        wait(10, SC_NS);
        cout << "[Testbench] Mode: LOAD, Data In: 42\n";

        // Mode 2: COUNT_UP
        mode = 2;
        wait(10, SC_NS);
        cout << "[Testbench] Mode: COUNT_UP\n";

        // Mode 3: COUNT_DOWN
        mode = 3;
        wait(10, SC_NS);
        cout << "[Testbench] Mode: COUNT_DOWN\n";

        // Mode 0: IDLE
        mode = 0;
        wait(10, SC_NS);
        cout << "[Testbench] Mode: IDLE\n";

        sc_stop();
    }

    SC_CTOR(Testbench)
        : fsm_inst("FSM"), counter_inst("Counter"), mux_inst("MUX") {

        fsm_inst.clk(clk);
        fsm_inst.reset(reset);
        fsm_inst.start(start);
        fsm_inst.mode(mode);
        fsm_inst.state(state);

        counter_inst.clk(clk);
        counter_inst.state(state);
        counter_inst.data_in(data_in);
        counter_inst.count_out(counter_out);

        mux_inst.sel(mux_sel);
        mux_inst.in1(counter_out);
        mux_inst.in2(const_val);
        mux_inst.out(mux_out);

        SC_THREAD(clk_gen);
        SC_THREAD(test);
    }
};

// ==== sc_main ====
int sc_main(int argc, char* argv[]) {
    Testbench tb("Testbench");

    sc_trace_file* wf = sc_create_vcd_trace_file("fsm_counter_waveform");
    sc_trace(wf, tb.clk, "clk");
    sc_trace(wf, tb.reset, "reset");
    sc_trace(wf, tb.start, "start");
    sc_trace(wf, tb.mode, "mode");
    sc_trace(wf, tb.state, "state");
    sc_trace(wf, tb.data_in, "data_in");
    sc_trace(wf, tb.counter_out, "counter_out");
    sc_trace(wf, tb.const_val, "const_val");
    sc_trace(wf, tb.mux_sel, "mux_sel");
    sc_trace(wf, tb.mux_out, "mux_out");

    sc_start();
    sc_close_vcd_trace_file(wf);
    return 0;
}
