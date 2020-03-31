./relu
for i in {0..15}
	do
		clear
		./relu_c
		python3 examplereluvisual.py $i
	done
