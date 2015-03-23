#include "gen-cpp/get_Url.h"
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;
using namespace std;

using namespace ::HTTP_Server;


int main(int argc, char **argv)
{
    	if (argc < 3)
	{
		// Tell the user how to run the program
		cerr<<"Usage: "<<argv[0]<<" Cache-Policy(1:FIFO 2:Random 3:Largest Size First) URL-Access-Pattern(1:Linear Run-Through 2:Random Run-Through) Number-of-URLS"<<"\n";
		/* "Usage messages" are a conventional way of telling the user
		 * how to run a program if they enter the command incorrectly.
		 */
		return 1;
    	}
	int policy=atoi(argv[1]);
	int access=atoi(argv[2]);
	int runs=atoi(argv[3]);
	shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  	shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	string body,line;
	struct timeval startTime, endTime;
	double totalTime=0;
	int count;
	ifstream myfile ("random_list.txt");
	get_UrlClient client(protocol);
	transport->open();
	if(access==1)
	{
		count=0;
		if (myfile.is_open())
	  	{
	    		while ( getline (myfile,line) &&count<runs)
	    		{
				count++;
				gettimeofday(&startTime, NULL);
				client.get(body, line,policy);
				gettimeofday(&endTime, NULL);
				totalTime += ((endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec)/1000000.0));
				cout<<"URL: "<<line<<" Time Taken: "<<totalTime*1000<<" milliseconds.\n";
	    		}
	    		if(count==0)
				cout<<"No Input Provided.\n";
	    		else
				cout<< "Average time to fetch page of the URLS is : "<< (totalTime/count*1.0)<<" seconds\n";
	    		myfile.close();
	  	}
	  	else
		{
			cout << "Unable to open file";
		}
	}
	else if(access==2)
	{
		unsigned int randseed = time(NULL);
		if (myfile.is_open())
	  	{
			std::vector<std::string> url_list;
		    	while ( getline (myfile,line))
		    	{
				url_list.push_back(line);
		    	}
			while(count<runs)
			{
				size_t index = rand_r(&randseed)%url_list.size();
				gettimeofday(&startTime, NULL);
				client.get(body,url_list[index],policy);
				gettimeofday(&endTime, NULL);
				totalTime += ((endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec - startTime.tv_usec)/1000000.0));
				cout<<"URL: "<<url_list[index]<<" Time Taken: "<<totalTime*1000<<" milliseconds.\n";
				count++;
			}
			if(count==0)
				cout<<"No Input Provided.\n";
	    		else
				cout<< "Average time to fetch page of the URLS is : "<< (totalTime/count*1.0)<<" seconds\n";
		}
    		myfile.close();
	}
	transport->close();
  	return 0;
}
