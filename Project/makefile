THRIFT_DIR = usr/include/thrift
INCS_DIRS  =-I/usr/include/thrift
LIBS_DIRS  =-L/usr/local/lib
CPP_DEFS   =-DHAVE_NETINT_IN_H -DHAVE_INITTYPES_H
CPP_OPTS   =-Wall -O2 -fpermissive
LIBS       =-lthrift -lcurl

GEN_SRC    = gen-cpp/get_Url.cpp  \
             gen-cpp/ProxyServer_types.cpp   \
             gen-cpp/ProxyServer_constants.cpp \
             gen-cpp/HTTPServer.cpp

GEN_OBJ    = gen-cpp/get_Url.o  \
             gen-cpp/types.o   \
             gen-cpp/constants.o

default: server client

server: gen-cpp/HTTPServer.cpp
	g++ ${CPP_OPTS} ${CPP_DEFS} -o gen-cpp/HTTPServer ${INCS_DIRS} ${GEN_SRC} ${LIBS_DIRS} ${LIBS}
	g++ -Wall ${INCS_DIRS} -c gen-cpp/ProxyServer_constants.cpp -o gen-cpp/constants.o
	g++ -Wall ${INCS_DIRS} -c gen-cpp/ProxyServer_types.cpp -o gen-cpp/types.o
	g++ -Wall ${INCS_DIRS} -c gen-cpp/get_Url.cpp -o gen-cpp/get_Url.o

client: HTTP_Client.cpp
	g++ -Wall -o HTTP_Client.o ${INCS_DIRS} -c HTTP_Client.cpp
	g++ ${LIBS_DIRS} HTTP_Client.o ${GEN_OBJ} -o HTTP_Client ${LIBS}
