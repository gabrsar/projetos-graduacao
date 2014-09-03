/* High Performance Computing Project
 * Class from undergraduate and graduate.
 *
 * Teacher:Dr. Aleardo Manacero Jr.
 * Student:Gabriel Henrique Martinez Saraiva
 *
 * Description:
 * This is a multi-client program, that will connect to several servers
 * via socket and send blocks of job.
 * Then receive the result from servers and use it.
 *
 * Send: Blocks of a vector (randomized).
 * Receive: Same block, but sorted.
 *
 */

// = LIBRARIES =================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "../Common/common.h"
#include "mergesort.h"
#include <unistd.h>
// =============================================================================

// = DEFINITIONS ===============================================================
#define DEFAULT_FILE_IP "./hosts.dat"
#define DEFAULT_FILE_RESULTS "./ordenado.txt"
#define DEFAULT_SERVERS_MAX 32
#define DEFAULT_SPLIT_SEGMENTS 32
#define SIZE_IP 16
#define TRANSFER_DATA_SEND 1
#define TRANSFER_DATA_RECV 2
#define MAX_RANDON 100
// =============================================================================

// = PROFILER ==================================================================
FILE *PROFILE;
double *BUBBLE_COMM_TIME;
// =============================================================================

// = GLOBAL - I will be carefull. I promiss ====================================
sem_t SEM_WORKERS;
sem_t SEM_SCREEN;
int MERGIN = 0; // It's Magic! -No, It's XGH. -But..., ok. It's XGH. :(
// =============================================================================

// = MY TYPES ==================================================================
typedef struct {
	int socket;
	int id;
	int *vector;
	int vectorSize;
	int *workers;
	int job;
} ThreadsParam;

/* Estrutura que representa um IP*/
typedef struct {
	char address[SIZE_IP];
	int port;
} SERVER;
// =============================================================================

// = FUNCTION HEADERS ==========================================================
void help();
int readServersList(SERVER ips[]);
void createRandomVector(int vector[], int vectorSize);
void noSlavesForYou();
void mergeSort(int vector[], int vectorSize);
int isSorted(int vector[], int size);
int recordResults(int vector[], int size);
void stopMagic();
void startMagic();
// =============================================================================

/* Open a file and read IPs from file, one by line.
 * IPs shoud be in octets format like: 192.168.0.1
 */
int readServersList(SERVER ips[]) {

	printf("    Reading file '%s': ", DEFAULT_FILE_IP);

	FILE *f = fopen(DEFAULT_FILE_IP, "rb");
	printf("OK!\n");

	if (!f) {
		EPRINTFF("Error! Cannot open file %s for reading.\n", DEFAULT_FILE_IP);
		return -1;
	}

	int c = 0;
	for (int i = 0; i < DEFAULT_SERVERS_MAX; i++) {
		if (feof(f)) {
			break;
		}
		fscanf(f, "%15[^:]:%d\n", ips[c].address, &(ips[c].port));
		ips[c].address[SIZE_IP - 1] = '\0';
		c++;

	}

	fclose(f);
	return c;
}

void createRandomVector(int vector[], int vectorSize) {

	int try = 0;

	do {

		for (int i = 0; i < vectorSize; i++) {
			vector[i] = rand() % MAX_RANDON;
		}

		if (vectorSize > 1 && isSorted(vector, vectorSize)) {
			PRINTFF("Trying again... ");
			try++;
		} else {
			try = 5;
		}

	} while (try < 5);

}

int recordResults(int vector[], int size) {

	FILE *f = fopen(DEFAULT_FILE_RESULTS, "w");

	if (!f) {
		if (size > 400000) {
			EPRINTFF("...\n");
			sleep(1);
			EPRINTFF("Are you kidding me?\n");
			sleep(1);
			EPRINTFF("HOW I CAN WRITE IN FILE IF I CANNOT OPEN IT??? ¬¬\n\n");
			sleep(1);
			EPRINTFF("I've Sorted a %d elements vector for NOTHING????\n", size);
			sleep(1);
			EPRINTFF("NEXT TIME I WILL WRITE IT OVER MBR!\n\n");
			sleep(2);
			system("eject");
			sleep(2);
			EPRINTFF("¬¬ have a nice day ...\n\n");

		} else {
			EPRINTFF("HOW I CAN WRITE IN FILE IF I CANNOT OPEN IT??? ¬¬\n\n");
			EPRINTFF("At least it is a small vector...\n");
		}

		return 0;
	}

	for (int i = 0; i < size - 1; i++) {
		if (i % 10 == 0) {
			fprintf(f, "\n");
		}

		fprintf(f, "%d\t", vector[i]);

	}

	fprintf(f, "%d", vector[size - 1]);

	fclose(f);

	return 1;

}

int createSockAndConnectToServer(int *sockfd, SERVER ip) {

	if (sockfd == NULL || ip.port < 0) {
		return 0;
	}

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (*sockfd < 0) {
		// Fail to create socket descriptor
		return -1;
	}

	struct hostent *server = gethostbyname(ip.address);

	if (server == NULL) {
		// Facil to get host. No such host!
		return -2;
	}

	struct sockaddr_in server_addr;
	memset((char*) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	memcpy(
			(char*) &server_addr.sin_addr.s_addr,	//DEST
			(char*) server->h_addr_list[0],  			//SOURCE
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

void help(char *argv0) {
	printf("\n\n");
	printf("Usage: %s [VECTOR SIZE]\n", argv0);
	printf("[VECTOR SIZE] should be a integer number grater than 0\n");
}

int isSorted(int vector[], int size) {

	size--;
	for (int i = 0; i < size; i++) {
		if (vector[i] > vector[i + 1]) {

			return 0;
		}
	}

	return 1;
}

void printStatus(int w, int job, char status) {

	// FIXME FOR FUTURE!
	// Next time use a vector and store data about tasks and threads states
	// Then just print vector.  DUMB!

	sem_wait(&SEM_SCREEN);

	PRINTFF("\r    Worker: ");
	MVF(job);
	PRINTFF("%d\b", w);
	MVD(1);
	PRINTFF("%c\b", status);
	MVU(1);

	sem_post(&SEM_SCREEN);

}

void run(void *parameters) {

	TIME timeCommStart,timeCommEnd;
	double timeComm=0;

	ThreadsParam job = *(ThreadsParam*) parameters;

	// Remove worker from pool
	job.workers[job.id] = 0;

	// Send vector size to server

	int yesIhaveWork = 1;

	printStatus(job.id, job.job, 'A');
	timeCommStart=gettime();
		int hw = send(job.socket, &yesIhaveWork, sizeof(int), 0);
	timeCommEnd=gettime();
	timeComm+=diff(timeCommStart,timeCommEnd);

	if (hw != sizeof(int)) {
		EPRINTFF("Error! Unable to send HaveWork to server!\n");
		exit(EXIT_FAILURE);
	}

	// Send vector size to server
	printStatus(job.id, job.job, 'B');

	timeCommStart=gettime();
		int s = send(job.socket, &(job.vectorSize), sizeof(int), 0);
	timeCommEnd=gettime();
	timeComm+=diff(timeCommStart,timeCommEnd);
	
	if (s != sizeof(int)) {
		EPRINTFF("Error! Unable to send size of vector to server!\n");
		exit(EXIT_FAILURE);
	}

	// = Send vector to server =================================================
	printStatus(job.id, job.job, 'C');

	timeCommStart=gettime();
		int sended = transferDataBlocks(
		TRANSFER_DATA_SEND, job.socket, job.vector, job.vectorSize);
	timeCommEnd=gettime();
	timeComm+=diff(timeCommStart,timeCommEnd);


	if (sended != job.vectorSize) {

		EPRINTFF("Error! Problem to send vector to server!\n");
		exit(EXIT_FAILURE);
	}
	// =========================================================================

	// = Receive data from server ==============================================
	printStatus(job.id, job.job, 'D');
	
	timeCommStart=gettime();
		int received = transferDataBlocks(
		TRANSFER_DATA_RECV, job.socket, job.vector, job.vectorSize);
	timeCommEnd=gettime();
	timeComm+=diff(timeCommStart,timeCommEnd);

	if (received != job.vectorSize) {
		EPRINTFF("Error! Problem to receive vector from server!\n");
		exit(EXIT_FAILURE);
	}
	// =========================================================================

	printStatus(job.id, job.job, 'E');
	// Put worker back to pool...
	job.workers[job.id] = 1;

	// Signal that worker has arrive to pool
	sem_post(&SEM_WORKERS);

	BUBBLE_COMM_TIME[job.id]=timeComm;

}

int getNextWorker(int workersPool[], int poolSize) {

	int workerID=-1;

	// Discover who is the worker...
	for (int i = 0; i < poolSize; i++) {
		if (workersPool[i] == 1) {
			workerID = i;
			workersPool[i] = 0;
			break;
		}
	}

	return workerID;

}

void printLegend() {

	printf("\n");
	printf("    Tasks Legend:\n");
	printf("    A - Signal to server that have a task to send.\n");
	printf("    B - Send task size to server.\n");
	printf("    C - Send vector block to server to sort (bubble-sort).\n");
	printf("    D - Waiting server send pre-sorted vector.\n");
	printf("    E - Task done!\n\n");

}

void printTaskID(int parts) {

	// FIXME FOR FUTURE!
	// Next time use a vector and store data about tasks.
	// Then just print vector.  DUMB!

	printf("    Worker: \n");
	printf("    Status: \n");
	printf("    Task I: \n");

	int k = printf("         D  ");

	MVU(1);
	for (int i = 1; i <= parts; i++) {
		if ((i / 10) == 0) {
			printf(" ");
		} else {
			printf("%d", (i / 10));
		}

	}
	printf("\n");
	MVF(k);
	for (int i = 1; i <= parts; i++) {
		printf("%d", i % 10);
	}
	MVU(3);

}
// Split, send vector to servers and get results.
int doTheJob(int serversNumber, int socketsfd[], int vector[], int vectorSize) {

	TIME bubbleTimeStart,bubbleTimeEnd;

    bubbleTimeStart=gettime();

	// To do this we will use THREADS :)

	// Initialize this bitches =================================================
	printf("    Creating Worker threads: ");
	pthread_t threads[serversNumber];
	pthread_attr_t threadsAtt[serversNumber];
	ThreadsParam tp[serversNumber];
	int workersStatus[serversNumber];

	// Init workers parameters...
	for (int i = 0; i < serversNumber; i++) {
		pthread_attr_init(&threadsAtt[i]);
		pthread_attr_setscope(&threadsAtt[i], PTHREAD_SCOPE_SYSTEM);
		tp[i].id = i;
		tp[i].socket = socketsfd[i];
		tp[i].workers = workersStatus;
		workersStatus[i] = 1;
	}

	sem_init(&SEM_WORKERS, 1, serversNumber);
	sem_init(&SEM_SCREEN, 1, 1);
	
	
	printf("OK!\n");

	// =========================================================================

	// = Calculate how work will be shared =====================================
	/* For example:
	 *
	 * If vector size is 32000 and DEFAULT_SPLIT_SEGMENT is 32:
	 * 32000/32 = 1000. No problem
	 *
	 * But, if vector is 32010/32 = 1000 and left 10.
	 * To avoid this we decrement DEFAULT_SPLIT_SEGMENT to 31, so
	 * 32000/31 = 1032 => 31*1032 = 31992.
	 * in this way we will transmit 31 blocks with 1032 and
	 * 1 block with 32000 - 31992 = 8
	 */

	printf("    Calculating load distribution:\n");
	int taskSize; // size that will send to each job. (may change in the last)
	int fixPartSize; // default size of job.

	int tasks;
	if (vectorSize >= DEFAULT_SPLIT_SEGMENTS) {
		tasks = DEFAULT_SPLIT_SEGMENTS;
		if (vectorSize % tasks) {
			fixPartSize = vectorSize / tasks;
		} else {
			fixPartSize = vectorSize / (tasks - 1);
		}
	} else {
		tasks = 1;
		fixPartSize = vectorSize;
	}

	taskSize = fixPartSize;

	printf("        %d task%s of %d byte%s", tasks, addPlural(tasks, "", "s"),
			taskSize, addPlural(taskSize, "", "s"));

	if (vectorSize % tasks != 0) {
		int b = (vectorSize - fixPartSize * tasks);
		printf(" + 1 task of %d byte%s", b, addPlural(b, "", "s"));
	}
	printf("\n");

	// =========================================================================

	int jobsDone = 0;

	// Do THE JOB CONCURRENTLY :S Maybe it works... maybe not O_o.
	// I hope that work

	printLegend();

	printTaskID(tasks);

	for (int i = 0; i < tasks; i++) {

		//Wait a Worker become available
		sem_wait(&SEM_WORKERS);

		int workerID = getNextWorker(workersStatus, serversNumber);

		// I'm using this way:
		// partSize = (vectorSize - (jobsDone * fixPartSize)) % fixPartSize;
		// But I want to test this new thing. I know this mode is more complex
		// than older mode, but... I'm learning macros ;D
		taskSize = MIN(fixPartSize, (vectorSize - jobsDone*fixPartSize));

		//Setup worker parameters...
		tp[workerID].vector = vector + jobsDone * taskSize;
		tp[workerID].vectorSize = taskSize;
		tp[workerID].job = jobsDone;

		pthread_create(&threads[workerID], &threadsAtt[workerID], (void*) &run,
				(void*) &tp[workerID]);

		jobsDone++;
	}

	// This is for wait ALL workers arrive :) Without it, the client
	// disconnect from all servers if someone is late. :)

	for (int i = 0; i < serversNumber; i++) {
		sem_wait(&SEM_WORKERS); // I'm Something like a barrier? I'm a Barrier!
	}

	PRINTFF("\n\n\n\n\n");

	printf("All jobs done! Disconnecting from servers!\n");

	// Bye bye Servers. Nice to meet you =D
	int noIhaveNoWork = 0;
	for (int i = 0; i < serversNumber; i++) {

		// This is needed to server know that all jobs are done!
		// And them, close connection successfuly too.
		int nw = send(tp[i].socket, (void*) &noIhaveNoWork, sizeof(int), 0);

		if (nw != sizeof(int)) {
			EPRINTFF("Problem to friendly disconnect from server %d! ", i + 1);
			EPRINTFF("Closing socket! :(\n");
		}
		close(tp[i].socket);
	}


	bubbleTimeEnd=gettime();

	double bubbleTime = diff(bubbleTimeStart,bubbleTimeEnd);
    
    fprintf(PROFILE,"%g;",bubbleTime);

	
	return 1;
}

void noSlavesForYou() {
	EPRINTFF("\nSorry! I need to connect at least to 1 server.\n");
	EPRINTFF("I can't do ALL the JOB myself :(\n\n");
}

int connectToServers(int *serverSockets[]) {

	// Load address :)
	SERVER servers[DEFAULT_SERVERS_MAX];
	int serversFound = readServersList(servers);

	printf("    Servers read from file: %d\n", serversFound);
	if (serversFound == 0) {
		return -3;
	}

	// Connect to servers found...
	int sockfds[serversFound];
	int serverNumber = 0;

	for (int i = 0; i < serversFound; i++) {

		int s = createSockAndConnectToServer(&sockfds[i], servers[i]);

		if (s != 1) {
			EPRINTFF("    [ FAIL (%2d) ] Cannot connect to Server %d (%s:%d)\n",
					s, i, servers[i].address, servers[i].port);
			sockfds[i] = -1;
			continue;
		}

		printf("    [ CONNECTED ] Server %d as Worker %d (%s:%d)\n", i,
				serverNumber, servers[i].address, servers[i].port);
		serverNumber++;

	}

	printf("    Connected to %d of %d servers\n\n", serverNumber, serversFound);
	if (serverNumber == 0) {
		return -4;
	}

// Copy connected sockets to a new vector and write to call back;
	int *connectedSockets = malloc(sizeof(int) * serverNumber);

	int c = 0;

	for (int i = 0; i < serversFound; i++) {
		if (sockfds[i] > 0) {
			connectedSockets[c] = sockfds[i];
			c++;
		}
	}

	*serverSockets = connectedSockets;

	return c;

}

int main(int argc, char *argv[]) {

	TIME totalTimeStart,totalTimeEnd;
	totalTimeStart = gettime();



	// = Validate input parameters =============================================
	if (argc != 2) {
		EPRINTFF("No parameters!\n");
		help(argv[0]);
		return -1;
	}

	int vectorSize = atol(argv[1]);
	if (vectorSize <= 0) {
		EPRINTFF("Error! Invalid vector size!\n");
		help(argv[0]);
		return -2;
	}
	// =========================================================================


	int fe=0;
    if(access("./profile.txt",W_OK) != -1)
    {
    	fe=1;
    }

	

	printf("\n[Connection]\n");
	int *serversSockets;
	int serversConnected = connectToServers(&serversSockets);
	if (serversConnected < 1) {
		noSlavesForYou();
		return -3;
	}
	
	PROFILE = fopen("profile.txt","a");
    if(!fe)
    {
		fprintf(PROFILE,"Size;Servers;Input(GenRand);Bubble(Total);");
		fprintf(PROFILE,"Merge(CPU);Output;BubbleCom(Media);Total;");

		fprintf(PROFILE,"\n");

	}

	fprintf(PROFILE,"%d;%d;",vectorSize,serversConnected);

	BUBBLE_COMM_TIME = malloc(sizeof(double)*serversConnected);

	// = Load Job ==============================================================

	TIME timeInputStart,timeInputEnd;
    timeInputStart=gettime();
	printf("\n[Loading work]\n");

	int bytes = sizeof(int) * vectorSize;

	printf("    Allocating array of %d bytes (%d elements): ", bytes,
			vectorSize);

	int *vector = (int*) malloc(bytes);

	if (vector == NULL) {
		EPRINTFF("WTF? Something get very wrong!");
		EPRINTFF("I can't create a vector of this size!");
		return -4;
	}
	printf("OK!\n");

	printf("    Randomizing vector: ");
	createRandomVector(vector, vectorSize);
	printf("OK!\n");
    
    timeInputEnd=gettime();
	
	double timeInput=diff(timeInputStart,timeInputEnd);

	fprintf(PROFILE,"%g;",timeInput);

	// =========================================================================

	// = Bubble-Sort - Let the magic begins :D =================================

	printf("\n[Servers Work]\n");
	int d = doTheJob(serversConnected, serversSockets, vector, vectorSize);

	if (d < 0) {
		EPRINTFF("Problem! Aborting!\n");
		return -5;
	}

	// =========================================================================

	printf("\n[SORTING - MERGE]\n");
	printf("    Sorting: ");

	TIME timeMergeStart,timeMergeEnd;

	timeMergeStart=gettime();
		startMagic();
		mergeSort(vector, vectorSize);
		stopMagic();
	timeMergeEnd=gettime();

	double timeMerge = diff(timeMergeStart,timeMergeEnd);
	fprintf(PROFILE,"%g;",timeMerge);


	printf("OK!\n");

	printf("    Testing vector: ");
	if (!isSorted(vector, vectorSize)) {
		EPRINTFF("Problem! Some idiot (not me, of couse) made a mistake. ");
		EPRINTFF("This vector is not sorted!!!\n");
		return -6;
	}
	printf("OK!");

	printf("\n[Recording Results]\n");
	printf("    Writing vector in file: ");

	TIME timeOutputStart,timeOutputEnd;
    timeOutputStart=gettime();
	int r = recordResults(vector, vectorSize);
    timeOutputEnd=gettime();

	double timeOutput = diff(timeOutputStart,timeOutputEnd);
	fprintf(PROFILE,"%g;",timeOutput);


	if (!r) {
		EPRINTFF("   Fail to write vector in '%s'. %d flag.\n",
				DEFAULT_FILE_RESULTS, r);
		return -7;
	}




    double s=0;
    for(int i=0;i<serversConnected;i++)
    {
    	s+=BUBBLE_COMM_TIME[i];
    }

    fprintf(PROFILE,"%g;",s/serversConnected);

	totalTimeEnd = gettime();
	double totalTime = diff(totalTimeStart,totalTimeEnd);

	fprintf(PROFILE,"%g",totalTime);
    fprintf(PROFILE,"\n");

	fclose(PROFILE);

	printf("OK!\n");

	printf("\n");
	PRINTFF("All done! Have a nice day!\n\n");
	return 0;

}


void mergin() {
	struct timespec st, st2;
	st.tv_sec = 0;
	st.tv_nsec = 200000000;
	int i = 0;

	while (MERGIN) {

		PRINTFF("%c", palitoGirante(i++));
		nanosleep(&st, &st2); // u in usleep is microseconds! O_O

	}
}
void stopMagic() {
	printf("\b");
	MERGIN = 0;
}

void startMagic() {
	MERGIN = 1;
	pthread_t merger;
	pthread_attr_t mergeratt;

	// Launch BUBBLER!
	pthread_attr_init(&mergeratt);
	pthread_attr_setscope(&mergeratt, PTHREAD_SCOPE_PROCESS);
	pthread_create(&merger, &mergeratt, (void*) &mergin, NULL);

}
