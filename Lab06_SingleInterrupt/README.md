# 🔔 Lab 06 – Single Interrupt System Simulation in SystemC

This project simulates a system with a single interrupt source and a basic interrupt controller that decides when to signal the CPU based on peripheral events. The design highlights real-time interrupt handling and CPU ISR processing.

---

## 🎯 Objective

To design and simulate:
- A `Peripheral` module generating interrupt pulses at fixed intervals
- An `InterruptController` that signals the CPU only if it is idle
- A `CPU` that handles the interrupt by running a 30ns Interrupt Service Routine (ISR)
- Signal tracing to VCD for waveform analysis

---

## 🧩 Component Overview

### ✅ Peripheral
- Generates an interrupt every **50 ns**
- Keeps the signal active for **10 ns**

### ✅ Interrupt Controller
- Monitors peripheral interrupt line
- Signals CPU only when CPU is not busy

### ✅ CPU
- Monitors the `interrupt` signal
- If idle, enters ISR for 30 ns
- Toggles `busy` status accordingly

---

## 🖼️ System Architecture


+-------------+ +---------------------+ +--------+
| Peripheral | ---> | Interrupt Controller | ---> | CPU |
+-------------+ +---------------------+ +--------+
| ↑
CPU busy ----------------------+


---

## 🧪 Sample Simulation Output

Starting simulation...
At 0 s: Normal operation
At 10 ns: Normal operation
At 20 ns: Normal operation
...
At 60 ns: Entering ISR
At 90 ns: Exiting ISR
At 120 ns: Entering ISR
...
Simulation finished.

---

## 👨‍💻 Author

AliAkbar Mohsennejad  
SystemC Lab – K. N. Toosi University of Technology

---

## 🏷️ Tags

`systemc` `interrupt` `cpu` `controller` `peripheral` `hardware-simulation` `digital-design` `vcd-tracing` `kntu`


