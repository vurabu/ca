#include "model.h"

const int field_width = 10;
const int field_height = 5;
Cell cells_field[2][field_height][field_width];

void print(int iteration_number) {
	printf("\n=== iteration: %d ===\n", iteration_number);
	for(int i=0; i<field_height; i++) {
		for(int j=0; j<field_width; j++)
			printf("(%02d,%02d) ", int(cells_field[iteration_number&1][i][j].state), int(cells_field[iteration_number&1][i][j].weight));
		puts("");
	}
}

void run() {
	calcEqualClasses();
	/*
	for(int i = 0; i < field_width; i++) {
		cells_field[0][0][i] = cells_field[1][0][i] = Cell((1<<6)-1, 0);
		cells_field[0][field_height-1][i] = cells_field[1][field_height-1][i] = Cell((1<<6)-1, 0);
	}
	for(int i = 0; i < field_height; i++) {
		cells_field[0][i][0] = cells_field[1][i][0] = Cell((1<<6)-1, 0);
		cells_field[0][i][field_width-1] = cells_field[1][i][field_width-1] = Cell((1<<6)-1, 0);
	}
	*/
#if 0
	for(int i = 1; i < field_height - 1; i++)
		for(int j = 1; j < field_width - 1; j++)
			cells_field[0][i][j] = Cell(rand()&((1<<6)-1), rand()%8);
#else
	cells_field[0][2][2] = Cell(9, 0);
#endif

	int cur_field = 0;
	int next_field = 1;
	for(int _=0; _<6; _++) {
		cur_iteration = (Cell*)cells_field[cur_field];
		next_iteration = (Cell*)cells_field[next_field];
		for(int i = 0; i < field_width; i++) {
			doBarrier(0, i);
			doBarrier(field_height - 1, i);
		}
		for(int i = 0; i < field_height; i++) {
			doBarrier(i, 0);
			doBarrier(i, field_height - 1);
		}

		for(int i = 1; i < field_height - 1; i++)
			for(int j = 1; j < field_width - 1; j++) {
				doShift(i, j);
				doImpact(&cells_field[next_field][i][j]);
			}

		swap(cur_field, next_field);
		
		print(_);
	}
}
