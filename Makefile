CC=gcc
CXX=g++
#CFLAGS=-O0 -g -Wall -Wextra -Wshadow
CFLAGS=-O3 -march=native -Wall -Wextra -Wshadow #-fopenmp
LDFLAGS=-lm
CXXFLAGS=${CFLAGS}
TARGET=ca.out

${TARGET}: main.o model.o cell.o visualizer.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@

binclean:
	rm -f *.bin

clean:
	rm -f *.o ${TARGET}

# vim: set noet:
