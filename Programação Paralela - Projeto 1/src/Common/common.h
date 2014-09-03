/*
 * common.h
 *
 *  Created on: 30/04/2014
 *      Author: gabriel
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>



#define EPRINTFF(...) 		{fprintf (stderr, __VA_ARGS__);fflush(stderr);}
#define PRINTFF(...)  		{printf(__VA_ARGS__);fflush(stdout);}
#define MIN(X,Y)			((X) < (Y) ? (X):(Y))
#define MOVE_CURSOR(X,Y) 	PRINTFF("\033[%d;%dH",(X),(Y))
#define MVU(a) 				for(int i=0;i<(a);i++){printf("\033[A");}
#define MVD(a) 				for(int i=0;i<(a);i++){printf("\033[B");}
#define MVF(a) 				for(int i=0;i<(a);i++){printf("\033[C");}
#define MVB(a) 				for(int i=0;i<(a);i++){printf("\033[D");}

#define TRANSMIT_DATA_SEND 1
#define TRANSMIT_DATA_RECV 2
#define BIT_HAPPENS_SLEEP_TIME 1
#define MAX_MSG_SIZE_BYTES (32*1024)
#define MAX_MSG_SIZE_ELEMENTS (MAX_MSG_SIZE_BYTES/sizeof(int))
#define MAX_FAILURE 10

#define BILLION 1000000000L    // Usado pelo clock_gettime

typedef struct timespec TIME;

int gprintff(FILE *fd, char format[], va_list paramList);
int dprint(char format[], int p, ...);
void clearScreen();
char* addPlural(int i, char singularSulfix[], char pluralSulfix[]);
char palitoGirante(int i);
void printVector(int vector[], size_t size);
int transferDataBlocks(  // :)
		int type,			// Send or receive
		int destSocketfd, 	// Socket of who will send or receive data
		int vector[],		// ...
		size_t vectorSize  // ...
		);

TIME gettime();
double diff(TIME start, TIME end);














#endif /* COMMON_H_ */
