#!/bin/bash

for i in {0..2};
do
    for j in {1..140};
    do
        for k in {1..100}; do cat ${1} | ./lpsplit.sh 10 ./kmeans $k $j ${2} $i ; done > ./test_data/test-${i}-${j}.txt
    done
done
