#include <systemc.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ======================== Interrupt Generators ========================
SC_MODULE(InterruptGenerator) {
    sc_out<bool> interrupt;
    sc_time on_time, off_time;
    std::string name;

    void generate() {
        while (true) {
            // Random decision: whether to activate the interrupt or skip this cycle
            bool active = rand() % 2; // 50% chance

            if (active) {
                interrupt.write(true);
                cout << "[" << name << "] Interrupt ON at " << sc_time_stamp()
                    << " (Duration: " << on_time << ")" << endl;
                wait(on_time);

                interrupt.write(false);
                cout << "[" << name << "] Interrupt OFF at " << sc_time_stamp()
                    << " (Duration: " << off_time << ")" << endl;
                wait(off_time);
            }
            else {
                // Skip this interrupt cycle
                interrupt.write(false);
                cout << "[" << name << "] Skipped at " << sc_time_stamp() << "\n";
                wait(on_time + off_time);
            }
        }
    }

    SC_HAS_PROCESS(InterruptGenerator);
    InterruptGenerator(sc_module_name n,
        sc_time on, sc_time off,
        std::string nm)
        : sc_module(n),
        on_time(on), off_time(off),
        name(nm) {
        SC_THREAD(generate);
    }
};

// ======================== Interrupt Controller ========================
SC_MODULE(InterruptController) {
    sc_in<bool> ig1, ig2, ig3;
    sc_out<int> irq; // 0 = none, 1 = IG1, 2 = IG2, 3 = IG3

    void update_irq() {
        while (true) {
            wait(10, SC_NS);
            if (ig1.read())
                irq.write(1);
            else if (ig2.read())
                irq.write(2);
            else if (ig3.read())
                irq.write(3);
            else
                irq.write(0);
        }
    }

    SC_CTOR(InterruptController) {
        SC_THREAD(update_irq);
    }
};

// ======================== CPU ========================
SC_MODULE(CPU) {
    sc_in<bool> clk;
    sc_in<int> irq;

    void run() {
        while (true) {
            wait(clk.posedge_event());
            switch (irq.read()) {
            case 1:
                cout << "[CPU] ISR IG1 at " << sc_time_stamp() << endl;
                wait(30, SC_NS);
                break;
            case 2:
                cout << "[CPU] ISR IG2 at " << sc_time_stamp() << endl;
                wait(30, SC_NS);
                break;
            case 3:
                cout << "[CPU] ISR IG3 at " << sc_time_stamp() << endl;
                wait(30, SC_NS);
                break;
            default:
                cout << "[CPU] Normal Operation at " << sc_time_stamp() << endl;
                break;
            }
        }
    }

    SC_CTOR(CPU) {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

// ======================== sc_main ========================
int sc_main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    sc_signal<bool> ig1_sig, ig2_sig, ig3_sig;
    sc_signal<int> irq_sig;
    sc_clock clk("clk", 10, SC_NS);

    InterruptGenerator ig1("IG1",
        sc_time(20, SC_NS), sc_time(30, SC_NS),
        "IG1");

    InterruptGenerator ig2("IG2",
        sc_time(20, SC_NS), sc_time(30, SC_NS),
        "IG2");

    InterruptGenerator ig3("IG3",
        sc_time(20, SC_NS), sc_time(30, SC_NS),
        "IG3");

    InterruptController ic("IC");
    CPU cpu("CPU");

    ig1.interrupt(ig1_sig);
    ig2.interrupt(ig2_sig);
    ig3.interrupt(ig3_sig);

    ic.ig1(ig1_sig);
    ic.ig2(ig2_sig);
    ic.ig3(ig3_sig);
    ic.irq(irq_sig);

    cpu.clk(clk);
    cpu.irq(irq_sig);

    sc_trace_file* tf = sc_create_vcd_trace_file("multi_interrupt_waveform");
    sc_trace(tf, clk, "clk");
    sc_trace(tf, ig1_sig, "IG1");
    sc_trace(tf, ig2_sig, "IG2");
    sc_trace(tf, ig3_sig, "IG3");
    sc_trace(tf, irq_sig, "IRQ");

    sc_start(500, SC_NS);
    sc_close_vcd_trace_file(tf);
    return 0;
}