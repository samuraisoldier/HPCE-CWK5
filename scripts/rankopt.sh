#!/bin/bash 

prev=0.0
tb=60.0
puzzle=rank
engines="ref opt"
now=`date +%Y-%m-%d_%H-%M-%S`
if [ ! -d results ]; then
	mkdir results
fi
echo "i,N,engine,time(s)" > results/${now}_${puzzle}_times.csv
n=0
for engine in $engines ;
do
# echo $engine
	for i in 450 4950 14450 28950 48450 72950 102450 136950 176450 220950 270450 324950 384450 448950 518450 592950 672450 756950 846450 940950 1040450 1144950 1254450 1368950 1488450 1612950 1742450 1876950 2016450 2160950 2310450 2464950 2624450 2788950 2958450 3132950 3312450 3496950 3686450 3880950 4080450 4284950 4494450 4708950 4928450 5152950 5382450 5616950 5856450 6100950 6350450 6604950 6864450 7128950 7398450 7672950 7952450 8236950 8526450 8820950
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

