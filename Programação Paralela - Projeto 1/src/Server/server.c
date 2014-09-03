/* High Performance Computing Project
 * Class from undergraduate and graduate.
 *
 * Teacher:Dr. Aleardo Manacero Jr.
 * Student:Gabriel Henrique Martinez Saraiva
 *
 * Description:
 * This is a server program. It will receive work from a client, process and
 * send back the results, using sockets.
 *
 * Receive: Receive a vector of integers
 * Send: Blocks of a vector sorted using BubbleSort
 */

// = LIBRARIES =================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../Common/common.h"
#include <pthread.h>
#include <time.h>

// =============================================================================

// = DEFINES ===================================================================
#define MAX_LENGHT_MESSAGE_SOCKET (64*1024)
// =============================================================================

// = GLOBALS ===================================================================
int BUBBLING = 0; // Used to show progress on BubbleSort. It's XGH? Maybe not :)
// =============================================================================

// = HEADERS ===================================================================
void bubbleSort(int vector[], int size);
int initializeServer(int port);
int acceptNewClient(int serverSocket);
int getVectorSize(int clientSocketfd);
int handleClient(int clientSocketfd);
// =============================================================================

// = BUBBLE SORT ALGORITHM =====================================================
void bubbling(void *t) {

	int i = 0;
	float total = (float) *((int*) t); // Yes... I know... Don't need this. BIW.

	struct timespec st, st2;
	st.tv_sec = 0;
	st.tv_nsec = 250000000;

	while (BUBBLING) {

		float p = 100 - BUBBLING / total * 100.0;

		PRINTFF("\rSorting: %3.2f%% %c\b", p, palitoGirante(i++));
		nanosleep(&st, &st2); // u in usleep is microseconds! O_O

	}

	PRINTFF("Sorting: 100.00%% :)\n");

}
void bubbleSort(int vector[], int size) {

	if (size == 1) {
		PRINTFF("Sorting: 100.00%% :)\n");
		// HUehurhurhuehruhruehruehruh
		return;
	}

	int aux;
	int nonstop = 1;

	BUBBLING = 1;
	pthread_t bubbler;
	pthread_attr_t bubblerAtt;

	// Launch BUBBLER!
	pthread_attr_init(&bubblerAtt);
	pthread_attr_setscope(&bubblerAtt, PTHREAD_SCOPE_PROCESS);
	pthread_create(&bubbler, &bubblerAtt, (void*) &bubbling, (void*) &size);
	// BLUUUUUBLLLLERRR  | / - \ | / - \ | / - \ | / - \ | / - \ | / - \ | ...

	for (int i = size; nonstop; i--) {

		nonstop = 0;
		BUBBLING = i; // This tell to Bubbler how much is already done!

		for (int j = 0; j < i; j++) {
			if (vector[j] > vector[j + 1]) {
				aux = vector[j + 1];
				vector[j + 1] = vector[j];
				vector[j] = aux;
				nonstop = 1;
			}

		}

	}

	BUBBLING = 0;

	pthread_join(bubbler, NULL);

	PRINTFF("OK!");

}
// =============================================================================

// = INITIALIZE SERVER =========================================================
/* This make all the process of create and bind sockets and all the things to
 * start the server. Then, return the serverSocket fd;
 */
int initializeServer(int port) {

	int serverSockfd;
	struct sockaddr_in myAddress;

	serverSockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSockfd < 0) {
		EPRINTFF("Error! Unable to create server socket!\n");
		return -1;
	}

	memset((char*) &myAddress, 0, sizeof(myAddress));

	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = INADDR_ANY;
	myAddress.sin_port = htons(port);

	int b = bind(serverSockfd, (struct sockaddr*) &myAddress,
			sizeof(myAddress));

	if (b < 0) {
		EPRINTFF("Error! Unable to bind server socket to port %d!\n", port);
		return -2;
	}

	return serverSockfd;

}
// =============================================================================

int isSorted(int vector[], int size) {
	size--;
	for (int i = 0; i < size; i++) {
		if (vector[i] > vector[i + 1]) {
			return 0;
		}
	}

	return 1;
}

// = ACCEPT NEW CLIENTS AND RETURN SOCKET OF CONNECTION ========================
int acceptNewClient(int serverSocket) {

	int clientSockfd = -1;
	struct sockaddr_in clientAddress;
	socklen_t clientLen;
	clientLen = sizeof(clientAddress);

	clientSockfd = accept(serverSocket, (struct sockaddr *) &clientAddress,
			&clientLen);

	if (clientSockfd < 0) {

		return -1;
	}

	return clientSockfd;

}

