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
├── rs/                 # Updated files (use them on piriority) 
├── matlab/             # MATLAB validation scripts
├── rs_encoder/         # (future work) VHDL implementation for FPGA
│
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

- **Make**

### Optional

- **MATLAB** with *Communications Toolbox* for validation

---

# Compilation

A **Makefile** is provided at the root of the project.

### Linux / macOS

```bash
make
```

### Windows (MinGW)

```bash
mingw32-make
```

This generates:

- object files in `obj/`
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

# Performance Measurement

The program measures execution time for:

- Polynomial division
- Encoding
- Decoding

This helps evaluate algorithm efficiency and potential **hardware acceleration on FPGA**.

---

# Future Work

Planned extensions include:

- Implementation of **FFT over finite fields**
- **VHDL hardware implementation**
- Integration into a **RISC-V processor**
- FPGA deployment and benchmarking
- Support for larger codes (e.g. **RS(255,223)**)

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
