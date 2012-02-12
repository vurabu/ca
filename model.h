#pragma once

#include "cell.h"
#include "visualizer.h"
#include "common.h"
#include "utils.h"

extern const int field_width;
extern const int field_height;

void print(int cur_it);
void init(const string& name_suffix);
void run();
