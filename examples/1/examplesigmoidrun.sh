./sigmoid
for i in {0..15}
	do
		clear
		./sigmoid_c
		python3 examplesigmoidvisual.py $i
	done
