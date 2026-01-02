## 빌드 방법

### 필수 요구사항
- CMake 3.20 이상
- C++17 지원 컴파일러
- Git
- OpenGL 지원 그래픽 카드

### 빌드
```bash
git clone https://github.com/TaehunKim0/optimization-openworld.git
cd OpenWorldOptimization
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

첫 빌드 시 GLFW, GLM을 자동으로 다운로드합니다. (인터넷 연결 필요)

### 실행
```bash
# Windows
Release\OptimizationDemo.exe