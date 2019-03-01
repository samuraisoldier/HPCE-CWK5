#!/bin/bash
ENGINE="rank.opt"
PUZZLE=${ENGINE%%.*}
SCALES="276450"
WORKING=.tmp
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
  
  bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
  echo ${SCALE} >> results/ranktbb.txt
    echo -e '\r' >> results/ranktbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle "ref" 2 > ${WORKING}/${PUZZLE}.${SCALE}.ref) 2>> results/ranktbb.txt
    echo -e '\r' >> results/ranktbb.txt
	echo -e '\r' >> results/ranktbb.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.ref ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
