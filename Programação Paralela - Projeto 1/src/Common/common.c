/*
 * common.c

 *
 *  Created on: 13/05/2014
 *      Author: gabriel
 */

#include "common.h"

void clearScreen() {
	system("clear");
}

char palitoGirante(int i) {
	int step = i % 4;

	switch (step) {
	case 0:
		return '|';
	case 1:
		return '/';
	case 2:
		return '-';
	case 3:
		return '\\';
	}

	return -1;
}

// =============================================================================

double diff(TIME start, TIME end)
{
    TIME temp;

    temp.tv_sec = (end.tv_sec - start.tv_sec);
    temp.tv_nsec = (end.tv_nsec - start.tv_nsec);


    double t;
    t = temp.tv_sec * BILLION;
    t += temp.tv_nsec;

    return t / BILLION;
}

TIME gettime()
{
    TIME t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t;
}                       



char* addPlural(int i, char singularSulfix[], char pluralSulfix[]) {
	return i > 1 ? pluralSulfix : singularSulfix;
}

int transferDataBlocks(int type, int destSocketfd, int *vector, size_t vectorSize) {

	char typeText[20];

	if (type == TRANSMIT_DATA_RECV) {
		strcpy(typeText, "Receiving");
	} else {
		strcpy(typeText, "Sending");
	}

	size_t transmited = 0;
	size_t toTransmit = 0;

	//PRINTFF("%s %d elements:", typeText, vectorSize);

	while (transmited < vectorSize) {

		size_t left = vectorSize - transmited;

		toTransmit = MIN(left,MAX_MSG_SIZE_ELEMENTS);

		size_t t;
		if (type == TRANSMIT_DATA_RECV) {

			t = recv(destSocketfd, (vector + transmited),
					toTransmit * sizeof(int), MSG_WAITALL);
		} else {
			t = send(destSocketfd, (vector + transmited),
					toTransmit * sizeof(int), MSG_WAITALL);
		}

		if (t != toTransmit * sizeof(int)) {
			return transmited + t;
		}

		transmited += toTransmit;

	}

	//PRINTFF("OK!\n");

	return transmited;

}

void printVector(int vector[], size_t size) {

	printf("Vector: %p - Size: %zu = {", vector, size);
	size--;
	for (size_t i = 0; i < size; i++) {

		printf("%d, ", vector[i]);

	}

	printf("%d}\n", vector[size]);

}

int dprint(char format[], int p, ...) {
	va_list paramList;
	va_start(paramList, p);

	return gprintff(stdout, format, paramList);

}

int gprintff(FILE *fd, char format[], va_list paramList) {

	int k = vfprintf(fd, format, paramList);

	fflush(fd);

	return k;
}

