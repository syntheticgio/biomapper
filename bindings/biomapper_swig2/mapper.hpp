//#include <stdbool.h>
#include <string>
#include <iostream>

//typedef struct {
//    int join_index;
//    long long int start_range;
//    long long int end_range;
//    bool zero_based;
//    bool has_header;
//    char delimiter;
//    char * file_path;
//} MapperFile;

//https://www.swig.org/Doc4.0/Python.html#Python_nn18
class MapperFile {
public:
	MapperFile(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, const char * file_path)
		: join_index_(join_index), start_range_(start_range), end_range_(end_range), zero_based_(zero_based), has_header_(has_header), delimiter_(delimiter), file_path_(file_path) {
		std::cout << "FP: " << file_path << std::endl;
		std::cout << "FP_: " << file_path_ << std::endl;
//		FP_ = file_path;
	}
	~MapperFile() {}

	void print_info();

private:
	// Member Variables
	int join_index_;
    long long int start_range_;
    long long int end_range_;
    bool zero_based_;
    bool has_header_;
    char delimiter_;
	std::string file_path_;

};


//MapperFile get_mapper_file(int join_index, long long int start_range, long long int end_range, bool zero_based, bool has_header, char delimiter, char * file_path);

void test_bindings(int test);
void test_string(char * t);
void test_vector(char ** t, int vcount);
void test_bool(bool t);

int test_return_int();
const char * test_return_string();
const char ** test_return_vector();
bool test_return_bool();