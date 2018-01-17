FILENAME=flooids
EXE=$(FILENAME)

CXX = clang++
CXXFLAGS = -std=c++14 -I./includes
LINK_FLAGS = -lglfw3 -lglew -lassimp -lSOIL -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit

src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

$(EXE): $(obj)
	$(CXX) -o $@ $^ $(COMPILE_FLAGS) $(LINK_FLAGS)

.PHONY: clean
clean:
	rm -rf $(obj) $(EXE)

run:
	./$(FILENAME)
