#include "model.h"

int main(int argc, char *argv[]) {
    string name_suffix = "";
    if(argc > 1) {
         name_suffix = string(argv[1]);
    }

    cerr << "using suffix \"" << name_suffix << "\"" << endl;

    init(name_suffix);
    run();
    return 0;
}
