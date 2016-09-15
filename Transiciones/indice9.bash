#!/bin/bash
for index in  {801..900}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
