#!/bin/bash 

prev=0.0
tb=60.0
puzzle=decompose
engines="ref tbb opencl opt"
now=`date +%Y-%m-%d_%H-%M-%S`
if [ ! -d results ]; then
	mkdir results
fi
echo "i,N,engine,time(s)" > results/${now}_${puzzle}_times.csv
n=0
for engine in $engines ;
do
# echo $engine
	for i in 35 103 183 271 364 461 562 667 774 884 996 1110 1227 1345 1465 1587 1711 1836 1962 2090 2219 2350 2482 2614 2748 2884 3020 3157 3295 3434 3575 3716 3858 4001 4144 4289 4434 4581 4728 4875
	do
		if [ ! -f w/$puzzle.$i.input ]; then
			bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
		fi

		EXEC_TIME=`(cat w/$puzzle.$i.input | bin/execute_puzzle $puzzle.$engine 2) 2>&1 | grep -a 'Begin execution\|Finished execution'`
		start=$(echo $EXEC_TIME | cut -d',' -f 2)
		end=$(echo $EXEC_TIME | cut -d',' -f 5)
		duration=$(echo $end - $start |bc -l)
		tot=$(echo $prev + $duration |bc -l)
		if (( $(echo "$tot > $tb" |bc -l) )); then
			under=$(echo $tb-$prev | bc -l)
			n=$(echo $n + $under/$duration |bc -l)
			echo $engine $n
			echo $n,$i,$engine,$tot >> results/${now}_${puzzle}_times.csv
			n=0
			prev=0.0
			break
		else
			prev=$tot
			echo $n,$i,$engine,$tot >> results/${now}_${puzzle}_times.csv
			n=$(echo $n + 1|bc -l)
		fi

	done
done

