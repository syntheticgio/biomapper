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


void cffi_test_bindings(int test) {
    std::cout << "Test: " << test << std::endl;
    return;
}


void cffi_test_string(char * t) {
    std::cout << "Test String: " << t << std::endl;
    return;
}

void cffi_test_vector(char ** t, int vcount) {
    for (int i = 0; i < vcount; i++) {
        std::cout << "String " << i << ": " << t[i] << std::endl;
    }
    
}

void cffi_test_bool(bool t) {
    std::cout << "The Bool is: " << t << std::endl;
    return;
}

int cffi_test_return_int() {
    return 5;
}
const char * cffi_test_return_string() {
    const char * h = "hello world";
    return h;
}
const char ** cffi_test_return_vector() {
    const char **c = (const char **)calloc (3,sizeof(char*));
    c[0] = "Vec 1";
    c[1] = "Vec 2";
    c[2] = "Vec 3";
    return c;
}

bool cffi_test_return_bool() {
    return true;
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

    extern void test_bindings(int test) 
    {
        return cffi_test_bindings(test);
    }

    extern void test_string(char * test) 
    {
        return cffi_test_string(test);
    }

    extern void test_vector(char ** test, int vcount) 
    {
        return cffi_test_vector(test, vcount);
    }

    extern void test_bool(bool test) 
    {
        return cffi_test_bool(test);
    }

    extern int test_return_int()
    {
        return cffi_test_return_int();
    }

    extern const char * test_return_string()
    {
        return cffi_test_return_string();
    }

    extern const char ** test_return_vector()
    {
        return cffi_test_return_vector();
    }

    extern bool test_return_bool()
    {
        return cffi_test_return_bool();
    }

}