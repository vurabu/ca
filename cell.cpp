#include "cell.h"

Cell *cur_iteration, *next_iteration;

int classByState[1<<6][1<<9];
vector<Cell> statesByClass[1<<(6+9)];

const int dir[][2] = { {1, -1}, {0, -1}, {-1, 0},   {-1, 1}, {0, 1}, {1, 0} };

void doBarrier(int row, int col) {
    Cell *cur_cell = &cur_iteration[row * field_width + col];
    cur_cell->state = 0;
    for(int i = 0; i < 6; i++) {
        int next_row = row + dir[i][1];
        int next_col = col + dir[i][0];
        if(next_row < 0 || next_row >= field_height || next_col < 0 || next_col >= field_width)
            continue;
        int t = (cur_iteration[next_row * field_width + next_col].state & (1<<i));
        cur_cell->state |= ((t<<3)|(t>>3))&((1<<6)-1);
    }
}

void doShift(int row, int col) {
    Cell *cur_cell = &next_iteration[row * field_width + col];
    cur_cell->state = 0;
    for(int i = 0; i < 6; i++) {
        int next_row = row + dir[i][1];
        int next_col = col + dir[i][0];
        cur_cell->state |= (cur_iteration[next_row * field_width + next_col].state & (1<<i));
    }
}

void doImpact(Cell *cell) {
    int myclass = classByState[cell->state][cell->weight];
    const vector<Cell>& available_states = statesByClass[myclass];
    *cell = available_states[rand()%available_states.size()];
}

typedef vector<uchar> StateType;
typedef map< pair<int, int>, StateType> EqType;
void calcEqualClasses() {
    const int weights[][2] = { {1, 1}, {2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1} };

    EqType eq_classes;
    for(uchar i = 0; i<(1<<6); i++) {
        int xres = 0, yres = 0;
        for(int j=0; j<6; j++) 
            if(i&(1<<j)) {
                xres += weights[j][0];
                yres += weights[j][1];
            }
        eq_classes[make_pair(xres, yres)].push_back(i);
    }
    int classes_count = 0;
    for(EqType::iterator it = eq_classes.begin(); it != eq_classes.end(); ++it) {
        //printf("x=%d y=%d\n", it->first.first, it->first.second);
        for(StateType::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
            //printf("%d\n", int(*jt));
            for(uchar i=0; i<8; i++) {
                int myweight = __builtin_popcount(*jt) + i;
                classByState[*jt][i] = classes_count + myweight;
                statesByClass[classes_count + myweight].push_back(Cell(*jt, i));
            }
        }
        classes_count += 14;
    }
#if 0
    for(int i=0; ;i++) if(__builtin_popcount(i) == 2) {
        int ttt = classByState[i][0];
        for(i=0; i<statesByClass[ttt].size(); i++) printf("[%d,%d] ", statesByClass[ttt][i].state, statesByClass[ttt][i].weight);
        puts("!!!");
        return ;
    }
    for(int ii=0; ii<8; ii++) {
        for(int i=0; i<statesByClass[ii].size(); i++) printf("[%d,%d] ", statesByClass[ii][i].state, statesByClass[ii][i].weight);
        puts("");
    }
    puts("");
    puts("");
#endif
}
