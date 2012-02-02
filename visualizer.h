#pragma once

#include <cstdio>
#include <cmath>

#include "common.h"
#include "cell.h"

extern const int field_width;
extern const int field_height;
extern Cell *cur_iteration;
const int sum_radius = 50;

void saveVisualizationConfig(const string& name_suffix = "");
bool addCell(size_t row, size_t col, double *sum);
void genBinary(size_t cur_it);
