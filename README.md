- **OS:** Windows 11  
- **Compiler:** MSYS2 MinGW64 (g++ / clang)  
- **CMake:** 4.1.2 
- **SFML:** 3.0.2-1 (installed via MSYS2 package manager)  
- **ImGui:** 1.91.9  
- **ImGui-SFML:** 3.0  
- **Path setup:** MSYS2’s `mingw64/bin` directory added to Windows `PATH` environment variable  

mkdir build
cmake -S . -B build
cmake --build build
.\build\MyApp.exe