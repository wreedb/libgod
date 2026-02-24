CXX := clang++
CXXFLAGS := -std=c++26 -fPIC -g3 -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -pipe -O0 -march=x86-64-v3 -mtune=generic -fdiagnostics-color -Wno-unused-command-line-argument
CPPFLAGS := -I./include

SOURCES  = input.cpp scanner.cpp token.cpp util.cpp parse.cpp
SOURCES := $(addprefix src/,${SOURCES})
OBJECTS := $(addsuffix .o,${SOURCES})

default: demo
all: default

%.cpp.o: %.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o $@ -c $<

demo: demo.cpp.o ${OBJECTS}
	${CXX} ${CXXFLAGS} -o $@ $^

clean:
	-rm -f ${OBJECTS} *.o demo