// AliAkbar Mohsennejad 40008873 - ALU
#include <systemc.h>
#include <iostream>
#include <string>
#include <bitset>

SC_MODULE(ALU) {
    sc_in<sc_uint<8>> A, B;
    sc_in<sc_uint<3>> op;
    sc_out<sc_uint<8>> result;
    sc_out<bool> carry;

    void process() {
        sc_uint<9> temp_result = 0;

        switch (op.read()) {
        case 0: // ADD
            temp_result = A.read() + B.read();
            result.write(temp_result.range(7, 0));
            carry.write(temp_result[8]);
            break;

        case 1: // SUB
            temp_result = A.read() - B.read();
            result.write(temp_result.range(7, 0));
            carry.write(temp_result[8]);
            break;

        case 2: // AND
            result.write(A.read() & B.read());
            carry.write(0);
            break;

        case 3: // OR
            result.write(A.read() | B.read());
            carry.write(0);
            break;

        case 4: // XOR
            result.write(A.read() ^ B.read());
            carry.write(0);
            break;

        case 5: // NOT A
            result.write(~A.read());
            carry.write(0);
            break;

        case 6: // INC A
            temp_result = A.read() + 1;
            result.write(temp_result.range(7, 0));
            carry.write(temp_result[8]);
            break;

        default:
            result.write(0);
            carry.write(0);
            break;
        }
    }

    SC_CTOR(ALU) {
        SC_METHOD(process);
        sensitive << A << B << op;
    }
};


int sc_main(int argc, char* argv[]) {
    sc_signal<sc_uint<8>> A_sig, B_sig, R_sig;
    sc_signal<sc_uint<3>> op_sig;
    sc_signal<bool> carry_sig;

    ALU alu("BinaryALU");
    alu.A(A_sig);
    alu.B(B_sig);
    alu.op(op_sig);
    alu.result(R_sig);
    alu.carry(carry_sig);

    std::string a_str, b_str;
    int op_input;

    std::cout << "Enter A (8-bit binary): ";
    std::cin >> a_str;

    std::cout << "Enter B (8-bit binary): ";
    std::cin >> b_str;

    std::cout << "Choose operation code:\n";
    std::cout << " 0 - ADD\n 1 - SUB\n 2 - AND\n 3 - OR\n 4 - XOR\n 5 - NOT A\n 6 - INC A\n";
    std::cout << "Enter op (0-6): ";
    std::cin >> op_input;


    sc_uint<8> a_val = static_cast<sc_uint<8>>(std::bitset<8>(a_str).to_ulong());
    sc_uint<8> b_val = static_cast<sc_uint<8>>(std::bitset<8>(b_str).to_ulong());

    A_sig.write(a_val);
    B_sig.write(b_val);
    op_sig.write(op_input);

    sc_start(1, SC_NS);

    std::cout << "\n===== RESULT =====" << std::endl;
    std::cout << "A      : " << std::bitset<8>(A_sig.read().to_uint()) << std::endl;
    std::cout << "B      : " << std::bitset<8>(B_sig.read().to_uint()) << std::endl;
    std::cout << "Result : " << std::bitset<8>(R_sig.read().to_uint()) << std::endl;
    std::cout << "Carry  : " << carry_sig.read() << std::endl;

    return 0;
}
