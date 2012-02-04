#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

#define E(x) { cerr << #x << " = " << (x) << "   "; }
#define Eo(x) { cerr << #x << " = " << (x) << endl; }
#define EO(x) Eo(x)

typedef unsigned char uchar;



extern int preBitcount[0x100];
void initBitcount();
inline int bitcount(uchar value) {
    return preBitcount[value];
}

int bitcount(int value);
