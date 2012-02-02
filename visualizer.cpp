#include "visualizer.h"

string name_suffix;

void saveVisualizationConfig(const string& new_name_suffix) {
    name_suffix = new_name_suffix;
    ofstream config_file("last_run.cfg");
    if(!config_file.is_open()) {
        cerr << "Can't open last_run.cfg" << endl;
        return ;
    }
    config_file << "FIELD_HEIGHT=" << field_height << endl;
    config_file << "FIELD_WIDTH=" << field_width << endl;
    config_file.close();
}

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

#if 1
    int psums[field_height][field_width];
    int cell_count[field_height][field_width];

    for(int i = 1; i < field_height - 1; i++)
        for(int j = 1; j < field_width - 1; j++) {
            bool is_normal_cell = !IS_BARRIER(cur_iteration[i * field_width + j]);
            int cell_weight = cur_iteration[i * field_width + j].getWeight();

            psums[i][j] = psums[i - 1][j] + psums[i][j - 1] - psums[i - 1][j - 1]
                + (is_normal_cell ? cell_weight : 0);

            cell_count[i][j] = cell_count[i - 1][j] + cell_count[i][j - 1] - cell_count[i - 1][j - 1]
                + is_normal_cell ? 1 : 0;
        }
#endif
    
    int allsum = 0;
    for(int i = 0; i < field_height; i++) {
        for(int j = 0; j < field_width; j++) {
#if 0
            double weight_sum = cur_iteration[i * field_width + j].getWeight();
            if(!IS_BARRIER(cur_iteration[i * field_width + j]))
                allsum += int(weight_sum);

            int cells_count = 1;
            for(int k = 1; k <= sum_radius; k++) {
                if(addCell(i, j + k, &weight_sum)) cells_count++;
                if(addCell(i, j - k, &weight_sum)) cells_count++;

                for(int l = 0; l < 2 * sum_radius - k + 1; l++) {
                    if(addCell(i - k, j + sum_radius - l, &weight_sum)) cells_count++;
                    if(addCell(i + k, j - sum_radius + l, &weight_sum)) cells_count++;
                }
            }
            weight_sum /= cells_count;
            fwrite(&weight_sum, sizeof(double), 1, cur_it_file);
#else
            int li = max(0, i - sum_radius);
            int gi = min(field_height - 1, i + sum_radius);
            int lj = max(0, j - sum_radius);
            int gj = min(field_width - 1, j + sum_radius);

            double weight_avg = (psums[gi][gj] + psums[li][lj] - psums[li][gj] - psums[gi][lj])
                / (cell_count[gi][gj] + cell_count[li][lj] - cell_count[li][gj] - cell_count[gi][lj]);
            
            fwrite(&weight_avg, sizeof(double), 1, cur_it_file);
#endif
        }
    }
    fprintf(stderr, "sum = %d\n", allsum);

    fclose(cur_it_file);
}
