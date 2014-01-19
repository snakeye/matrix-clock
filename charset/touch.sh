#/bin/bash
for i in 2 3 4 5 6 7 8 9 0 a b c d e f
do
  for j in 0 1 2 3 4 5 6 7 8 9 a b c d e f
  do
    touch $i$j.txt
  done
done

