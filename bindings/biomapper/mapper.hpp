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

void cffi_test_bindings(int test);
void cffi_test_string(char * t);
void cffi_test_vector(char ** t, int vcount);
void cffi_test_bool(bool t);

int cffi_test_return_int();
const char * cffi_test_return_string();
const char ** cffi_test_return_vector();
bool cffi_test_return_bool();




// MapperFile cffi_get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path);