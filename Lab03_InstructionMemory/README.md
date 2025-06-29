# 💾 Lab 03 – Instruction Memory in SystemC

This project implements a 32-bit **Instruction Memory** in SystemC, capable of loading instructions from a `.hex` file and returning them based on an 8-bit address input.

---

## 🎯 Objective

To simulate a ROM-based memory module that:
- Loads a list of 32-bit MIPS-like instructions from a hex file
- Reads a given address and outputs the corresponding instruction
- Handles invalid address access with proper warnings

---

## 📥 File Loader Logic

- Accepts a `.hex` file containing 32-bit instructions in hexadecimal format
- Supports comment removal (`#`) and ignores whitespace
- Only the first 256 instructions are loaded (memory limit)
- Displays confirmation message upon successful loading


---

## 🔧 Code Highlight

```cpp
void load_program(const char* filename) {
    ...
    std::stringstream converter;
    converter << std::hex << clean_line;
    converter >> temp;
    mem[index++] = temp;
}

```

## ▶️ Sample I/O
```cpp
Loaded 5 instructions successfully.

Starting instruction memory test...

Address: 0 Instruction: 20100001
Address: 1 Instruction: 20110002
Address: 2 Instruction: 02304020
Address: 3 Instruction: 8e120000
Address: 4 Instruction: ae130000
Address: 5 Instruction: 0
...
Invalid address test: 0
```

## 🧪 program.hex Sample
```cpp
# Sample instruction memory
20100001 # addi $s0, $zero, 1

20110002 # addi $s1, $zero, 2
02304020 # add $t0, $s1, $s0
8E120000 # lw $s2, 0($s0)
AE130000 # sw $s3, 0($s0)
```

## 🏷️ Tags
systemc instruction-memory rom hex-loader hardware-simulation digital-systems kntu
