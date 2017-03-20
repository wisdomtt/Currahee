/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
class RoborioHost
{
	RoborioHost();
	~RoborioHost();
	void error(const char* msg);
	int Run(int argc, char *argv[]);

};
