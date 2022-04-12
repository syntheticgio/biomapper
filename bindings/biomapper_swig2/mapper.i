%module mapper

//%begin %{
//#define SWIG_PYTHON_STRICT_BYTE_CHAR
//%}


%inline %{
#define SWIG_FILE_WITH_INIT
#include "mapper.hpp"
%}
// change "(const char* data, int len)" to match your functions declaration
//%include "typemaps.i"
//
//%apply (char *STRING, size_t LENGTH) { (char ** t, int vcount) }
%include "mapper.hpp"


