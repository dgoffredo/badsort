results.dta: results parse.py
	>$@ <$< python3 parse.py

results: main.cpp run results
	for n in $$(seq 1 1000); do echo n: $$n >>$@; ./run $$n 100000000 >>$@; echo >>$@; done
