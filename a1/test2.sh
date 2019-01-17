#! /bin/bash
for i in t4 t5 t6;
do
    for j in 1 2 3; 
    do
        echo "$i/$j"
        time ./readchain tests/$i.dat > $i.txt
        cmp $i.txt tests/$i.txt && echo "PASS" $i
    done
done
