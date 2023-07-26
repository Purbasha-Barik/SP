#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a line in the circular buffer
typedef struct {
    char* content;
} Line;

// Function to free the memory allocated for a line
void free_line(Line* line) {
    free(line->content);
}

// Function to print the last 'n' lines from the circular buffer
void print_last_n_lines(Line* buffer, int buffer_size, int num_lines) {
    int start_idx = 0;
    int count = 0;

    // Calculate the start index based on the number of lines to print
    if (num_lines > buffer_size)
        start_idx = (start_idx + buffer_size - num_lines) % buffer_size;

    // Print the last 'num_lines' lines
    for (int i = 0; i < num_lines; i++) {
        int current_idx = (start_idx + i) % buffer_size;
        printf("%s", buffer[current_idx].content);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_lines>\n", argv[0]);
        return 1;
    }

    int buffer_size = atoi(argv[1]);
    if (buffer_size <= 0) {
        printf("Invalid value for num_lines. Please provide a positive integer.\n");
        return 1;
    }

    // Allocate memory for the circular buffer
    Line* buffer = (Line*)malloc(buffer_size * sizeof(Line));
    if (buffer == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    // Initialize the buffer
    for (int i = 0; i < buffer_size; i++) {
        buffer[i].content = NULL;
    }

    char* line = NULL;
    size_t line_len = 0;
    ssize_t read_len;

    // Read lines from input and store them in the circular buffer
    while ((read_len = getline(&line, &line_len, stdin)) != -1) {
        int current_idx = (buffer_size + 1) % buffer_size; // Move the index to the next position

        // Free the memory of the oldest line before reusing the buffer position
        if (buffer[current_idx].content != NULL) {
            free_line(&buffer[current_idx]);
        }

        // Allocate memory for the new line and copy the content
        buffer[current_idx].content = (char*)malloc(read_len);
        strcpy(buffer[current_idx].content, line);
    }

    // Free memory for the last read line
    free(line);

    // Print the last 'buffer_size' lines from the circular buffer
    print_last_n_lines(buffer, buffer_size, buffer_size);

    // Free memory for the circular buffer
    for (int i = 0; i < buffer_size; i++) {
        if (buffer[i].content != NULL) {
            free_line(&buffer[i]);
        }
    }
    free(buffer);

    return 0;
}
