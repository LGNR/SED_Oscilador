#!/bin/bash
for index in  {401..500}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
