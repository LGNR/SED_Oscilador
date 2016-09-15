#!/bin/bash
for index in  {501..600}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
