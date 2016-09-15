#!/bin/bash
for index in  {301..400}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
