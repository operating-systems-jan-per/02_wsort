#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
 * Betriebssysteme - RUB 2024
 * Jan Herbrechter - Matr.Nr 108 011 239 576
 * Per Starke - Matr.Nr 108 024 11272 9
 */

#define MAX_WORD_LENGTH 100
#define INITIAL_CAPACITY 10

// Function prototypes
static int compareStrings(const void *a, const void *b);
static void sortWords(char **words, int count);
static void freeWords(char **words, int count);

int main() {
    char **words = NULL;
    char buffer[MAX_WORD_LENGTH + 2]; // +2 for newline and null terminator
    int capacity = INITIAL_CAPACITY;
    int count = 0;

    // Initialize word list
    words = malloc(capacity * sizeof(char*));
    if (!words) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    // Read words from stdin
    while (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer);

        // Handle overlong words
        if (buffer[len - 1] != '\n' && !feof(stdin)) {
            fprintf(stderr, "Word exceeds maximum length and will be ignored\n");
            // Skip rest of the overlong word
            while (fgets(buffer, sizeof(buffer), stdin) && buffer[strlen(buffer) - 1] != '\n');
            continue;
        }

        // Remove trailing newline if present
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            --len;
        }

        // Ignore empty lines
        if (len == 0) continue;

        // Check if more space is needed
        if (count == capacity) {
            capacity *= 2;
            char **new_words = realloc(words, capacity * sizeof(char*));
            if (!new_words) {
                fprintf(stderr, "Memory allocation failed\n");
                freeWords(words, count);
                return EXIT_FAILURE;
            }
            words = new_words;
        }

        // Store the word
        words[count] = strdup(buffer);
        if (!words[count]) {
            fprintf(stderr, "Memory allocation failed\n");
            freeWords(words, count);
            return EXIT_FAILURE;
        }
        count++;
    }

    // Sort and print words
    sortWords(words, count);
    for (int i = 0; i < count; i++) {
        puts(words[i]);
    }

    // Cleanup
    freeWords(words, count);
    return EXIT_SUCCESS;
}

// Compare two strings case-insensitively
static int compareStrings(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Sort words using qsort
static void sortWords(char **words, int count) {
    qsort(words, count, sizeof(char *), compareStrings);
}

// Free the memory used by words
static void freeWords(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}