int getVectorSize(int clientSocketfd) {
	int vectorSize = -1;
	int sizeRead = recv(clientSocketfd, &vectorSize, sizeof(int), 0);
	if (sizeRead != (sizeof(int))) {
		EPRINTFF("Error! Unable to get size of vector from client!\n");
		return -1;
	}

	if (vectorSize < 1) {
		EPRINTFF("Incorrect value of vector size.\n");
		return -2;
	}

	return vectorSize;

}

// =============================================================================

// = Talk with client and negotiate value of service :) = LOL ==================
/* Receive service, do it, and them, send result back to client */
int handleClient(int clientSocketfd) {

	int vectorSize = getVectorSize(clientSocketfd);
	if (vectorSize < 0) {
		EPRINTFF("Problem to get vector size!\n");
		return -1;
	}

	int *vector = (int*) malloc(sizeof(int) * vectorSize);

	if (vector == NULL) {
		EPRINTFF("Problem to allocate memory for this vector.\n");
		return -2;
	}

	// = Read vector from client ===============================================

	int received = transferDataBlocks(TRANSMIT_DATA_RECV, clientSocketfd,
			vector, vectorSize);

	if (received != vectorSize) {

		EPRINTFF("Problem to receive vector.\n");

		free(vector);
		return -3;
	}
	// =========================================================================

	bubbleSort(vector, vectorSize);

	// = Send back result to client ============================================
	int sended = transferDataBlocks(TRANSMIT_DATA_SEND, clientSocketfd, vector,
			vectorSize);

	if (sended != vectorSize) {
		EPRINTFF("Problem to send vector sorted to client.\n");

		free(vector);
		return -5;
	}
	// =========================================================================

	free(vector);
	return 1; // :)

}
// =============================================================================

int main(int argc, char *argv[]) {

	clearScreen();
	PRINTFF("\n\n");

	// = CHECK INPUT PARAMETERS ================================================
	if (argc != 2) {

		EPRINTFF("Error! How I can wait for clients if you don't tell ");
		EPRINTFF("me what port should I listen?\n");
		EPRINTFF("Use: %s [PORT]\n", argv[0]);

		return -1;
	}

	int port = atoi(argv[1]);
	if (port < 1024) {
		EPRINTFF("Sorry pal! I can't use this port! Choose other...\n");
		return -2;
	}

	// =========================================================================

	PRINTFF(" ======= Bubble-Sorter Server =======\n");
	PRINTFF("  We have the fastest bubble sorting\n\n");

	PRINTFF("Initializing server...\n");
	int serverSocketfd = initializeServer(port);

	if (serverSocketfd < 0) {
		return -2;
	}

	PRINTFF("Server successfull initialized on port %d\n", port);

	if (listen(serverSocketfd, 1) == -1) {
		EPRINTFF(
				"Error! Something gets wrong when start listening for clients!\n");
		return -3;
	}

	int servedClients = 0;

	while (1) { // Still waiting clients for ever :)

		PRINTFF("\nWaiting for a new client...\n");

		int clientSockfd = acceptNewClient(serverSocketfd);

		if (clientSockfd < 1) {
			EPRINTFF(
					"Error! Something gets wrong while accepting a new client.\n");
			continue;
		}

		clearScreen();

		servedClients++;
		PRINTFF("New client arrive! It is our %dº client!\n", servedClients);

		int haveWork = 1; // Server will continue until client has no more works.
		int jobs = 1;

		while (haveWork) {

			int wa = recv(clientSockfd, &haveWork, sizeof(int), 0);

			if (wa != sizeof(int)) {
				EPRINTFF("Error! Client don't aswer for HaveWork!\n");
				break;
			}

			if (haveWork == 0) {
				PRINTFF("\nNo more jobs from this client!\n");
				break;
			}

			PRINTFF("\nClient %d - Job %d\n", servedClients, jobs);
			int ret = handleClient(clientSockfd);

			if (ret < 0) {
				EPRINTFF("Aborting connection now! (%d)\n", ret);
				break;
			}

			jobs++;

		}

		PRINTFF("Client disconnected!\n");
		close(clientSockfd);
	}

}

