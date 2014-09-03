#include <stdlib.h>
#include <stdio.h>

void merge(int v[], int size) {

	int mid; // Mid of vector (size/2)

	int i, j, k;

	int* tmp; //Use this to help sort vector

	tmp = (int*) malloc(size * sizeof(int));

	if (tmp == NULL) {

		printf("\n\n\nPC DA XUXA!\n\n\n");
		fflush(stdout);
		exit(-1);
	}
	mid = size / 2;

	i = 0; 		// Index of begining of vector
	j = mid; 	// Index of mid of vector
	k = 0;		// Index to new vector free position;

	/* Enquanto estiver dentro dos limites do dos sub vetores copia dos dados
	 * de V ordenando em TMP */
	while (i < mid && j < size) {
		//pfddf(4);
		if (v[i] <= v[j]) {
			tmp[k] = v[i];
			i++;
		} else {
			tmp[k] = v[j];
			j++;
		}
		k++;
	}

	/* Copia o que faltou (a outra metade)*/
	if (i == mid) {
		while (j < size) {
			tmp[k++] = v[j++];
		}
	} else {
		while (i < mid) {
			tmp[k++] = v[i++];

		}
	}

	for (i = 0; i < size; ++i) {
		v[i] = tmp[i];
	}


}

void mergeSort(int vec[], int vecSize) {
	int mid;

	if (vecSize > 1) {
		mid = vecSize / 2;
		mergeSort(vec, mid);
		mergeSort(vec + mid, vecSize - mid);
		merge(vec, vecSize);
	}
}

