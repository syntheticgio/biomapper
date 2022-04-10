#include <stdbool.h>

typedef struct {
    int join_index;
    long long int start_range;
    long long int end_range;
    bool zero_based;
    bool has_header;
    char delimiter;
    char * file_path;
} MapperFile;

MapperFile get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path);

void test_bindings(int test);

// MapperFile cffi_get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path);