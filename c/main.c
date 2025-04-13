#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Function to perform sequential access
double sequential_access(int size)
{
    clock_t start, end;

    // Memory allocation
    char *array = malloc(size);
    int *sequential_indices = malloc(size * sizeof(int)); // Array for sequential indices

    // Pre-generate sequential indices
    for (int i = 0; i < size; i++)
    {
        sequential_indices[i] = i;
    }

    // Sequential access processing
    start = clock();
    for (int i = 0; i < size; i++)
    {
        array[sequential_indices[i]] = i % 256; // Use pre-generated sequential indices
    }
    end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sequential access: %f sec (Memory size: %d MB)\n", execution_time, size / (1024 * 1024));

    // Free memory
    free(array);
    free(sequential_indices);

    return execution_time;
}

// Function to perform random access
double random_access(int size)
{
    clock_t start, end;

    // Memory allocation
    char *array = malloc(size);
    int *random_indices = malloc(size * sizeof(int)); // Array for random indices

    // Pre-generate random indices
    for (int i = 0; i < size; i++)
    {
        random_indices[i] = rand() % size;
    }

    // Random access processing
    start = clock();
    for (int i = 0; i < size; i++)
    {
        array[random_indices[i]] = i % 256; // Use pre-generated random indices
    }
    end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Random access: %f sec (Memory size: %d MB)\n", execution_time, size / (1024 * 1024));

    // Free memory
    free(array);
    free(random_indices);

    return execution_time;
}

int main()
{
    // Default memory size
    int size = 1024 * 1024 * 12;
    const int iterations = 10; // Number of iterations

    printf("=== Sequential Access ===\n");
    double sequential_sum = 0.0;
    for (int i = 0; i < iterations; i++)
    {
        printf("Run %d: ", i + 1);
        sequential_sum += sequential_access(size);
    }
    printf("Average execution time: %f sec\n\n", sequential_sum / iterations);

    printf("=== Random Access ===\n");
    double random_sum = 0.0;
    for (int i = 0; i < iterations; i++)
    {
        printf("Run %d: ", i + 1);
        random_sum += random_access(size);
    }
    printf("Average execution time: %f sec\n", random_sum / iterations);

    return 0;
}