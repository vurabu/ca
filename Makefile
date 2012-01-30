CC=gcc
CXX=g++
CFLAGS=-O0 -g -Wall -Wextra -Wshadow
CXXFLAGS=${CFLAGS}
TARGET=ca.out

${TARGET}: main.o model.o cell.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.o ${TARGET}
