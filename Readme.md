Step to practice

I. Create project

1. Create file src/main.cpp
```
#include <iostream>

int main() {
    std::cout << "Hello CI/CD Project!" << std::endl;
    return 0;
}
```

2. Create CMakeLists.txt
```
cmake_minimum_required(VERSION 3.20)

project(CppCiCdProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp_ci_cd
    src/main.cpp
)
```

3. Build

3.1 Generate Build system

```
cmake -S . -B build
```

'-S .' : Source . -> source directory là '.'
'-B build' : Build build: build directory là 'build'

3.2 Compiler Project

main.cpp -> Compiler -> main.obj/main.o -> Linker -> cpp_ci_cd.exe

```
cmake --build build
```

II. Create Git 

1. Create .gitignore
```
# Build directory
build/

# CMake generated files
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile

# IDE
.vscode/
.idea/
.vs/

# Visual Studio
*.sln
*.vcxproj
*.vcxproj.filters
*.vcxproj.user

# Executables
*.exe
*.out
```
