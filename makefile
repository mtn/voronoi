CC = g++
CXXFLAGS  = -Wall
FRAMEWORK = -framework sdl2

src = $(wildcard src/*.cpp)
lib = $(wildcard src/lib/*.h)
obj = $(src:.cpp=.o)

thiessen: $(obj) $(lib)
	$(CC) -o $@ $(obj) $(CXXFLAGS) $(FRAMEWORK)

.PHONY: clean
clean:
	rm -f $(obj) thiessen

