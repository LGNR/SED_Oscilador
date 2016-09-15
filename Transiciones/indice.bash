#!/bin/bash
for index in  {1..100}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
