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

3. Xem CI chạy

4. Thử nghiệm CI fail -> Sửa UnitTest -> Push lên git

5. Sửa lại cho đúng rồi push lên git

-> Pipeline hiện tại 

```
                  GitHub
                     │
              git push / PR
                     │
                     ▼
              ┌─────────────┐
              │ C++ CI       │
              └──────┬──────┘
                     │
                     ▼
             Ubuntu Runner
                     │
             ┌───────┴────────┐
             ▼                ▼
         CMake Build       CTest
             │                │
             └───────┬────────┘
                     ▼
                 PASS / FAIL
```

V. Code Formatting với clang-format.

CI sẽ kiểm tra code có tuân thủ code style không?

1. Cài clang-format ở local

Cài đến khi có thể chạy
```
clang-format --version
```

2. Tạo file .clang-format

3. Format toàn bộ code

powershell sau, sau đó commit
```
Get-ChildItem -Path . -Include *.cpp,*.h -Recurse |
ForEach-Object {
    clang-format -i $_.FullName
}
```

4. Kiểm tra format local
```
clang-format --dry-run --Werror src/main.cpp
```
Nếu có lỗi -> báo lỗi. Hãy thử tạo lỗi để xem kqua

5. Thêm Format Check vào GitHub Actions -> thêm vào ci.yml

6. Push lên git

7. Cố tình làm CI fail

Sau đó format lại:
```
clang-format -i src/Calculator.cpp
```

VI. clang-tidy Static Analysis

1. Kiểm tra clang-tidy local
```
clang-tidy --version
```

2. Tạo .clang-tidy

```
Checks: >
  bugprone-*,   ------------> Tìm những code pattern có khả năng gây bug
  performance-*,      --------> Tìm những đoạn code có thể gây vấn đề performance
  modernize-*,        --------> Gợi ý dùng C++ hiện đại hơn
  readability-*,      --------> Kiểm tra readability/code practice
  -readability-identifier-length

WarningsAsErrors: ''    ----------> Warning k làm CI fail
HeaderFilterRegex: '^(include|src|tests)/'
```

3. Chạy clang-tidy local
```
cmake -S . -B build
cmake --build build
```
```
clang-tidy \
    src/Calculator.cpp \
    -- \
    -Iinclude
```
hoặc:
```
clang-tidy src/Calculator.cpp -- -Iinclude
```

4. Bật compile_commands.json

update CMakeLists.txt

```
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

Sau đó xóa build và build lại bằng Ninja để tạo file compile_commands.json.

Nếu chưa có Ninja -> cài nó

```
Remove-Item -Recurse -Force build
cmake -S . -B build -G "Ninja"
cmake --build build
```

Expected -> thấy file compile_commands.json tại build/

5. Chạy clang-tidy bằng compile database

```
clang-tidy -p build src/Calculator.cpp
```

-p build: lấy copile config từ build/compile_commands.json

6. Thêm clang-tidy vào CI.yml

VII. Multi platform CI

Update ci.yml

- Thay vì tạo 2 yml -> sử dụng matrix

- fail-fast: false ---> Fail vẫn chạy tiếp

- Lưu ý quan trọng

+ Step format hiện tại dùng 'find' 'xargs' -> chỉ dùng cho Ubuntu

+ Nên cần thêm điều kiện 'if' vào clang-format và clang-tidy


VIII. CD (Continuous Delivery/Development)

1. Tạo release workflow -> create release.yml

- Trigger CD bằng Git Tag 'v*'

- Thêm build Linux với '-DCMAKE_BUILD_TYPE=Release' và '--config Release'

- Thêm build windows với '--config Release'

- Đóng gòi file release thành artifact
```
cpp-ci-cd-linux-v1.0.0.tar.gr
cpp-ci-cd-windows-v1.0.0.zip
```

- Upload artifact

- Tạo github release
```
    needs:
      - build-linux
      - build-windows
```
Chỉ chạy khi 2 job đều thành công



IX. Tách CI thành nhiều job - quality gate

- Hiện tại pipeline đang có dạng : CI > build and test

- Nên tách thành

+ build
+ test (unit test)
+ quality (format , tidy)

- Nhưng mỗi Github Action Job chạy trên runner riêng, để build xong test, cần Artifact

1. Build Artifact

Build > Upload Artifact > Github Storage

2. Test Job download Artifact

Build > Upload Artifact > Github Storage > Test Job > Download Artifact > CTest

Chú ý trước khi test, sau khi download phải cấp quyền excutive cho nó -> tránh lỗi [permission denied]

3. Quality Job

4. Finaly

```
                         CI
                          │
                     ┌────┴────┐
                     │  Build  │
                     └────┬────┘
                          │
                 ┌────────┴────────┐
                 ▼                 ▼
              Test             Quality
                 │                 │
                 │           ┌─────┴─────┐
                 │           ▼           ▼
                 │       format       clang-tidy
                 │           │           │
                 └───────────┴───────────┘
                             │
                             ▼
                          CI PASS
