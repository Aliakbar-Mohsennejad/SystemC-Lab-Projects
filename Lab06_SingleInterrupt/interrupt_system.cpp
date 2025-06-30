#include <systemc.h>
#include <iostream>

SC_MODULE(Peripheral) {
    sc_out<bool> interrupt_out;
    sc_in<bool> clk;
    sc_event interrupt_event;

    void generate_interrupt() {
        while (true) {
            wait(50, SC_NS);
            interrupt_event.notify();
            interrupt_out.write(true);
            wait(10, SC_NS);
            interrupt_out.write(false);
        }
    }

    SC_CTOR(Peripheral) {
        SC_THREAD(generate_interrupt);
        sensitive << clk.pos();
    }
};

SC_MODULE(InterruptController) {
    sc_in<bool> peripheral_interrupt;
    sc_out<bool> cpu_interrupt;
    sc_in<bool> cpu_busy;
    sc_event interrupt_handled;

    void handle_interrupt() {
        while (true) {
            wait(peripheral_interrupt.posedge_event());

            if (!cpu_busy.read()) {
                cpu_interrupt.write(true);
                interrupt_handled.notify();
                wait(peripheral_interrupt.negedge_event());
                cpu_interrupt.write(false);
            }
        }
    }

    SC_CTOR(InterruptController) {
        SC_THREAD(handle_interrupt);
    }
};

SC_MODULE(CPU) {
    sc_in<bool> clk;
    sc_in<bool> interrupt;
    sc_out<bool> busy;
    int time_counter;
    sc_event isr_start, isr_end;

    void cpu_process() {
        while (true) {
            wait(clk.posedge_event());
            time_counter = sc_time_stamp().to_double() * 1000;

            if (interrupt.read() && !busy.read()) {
                busy.write(true);
                isr_start.notify();
                cout << "At " << sc_time_stamp() << ": Entering ISR" << endl;

                wait(30, SC_NS);
                isr_end.notify();
                cout << "At " << sc_time_stamp() << ": Exiting ISR" << endl;
                busy.write(false);
            }
            else if (!busy.read()) {
                cout << "At " << sc_time_stamp() << ": Normal operation" << endl;
            }
        }
    }

    SC_CTOR(CPU) : time_counter(0) {
        SC_THREAD(cpu_process);
        sensitive << clk.pos();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_trace_file* tf = sc_create_vcd_trace_file("interrupt_trace");

    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> peripheral_to_controller;
    sc_signal<bool> controller_to_cpu;
    sc_signal<bool> cpu_busy;

    Peripheral peripheral("peripheral");
    peripheral.clk(clk);
    peripheral.interrupt_out(peripheral_to_controller);

    InterruptController int_controller("int_controller");
    int_controller.peripheral_interrupt(peripheral_to_controller);
    int_controller.cpu_interrupt(controller_to_cpu);
    int_controller.cpu_busy(cpu_busy);

    CPU cpu("cpu");
    cpu.clk(clk);
    cpu.interrupt(controller_to_cpu);
    cpu.busy(cpu_busy);

    sc_trace(tf, clk, "clk");
    sc_trace(tf, peripheral_to_controller, "peripheral_int");
    sc_trace(tf, controller_to_cpu, "cpu_int");
    sc_trace(tf, cpu_busy, "cpu_busy");

    cout << "Starting simulation..." << endl;
    sc_start(200, SC_NS);
    sc_close_vcd_trace_file(tf);

    cout << "Simulation finished." << endl;
    return 0;
}