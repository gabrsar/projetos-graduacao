#!/bin/bash

testes=5
nodes=(1 2 4 8 16)

for teste in $(seq $testes)
do
	for inp in $(ls ./testes/*.INPUT.txt)
	do
		for n in "${nodes[@]}"
		do

			cat "./testes/hosts" | head -n $n >> hosts
			echo "Testando $testeª vez. $node workers - Arquivo: $inp"
			mpirun --hostfile hosts ./exe $inp;
		done
	done
done



		

