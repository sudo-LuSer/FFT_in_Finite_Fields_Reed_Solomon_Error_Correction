CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS   =

TARGET    = RS_RUN

SRCDIR    = src
INCDIR    = include
OBJDIR    = obj

SRCS      = $(wildcard $(SRCDIR)/*.cpp)
OBJS      = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir $(OBJDIR)    
	$(CXX) $(CXXFLAGS) -c $< -o $@

DEPS = $(OBJS:.o=.d)
-include $(DEPS)

$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	-mkdir $(OBJDIR)      # <- idem ici
	$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run