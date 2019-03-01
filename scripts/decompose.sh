#!/bin/bash
ENGINE="decompose.opt"
ENGINE2="decompose.tbbif"
PUZZLE=${ENGINE%%.*}
SCALES="3716 3858 4001 4144 4289 4434 4581 4728 4875"
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
#	if [ ! -f w/$puzzle.$i.input ]; then
#		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
#	fi
 bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
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
