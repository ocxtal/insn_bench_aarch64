
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -mcpu=native -static -Ixbyak_aarch64/xbyak_aarch64
LDFLAGS = -Lxbyak_aarch64/lib -lxbyak_aarch64

all: bench

bench: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) bench

.PHONY: all
