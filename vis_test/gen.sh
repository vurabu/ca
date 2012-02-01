#!/bin/sh

if [ -z "$2" ]; then
    echo usage: $0 xrange yrange
    exit
fi

rm -f *.bin
../ca.out

cat > movie_gif.plt << EOF
set terminal gif animate  # set output format: PNG animated picture
set xrange[0:$1]          # set X scale range            
set yrange[0:$2]          # set Y scale range            
set cbrange[0:7]          # set color bar range (Z scale)
set output "movie.gif"    # set output file name
EOF
for i in `ls *.bin`; do echo "plot \"${i}\" binary array=$1x$2 format=\"%lf\" with image" >> movie_gif.plt; done
gnuplot movie_gif.plt
