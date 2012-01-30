#pragma once

#include "common.h"
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct Cell {
    uchar state;
    uchar weight;

    Cell() {}
    Cell(uchar s, uchar w) : state(s), weight(w) {}
};

extern const int field_width, field_height;

void doBarrier(int row, int col);
void doShift(int row, int col);
void doImpact(Cell *cell);

void calcEqualClasses();
