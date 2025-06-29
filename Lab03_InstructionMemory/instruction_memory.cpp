#include <systemc.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

#define MEM_SIZE 256  // تعداد سلول‌های حافظه

SC_MODULE(InstructionMemory) {
    sc_in<sc_uint<8>> addr;             // آدرس ورودی (0 تا 255)
    sc_out<sc_uint<32>> instruction;    // خروجی دستورالعمل 32 بیتی

    sc_uint<32> mem[MEM_SIZE];          // آرایه حافظه

    // تابع خواندن دستورالعمل بر اساس آدرس
    void read_instruction() {
        if (addr.read() < MEM_SIZE) {
            instruction.write(mem[addr.read()]);
        }
        else {
            instruction.write(0);
            std::cout << "[ERROR] Invalid address access: " << addr.read() << std::endl;
        }
    }

    // تابع بارگذاری فایل .hex
    void load_program(const char* filename) {
        std::ifstream file(filename);
        std::string line;
        int index = 0;

        if (!file) {
            std::cerr << "[ERROR] Could not open hex program file." << std::endl;
            return;
        }

        while (std::getline(file, line)) {
            // حذف کامنت
            size_t comment_pos = line.find('#');
            if (comment_pos != std::string::npos) {
                line = line.substr(0, comment_pos);
            }

            // حذف فاصله‌ها
            std::stringstream ss;
            for (char ch : line)
                if (!isspace(ch)) ss << ch;

            std::string clean_line = ss.str();
            if (clean_line.empty())
                continue;

            // تبدیل به عدد
            uint32_t temp;
            std::stringstream converter;
            converter << std::hex << clean_line;
            converter >> temp;
            sc_uint<32> instr = temp;

            if (index < MEM_SIZE) {
                mem[index++] = instr;
            }
            else {
                std::cerr << "[WARNING] Program too big. Only first 256 instructions loaded." << std::endl;
                break;
            }
        }

        std::cout << "Loaded " << index << " instructions successfully." << std::endl;
    }


    SC_CTOR(InstructionMemory) {
        SC_METHOD(read_instruction);
        sensitive << addr;
    }
};



int sc_main(int argc, char* argv[]) {
    sc_signal<sc_uint<8>> addr;
    sc_signal<sc_uint<32>> instr;

    // نمونه‌سازی ماژول
    InstructionMemory imem("IMEM");
    imem.addr(addr);
    imem.instruction(instr);

    // بارگذاری برنامه از فایل
    imem.load_program("program.hex");

    // اجرای تست
    std::cout << "\nStarting instruction memory test...\n" << std::endl;
    for (int i = 0; i < 10; i++) {
        addr.write(i);
        sc_start(1, SC_NS);
        std::cout << "Address: " << std::dec << i
            << " Instruction: " << std::hex << std::setw(8)
            << std::setfill('0') << instr.read().to_uint() << std::endl;
    }

    // تست آدرس نامعتبر
    addr.write(300);
    sc_start(1, SC_NS);
    std::cout << "Invalid address test: " << instr.read().to_uint() << std::endl;

    return 0;
}