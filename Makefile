CC=gcc
CXX=g++
#CFLAGS=-O0 -g -Wall -Wextra -Wshadow
CFLAGS=-O3 -march=native -Wall -Wextra -Wshadow -fopenmp
LDFLAGS=-lm
CXXFLAGS=${CFLAGS}
TARGET=ca.out

${TARGET}: main.o model.o cell.o visualizer.o utils.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.o ${TARGET}
	rm -f *.bin
# vim:set noet:
