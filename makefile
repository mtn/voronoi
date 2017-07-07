CC = g++
CXXFLAGS  = -Wall

src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

thiessen: $(obj)
	$(CC) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) thiessen

