#include <chrono>

using namespace std;

static chrono::time_point<chrono::high_resolution_clock> start;
static chrono::time_point<chrono::high_resolution_clock> local_end;

void begin() {
    start = chrono::high_resolution_clock::now();
}

auto finish() {
    local_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(local_end - start).count();
}
