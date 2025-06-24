# 🔀 Lab 01 – XOR Gate Simulation with SystemC

This project simulates a basic **XOR gate** using SystemC. It is the first lab in a series of projects focusing on hardware design at the system level.

---

## 🎯 Objective

To implement a simple 1-bit XOR gate using `SC_MODULE` in SystemC and verify its output through terminal-based user input.

---

## 💡 Functionality

- The module takes two boolean inputs: `A` and `B`
- Computes `Y = A XOR B`
- Accepts values from the user at runtime (0 or 1)
- Outputs the result to console


---

## ⚙️ Code Overview

```cpp
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
```
---

## ▶️ Sample I/O

```cpp
Enter input A (0 or 1): 1
Enter input B (0 or 1): 0
Result (A XOR B): 1

```
## 🏷️ Tags

systemc xor-gate hardware-simulation digital-logic systemc-lab kntu

