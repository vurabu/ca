CC=gcc
CXX=g++
#CFLAGS=-O0 -g -Wall -Wextra -Wshadow
CFLAGS=-O3 -march=native -Wall -Wextra -Wshadow -fopenmp
CXXFLAGS=${CFLAGS}
TARGET=ca.out

${TARGET}: main.o model.o cell.o visualizer.o common.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.o ${TARGET}
	rm -f *.bin
