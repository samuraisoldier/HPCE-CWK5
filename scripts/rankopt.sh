#!/bin/bash 

prev=0.0
tb=60.0
puzzle=rank
engines="ref"
now=`date +%Y-%m-%d_%H-%M-%S`
if [ ! -d results ]; then
	mkdir results
fi
echo "i,N,engine,time(s)" > results/${now}_${puzzle}_times.csv
n=0
for engine in $engines ;
do
# echo $engine
	for i in 450 2550 6450 12150 19650 28950 40050 52950 67650 84150 102450 122550 144450 168150 193650 220950 250050 280950 313650 348150 384450 422550 462450 504150 547650 592950 640050 688950 739650 792150 846450 902550 960450 1020150 1081650 1144950 1210050 1276950 1345650 1416150 1488450 1562550 1638450 1716150 1795650 1876950 1960050 2044950 2131650 2220150
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

