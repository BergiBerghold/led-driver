
mkdir -p Bin

hash++ g++ -g -rdynamic -Wpedantic -std=c++17 -o ./Bin/ProtoPWM ProtoPWM.cpp -lusb-1.0
