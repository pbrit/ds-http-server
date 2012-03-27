//main.c

#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <boost/scoped_array.hpp> 
#include <boost/scoped_ptr.hpp>


#include "http_parser.h"

#define HOST "127.0.0.1"
#define PORT 9003 

#define MAX_PORT_LENGTH 256
#define INPUT_BUFFER_LENGHT 1024*64

#define RESPONSE "HTTP/1.1 301 FOUND\nContent-Length: 0\nLocation: http://www.yandex.ru/\n\n"

#define SURE(result,line) 	if ( result == -1 ) 		\
							{ 							\
								perror(NULL);			\
								cout << line << endl;	\
								return EXIT_FAILURE;	\
							}							\

using namespace std;

int main ( int argc, char ** argv, char ** env )
{
	int server_sock;
	int status;

	//Resolv host name & initialize accept process
	{
		boost::scoped_array<char> port_str(new char[MAX_PORT_LENGTH]());

		addrinfo   ai_hints;
		addrinfo * ai_list = NULL;

		sprintf ( port_str.get(), "%d", PORT );

		memset ( &ai_hints, 0, sizeof ( ai_hints ) );

		ai_hints.ai_family = AF_UNSPEC;
        ai_hints.ai_socktype = SOCK_STREAM;

		SURE(getaddrinfo ( HOST, port_str.get(), &ai_hints, &ai_list ), __LINE__);

		for ( addrinfo * ai_ptr = ai_list; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next )
		{

			server_sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);

			SURE(server_sock, __LINE__);

			status = bind ( server_sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen);

			SURE(status, __LINE__);

			status = listen ( server_sock, 1024 );

			SURE(status, __LINE__);
		}
	}

	//Initialize parser
	{
		cout << "Start accept" << endl;
	
		boost::scoped_ptr<http_parser_settings> settings(new http_parser_settings());
		boost::scoped_ptr<http_parser> parser(new http_parser());
		boost::scoped_array<char> buffer(new char[INPUT_BUFFER_LENGHT]);

		http_parser_init(parser.get(), HTTP_REQUEST);

		//Start working with client
		for (;;)
		{
			socklen_t socklen;
			sockaddr sock_addr;
			
			int client_sock = accept ( server_sock, &sock_addr, &socklen );

			SURE(client_sock,__LINE__);

			int recved = recv (client_sock, buffer.get(), INPUT_BUFFER_LENGHT, 0);

			SURE(recved, __LINE__);

			int nparsed = http_parser_execute(parser.get(), settings.get(), buffer.get(), recved);

			status = send ( client_sock, (void *) RESPONSE, sizeof(RESPONSE), 0);

			SURE(status, __LINE__);

			close(client_sock);

			cout << parser->http_major << endl;

		}

	}

}

