GEN_SRC := TimeService_server.skeleton.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))

THRIFT_DIR := /usr/include/thrift
BOOST_DIR := /usr/include/boost

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR)

.PHONY: all

all: TimeService_server.skeleton

%.o: %.cpp
	$(CXX) -Wall -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H $(INC) -c $< -o $@

TimeService_server.skeleton: TimeService_server.skeleton.o $(GEN_OBJ)
	$(CXX) $^ -o $@ -L/usr/local/lib -lthrift -g
#http://wiki.apache.org/thrift/ThriftUsageC%2B%2B
