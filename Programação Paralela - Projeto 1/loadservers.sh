#!/bin/bash


#Script para execução LOCAL (testes)

echo "Matando servidores ativos..."
./killservers.sh

echo "Iniciando servidores..."



for i in $(seq 7000 100 7800)
do
	./serverExe $i > /dev/null &
done

while [ true ];
do 
	echo 'Estatisticas dos Servidores Rodando'
	echo 'PID CPU MEM PROCESSO PORTA'

	ps aux | grep serverExe | grep -v grep | sed 's/\s\+/ /g' | cut -d " " -f 2,3,4,11,12

	echo ""
	echo 'Pressione CTRL+C para finalizar esse script.'
	echo 'Execute killservers.sh para finalizar os servidores.'
	sleep 1
	clear
done
