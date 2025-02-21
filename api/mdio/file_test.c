#include "sf_file.h"

int main() {
    // Call sf_file_error with true to simulate an error
    sf_file_error(1);

    // This line should never execute if sf_file_error() exits on error
    return 0;
}