```

                        
5. Tạo Quality Gate

- thêm gate vào ci.yml

```
                    SOURCE
                       │
                    git push
                       │
                       ▼
                    ┌─────┐
                    │Build│
                    └──┬──┘
                       │
              ┌────────┴────────┐
              │                 │
              ▼                 ▼
           ┌──────┐         ┌─────────┐
           │ Test │         │ Quality │
           └───┬──┘         └────┬────┘
               │                 │
               └────────┬────────┘
                        ▼
                   ┌─────────┐
                   │  Gate   │
                   └────┬────┘
                        │
                       PASS
                        │
                        ▼
                     CD
```

X. Quản lý dependence C++

- Hiện tại project đang sử dụng GoogleTest thông qua FetchContent online

- Có nghĩa là mỗi lần CI chạy: Github Runner > CMake > Internet > Download GoogleTest > Build

- Nếu GitHub Dependence gặp vấn đề thì CI fail -> Đây là một vấn đề quan trọng

1. Về FetchContent:

- Hiện tại đang down từ URL: https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip

- Version cố định là một điểm tốt. Không nên để latest

2. Dependence Management có 3 lớp cần quan tâm:

- Version

- Source

- Reproducibility: phải có khả năng build lại sau này cùng dependency version 

3. Vấn đề CI hiện tại

- Mỗi Github Runner là một môi trường mới -> download lại dependency -> Cần Cache

4. Thêm Cache cho CMake

update ci.yml

- Chú ý: phần key - lưu hash của CMakeLists.txt

- Commit lại để thấy cache đã được lưu

XI. Build matrix với nhiều complier

C++ complier khác nhau có thể phát hiện những vấn đề khác nhau

Chúng ta muốn 
```
                    Build
                      │
              ┌───────┴───────┐
              ▼               ▼
            GCC             Clang
              │               │
              ▼               ▼
            Test             Test
```

1. Kiểm tra clang,g++ local
```
clang++ --version
g++ --version
```

2. Thay đổi build jobs

> update ci.yml

```
build:

  strategy:
    fail-fast: false

    matrix:
      compiler:
        - gcc
        - clang

  runs-on: ubuntu-latest
```

3. Chọn complier

Để dễ dúng, sửa matrix:
```
  matrix:
    compiler:
      - cc: gcc
        cxx: g++
      - cc: clang
        cxx: clang++
```
Sửa config CMake
```
      - name: Configure CMake
        run: |
          cmake -S . -B build \
            -DCMAKE_C_COMPILER=${{ matrix.compiler.cc }} \
            -DCMAKE_CXX_COMPILER=${{ matrix.compiler.cxx }}
```

4. Sửa tên key và artifact name

5. Thử push để xem kết quả trước

- build (clang, clang++) -> PASS

- build (gcc, g++) -> PASS

- quality -> FAIL

- test -> FAIL

6. Thêm Matrix cho Test

- Thêm strategy cho test
```
    strategy:
      fail-fast: false

      matrix:
        compiler:
          - g++
          - clang++
```

- Update tên Artifact

- Vấn đề: phần matrix không giống phần build -> thêm tên cho matrix
```
      matrix:
        compiler:
          - name: gcc
            cc: gcc
            cxx: g++
          - name: clang
            cc: clang
            cxx: clang++
```
```
      matrix:
        compiler:
          - gcc
          - clang
```

7. Push để xem kết quả
```
                         Build Matrix
                       /             \
                      /               \
                     ▼                 ▼
               Build GCC          Build Clang
                    │                 │
                    │                 │
                    ▼                 ▼
               build-gcc         build-clang
                    │                 │
                    ▼                 ▼
                Test GCC          Test Clang
                    │                 │
                    └────────┬────────┘
                             │
                             ▼
                          Quality
                             │
                             ▼
                            Gate
```

Fail Quality

8. Thêm quality matrix

- Sửa artifact download

- Clang-tidy bản thân là Clang-based static analyzer. Nên chúng ta chỉ cần chạy Clang-tidy 1 lần

- Nhưng để hiểu matrix tốt hơn -> Quality cho cả GCC và Clang

9. Push để xem kết quả

```
                         BUILD
                    ┌──────┴──────┐
                    ↓             ↓
                  GCC           Clang
                    │             │
                    ↓             ↓
                  TEST          TEST
                    │             │
                    │             │
                    ↓             ↓
                QUALITY        QUALITY
                    │             │
                    └──────┬──────┘
                           ↓
                          GATE
