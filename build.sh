#!/bin/bash

# gccコマンドを実行してソースファイルをコンパイル
gcc -o ./build/server src/server.c
gcc -o ./build/client src/client.c