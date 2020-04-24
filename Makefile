README.md: results.svg results_small.svg

%.svg: %.plt results.dta
	gnuplot $<

results.dta: results parse.py
	>$@ <$< python3 parse.py

.PRECIOUS: results
results: main.cpp run
	for n in $$(seq 1 1000); do echo n: $$n >>$@; ./run $$n 100000000 >>$@; echo >>$@; done
