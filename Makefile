BUILD_DEBUG=yes
CXX = g++

LDLIBS := -ldl\

#------------------module---------------------------
INC_DIR =  -I./core/


CFLAGS := $(INC_DIR) -Wall -fPIC 

ifeq ($(BUILD_DEBUG), yes)
CFLAGS += -g -O2
endif

CXXFLAGS := $(CFLAGS) -std=c++11 -lpthread

LIB_SRCS_CXX=$(wildcard ./core/*.cpp) 

LIB_DSTS := $(patsubst ./core%.cpp, ./core%.o, $(LIB_SRCS_CXX)) 


CXX_VALUE = 1

ifeq ($(CXX_VALUE), 1)
$(warning $(CXX_VALUE))
CXXFLAGS+= -DCXX_VALUE
endif

all:main

main: main.cpp $(LIB_DSTS)
	@echo "Compiling " $@
	$(CXX) $^ $(CXXFLAGS) $(LDLIBS)  -o $@


clean:
	@echo "Cleaning"
	@rm -f  $(LIB_DSTS)

