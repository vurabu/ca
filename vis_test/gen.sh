#!/bin/sh

CONFIG_FILE="./last_run.cfg"

rm -f *.bin
../ca.out

if [ ! -r ${CONFIG_FILE} ]; then
    echo "can't read ${CONFIG_FILE}"
    exit
fi
. ${CONFIG_FILE}

echo "width = ${FIELD_WIDTH}, height = ${FIELD_HEIGHT}"

cat > movie_gif.plt << EOF
set terminal gif animate  # set output format: PNG animated picture
set xrange[0:${FIELD_WIDTH}]          # set X scale range            
set yrange[0:${FIELD_HEIGHT}]          # set Y scale range            
set cbrange[0:7]          # set color bar range (Z scale)
set output "movie.gif"    # set output file name
#set palette rgbformulae 7,5,15
set palette file "-"
10 0.1 0.1 0.1
20 0.0 0.9 0.4
30 0.2 0.1 0.4
40 0.9 0.9 0.0
50 0.1 0.1 0.1
e

EOF
for i in `ls *.bin`; do echo "plot \"${i}\" binary array=${FIELD_WIDTH}x${FIELD_HEIGHT} format=\"%lf\" with image" >> movie_gif.plt; done
gnuplot movie_gif.plt
