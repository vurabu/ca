#pragma once

#include <vector>
#include <map>

#include "common.h"
#include "utils.h"

struct Cell {
    uchar state;
    uchar weight;

#define CELL_BARRIER_BIT 0x10
#define CELL_SOURCE_BIT  0x40
#define CELL_ASOURCE_BIT 0x20
#define CELL_ANOTHER_ENV 0x80

#define CELL_WEIGHT_BITS 0x7
#define CELL_STATE_BITS  0x3f

#define SET_BARRIER(x)  ((x).weight |= CELL_BARRIER_BIT)
#define SET_SOURCE(x)   ((x).weight |= CELL_SOURCE_BIT)
#define SET_ASOURCE(x)  ((x).weight |= CELL_ASOURCE_BIT)
#define SET_AENV(x)     ((x).weight |= CELL_ANOTHER_ENV)

#define IS_BARRIER(x)   ((x).weight & CELL_BARRIER_BIT)
#define IS_SOURCE(x)    ((x).weight & CELL_SOURCE_BIT)
#define IS_ASOURCE(x)   ((x).weight & CELL_ASOURCE_BIT)
#define IS_AENV(x)      ((x).weight & CELL_ANOTHER_ENV)

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
