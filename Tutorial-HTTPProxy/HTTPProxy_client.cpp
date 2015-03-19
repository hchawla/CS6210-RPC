#include "gen-cpp/HttpProxy.h"
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <string>
#include <time.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

using namespace std;

using namespace ::HTTP_Proxy;


int main(int argc, char **argv) {
  // Enter IP address of the proxy server here
  shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  HTTPProxy body;
  string line="www.google.com";
    printf("Fetching URL %s\n",line.c_str());
  struct timeval startTime, endTime;
  HttpProxyClient client(protocol);
  transport->open();
  gettimeofday(&startTime, NULL);
  client.get(body,line);
  printf("Result Code: %d\n",body.result_code);
  printf("%s",body.document.c_str());
  gettimeofday(&endTime, NULL);
  transport->close();
  return 0;
}
