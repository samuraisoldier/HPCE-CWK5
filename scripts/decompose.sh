#!/bin/bash
ENGINE="decompose.opt"
ENGINE2="decompose.tbbif"
PUZZLE=${ENGINE%%.*}
SCALES="35 103 183 271 364 461 562 667 774 884 996 1110 1227 1345 1465 1587 1711 1836 1962 2090 2219 2350 2482 2614 2748 2884 3020 3157 3295 3434"
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
	if [ ! -f w/$puzzle.$i.input ]; then
		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
	fi
  
  echo ${SCALE} >> results/decomposetbb.txt
    echo -e '\r' >> results/decomposetbb.txt
	echo 'opt' >> results/decomposetbb.txt
	echo -e '\r' >> results/decomposetbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/decomposetbb.txt
    echo -e '\r' >> results/decomposetbb.txt
	echo 'tbbif' >> results/decomposetbb.txt
	echo -e '\r' >> results/decomposetbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
  echo -e '\r' >> results/decomposetbb.txt
 # bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/decomposetbb.txt
  #echo -e '\r' >> results/decomposetbb.txt
  #echo -e '\r' >> results/decomposetbb.txt
  #echo -e '\r' >> results/decomposetbb.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
