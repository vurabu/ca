#include "visualizer.h"

string name_suffix;

#if 0
const int image_width  = 300;
const int image_height = 200;
#else
const int image_height = field_height;
const int image_width  = field_width;
#endif

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

    for(int i = 1; i < field_height; i++)
        for(int j = 1; j < field_width; j++) {
            bool is_normal_cell = !IS_BARRIER(cur_iteration[i * field_width + j]);
            int cell_weight = cur_iteration[i * field_width + j].getWeight();

            psums[i][j] = psums[i - 1][j] + psums[i][j - 1] - psums[i - 1][j - 1]
                + (is_normal_cell ? cell_weight : 0);

            cell_count[i][j] = cell_count[i - 1][j] + cell_count[i][j - 1] - cell_count[i - 1][j - 1]
                + 1; // + (is_normal_cell ? 1 : 0);
        }

    for(int ii = 0; ii < image_height; ii++) {
        for(int jj = 0; jj < image_width; jj++) {
            int i = ii * field_height / image_height;
            int j = jj * field_width  / image_width;

            int li = max(0, i - sum_radius);
            int gi = min(field_height - 1, i + sum_radius);
            int lj = max(0, j - sum_radius);
            int gj = min(field_width - 1, j + sum_radius);

            double weight_avg = double(psums[gi][gj] + psums[li][lj] - psums[li][gj] - psums[gi][lj])
                / (cell_count[gi][gj] + cell_count[li][lj] - cell_count[li][gj] - cell_count[gi][lj]);

            assert(fwrite(&weight_avg, sizeof(double), 1, cur_it_file)); // assert? WTF???
        }
    }

    fclose(cur_it_file);
}
