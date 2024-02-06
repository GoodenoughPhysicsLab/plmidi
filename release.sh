rm -rf ./proInclude.exe release/
clang++ proInclude.cpp -o proInclude -std=c++23 -Ofast -fno-exceptions -fno-rtti
./proInclude plmidi_cpp/setup.cpp
