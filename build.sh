#! /bin/bash

set -xe 
clang -o build/main src/main.c -I./include -I./external/raylib/include -I./external/freetype/include -L./external/raylib/lib -L./external/freetype/lib -L./external/brotli/lib -l:libraylib.a -l:libfreetype.a -l:libbrotlidec.a -l:libbrotlicommon.a -lgdi32 -lwinmm

# clang -o build/exportdata src/exportdata.c -I./external/raylib/include -L./external/raylib/lib -l:libraylib.a -lgdi32 -lwinmm
