#!/bin/bash
ENGINE="decompose.ref"
ENGINE2="decompose.tbb"
ENGINE3="decompose.opencl"
ENGINE4="decompose.opt"
PUZZLE=${ENGINE%%.*}
SCALES="35 103 183 271 364 461 562 667 774 884 996 1110 1227 1345 1465 1587 1711 1836 1962 2090"
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
#	if [ ! -f w/$puzzle.$i.input ]; then
#		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
#	fi
 bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
  echo ${SCALE} >> results/decomposeres.txt
    echo -e '\r' >> results/decomposeres.txt
	echo 'ref' >> results/decomposeres.txt
	echo -e '\r' >> results/decomposeres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/decomposeres.txt
    echo -e '\r' >> results/decomposeres.txt
	echo 'tbb' >> results/decomposeres.txt
	echo -e '\r' >> results/decomposeres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/decomposeres.txt
  echo -e '\r' >> results/decomposeres.txt
	echo 'opencl' >> results/decomposeres.txt
	echo -e '\r' >> results/decomposeres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE3} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE3}.got) 2>> results/decomposeres.txt
  echo -e '\r' >> results/decomposeres.txt
	echo 'opt' >> results/decomposeres.txt
	echo -e '\r' >> results/decomposeres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE4} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE4}.got) 2>> results/decomposeres.txt
  echo -e '\r' >> results/decomposeres.txt
  echo -e '\r' >> results/decomposeres.txt
 # bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/decomposeres.txt
  #echo -e '\r' >> results/decomposeres.txt
  #echo -e '\r' >> results/decomposeres.txt
  #echo -e '\r' >> results/decomposeres.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
