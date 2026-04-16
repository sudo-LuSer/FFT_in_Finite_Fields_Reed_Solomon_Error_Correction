# Reed–Solomon Error Correction on Finite Fields  
### FFT in Finite Fields – Application to Digital Architecture

This project focuses on the **design and implementation of a Reed–Solomon error-correcting code over finite fields**.  
The project combines **software simulation in C++**, **validation using MATLAB**, and prepares the ground for a **future hardware implementation on FPGA within a RISC-V architecture**.

Reed–Solomon codes are widely used in **digital communication and storage systems**, including satellite communications, QR codes, CDs/DVDs, and deep-space communication.

---

# Project Structure

```
projet_ECC/
│
├── doc/                # Documentation (subject,proposition , references)
├── include/            # C++ header files (.hpp)
├── src/                # C++ source files (.cpp)
├── rs/                 # Updated files (use them on piriority but before you need to install & copy them on your hulotte file and change the CMake then Run) <a href = 'https://github.com/sudo-LuSer/hulotte'>Here myFork of hulotte</a>
├── matlab/             # MATLAB validation scripts
├── VHDL/               # VHDL implementation for FPGA for simple polynomial
├── WinNTL              # Number Theory Library for running the NTL version of the encoder
├── ntl                 # Source of Number theory Lib <a href = 'https://github.com/sudo-LuSer/ntl'>Here myFork of ntl</a>
└── README.md           # Project documentation
```

---

# Project Objectives

The main objectives of this project are:

- Implement arithmetic over **finite fields GF(2^m)**
- Implement a **Reed–Solomon encoder**
- Implement a **Reed–Solomon decoder capable of correcting errors**
- Validate the implementation using **MATLAB**
- Prepare the architecture for **FPGA implementation using VHDL**
- Evaluate **algorithm performance and execution time**

---

# Requirements

To compile and run the project, you need:

### Required

- **C++ compiler supporting C++17**
  - GCC ≥ 7
  - Clang ≥ 5
  - MSVC ≥ 2017

- **Program supporting VHDL simulations**
  - Vivado

- **Make**

### Optional

- **MATLAB** with *Communications Toolbox* for validation

---

# Compilation

A **Makefile** is provided at the root of the project.

### Linux / macOS

```bash
mkdir build && cd build 
cmake .. 
make 
```

### Windows (MinGW)

```bash
mingw32-cmake (I guess else use wsl)
```

This generates:

- object files in `build/`
- executable `RS_RUN` (or `RS_RUN.exe`)

---

# Cleaning Generated Files

```bash
make clean
```

---

# Running the Program

After compilation:

### Linux / macOS

```bash
./RS_RUN
```

### Windows

```bash
RS_RUN.exe
```

---

# Demonstration Workflow

The program performs the following steps:

1. Construction of the **Galois Field GF(2³)**
2. Encoding of a **message of length k = 3**
3. Generation of a **Reed–Solomon codeword (7,3)**
4. Injection of an artificial error
5. Computation of **syndromes**
6. Error location using **Berlekamp–Massey algorithm**
7. Error search using **Chien search**
8. Error magnitude computation using **Forney algorithm**
9. Correction of the corrupted symbol

Execution times for major operations are displayed in the console.

---

# Reed–Solomon Parameters

Example parameters used in the demonstration:

| Parameter | Value |
|----------|------|
| Field | GF(2³) |
| Code | RS(7,3) |
| Message length | k = 3 |
| Codeword length | n = 7 |
| Error correction capability | t = 2 |

Where:

```
t = (n - k) / 2
```

---

# Implemented Algorithms

## Finite Field Arithmetic

- Galois Field generation **GF(2^m)**
- Primitive polynomial tables
- Log / Anti-log tables

Supported range:

```
2 ≤ m ≤ 20
```

---

## Reed–Solomon Encoder

- Systematic encoding
- Generator polynomial construction
- Polynomial division over GF(2^m)

---

## Reed–Solomon Decoder

The decoder includes:

1. **Syndrome computation**
2. **Berlekamp–Massey algorithm** (error locator polynomial)
3. **Chien search** (error position detection)
4. **Forney algorithm** (error magnitude evaluation)
5. **Error correction**

---

# MATLAB Validation

The script:

```
matlab/rs_test.m
```

reproduces the same Reed–Solomon encoding and decoding process using MATLAB’s **Communications Toolbox**.

Run in MATLAB:

```matlab
rs_test
```

This allows verification of:

- encoder correctness
- decoder correctness
- finite field arithmetic

---

# VHDL implementation

Testbench :
VHDL/EncoderDecoderRS/EncoderDecoderRS.srcs/sim_1/new/tb_rs_encoder.vhd

This will test the encoding process for a simple polynomial :
RS(7, 3)
G(x) = x^4 + 3x^3 + x^2 + 2x + 3
Primitif polynomial : x^3 + x + 1

We send the following message : "1, 2, 3"

The output simulates the encoded message - for each cycle we output 1 symbol (data_out)

valid_out stays high while we output the encoded message : 1,2,3,5,6,1,6

# Performance Measurement

The program measures execution time for:

- Polynomial division
- Encoding
- Decoding

This helps evaluate algorithm efficiency and potential **hardware acceleration on FPGA**.

---

# Future Work

A simple test in a large base of digital communication circuit can be amusing : 

```mermaid
flowchart LR
    A["Message (PC)"] -->|Ethernet| B["Encodeur FPGA (systématique)"]
    B -->|Ethernet| C["ENIGMA ENC FPGA"]
    C --> D["Injecteur d'erreurs FPGA"]
    D --> E["ENIGMA DEC FPGA"]
    E -->|Ethernet| F["Décodeur C++"]
    F --> G["Message d'origine"]
```

Planned extensions include:

- Implementation of **FFT over finite fields** : Impossible because n = 1 << m -1 so doesn't work 
- **VHDL hardware implementation** : We have already a hardware encoder 
- Integration into a **RISC-V processor** : I have no idea how 
- FPGA deployment and benchmarking : the circuit above may be very amusing 
- Support for larger codes (e.g. **RS(255,223)**) : our encodec can support to : 2^20 so very performative

---

# Applications

Reed–Solomon codes are used in:

- Digital communications
- Optical storage (CD/DVD/Blu-ray)
- Satellite communication
- QR codes
- Data storage systems
- Space communication

---

# Authors

**Nasr-Allah HITAR**  
**Khalil BITAR**

ENSEIRB-MATMECA – Bordeaux INP

Project developed within the course:

**UE Projet Thématique**

Supervisor:  
**Camille Leroux**

---

# License

This project is intended for **academic and research purposes**.
