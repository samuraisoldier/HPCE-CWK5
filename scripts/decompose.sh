#!/bin/bash
ENGINE="decompose.ref"
ENGINE2="decompose.opt"
PUZZLE=${ENGINE%%.*}
SCALES="35  183    562    996    1465    1962 "
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
#	if [ ! -f w/$puzzle.$i.input ]; then
#		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
#	fi
 bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
  echo ${SCALE} >> results/decomposeopt.txt
    echo -e '\r' >> results/decomposeopt.txt
	echo 'opt' >> results/decomposeopt.txt
	echo -e '\r' >> results/decomposeopt.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/decomposeopt.txt
    echo -e '\r' >> results/decomposeopt.txt
	echo 'tbbif' >> results/decomposeopt.txt
	echo -e '\r' >> results/decomposeopt.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/decomposeopt.txt
  echo -e '\r' >> results/decomposeopt.txt
  echo -e '\r' >> results/decomposeopt.txt
  bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/decomposeopt.txt
  echo -e '\r' >> results/decomposeopt.txt
  echo -e '\r' >> results/decomposeopt.txt
  echo -e '\r' >> results/decomposeopt.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
