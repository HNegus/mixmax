
# Introduction

Run C++ version:
```
./Max <input filename> <output filename> <N frames> [--show]
```

Optional parameter `--show` to immediately see output.

## Build steps

C++ version:
```
# First build opencv
cd src/opencv
mkdir build
cmake -B build -DBUILD_LIST=videoio,video,highgui,core .
cmake --build build

# Then build executable
cd ../../
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build .
```

## Dependencies

C++ version:

- Cmake
- C++17 compiler
- OpenCV
