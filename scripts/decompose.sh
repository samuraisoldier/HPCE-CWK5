#!/bin/bash
ENGINE="decompose.tbb"
PUZZLE=${ENGINE%%.*}
SCALES="35 103 183 271 364 461 562 667 774 884 996 1110 1227 1345 1465 1587 1711  1836 1962 2090 2219 2350 2482 2614 2748 2884 3020 3157 3295 3434 3575 3716 3858 4001"
WORKING=.tmp
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
  
  bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
  echo ${SCALE} >> results/decomposetbb.txt
    echo -e '\r' >> results/decomposetbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle "ref" 2 > ${WORKING}/${PUZZLE}.${SCALE}.ref) 2>> results/decomposetbb.txt
    echo -e '\r' >> results/decomposetbb.txt
	echo -e '\r' >> results/decomposetbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.ref ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
