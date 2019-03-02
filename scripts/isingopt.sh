#!/bin/bash
prev=0.0 
tb=60.0 
puzzle=ising 
engines="ref tbb opt" 
now=`date +%Y-%m-%d_%H-%M-%S` 
if [ ! -d results ]; then
	mkdir results 
fi 
echo "i,N,engine,time(s)" > results/${now}_${puzzle}_times.csv 
n=0 
for engine in $engines ; 
do
# echo $engine
	for i in 25 45 65 85 105 125 145 165 185 205 225 245 265 285 305 325 345 365 385 405 425 445 465 485 505 525 545 565 585 605 625 645 665 685 705 725 745 765 785 805 825 845 865 880 900 920 940 960 980 1000 1020 1040 1060 1080 1100 1120 1140 1160 1180 1200 1220 1240 1260 1280 1300 1320 1340 1360 1380 1400 1420 1440 1460 1480 1500 1520 1540 1560 1580 1600 1620 1640 1660 1680 1700 1720 1740 1760 1780 1800 1820 1840 1860 1880 1900 1920 1940 1960 1980 2000 2020
	#echo $i
	do
		if [ ! -f w/$puzzle.$i.input ]; then
			bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
		fi
		echo $i
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
