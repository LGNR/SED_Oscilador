#!/bin/bash
for index in  {901..1000}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
