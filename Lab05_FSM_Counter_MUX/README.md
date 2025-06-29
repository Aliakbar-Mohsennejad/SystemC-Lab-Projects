# 🔁 Lab 05 – FSM-Controlled Counter with MUX in SystemC

This lab demonstrates the integration of three key digital components — **FSM (Finite State Machine)**, **Counter**, and **2-to-1 MUX** — all simulated and connected in SystemC. The design also generates a waveform output (`.vcd`) for visual debugging.

---

## 🎯 Objective

To design and simulate a system-level model consisting of:
- A control FSM driven by `mode` input
- A 32-bit synchronous counter that responds to FSM states
- A 2-to-1 MUX switching between counter output and a constant value
- Signal tracing for simulation visualization using `sc_trace`

---

## 🧩 FSM Description

| Mode | State      | Behavior                  |
|------|------------|---------------------------|
| 0    | IDLE       | Holds counter             |
| 1    | LOAD       | Loads data_in into counter|
| 2    | COUNT_UP   | Increments counter        |
| 3    | COUNT_DOWN | Decrements counter        |

> FSM changes state only when `start` is high.  
> Reset forces the system to IDLE and counter to 0.

---

## ⚙️ Module Interaction

```text
          +---------+     +---------+     +-------+
 Mode --> |  FSM    | --> | Counter | --> | MUX   | --> Output
 Start    +---------+     +---------+     +-------+
 Reset         ↑               ↑
 CLK ----------+---------------+
```
