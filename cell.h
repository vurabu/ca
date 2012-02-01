#pragma once

#include "common.h"
#include <vector>
#include <map>

struct Cell {
    uchar state;
    uchar weight;

#define CELL_BARRIER_BIT 0x10
#define CELL_SOURCE_BIT  0x40
#define CELL_ASOURCE_BIT 0x20
#define CELL_WEIGHT_BITS 0xf
#define CELL_STATE_BITS  0x3f

#define SET_BARRIER(x)  ((x).weight |= CELL_BARRIER_BIT)
#define SET_SOURCE(x)   ((x).weight |= CELL_SOURCE_BIT)
#define SET_ASOURCE(x)  ((x).weight |= CELL_ASOURCE_BIT)

#define IS_BARRIER(x)   ((x).weight & CELL_BARRIER_BIT)
#define IS_SOURCE(x)    ((x).weight & CELL_SOURCE_BIT)
#define IS_ASOURCE(x)   ((x).weight & CELL_ASOURCE_BIT)

    Cell() {}
    Cell(uchar s, uchar w) : state(s), weight(w) {}

    int getWeight();
};

extern const int field_width, field_height;
extern Cell *cur_iteration, *next_iteration;

void doBarrier(int row, int col);
void doShift(int row, int col);
void doImpact(Cell *cell);

Cell generateCell(int state_weight, int weight, uchar type_bits);
Cell generateCell(int sum_weight, uchar type_bits);

void calcEqualClasses();
