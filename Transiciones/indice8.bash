#!/bin/bash
for index in  {701..800}
do
command="./oscilador 1 >> histograma"$index
eval $command
done
