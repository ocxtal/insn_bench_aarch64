
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -mcpu=native -Iinclude -Ixbyak_aarch64/xbyak_aarch64
LDFLAGS = -Lxbyak_aarch64/lib -lxbyak_aarch64
TARGET = bench
SRCS = $(shell bash -c "ls src/*.cc")
OBJS = $(SRCS:cc=o)

all: $(TARGET)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD -MP -MF $(@:.o=.d)

xbyak_aarch64/lib/libxbyak_aarch64.a:
	$(MAKE) -C xbyak_aarch64

$(TARGET): $(OBJS) xbyak_aarch64/lib/libxbyak_aarch64.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -f $(TARGET) src/*.{o,d}

-include $(SRCS:cc=d)

.PHONY: all

