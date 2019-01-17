#! /bin/bash
for i in t1 t2 t3;
do
    ./readchain tests/$i.dat > $i.txt
    cmp $i.txt tests/$i.txt && echo "PASS" $i
done

