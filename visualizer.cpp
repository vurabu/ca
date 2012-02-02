#include "visualizer.h"

string name_suffix;

#if 1
const int image_height = 400;
const int image_width  = 400;
#else
const int image_height = field_height;
const int image_width  = field_width;
#endif

const int rowoffset = field_height / image_height + 2;
const int coloffset = field_width  / image_width  + 2;


void saveVisualizationConfig(const string& new_name_suffix) {
    name_suffix = new_name_suffix;
    ofstream config_file("last_run.cfg");
    if(!config_file.is_open()) {
        cerr << "Can't open last_run.cfg" << endl;
        return ;
    }
    config_file << "FIELD_HEIGHT=" << image_height << endl;
    config_file << "FIELD_WIDTH=" << image_width << endl;
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

    sprintf(filename, "%06lu%s.bin", cur_it, name_suffix.c_str());
    assert(cur_it_file = fopen(filename, "w"));

    int psums[field_height][field_width];
    int cell_count[field_height][field_width];

    memset(psums, 0, sizeof(psums));
    memset(cell_count, 0, sizeof(cell_count));

    for(int i = 1; i < field_height ; i++)
        for(int j = 1; j < field_width ; j++) {
            bool is_normal_cell = !IS_BARRIER(cur_iteration[i * field_width + j]);
            int cell_weight = cur_iteration[i * field_width + j].getWeight();

            psums[i][j] = psums[i - 1][j] + psums[i][j - 1] - psums[i - 1][j - 1]
                + (is_normal_cell ? cell_weight : 0);

            cell_count[i][j] = cell_count[i - 1][j] + cell_count[i][j - 1] - cell_count[i - 1][j - 1]
                + (is_normal_cell ? 1 : 0);
        }

    //int allsum = cell_count[field_height - 1][field_width - 1];
#if 1
    for(int i = 0; i < image_height; i++) {
        for(int j = 0; j < image_width; j++) {
            int row_start =   i   * field_height / image_height - sum_radius;
            int row_end   = (i+1) * field_height / image_height + sum_radius;
            int col_start =   j   * field_width  / image_width  - sum_radius;
            int col_end   = (j+1) * field_width  / image_width  + sum_radius;

            int li = max(0, row_start);
            int gi = min(field_height - 1, row_end);
            int lj = max(0, j - col_start);
            int gj = min(field_width - 1, j + col_end);

            double weight_sum = psums[gi][gj] + psums[li][lj] - psums[li][gj] - psums[gi][lj];
            int weight_count  = cell_count[gi][gj] + cell_count[li][lj] - cell_count[li][gj] - cell_count[gi][lj];

            if(weight_count)
                weight_sum /= weight_count;
            else
                weight_sum = 0;

            fwrite(&weight_sum, sizeof(double), 1, cur_it_file);
        }
    }
#else
    for(int i = 0; i < field_height; i++) {
        for(int j = 0; j < field_width; j++) {
            int li = max(0, i - sum_radius);
            int gi = min(field_height - 1, i + sum_radius);
            int lj = max(0, j - sum_radius);
            int gj = min(field_width - 1, j + sum_radius);

            double weight_sum = psums[gi][gj] + psums[li][lj] - psums[li][gj] - psums[gi][lj];
            int weight_count  = cell_count[gi][gj] + cell_count[li][lj] - cell_count[li][gj] - cell_count[gi][lj];

            if(weight_count)
                weight_sum /= weight_count;
            else
                weight_sum = 0;

            fwrite(&weight_sum, sizeof(double), 1, cur_it_file);
        }
    }
#endif
    //fprintf(stderr, "sum = %d\n", allsum);
    //cerr << "count = " << cell_count[field_height-1][field_width-1] << endl;

    fclose(cur_it_file);
}
