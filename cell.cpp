#include "cell.h"

int classByState[1<<6][1<<9];
vector<Cell> statesByClass[1<<(6+9)];

#if 1 /* rectangle */
const int dir[2][6][2] = { { {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {0, 1}, {1, 0} },
                           { {1,  0}, {0, -1}, {-1,  0}, {-1, 1}, {0, 1}, {1, 1} } };
#else /* parallelogram */
const int dir[2][6][2] = { { {1, -1}, {0, -1}, {-1, 0},  {-1, 1}, {0, 1}, {1, 0} },
                         { {1, -1}, {0, -1}, {-1, 0},  {-1, 1}, {0, 1}, {1, 0} } };
#endif

inline bool isValidCoord(int row, int col) {
    return 0 <= row && row < field_height && 0 <= col && col < field_width;
}

void doBarrier(int row, int col) {
    Cell *cur_cell = &cur_iteration[row * field_width + col];
    //E(row); Eo(col);
    cur_cell->state = 0;
    cur_cell->weight &= ~CELL_WEIGHT_BITS;
    int binrow = row&1;
    for(int i = 0; i < 6; i++) {
        int next_row = row + dir[binrow][i][0];
        int next_col = col + dir[binrow][i][1];
        if(!isValidCoord(next_row, next_col)) continue;
        int t = (cur_iteration[next_row * field_width + next_col].state & (1<<i));
        cur_cell->state |= ((t<<3)|(t>>3))&((1<<6)-1);
    }
}

void doShift(int row, int col) {
    Cell *cur_cell = &next_iteration[row * field_width + col];
    cur_cell->state = 0;
    cur_cell->weight = cur_iteration[row * field_width + col].weight;
    int binrow = row&1;
    for(int i = 0; i < 6; i++) {
        int next_row = row + dir[binrow][i][0];
        int next_col = col + dir[binrow][i][1];
        if(!isValidCoord(next_row, next_col)) continue;
        cur_cell->state |= (cur_iteration[next_row * field_width + next_col].state & (1<<i));
    }
}

void doImpact(Cell *cell) {
    uchar prev_bits = cell->weight & ~CELL_WEIGHT_BITS;
    int myclass = classByState[cell->state][cell->weight & CELL_WEIGHT_BITS];
    const vector<Cell>& available_states = statesByClass[myclass];
    *cell = available_states[rand()%available_states.size()];
    cell->weight |= prev_bits;
}

Cell generateCell(int state_weight, int weight, uchar type_bits = 0) {
    type_bits &= ~CELL_WEIGHT_BITS;
    int arr[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20};
    random_shuffle(arr, arr+6);
    int t = 0;
    for(int i = 0; i < state_weight; i++) t |= arr[i];
    return Cell(t, weight | type_bits);
}

Cell generateCell(int sum_weight, uchar type_bits = 0) {
    int state_weight_min = max(0, sum_weight - 7);
    int state_weight_max = min(6, sum_weight);
    int state_weight = state_weight_min + rand() % (state_weight_max - state_weight_min + 1);
    return generateCell(state_weight, sum_weight - state_weight, type_bits);
}

typedef vector<uchar> StateType;
typedef map< pair<int, int>, StateType> EqType;
const int edge_dir[][2] = { {1, 1}, {2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1} };
void calcEqualClasses() {

    EqType eq_classes;
    for(uchar i = 0; i < (1<<6); i++) {
        int xres = 0, yres = 0;
        for(int j = 0; j < 6; j++) 
            if(i&(1<<j)) {
                xres += edge_dir[j][0];
                yres += edge_dir[j][1];
            }
        eq_classes[make_pair(xres, yres)].push_back(i);
    }
    int classes_count = 0;
    for(EqType::iterator it = eq_classes.begin(); it != eq_classes.end(); ++it) {
        for(StateType::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
            for(uchar i=0; i<8; i++) {
                int myweight = __builtin_popcount(*jt) + i;
                classByState[*jt][i] = classes_count + myweight;
                statesByClass[classes_count + myweight].push_back(Cell(*jt, i));
            }
        }
        classes_count += 6 + (1<<3);
    }
}


int Cell::getWeight() { 
    return __builtin_popcount(state) + (weight & CELL_WEIGHT_BITS); 
    int res = 0;
    for(int i = 0; i < 6; i++) if(state&(1<<i))
        res += edge_dir[i][0];

    if(res > 0)
        return 0;
    else if(res == 0)
        return 7;
    else
        return 6;
}
