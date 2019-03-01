#!/bin/bash 

prev=0.0
tb=60.0
puzzle=rank
engines="ref opt"
now=`date +%Y-%m-%d_%H-%M-%S`
if [ ! -d results ]; then
	mkdir results
fi
if [ ! -d w ]; then
	mkdir w
fi
echo "i,N,engine,time(s)" > results/${now}_${puzzle}_times.csv
n=0
for engine in $engines ;
do
# echo $engine
	for i in 275 2075 5675 11075 18275 27275 38075 50675 65075 81275 99275 119075 140675 164075 189275 216275 245075 275675 308075 342275 378275 416075 455675 497075 540275 585275 632075 680675 731075 783275 837275 893075 950675 1010075 1071275 1134275 1199075 1265675 1334075 1404275 1476275 1550075 1625675 1703075 1782275 1863275 1946075 2030675 2117075 2205275 2295275 2387075 2480675 2576075 2673275 2772275 2873075 2975675 3080075 3186275 3294275 3404075 3515675 3629075 3744275  861275 3980075 4100675 4223075 4347275 4473275 4601075 4730675 4862075 4995275 5130275 5267075 5405675 5546075 5688275 5832275 5978075 6125675 6275075 6426275 6579275 6734075 6890675 7049075 7209275 7371275 7535075 7700675
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

