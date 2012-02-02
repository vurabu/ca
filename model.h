#pragma once

#include "common.h"
#include "cell.h"
#include "visualizer.h"

extern const int field_width;
extern const int field_height;

void print(int cur_it);
void init(const string& name_suffix);
void run();
