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
## 🖼️ System Overview

<img src="images/module_interaction.jpg" alt="Module Interaction Diagram" width="700"/>

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
---
## ▶️ Sample Output

```code
[MUX] Selected Output: 0
[MUX] Selected Output: 0

Info: (I702) default timescale unit used for tracing: 1 ps (fsm_counter_waveform.vcd)
[Counter] Output: 0
[FSM] Mode: 1, State: 0
[Counter] Output: 0
[Testbench] Mode: LOAD, Data In: 42
[FSM] Mode: 2, State: 1
[Counter] Output: 42
[MUX] Selected Output: 0
[Testbench] Mode: COUNT_UP
[FSM] Mode: 3, State: 2
[Counter] Output: 43
[MUX] Selected Output: 42
[Testbench] Mode: COUNT_DOWN
[FSM] Mode: 0, State: 3
[Counter] Output: 42
[MUX] Selected Output: 43
[Testbench] Mode: IDLE
```
---

## 📊 Waveform Visualization
<img src="images/fsm_counter_waveform.jpg" alt="Waveform Output" width="800"/>
Use GTKWave or any compatible viewer to load the .vcd file and inspect signal transitions and timing.

