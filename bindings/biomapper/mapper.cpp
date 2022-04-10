#include "mapper.hpp"
#include <iostream>


#define DEBUG

MapperFile get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path) {
    MapperFile file = {join_index, start_range, end_range, zero_based, has_header, delimiter, file_path};
    #ifdef DEBUG
			std::cout << "Generating Mapper File. " << file.file_path << std::endl;
    #endif
    return file;
}

void test_2(int test) {
    std::cout << "Test: " << test << std::endl;
    return;
}


void test_bindings(int test) {
    test_2(test);
    return;
}

int main() {
    return 0;
}

extern "C"
{
    extern MapperFile cffi_get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path)
    {
        return get_mapper_file(join_index, start_range, end_range, zero_based, has_header, delimiter, file_path);
    }

    extern void cffi_test_bindings(int test) 
    {
        return test_bindings(test);
    }
}