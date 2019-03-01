n#!/bin/bash
ENGINE="integral.opt"
PUZZLE=${ENGINE%%.*}
SCALES="25 45 65 85 105 125 145 165 185 205 225 245 265 285 305 325 345 365 385 405 425 445 465 485 505 525 545 565 585 605 625 645 665 685 705 725 745 765 785 805 825 845 865 880 900 920 940 960 980 1000 1020 1040 1060 1080 1100 1120 1140 1160 1180 1200 1220 1240 1260 1280 1300 1320 1340 1360 1380 1400 1420 1440 1460 1480 1500 1520 1540 1560 1580 1600 1620 1640 1660 1680 1700 1720 1740 1760 1780 1800"
WORKING=.tmp
mkdir ${WORKING}
for SCALE in $SCALES ; do
  mkdir -p w
  
  bin/create_puzzle_input ${PUZZLE} ${SCALE} > ${WORKING}/${PUZZLE}.${SCALE}.input
  echo ${SCALE} >> results/integralopencl.txt
    echo -e '\r' >> results/integralopencl.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle "ref" 2 > ${WORKING}/${PUZZLE}.${SCALE}.ref) 2>> results/integralopencl.txt
    echo -e '\r' >> results/integralopencl.txt
	echo -e '\r' >> results/integralopencl.txt
  time(cat ${WORKING}/${PUZZLE}.${SCALE}.input | bin/execute_puzzle ${ENGINE} 2 > ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got) 2>> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  bin/compare_puzzle_output ${WORKING}/${PUZZLE}.${SCALE}.input ${WORKING}/${PUZZLE}.${SCALE}.ref ${WORKING}/${PUZZLE}.${SCALE}.${ENGINE}.got 2>> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  echo -e '\r' >> results/integralopencl.txt
  
  if [[ $? -ne 0 ]] ; then
    >&2 echo "FAIL"
    exit 1
  fi
done
