
SRCS   := $(shell bash -c "ls *.cc")
OBJS   := $(SRCS:cc=o)
TARGET := insn_bench_aarch64

INCFLAGS += -I../utils -I../xbyak_aarch64/xbyak_aarch64
LDFLAGS  += -L../xbyak_aarch64/lib -lxbyak_aarch64 ../utils/utils.o

all: $(TARGET)

%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -DMHZ=3200 -c -o $@ $< -MMD -MP -MF $(@:.o=.d)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -f $(TARGET) *.o *.d

-include $(SRCS:cc=d)

.PHONY: all
