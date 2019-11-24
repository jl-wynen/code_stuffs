CXX = g++
CXXFLAGS = --std=c++17 -g -O0

CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic \
        -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual \
        -Wconversion -Wnull-dereference \
        -Wdouble-promotion -Wformat=2 -Wmisleading-indentation -Wduplicated-cond \
        -Wduplicated-branches -Wlogical-op -Wuseless-cast

# CXXFLAGS += -fsanitize=memory -g -fno-omit-frame-pointer


.PHONY: all
all: run addon.so

libbase.so : base.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^

run: main.cpp libbase.so
	$(CXX) $(CXXFLAGS) -ldl -L./ -Wl,-rpath ./ -lbase -o $@ main.cpp

addon.so: addon.cpp libbase.so
	$(CXX) $(CXXFLAGS) -shared -fPIC -L./ -Wl,-rpath ./ -lbase -o $@ addon.cpp

.PHONY: clean
clean:
	rm -f run *.so
