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
		cerr<<"Usage: "<<argv[0]<<" Cache-Policy(1:FIFO 2:Random 3:Largest Size First) URL-Access-Pattern(1:Linear Run-Through 2:Random Run-Through 3:Real-World Static Run-Through 4:Real-World Probabilistic Run-Through) Number-of-URLS"<<"\n";
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
	struct timeval start, end;
	double totalTime=0;
	int count;
	ifstream myfile ("URLS/Alexatop.txt");
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
				gettimeofday(&start, NULL);
				client.get(body, line,policy);
				gettimeofday(&end, NULL);
				totalTime=(end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec);
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
			vector<string> url_list;
	    	while ( getline (myfile,line))
	    	{
				url_list.push_back(line);
	    	}
			while(count<runs)
			{
				size_t index = rand_r(&randseed)%url_list.size();
				gettimeofday(&start, NULL);
				client.get(body,url_list[index],policy);
				gettimeofday(&end, NULL);
				totalTime=(end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec);
				cout<<"URL: "<<url_list[index]<<" Time Taken: "<<totalTime*1000<<" milliseconds.\n";
				count++;
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

	//Access policy replicating close to real-world usage (1 2 3 4 5 1 2 6 7 8 1 2 9 10 11 1 2 .......)
	else if(access==3)
	{
		int common, sparse, index;
		common = 0;
		sparse = 0;
		index = 0;

		if (myfile.is_open())
	  	{
			vector<string> url_list;
	    	while ( getline (myfile,line))
	    	{
				url_list.push_back(line);
	    	}
			while(count<runs)
			{
				//size_t index = rand_r(&randseed)%url_list.size();
				gettimeofday(&start, NULL);
				client.get(body,url_list[index],policy);
				gettimeofday(&end, NULL);
				totalTime += ((end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)/1000000.0));
				cout<<"URL: "<<url_list[index]<<" Time Taken: "<<totalTime<<" milliseconds.\n";
				count++;				

				if(count % 5 == 0) {
					common = 0;
					index = 0;
				}
				else {
					if (common == 0) {
						index = 1;
						common = 1;
					}
					else if (common == 1) {
						if (sparse < 1)
						{
							sparse += 2;
						}
						else {
							sparse += 1;
						}
						index = sparse;
						common = 2;
					}
					else {
						sparse += 1;
						index = sparse;
					}
				}

			}
			if(count==0)
				cout<<"No Input Provided.\n";
    		else
				cout<< "Average time to fetch page of the URLS is : "<< (totalTime/count*1.0)<<" milliseconds\n";
			myfile.close();
		}
		else
		{
			cout << "Unable to open file";
		}

	}

	//Access policy replicating close to real-world usage with randomness.
	//>60% of the time one of the first 10 urls (Frequently visited urls) in the url list will be accessed.
	//<40% of the time, a new page will be accessed.
	else if(access==4)
	{
		unsigned int randseed = time(NULL);
		size_t index;
		
		if (myfile.is_open())
	  	{
			vector<string> url_list;
	    	while ( getline (myfile,line))
	    	{
				url_list.push_back(line);
	    	}
			while(count<runs)
			{
				int coin = rand_r(&randseed)%100;
				if (coin <= 60) {
					index = rand_r(&randseed)%10;
				}
				else {
					index = rand_r(&randseed)%url_list.size();
				}				
				gettimeofday(&start, NULL);
				client.get(body,url_list[index],policy);
				gettimeofday(&end, NULL);
				totalTime += ((end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)/1000000.0));
				cout<<"URL: "<<url_list[index]<<" Time Taken: "<<totalTime*1000<<" milliseconds.\n";
				count++;				
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
	transport->close();
  	return 0;
}
