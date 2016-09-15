#!/bin/bash
for index in  {101..200}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
