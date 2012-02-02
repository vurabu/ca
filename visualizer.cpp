#include "visualizer.h"

bool addCell(int row, int col, double *sum) {
    if(0 < row && row < field_height - 1 && 0 < col && col < field_width - 1) {
        (*sum) += cur_iteration[row * field_width + col].getWeight();
        return true;
    }
    return false;
}

void genBinary(size_t cur_it) {
    fprintf(stderr, "iteration = %05lu  array = %p\n", cur_it, cur_iteration);

    char filename[1<<5];
    FILE *cur_it_file;

    sprintf(filename, "%06lu.bin", cur_it);
    assert(cur_it_file = fopen(filename, "w"));
    
    int allsum = 0;
    for(int i = 0; i < field_height; i++) {
        for(int j = 0; j < field_width; j++) {
            double weight_sum = cur_iteration[i * field_width + j].getWeight();
            if(i*j*(field_height-i-1)*(field_width-j-1)) allsum += int(weight_sum);
            int cells_count = 1;

#if 1
            for(int k = 1; k <= sum_radius; k++) {
                if(addCell(i, j + k, &weight_sum)) cells_count++;
                if(addCell(i, j - k, &weight_sum)) cells_count++;

                for(int l = 0; l < 2 * sum_radius - k + 1; l++) {
                    if(addCell(i - k, j + sum_radius - l, &weight_sum)) cells_count++;
                    if(addCell(i + k, j - sum_radius + l, &weight_sum)) cells_count++;
                }
            }
            weight_sum /= cells_count;
#endif
            fwrite(&weight_sum, sizeof(double), 1, cur_it_file);
        }
    }
    fprintf(stderr, "sum = %d\n", allsum);

    fclose(cur_it_file);
}