```

Pass all

10. Thử với Quality fail (bỏ dấu cách trong hàm add) -> Push thử để xem kqua

XII. CI cho Pull Request

1. Tạo branch
```
git checkout -b feature/test-ci
```
2. Push và tạo PR - Quan sát quá trình CI

=> PASS

3. Thử với một PR fail. Push và tạo PR - Quan sát quá trình CI

=> FAIL

Nhưng vẫn Merger được

4. Bật Rule

- Vào Setting > Rulesets để tạo rule

- Require a pull request before merging

- Require status checks to pass before merging > Add 'gate'

- Quan trọng là phải Active
                    

XIII. 

1. Sửa Gate thật sự kiểm tra CI

2. Tạo Release Build Artifact trong CI

Thêm job release-build-linux và release-build-windows

```
                    Build Matrix
                 ┌──────┴──────┐
                GCC          Clang
                 │              │
                 └──────┬───────┘
                        ↓
                  ┌───────────┐
                  │   Test    │
                  │  Quality  │
                  └─────┬─────┘
                        ↓
                       Gate
                        │
                 ┌──────┴──────┐
                 ↓             ↓
          Linux Release   Windows Release
                 │             │
                 ↓             ↓
          release-linux  release-windows
```
3. Cho CD dùng Artifact từ CI

Artifact giữa các job trong cùng workflow rất dễ truyền bằng upload-artifact → download-artifact.

Còn:

Artifact giữa hai workflow độc lập không tự động được chia sẻ theo cách đơn giản đó.

Trước tiên ta gộp Release Build vào cùng workflow CI:

- Sửa trigger tại 'on:' -> thêm tags:

- Thêm điều kiện cho release build: 'if: startsWith(github.ref, 'refs/tags/v')'

- Thêm job packet-linux và packet-windows 

- Thêm 'create-release:'

- Kiến trúc hoàn chỉnh bây giờ là:

```
                         build
                    ┌──────┴──────┐
                   GCC           Clang
                    │               │
                    └──────┬────────┘
                           ↓
                    ┌──────────────┐
                    │ Test Quality │
                    └───────┬──────┘
                            ↓
                           Gate
                            │
              ┌─────────────┴─────────────┐
              │                           │
          tag v*                       main / PR
              │                           │
              ↓                           X
     ┌──────────────────┐
     │                  │
     ↓                  ↓
Linux Release      Windows Release
     │                  │
     ↓                  ↓
Package Linux      Package Windows
     │                  │
     └────────┬─────────┘
              ↓
       GitHub Release
```

4. Dọn release.yml cũ + hoàn thiện kiến trúc


XIV. Docker / Container CI/CD

Mục tiêu:
```
GIt Push > CI > Test > Docker Build > Docker Image > Push Registry > Deploy
```

1. Tạo dockerfile

- Container bắt đầu từ Ubuntu.

- Docker tạo và chuyển vào /app

- Cài compiler và build tôols

- COPY . . -- copy cpp-ci-cd vào app

- Build

- CMD ["./build/cpp_ci_cd"] - Đây là command mặc định khi container được chạy.

2. Build Docker image

- Tải Docker Desktop

- Mở Docker và chờ trạng thái Running

- Thêm .dockerignore
```
build/
.git/
.vscode/
*.user
*.suo
```

- Chạy:
```
docker build -t cpp-ci-cd:1.0 .
```
+ Build docker image

+ Tên là cpp-ci-cd - Version 1.0

+ Thư mục hiện tại

- Kiểm tra
```
docker images
```

- Chạy container
```
docker run --rm cpp-ci-cd:1.0
```

+ Chạy docker image

+ Chạy xong tự động xóa

+ Chạy file với tên và version

3. Multi-stage Docker Build

Để loại bỏ các phần k cần thiết trong lúc Runtime -> chia thành 2 stage: Buildẻ và Runtime

-> Update dockerfile
```
COPY --from=builder /app/build/cpp_ci_cd .
```
-> Lấy file cpp-ci-cd từ stage buider sang runtime

Build và run lại.

4. Đưa Docker lên git

Mục tiêu:
```
Gate
 ↓
Docker Build
 ↓
Docker Image
```

- Thêm 'docker-build:' sau 'gate:'

- Sử dụng 'docker build -t cpp-ci-cd:${{ github.sha }} .' thay vì version cụ thể

- Push lên git và quan sát. Lúc này project có thể được đóng gói thành docker image chứ chưa được lưu vào Docker Hub/GHCR

5. Docker Build Cache trong GitHub Actions

- Lấy Docker cache từ GitHub Actions cache. Lưu Docker build layers vào GitHub Actions cache.
```
cache-from: type=gha
cache-to: type=gha,mode=max
```

- Không push image lên registry. Chỉ build.
```
push: false
```

- Push commit và xem thời gian

6. Push Docker Image lên GitHub Container Registry (GHCR)

- Thêm permission
```
packages: write cho phép workflow push Docker image lên GHCR.
```

- Push lên git và kiểm tra 'Packages'

7. Tag Docker Image theo Version

- Tạo Docker chỉ khi push vào main hoặc theo Tag

-> Thêm 'if:'

- Sửa 'tags' để có thể tạo ra 2 tag, 1 tag version hoặc main, 1 tag là SHA














