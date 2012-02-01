#include "model.h"

#if 1
const int field_width = 500 ;
const int field_height = 200 ;
const size_t iteration_count = 4000;
#else /* debug */
const int field_width = 5 ;
const int field_height = 5 ;
const size_t iteration_count = 8;
#endif

Cell cells_field[2][field_height][field_width];
Cell *cur_iteration, *next_iteration;

void print(int cur_it) {
    printf("\n=== iteration: %d ===\n", cur_it);
    for(int i = 0; i < field_height; i++) {
        for(int j = 0; j < field_width; j++)
            printf("(%02d,%02d) ", int(cur_iteration[i * field_height + j].state), int(cur_iteration[i * field_width + j].weight));
        puts("");
    }
}

void init() {
    calcEqualClasses();
    
#if 1
    const int t = 0;
    for(int i = 1+t; i < field_height - 1-t; i++)
        for(int j = 1+t; j < field_width - 1-t; j++) {
            int req_weight = 2;
            while(true) {
                int cell_state = rand()&((1<<6)-1);
                int cell_weight = req_weight - __builtin_popcount(cell_state);
                if(cell_weight < 0) continue;
                cells_field[0][i][j] = Cell(cell_state, cell_weight);
                break;
            }
        }
#else /* debug */
    cells_field[0][2][2] = Cell(63, 0);
#endif
#if 1
    for(int k = 0; k < 2; k++) {
        for(int i = 0; i < field_width; i++) {
            SET_BARRIER(cells_field[k][0][i]);
            SET_BARRIER(cells_field[k][field_height-1][i]);
        }
        for(int i = 0; i < field_height; i++) {
            SET_BARRIER(cells_field[k][i][0]);
            SET_BARRIER(cells_field[k][i][field_width-1]);
        }
        for(int i = 0; i < field_height; i++) {
            SET_SOURCE(cells_field[k][i][1]);
            SET_ASOURCE(cells_field[k][i][field_width-2]);
        }
        for(int j = 200; j < 225; j++) {
            for(int i = 50; i < 150; i++) SET_BARRIER(cells_field[k][i][j]);
        }
    }
#endif
}

/*
 *  Simulation
 */

size_t iteration;
int cur_field, next_field;

void doPrepareRow(int row_number) {
    for(int j = 0; j < field_width; j++) {
        if(IS_BARRIER(cells_field[cur_field][row_number][j])) {
            doBarrier(row_number, j);
        } else if(IS_SOURCE(cells_field[cur_field][row_number][j])) {
            if(iteration % 2000 < 300 || true) {
                Cell& cell = cells_field[cur_field][row_number][j];
                cell = generateCell(6, cell.weight);
            }
        } else if(IS_ASOURCE(cells_field[cur_field][row_number][j])) {
            Cell& cell = cells_field[cur_field][row_number][j];
            cell = generateCell(3, cell.weight);
        }
    }
}

void doShifRow(int row_number) {
    for(int j = 0; j < field_width; j++) {
        doShift(row_number, j);
        doImpact(&cells_field[next_field][row_number][j]);
    }
}

void doImpactRow(int) {

}

void run() {
    cur_field = 0;
    next_field = 1;
    #pragma omp parallel
    {
        for(iteration = 0; iteration < iteration_count; iteration++) {
            cur_iteration = &cells_field[cur_field][0][0];
            next_iteration = &cells_field[next_field][0][0];
#if 0
            /*
               for(int i = 0; i < field_height; i++) {
               if(iteration % 1000 < 300) {
               cur_iteration[field_width * i + 1] = generateCell(5);
               }
               cur_iteration[field_width * i + field_width - 2] = generateCell(2);
               }
             */
            for(int i = 0; i < field_width; i++) {
                doBarrier(0, i);
                doBarrier(field_height - 1, i);
            }
            for(int i = 0; i < field_height; i++) {
                doBarrier(i, 0);
                doBarrier(i, field_width - 1);
            }
            for(int i = 1; i < field_height - 1; i++)
                for(int j = 1; j < field_width - 1; j++) {
                    doShift(i, j);
                    doImpact(&cells_field[next_field][i][j]);
                }
#elsif 0
            for(int i = 0; i < field_height; i++) doPrepareRow(i);
            for(int i = 0; i < field_height; i++) doShifRow(i);
#else
#pragma omp single
            {
                doPrepareRow(0);
            }
#pragma omp for
            for(int i = 0; i < field_height - 1; i++) {
                doPrepareRow(i + 1);
                doShifRow(i);
            }
#pragma omp single
            {
                doShifRow(field_height - 1);
            }
#endif
#pragma omp single
            {
                swap(cur_field, next_field);
                if(!(iteration & 15)) genBinary(iteration);
            }
            //genBinary(iteration);
            //print(iteration);
        }
    }
}
