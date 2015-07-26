
INC=-I $(MRUBY)/include -I ..
LIB=-L $(MRUBY)/build/host/lib -lmruby

MRUBYBIND_SRCDIR=..
MRUBYBIND_OBJ=mrubybind.o

SRCS=$(wildcard *.cc)
EXES=$(SRCS:%.cc=%)

CXX?= CXX
CXXFLAGS=-Wall -Wextra -Werror -g -std=c++11 -DEXIT_SUCCESS=0 -DEXIT_FAILURE=1

all:	$(EXES)

clean:
	rm -rf *.o $(EXES)

test:	$(EXES)
	./test.sh

%.o:	%.cc
	$(CXX) -c -o $@ $(INC) $(CXXFLAGS) $<

mrubybind.o:	$(MRUBYBIND_SRCDIR)/mrubybind.cc
	$(CXX) -c -o $@ $(INC) $(CXXFLAGS) $<

void:	void.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

int:	int.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

float:	float.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

string:	string.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

cptr:	cptr.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

class:	class.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

module:	module.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

wrong_type:	wrong_type.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

wrong_arg_num:	wrong_arg_num.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

class_value:	class_value.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

mruby_ref:	mruby_ref.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

many_bind:	many_bind.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

class_convert:	class_convert.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

reference:	reference.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

avoid_gc:	avoid_gc.o $(MRUBYBIND_OBJ)
	$(CXX) -o $@ $^ $(LIB) $(INC) $(CXXFLAGS)

#
