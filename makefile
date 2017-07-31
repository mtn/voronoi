CC = g++
CXXFLAGS  = -Wall
FRAMEWORK = -framework sdl2

src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

thiessen: $(obj)
	$(CC) -o $@ $^ $(CXXFLAGS) $(FRAMEWORK)

.PHONY: clean
clean:
	rm -f $(obj) thiessen

