CXX = clang++
CXXFLAGS = --std=c++17 -Wall -Wextra -pedantic -pthread -g -O0
CXXFLAGS += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic

# CXXFLAGS += -fsanitize=memory -g -fno-omit-frame-pointer

.PHONY: all
all: run

run: main.cpp addon.so
	$(CXX) $(CXXFLAGS) -ldl -o run main.cpp

addon.so: addon.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^

.PHONY: clean
clean:
	rm -f run *.so
