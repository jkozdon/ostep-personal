#!/usr/bin/bash

SIZE=1
for i in {0..11}; do
	./build/tlb $SIZE 1000000
	SIZE=$((SIZE * 2))
done
