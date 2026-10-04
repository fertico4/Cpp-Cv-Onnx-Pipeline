# C++ Computer Vision & ONNX Inference Pipeline

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg)](https://cmake.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-5.0%2B-red.svg)](https://opencv.org/)
[![ONNX Runtime](https://img.shields.io/badge/ONNX_Runtime-1.x-brightgreen.svg)](https://onnxruntime.ai/)
[![License: All Rights Reserved](https://img.shields.io/badge/License-All_Rights_Reserved-red.svg)](#license)

A high-performance, cross-platform C++20 computer vision pipeline designed for real-time image preprocessing, deep learning tensor preparation, and interactive parameter tuning using **OpenCV** and **ONNX Runtime**.

## Key Features

- **Modern C++20 Architecture**: Clean separation between image processing routines, tensor transformations, and inference engine management.
- **Native OS File Selector**: Integrated native system dialogs (`osascript` on macOS, `zenity` on Linux) to select local images dynamically.
- **Interactive HighGUI UI**: Real-time parameter adjustment via UI sliders with instant visual feedback.
- **NCHW Tensor Conversion**: Fast transformation of standard OpenCV `cv::Mat` (HWC) memory layouts into normalized planar NCHW tensors required by ONNX Neural Networks.
- **Cross-Platform CMake System**: Automatic dependency lookup for Homebrew (Apple Silicon M1-M4) and standard environment paths on Linux/Windows.

---

## Interactive Controls

When running the application:
1. A **Native File Picker** opens to select any `.jpg` or `.png` image from your system.
2. The **Interactive Preview Window** allows adjusting edge detection thresholds in real time using trackbars.
3. Press **`S`** to export the processed output image to disk (`output_edges.png`).
4. Press **`ESC`** or **`Q`** to exit.

---

## Project Structure

```text
cpp-cv-onnx-pipeline/
├── CMakeLists.txt          # Cross-platform build script
├── .clang-format           # Allman/BSD brace style rules
├── README.md               # Project documentation
├── assets/                 # Sample assets and ONNX models
├── include/                # Public headers
│   ├── ImageProcessor.hpp  # Image processing operations & tensor transformations
│   └── InferenceEngine.hpp # ONNX Runtime wrapper interface
└── src/                    # Implementation files
    ├── ImageProcessor.cpp
    ├── InferenceEngine.cpp
    └── main.cpp            # Application entry point with HighGUI UI
```

---

## Prerequisites

### macOS

- Install dependencies using Homebrew:
```terminaloutput
brew install cmake opencv onnxruntime
```

### Ubuntu / Linux

```terminaloutput
sudo apt-get update
sudo apt-get install -y libopencv-dev cmake build-essential
# Download ONNX Runtime C++ release from [https://github.com/microsoft/onnxruntime/releases](https://github.com/microsoft/onnxruntime/releases)
```

### Windows

- Visual Studio 2022 (with C++ Workload)
- OpenCV (via `vcpkg` or binary installer)
- ONNX Runtime C++ package

---

## Build and Execution

### 1. Clone the repository

```terminaloutput
git clone [https://github.com/your-username/cpp-cv-onnx-pipeline.git](https://github.com/your-username/cpp-cv-onnx-pipeline.git)
cd cpp-cv-onnx-pipeline
```

### 2. Configure and Build

```terminaloutput
mkdir build && cd build
cmake ..
cmake --build .
```

### 3. Run Executable

```terminaloutput
./cpp-cv-onnx-pipeline
```

---

## License

Copyright (c) 2026 Fernando Jesús Pérez Martín. All Rights Reserved.
Unauthorised copying, modification, or distribution of this file via any medium is strictly prohibited.