#! /bin/bash

gcc -g -Wall -o closestPairDeterministic closestPairDeterministic.c -lm
gcc -g -Wall -o closestPairRandomized closestPairRandomized.c -lm
gcc -g -Wall -o generatePointSet generatePointSet.c
gcc -g -Wall -o biconnectedComponents biconnectedComponents.c

echo -e "Number of points\tDeterministic\tRandomized" > time_plot.txt

for testcase in $(seq 1 20)
do
    n=$[testcase * 50000]
    echo -e "$n\n100000000" | ./generatePointSet > "$testcase.txt"
    t1=`command time -f "%U" ./closestPairDeterministic <"$testcase.txt" 2>&1 1>temp`
    cat temp
    echo "Time taken by deterministic algorithm = $t1 seconds"
    t2=`command time -f "%U" ./closestPairRandomized <"$testcase.txt" 2>&1 1>temp`
    cat temp
    echo "Time taken by randomized algorithm = $t2 seconds"
    echo -e "$n\t$t1\t$t2" >> time_plot.txt
done

gnuplot -e "set terminal dumb; set title 'Time taken by closest pair algorithms'; set xlabel 'Number of points'; set ylabel 'Time (seconds)'; plot \"time_plot.txt\" using 1:2 title 'Deterministic' with points pointtype 4, '' using 1:3 title 'Randomized' with points pointtype 18"
