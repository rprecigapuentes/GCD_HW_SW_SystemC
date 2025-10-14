# GCD_HW_SW_SystemC

Hardware/Software co-design of the **Greatest Common Divisor (GCD)** algorithm using **SystemC**.  
This project was developed as part of the *Digital Systems Verification* course (Universidad Nacional de Colombia, 2025-I).

---

## 🧩 Overview

The objective of this project was to compare the performance and design process between a **software implementation** and a **hardware implementation** of Euclid’s GCD algorithm.

- **Software version:** Executed on a simplified RISC processor modeled in SystemC.  
- **Hardware version:** Implemented using a dedicated Arithmetic Logic Unit (ALU) and a Finite State Machine (FSM)–based control unit.  
- **Tools used:**  
  - SystemC (IEEE 1666 standard)  
  - GTKWave for waveform analysis  
  - Python (for automated test generation and log parsing)

---

## ⚙️ Software Implementation

The software version was executed on a custom **RISC processor** built in SystemC, including modules for:
- **Instruction cache** (`icache`)
- **Decoder**
- **Execution unit**

The program implementing the GCD algorithm was written in **assembly** and executed through the SystemC simulator.

### 🔍 Debugging and Findings

During implementation, we found a **critical bug** in the processor’s decoder (`decode.cpp`):  
the **program counter (PC)** was being **overwritten** after branch instructions, which caused jumps (`beq`, `blt`) to be ignored.

```cpp
if (srcC_tmp == srcA_tmp) {
    branch_target_address.write(pc_reg + label_tmp);
    ...
}
//branch_target_address.write(pc_reg + 1);
```

Commenting out that last line fixed the issue, allowing conditional jumps to execute correctly.

We also discovered that the assembler (`assembler.pl`) inserted **empty memory lines** before and after each instruction (`0x00000000`), which affected the effective jump addresses.  
As a result, branch destinations had to point to the line **before** the intended instruction in memory.

---

## 🧠 Automation with Python

To validate the implementation, two Python scripts were created:

- **`gcd_auto.py`** – Generates 30 random test cases and runs them automatically.  
- **`gcd_prueba.py`** – Runs a fixed set of 30 input pairs for comparison between hardware and software.  

Each run:
1. Reassembles the code (`perl assembler.pl gcd.asm -code > icache.img`)  
2. Cleans and rebuilds the simulator (`make clean && make`)  
3. Executes the test and logs results to `resultados.csv`

The script parses simulation output to extract:
- Number of iterations  
- Total simulation cycles  
- Final register values  

---

## 🔧 Hardware Implementation

The hardware implementation includes:
- **ALU** – Performs AND, ADD, SUB, and MSB extraction operations.  
- **Control Unit** – FSM-based design controlling the algorithm flow (states: IDLE, COMPARISON, MSBAnalysis, SWAP, SUBTRACT, FINISH).  
- **Testbench** – Runs 30 predefined input cases, compares expected vs obtained results, and measures clock cycles.

### Example of console output:
```
Test #1: A = 18, B = 30 | Result: 6 (Expected: 6) | Cycles: 17
Test #2: A = 20, B = 5  | Result: 5 (Expected: 5) | Cycles: 11
Test #3: A = 9, B = 16  | Result: 1 (Expected: 1) | Cycles: 28
```

The simulation results are also available in `gcd_wave.vcd` for waveform visualization in GTKWave.

---

## 📊 Results Summary

| Implementation | Avg. Cycles | Notes |
|----------------|-------------|-------|
| **Software (SystemC RISC)** | 1× baseline | Flexible but slower |
| **Hardware (Custom FSM)**   | ~25–60× faster | Specialized and efficient |

In specific cases (e.g., equal inputs or zero values), the hardware design achieved **up to 67× better performance** than the software version.

---

## 🧾 Repository Structure

```
├─ GCD_HW/
│  ├─ ALU/ ControlUnit/ GCD_System/ MUX_1_2/ MUX_1_4/
│  ├─ GCD_tb.cpp / GCD_test.cpp / Makefile / gcd_wave.vcd
│  └─ README.md
│
├─ GCD_SW/
│  ├─ assembler/ decode/ fetch/ icache/ register/ ...
│  ├─ gcd.asm / gcd_auto.py / gcd_prueba.py / Makefile
│  └─ README.md
│
└─ README.md (this file)
```

---

## 📚 References

- David Harris, Sarah Harris. *Digital Design and Computer Architecture.*  
- IEEE Std 1666-2011 — *SystemC Language Reference Manual.*  
- [SystemC Official Site](https://systemc.org/overview/systemc/)  
- [GCD_HW_SW_SystemC GitHub Repository](https://github.com/rprecigapuentes/GCD_HW_SW_SystemC)

---

### ✉️ Authors
- **Rosemberth Steeven Preciga Puentes**  
- **Luis Guillermo Vaca Rincón**

National University of Colombia – Faculty of Engineering  
Course: Digital Systems Verification – 2025-I
