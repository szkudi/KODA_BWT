#!/bin/bash

for i in {4..9}
do

echo $i

$DATA_SIZE = $(stat -c%s "$1")

time ./koda_bwt -e -i $1 -o encoded -$i

$OUT_SIZE = $(stat -c%s encoded)

time ./koda_bwt -d -i encoded -o decoded -$i

echo -e "Porownanie: \n"
$return = $(diff $1 decoded)

echo -e "Kompresja: \n"
$KOM = $(($OUT_SIZE/$DATA_SIZE*100))
echo $KOM %

done
