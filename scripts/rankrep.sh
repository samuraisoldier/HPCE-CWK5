#!/bin/bash
ENGINE="rank.ref"
ENGINE2="rank.tbb"
ENGINE4="rank.opt"
PUZZLE=${ENGINE%%.*}
SCALES="275 2075 5675 11075 18275 27275 38075 50675 65075 81275 "
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
#	if [ ! -f w/$puzzle.$i.input ]; then
#		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
#	fi
 bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
  echo ${SCALE} >> results/rankres.txt
    echo -e '\r' >> results/rankres.txt
	echo 'ref' >> results/rankres.txt
	echo -e '\r' >> results/rankres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/rankres.txt
    echo -e '\r' >> results/rankres.txt
	echo 'tbb' >> results/rankres.txt
	echo -e '\r' >> results/rankres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/rankres.txt
  echo -e '\r' >> results/rankres.txt
	echo 'opt' >> results/rankres.txt
	echo -e '\r' >> results/rankres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE4} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE4}.got) 2>> results/rankres.txt
  echo -e '\r' >> results/rankres.txt
  echo -e '\r' >> results/rankres.txt
 # bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/rankres.txt
  #echo -e '\r' >> results/rankres.txt
  #echo -e '\r' >> results/rankres.txt
  #echo -e '\r' >> results/rankres.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
