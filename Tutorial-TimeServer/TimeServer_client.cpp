#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/TimeService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace TimeServer;

int main(int argc, char** argv)
{
	unsigned short port;
	if(argc < 3)
	{
		cout<<"Usage: "<<argv[0]<<" <ip-address> <port>"<<endl;
		return 0;
	}
	
	port = (unsigned short) strtol(argv[2],NULL,10);
	cout<<"Querying time from server "<<argv[1]<<':'<<port<<endl;
	boost::shared_ptr<TTransport> socket(new TSocket(argv[1], port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	TimeServiceClient client(protocol);
	TimeInfo ti;

	try
	{
    		transport->open();
		client.get_Time(ti);
		cout<<"Got Response: "<<ti.timestamp<<endl;
		transport->close();
	}
	catch (TException &tx)
	{
		cout<<"ERROR: "<<tx.what()<<endl;
		return 1;
	}

	return 0;
}
