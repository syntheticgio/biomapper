all: mapper

clean:
	rm -f *.o *.so *.html _mapper.c mapper.h.preprocessed

libmapper.so: mapper.o
	g++ -shared $^ -o $@

%.o: %.cpp
	g++ -c -Wall -Werror -fPIC $^

mapper: export LD_LIBRARY_PATH = $(shell pwd)
mapper: libmapper.so
	./build_mapper.py

