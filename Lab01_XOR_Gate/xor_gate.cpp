// AliAkbar Mohsennejad 40008873
#include <systemc.h>

SC_MODULE(XORGate) {
    sc_in<bool> A, B;
    sc_out<bool> Y;

    void do_xor() {
        Y.write(A.read() ^ B.read());
    }
    SC_CTOR(XORGate) {
        SC_METHOD(do_xor);
        sensitive << A << B;
    }
};

int sc_main(int argc, char* argv[]) {
 
    sc_signal<bool> A_sig, B_sig, Y_sig;

    XORGate xor_gate("XOR_Module");
  
    xor_gate.A(A_sig);
    xor_gate.B(B_sig);
    xor_gate.Y(Y_sig);

    int a_input, b_input;
    std::cout << "Enter input A (0 or 1): ";
    std::cin >> a_input;
    std::cout << "Enter input B (0 or 1): ";
    std::cin >> b_input;

    A_sig.write(a_input);
    B_sig.write(b_input);

    sc_start(1, SC_NS);  
 
    std::cout << "Result (A XOR B): " << Y_sig.read() << std::endl;

    return 0;
}
