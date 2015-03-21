#!/usr/local/bin/thrift --gen cpp

namespace cpp HTTP_Server

struct HTTPServer
{
	1: map<string,string> url_Body
}

service get_Url {
	string get(1:string url,2:i32 policy)
}
