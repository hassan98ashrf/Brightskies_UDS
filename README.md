# 🚗 Automotive Diagnostic Protocols (UDS & CAN-TP) Unit Testing Framework

## 📖 Introduction
This project provides a robust Unit Testing framework for Automotive Embedded Systems, specifically targeting the **UDS (Unified Diagnostic Services - ISO 14229)** and **CAN-TP (Transport Protocol - ISO 15765-2)** communication layers. 

### 🔍 What is UDS and How Does it Work?
**UDS** is a standardized protocol used by diagnostic systems to communicate with Electronic Control Units (ECUs) in vehicles. It operates on a **Client-Server architecture**:
* **Client (Tester Tool):** Sends a diagnostic request (e.g., "Read Engine Temperature").
* **Server (ECU):** Receives the request, processes it, and sends a Positive or Negative Response.

Since CAN bus frames are typically limited to 8 bytes, the **CAN-TP** layer acts as a bridge. It handles the **Segmentation** (breaking down long UDS messages into small 8-byte frames for transmission) and **Reassembly** (combining received 8-byte frames back into a complete UDS message).

---

## 🎯 Tested Scenarios
To ensure maximum reliability, the reception logic (CAN-TP Rx) was subjected to rigorous testing covering both normal operations and edge cases. Based on our test plan, the scenarios include:
* **Basic Reception:** Verifying the successful reception and storage of a single valid CAN frame.
* **Consecutive Frames & Split Data:** Testing the reception and successful reassembly of multi-frame messages split across several CAN frames.
* **Boundary & Edge Cases:** Handling the maximum allowed frame size (payload limit) and zero-length/minimum non-zero length frames.
* **Error & Interruption Handling:** Validating the system's response to interrupted transmissions, concurrent receptions, and invalid frame formats (e.g., invalid DLC).
* **Robustness & Validation:** Simulating buffer overflows, delayed frames (timeout handling), invalid CAN IDs, and corrupted CRC values to ensure the system returns the appropriate error codes without failing.

---

## 🛠️ Tools Used
* **Google Test (GTest):** A popular C++ testing framework developed by Google. It was used because it facilitates writing and running tests with simple syntax and a rich set of assertions to check values.
* **LCOV (Coverage Tool):** A tool for code coverage analysis that helps developers understand exactly how much of their code is executed during testing. It ensures thorough testing, improves code reliability, and generates detailed graphical coverage reports.

---

## 🚀 Technical Challenges & Solutions

### 1. Cross-Language Testing (C vs. C++ Integration)
* **The Challenge:** The core Automotive modules (CAN-TP & UDS) are strictly written in pure **C** (complying with embedded systems standards), while Google Test (GTest) is fundamentally a **C++** framework.
* **The Solution:** We successfully bridged the two environments by utilizing `extern "C"` wrappers around the C header inclusions within the `.cpp` test files. This prevented the C++ compiler's name-mangling, allowing GTest to seamlessly link and execute the C functions.

### 2. Code Coverage Generation on Windows (Linux Simulation)
* **The Challenge:** Generating graphical code coverage reports utilizing `LCOV` and `gcov` is natively supported on Linux architectures. Running these tools directly on a standard Windows development environment presents compatibility issues.
* **The Solution:** We configured a Linux simulation environment on Windows (using Windows Subsystem for Linux - WSL)[cite: 7]. This allowed us to compile the binaries using `g++` with coverage flags (`--coverage` / `-fprofile-arcs -ftest-coverage`) and utilize Linux-native tools to parse the `.info` files and generate the final HTML coverage reports directly from the Windows directory path (`/mnt/g/...`).

---

## 📊 Code Coverage Reports
The integration of LCOV successfully identified tested and untested parts of the codebase.
*(Check the `images/` folder for full LCOV graphical reports showing Line Coverage and Functions Coverage).*