#!/bin/bash
for index in  {201..300}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
