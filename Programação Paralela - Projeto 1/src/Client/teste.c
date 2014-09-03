int createSockAndConnectToServer(int *sockfd, SERVER ip) {

	if (sockfd == NULL || ip.port < 0) {
		return 0;
	}

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (*sockfd < 0) {
		// Fail to create socket descriptor
		return -1;
	}

	printf("----------IP:%s\n\n\n",ip.address);
	struct hostent *server = gethostbyname(ip.address);

	if (server == NULL) {
		// Facil to get host. No such host!
		return -2;
	}

	struct sockaddr_in server_addr;
	memset((char*) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	memcpy((char*) server->h_addr_list[0],  			//SOURCE
			(char*) &server_addr.sin_addr.s_addr,	//DEST
			server->h_length						//SIZE :)
			);

	server_addr.sin_port = htons(ip.port);

	// try connect :D

	int r = connect(*sockfd, (struct sockaddr *) &server_addr,
			sizeof(server_addr));
	if (r < 0) {
		// Fail to connect :S
		return -3;
	}

	// All this worked. And probably the connection has successful. Thanks God!
	return 1;
}
