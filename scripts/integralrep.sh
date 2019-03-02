#!/bin/bash
ENGINE="integral.ref"
ENGINE2="integral.tbb"
ENGINE3="integral.opencl"
ENGINE4="integral.opt"
PUZZLE=${ENGINE%%.*}
SCALES="25 45 65 85 105 125 145 165 185 205 225 245 265 285 305 325 345 365 385 405 425 445 465 485 505 525 545 565 585 605 625 645 665 685 705 725 745 765 785 805 825 845 865 880 900 920 940 960 980 1000 "
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
#	if [ ! -f w/$puzzle.$i.input ]; then
#		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
#	fi
 bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
  echo ${SCALE} >> results/integralres.txt
    echo -e '\r' >> results/integralres.txt
	echo 'ref' >> results/integralres.txt
	echo -e '\r' >> results/integralres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/integralres.txt
    echo -e '\r' >> results/integralres.txt
	echo 'tbb' >> results/integralres.txt
	echo -e '\r' >> results/integralres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/integralres.txt
  echo -e '\r' >> results/integralres.txt
	echo 'opencl' >> results/integralres.txt
	echo -e '\r' >> results/integralres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE3} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE3}.got) 2>> results/integralres.txt
  echo -e '\r' >> results/integralres.txt
	echo 'opt' >> results/integralres.txt
	echo -e '\r' >> results/integralres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE4} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE4}.got) 2>> results/integralres.txt
  echo -e '\r' >> results/integralres.txt
  echo -e '\r' >> results/integralres.txt
 # bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/integralres.txt
  #echo -e '\r' >> results/integralres.txt
  #echo -e '\r' >> results/integralres.txt
  #echo -e '\r' >> results/integralres.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
