// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "get_Url.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <queue>
#include <vector>
#include <curl/curl.h>


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::HTTP_Server;

struct Priority_Queue
{
	string URL;
	int32_t size;
};

struct wd_in {
  size_t size;
  size_t len;
  char *data;
};

class get_UrlHandler : virtual public get_UrlIf
{
	
	public:
		url_Body URLMap;

		map<int32_t,string> URL;
		int32_t index;
		int32_t Cache_Hit;
		
		priority_queue<Priority_Queue,vector<Priority_Queue>,queue_Func> lsf_queue;
	
		get_UrlHandler()
		{
	    		// Your initialization goes here
			index=0;
			Cache_Hit=0;
	  	}
		int32_t get_MapSize()
		{
			int32_t tot_size=0;
			std::map<string,string>::iterator it;
			for (it=URLBody.url_Body.begin(); it!=URLBody.url_Body.end(); ++it)
			{
  			tot_size += it->second.size();
  			}
			cout<<"Returning Total Size of Map: "<<tot_size<<"\n";
  			return tot_size;  
		}
		void remove_fromCache(double remove_Size)
		{
			double free_Size=Cache_Size-get_MapSize();
			if(policy==1)
			{
				cout<< "FIFO Cache Replacment.\n";
				while(remove_Size>free_Size)
				{
				  	int32_t start;
				  	std::map<int32_t, string>::iterator it = URL.begin();
					start = it->first;  		
					it = URL.find(start);
					string keyURL = URL[start];
				  	URL.erase(start);
				  	URLBody.url_Body.erase(keyURL);
				  	cout << "URL to be evicted is : " << keyURL << endl;
					free_Size=Cache_Size-get_MapSize();
				}
				
			}
			if(policy==2)
			{
				cout<< "Random Cache Replacement.\n";
				while(remove_Size>free_Size)
				{
					  	int32_t start, end, randNum;
					  	std::map<int32_t, string>::iterator it = URL.begin();
					  	start = it->first;
					  	end = index;
					  	while(1)
						{
					  		srand(time(NULL));
					  		randNum = rand() % end + start; 
					  		it = URL.find(randNum);
					  		if(it != URL.end())
							{
					  			break;
							}
					  	}  	
					  	string keyURL = URL[randNum];
					  	URL.erase(randNum);
					  	URLBody.url_Body.erase(keyURL);
					  	cout << "URL to be evicted is : " << keyURL << endl;
						free_Size=Cache_Size-get_MapSize();
				}

			}
			if(policy==3)
			{
				cout<< "Largest Size First Cache Replacement.\n";
				while(remove_Size>free_Size)
				{
					Priority_Queue node = lsf_queue.top();
				 	cout << "URL to be evicted is : " << node.URL << "\n";
					URLBody.url_Body.erase(node.URL);
				  	lsf_queue.pop();
					free_Size=Cache_Size-get_MapSize();
				}

			}
		}
		void get(std::string& _return, const std::string& url, const int32_t policy)
		{
			// Your implementation goes here
			CURL *curl;
			CURLcode res;
			struct wd_in wdi;
			memset(&wdi, 0, sizeof(wdi));
			cout<<"Requested URL: "<<url<<"\n";
			switch(policy)
			{
				case 1:
					cout<<"FIFO Cache Replacment.\n"
				case 2:
					cout<<"Random Cache Replacment.\n"
				case 3:
					cout<<"Largest Size First Cache Replacment.\n"
				default:
					cout<<"Invalid Policy.\n"
			}
			
			map<string,string>::iterator it;
			it= URLBody.url_Body.find(url);

			if(it == URLBody.url_Body.end())
			{
				cout<<"No entry found in Map. Cache Miss";
				curl = curl_easy_init();

  				if(NULL != curl)
				{
					wdi.size = 1024;
					/* Check for malloc failure in real code. */
					wdi.data = malloc(wdi.size);

					/* Set the URL for the operation. */
					curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

					/* "write_data" function to call with returned data. */
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

					/* userp parameter passed to write_data. */
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wdi);

					/* Actually perform the query. */
					res = curl_easy_perform(curl);

				}
				else
				{
					fprintf(stderr, "Error: could not get CURL handle.\n");
				    	exit(EXIT_FAILURE);
				}


				if(wdi.size()>Cache_Size)
				{
					cout<<"Page Size is greater than Cache Size";
				}
				else
				{
					double tot_size = (get_MapSize() + wdi.size());
					if(tot_size> Cache_Size)
					{
						cout<<"Cache Limit exceeded. Replacement needs to be done.";
						remove_fromCache(wdi.size());
						URLBody.url_Body[URL]= wdi.data;
						if(policy ==1 || policy==2) //FIFO or Random
						{
							URL[index++]= url;
						}
						else if(policy==3)	//Largest Size
						{
							Priority_Queue node;
							node.URL= URL;
							string body = URLBody.url_Body[URL];
							node.size== body.size()
							cout<<"URL: "<<url<<"has size: "<<node.size()<<"\n";
							lsf._queue.push(node);
							cout<<"URL inserted in the Cache";
						}
					}
					else
					{
						cout<<"Space available in Cache. No replacement required.\n";
						URLBody.url_Body[URL]= wdi.data;
						if(policy==1 || policy==2)
						{
							URL[index++]=url;
							cout<<"Inserted into Map";
						}
						else if(policy==3)
						{
							Priority_Queue node;
							node.URL= URL;
							string body = URLBody.url_Body[URL];
							node.size== body.size()
							cout<<"URL: "<<url<<"has size: "<<node.size()<<"\n";
							lsf._queue.push(node);
							cout<<"URL inserted in the Cache";	
						}
					}
				}
				_return.result_code=1;
				_return.document= wdi.data;
				/* Clean up after ourselves. */
				curl_easy_cleanup(curl);
				/* Now wdi.data has the data from the GET and wdi.len is the length of the data available, so do whatever. */
				/* Write the content to stdout. */
				write(STDOUT_FILENO, wdi.data, wdi.len);
				/* cleanup wdi.data buffer. */
				free(wdi.data);			    
				cout << "No. of elements in cache: " << URLBody.url_Body.size() << "\n";
			    	cout << "Size of cache: " << get_MapSize() << "\n";
		            	cout << "Cache hits: " <<Cache_Hit <<"\n";
			}
			else
			{
				cout<<"Entry found in Cache. Cache Hit";
				_return.result_code=1;
				_return.document= URLBody.url_Body[url];
				Cache_Hit++;
				cout << "No. of elements in cache: " << URLBody.url_Body.size() << "\n";
			    	cout << "Size of cache: " << get_MapSize() << "\n";
		            	cout << "Cache hits: " <<Cache_Hit <<"\n";	
			}

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<get_UrlHandler> handler(new get_UrlHandler());
  shared_ptr<TProcessor> processor(new get_UrlProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
