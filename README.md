## ⚙️ Environment

| Component       | Version / Info |
|-----------------|----------------|
| **OS**          | Windows 11 |
| **Compiler**    | MSYS2 MinGW64 (g++ / clang) |
| **CMake**       | 4.1.2 |
| **SFML**        | 3.0.2-1 *(installed via MSYS2)* |
| **ImGui**       | 1.91.9 |
| **ImGui-SFML**  | 3.0 |
| **ImPlot**      | https://github.com/epezent/implot |
| **PATH setup**  | MSYS2’s `mingw64/bin` added to Windows `PATH` | 

## Build & Run
```bash
mkdir build
cmake -S . -B build
cmake --build build
.\build\MyApp.exe
