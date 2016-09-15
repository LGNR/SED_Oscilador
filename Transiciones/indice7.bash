#!/bin/bash
for index in  {601..700}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
