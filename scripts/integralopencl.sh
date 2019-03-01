n#!/bin/bash
ENGINE="integral.opt"
PUZZLE=${ENGINE%%.*}
SCALES="1825 1845 1865 1885 1905 1925 1945 1965 1985 2005 2025 2045 2065 2085 2105 2125 2145 2165 2185 2205 2225 2245 2265 2285 2305 2325 2345 2365 2385 2405 2425 2445 2465 2485 2505 2525 2545 2565 2585 2605 2625 2645 2665 2685 2705 2725 2745 2765 2785 2805 2825 2845 2865 2885 2905 2925 2945 2965 2985 3005 3025"
WORKING=.tmp
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
  
  bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
  echo ${SCALE} >> results/integralopencl.txt
    echo -e '\r' >> results/integralopencl.txt
 # time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle "ref" 2 > ${WORKING}/${PUZZLE}.${SCALE}.ref) 2>> results/integralopencl.txt
 #   echo -e '\r' >> results/integralopencl.txt
#	echo -e '\r' >> results/integralopencl.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/integralopencl.txt
 # echo -e '\r' >> results/integralopencl.txt
 # echo -e '\r' >> results/integralopencl.txt
 # bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.ref ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/integralopencl.txt
 # echo -e '\r' >> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done

