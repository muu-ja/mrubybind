
INC=-I $(MRUBY)/include -I ..
LIB=-L $(MRUBY)/build/host/lib -lmruby

all: a.out

clean:
	rm -rf *.o a.out

a.out:	main.cc ../mrubybind.cc ../mrubybind.h ../mrubybind_types.h ../mrubybind.inc ../mrubybind.dat
	g++ -std=c++11 -o a.out -Wall -Wextra main.cc ../mrubybind.cc $(INC) $(LIB)

