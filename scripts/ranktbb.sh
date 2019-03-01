#!/bin/bash
ENGINE="rank.opt"
PUZZLE=${ENGINE%%.*}
SCALES="275 2075 5675 11075 18275 27275 38075 50675 65075 81275 99275 119075 140675 164075 189275 216275 245075 275675 308075 342275 378275 416075 455675 497075 540275 585275 632075 680675 731075 783275 837275 893075 950675 1010075 1071275 1134275 1199075 1265675 1334075 1404275 1476275 1550075 1625675 1703075 1782275 1863275 1946075 2030675 2117075 2205275 2295275 2387075 2480675 2576075 2673275 2772275 2873075 2975675 3080075 3186275 3294275 3404075 3515675 3629075 3744275  861275 3980075 4100675 4223075 4347275 4473275 4601075 4730675 4862075 4995275 5130275 5267075 5405675 "
WORKING=.tmp
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
  
  bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
  echo ${SCALE} >> results/ranktbb.txt
    echo -e '\r' >> results/ranktbb.txt
  #time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle "ref" 2 > ${WORKING}/${PUZZLE}.${SCALE}.ref) 2>> results/ranktbb.txt
  #  echo -e '\r' >> results/ranktbb.txt
  #	echo -e '\r' >> results/ranktbb.txt
   time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/ranktbb.txt
  #echo -e '\r' >> results/ranktbb.txt
  #echo -e '\r' >> results/ranktbb.txt
  #bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.ref ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  echo -e '\r' >> results/ranktbb.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
