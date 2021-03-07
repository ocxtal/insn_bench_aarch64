
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wno-unused-variable -Og -mcpu=native

all: xbyak_aarch64/lib/libxbyak_aarch64.a
	$(MAKE) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" -C src all
	$(MAKE) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" -C extra all

xbyak_aarch64/lib/libxbyak_aarch64.a:
	$(MAKE) CXX=$(CXX) -C xbyak_aarch64

clean:
	$(MAKE) -C src clean
	$(MAKE) -C extra clean

-include $(SRCS:cc=d)

.PHONY: all

