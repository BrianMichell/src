#include "../c/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024   

// Function to process input from a Unix pipe
void process_pipe(void) {
    char buffer[BUFFER_SIZE];

    // Read from stdin and process line-by-line
    while (fgets(buffer, sizeof(buffer), stdin)) {
        // Simulated processing: echo the input
        printf("Opening mdio file: %s", buffer);
    }

    // open a file
    sf_file mdio_file = sf_input(buffer);

    sf_describe_file(mdio_file);

    // Check for end-of-file (EOF) or error
    if (ferror(stdin)) {
        sf_file_error(1); // Error occurred
    }
}

int main() {
    process_pipe();  // Call function to process Unix pipe data
    return 0;
}
