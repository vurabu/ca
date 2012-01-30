#pragma once

#include "common.h"
#include <algorithm>
#include "cell.h"

extern Cell *cur_iteration, *next_iteration;
extern int arr_size;

extern const int field_width;
extern const int field_height;

void run();
