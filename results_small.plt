
set xrange [0:11]

set key left top

set ylabel 'wall time (nanoseconds)'
set xlabel 'number of elements'

set term svg enhanced
set output 'results_small.svg'

plot 'results.dta' using 1:2 title 'selection sort', \
     'results.dta' using 1:3 title 'bubble sort', \
     'results.dta' using 1:4 title 'insertion sort', \
     'results.dta' using 1:5 title 'optimistic insertion sort'
     
