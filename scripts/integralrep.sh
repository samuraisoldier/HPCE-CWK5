#!/bin/bash
ENGINE="integral.ref"
#ENGINE2="integral.tbb"
#ENGINE3="integral.opencl"
ENGINE4="integral.rep"
PUZZLE=${ENGINE%%.*}
SCALES=" 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80"
WORKING=w
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
	if [ ! -f w/$puzzle.$i.input ]; then
		bin/create_puzzle_input $puzzle $i > w/$puzzle.$i.input
	fi
# bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
 echo ${SCALE} 
  echo ${SCALE} >> results/integralres.txt
    echo -e '\r' >> results/integralres.txt
	echo 'ref' >> results/integralres.txt
	echo -e '\r' >> results/integralres.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/integralres.txt
  #  echo -e '\r' >> results/integralres.txt#
#	echo 'tbb' >> results/integralres.txt
#	echo -e '\r' >> results/integralres.txt
 # time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE2} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE2}.got) 2>> results/integralres.txt
 # echo -e '\r' >> results/integralres.txt
#	echo 'opencl' >> results/integralres.txt
#	echo -e '\r' >> results/integralres.txt
 # time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE3} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE3}.got) 2>> results/integralres.txt
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
