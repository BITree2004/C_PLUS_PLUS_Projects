#include "opencl_algorithm.h"
#include "omp_algorithm.h"
using namespace std;

void parse_args(int argc, char *argv[]) {
    --argc;
    if (argc == 1) {
        if (string(argv[1]) == "--help") {
            cout << "lab0.exe < --input file_name > \\\n"
                      << "< --output file_name > \\\n"
                      << "[ --device-type { dgpu | igpu | gpu | cpu | all } ]\n"
                      << "[ --device-index index ]\n"
                      << "[ --realization index ]\n";
            return;
        }
    }
    if (argc % 2 == 1) {
        throw runtime_error("Unsupported number of args, use --help!");
    }
    string input_file;
    string output_file;
    string type = "all";
    int index = 0;
    int realization = 0;
    for (int i = 1; i <= argc; i += 2) {
        string arg = argv[i];
        if (arg == "--input") {
            input_file = argv[i + 1];
        } else if (arg == "--output") {
            output_file = argv[i + 1];
        } else if (arg == "--device-type") {
            type = argv[i + 1];
        } else if (arg == "--device-index") {
            index = stoi(argv[i + 1]);
        } else if (arg == "--realization") {
            realization = stoi(argv[i + 1]);
        } else {
           throw runtime_error(format("Unsupported args, use --help! Token: {}.", arg));
        }
    }
    if (input_file.empty()) {
        throw runtime_error(format("Unsupported args, use --help! Not founded {}file!", "input"));
    }
    if (output_file.empty()) {
        throw runtime_error(format("Unsupported args, use --help! Not founded {}file!", "output"));
    }
    if (realization > 0) {
        opencl(input_file, output_file, type, index, realization).run();
    } else {
        omp(input_file, output_file, type, index, realization).run();
    }
}

signed main(int argc, char *argv[]) {
    try {
        parse_args(argc, argv);
    } catch (const exception &e) {
        cerr << "ERROR:!" << e.what() << endl;
        return 1;
    }
    return 0;
}
