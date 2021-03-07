
SRCS   := $(shell bash -c "ls *.cc")
OBJS   := $(SRCS:cc=o)

INCFLAGS += -I../utils -I../xbyak_aarch64/xbyak_aarch64

%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c -o $@ $< -MMD -MP -MF $(@:.o=.d)

clean:
	$(RM) -f $(TARGET) *.o *.d

-include $(SRCS:cc=d)
