#!/bin/bash

mv profile.txt profile.old.txt
testes=5
nodes=(1 2 4 8)
loads=(200000 400000 800000 1600000)

for teste in $(seq $testes)
do
	for load in "${loads[@]}"
	do
		for n in "${nodes[@]}"
		do

			echo "Testando com $load elementos e $n servidores. Teste $teste."
			cat "HOSTS_LIST" | head -n $n > hosts.dat
			./clientExe $load
sleep 1
		
		done
	done
done



		

