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

2. init git and commit
```
git init
git add .
git commit -m "Initial C++ project with CMake"
```

3. Tạo repo trên github

4. Connect local with github

III. Add UnitTest with GoogleTest

1. Tạo class Calculator -> see change in git commit

Build Cmake again, and run to see a result

2. Add GoogleTest

Sử dụng 'FetchContent' cho practice

- Update CMakeLists.txt

- Tạo UnitTest: CalculatorTest.cpp

- Build:

```
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build
```

- Run Test

```
cmake --build build
```

Knowledge:

GoogleTest là framework để viết test
```
TEST()
EXPECT_EQ()
```

CTest là tool CMake để quản lý Test

IV. Tạo CI với GitHub Actions

1. Tạo file ci.yml trong .github/workflows

on:
```
on:
  push:
    branches:
      - main

    pull_request:
    branches:
        - main
```
Khi có push vào nhánh main thì chạy CI
Khi có PR vào merger vào nhánh main cũng chạy CI

jobs:
```
jobs:
├── build-and-test
├── static-analysis
├── windows-build
└── release
```

```
jobs:
  build-and-test:

    runs-on: ubuntu-latest
    // GitHub sẽ tạo ra một máy Ubuntu tạm thời để chạy CI.
    // Sau khi job kết thúc, machine này sẽ bị hủy
```

2. Push lên GitHub













