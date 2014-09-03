#!/bin/bash

# Testador do multiplicador de matrizes por MPI

tamanhos=(200 400 600 800 1000 1200 1400 1600 1800 2000)
processos=(1 2 4 8 16)

testes=5

logname=$(date +%s)
logname=$logname".log"


echo $logname


for t in ${tamanhos[@]}
do
	for p in ${processos[@]}
	do

		echo "Testando $t x $t em $p processos"

		for i in {1..5}
        do
			
			t0=$(date +%s)

			tempoEfetivo=$(mpirun -np $p ./summa-mpi $t -q) 

			t1=$(date +%s)

			tempo=$(($t1 - $t0))


			echo "$t $p $tempo $tempoEfetivo" >> $logname
			echo "Teste $i: ${tempo}s ${tempoEfetivo}s"

        done
	done
done
