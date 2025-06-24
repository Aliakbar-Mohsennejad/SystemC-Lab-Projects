# 🧮 Lab 02 – 8-bit ALU Simulation in SystemC

This project implements a simple **8-bit Arithmetic Logic Unit (ALU)** using SystemC. It demonstrates how basic arithmetic and logic operations are performed using digital circuit modeling.

---

## 🎯 Objective

To design an ALU capable of performing common operations like ADD, SUB, AND, OR, XOR, NOT, and INC using 8-bit unsigned integers, and simulate its behavior through console input.

---

## 🧠 Supported Operations

| Code | Operation | Description         |
|------|-----------|---------------------|
| 0    | ADD       | A + B               |
| 1    | SUB       | A - B               |
| 2    | AND       | Bitwise AND         |
| 3    | OR        | Bitwise OR          |
| 4    | XOR       | Bitwise XOR         |
| 5    | NOT A     | Bitwise NOT of A    |
| 6    | INC A     | A + 1               |

---


## 🔧 Code Highlight

```cpp
case 0: // ADD
    temp_result = A.read() + B.read();
    result.write(temp_result.range(7, 0));
    carry.write(temp_result[8]);
    break;
```
---

## ▶️ Sample I/O

```cpp
Enter A (8-bit binary): 00011001
Enter B (8-bit binary): 00001011
Choose operation: 0
===== RESULT =====
A      : 00011001
B      : 00001011
Result : 00100000
Carry  : 0

```
---

## 🏷️ Tags
systemc alu digital-logic arithmetic-unit hardware-simulation kntu sc_uint verilog-style
