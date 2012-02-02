#!/bin/sh

CONFIG_FILE="./last_run.cfg"
NAME_SUFFIX=""

if [ -n "$1" ]; then
    NAME_SUFFIX=$1
fi

OUTPUT_FILES="*${NAME_SUFFIX}.bin"
rm -f ${OUTPUT_FILES}
../ca.out $NAME_SUFFIX

if [ ! -r ${CONFIG_FILE} ]; then
    echo "can't read ${CONFIG_FILE}"
    exit
fi

. ${CONFIG_FILE}
OUTPUT_NAME_PLT="movie${NAME_SUFFIX}.plt"
OUTPUT_NAME_GIF="movie${NAME_SUFFIX}.gif"
echo "width = ${FIELD_WIDTH}, height = ${FIELD_HEIGHT}"

cat > ${OUTPUT_NAME_PLT} << EOF
set terminal gif animate  # set output format: PNG animated picture
set xrange[0:${FIELD_WIDTH}]          # set X scale range            
set yrange[0:${FIELD_HEIGHT}]          # set Y scale range            
set zrange[0:6]  noreverse nowriteback
set cbrange[0:6]          # set color bar range (Z scale)
set output "${OUTPUT_NAME_GIF}"    # set output file name

set pm3d
set style line 3  linetype -1 linewidth 0.01
set pm3d depthorder hidden3d 3
set style fill  transparent solid 1 border  
#set style data lines
set palette
set hidden3d

#unset xtics ; unset ytics ; unset ztics
#unset border ; unset colorbox ; unset key
#set ticslevel 0

unset surface
set border 895 front linetype -1 linewidth 1.000

EOF


for i in `ls ${OUTPUT_FILES}`; do echo "splot \"${i}\" binary array=${FIELD_WIDTH}x${FIELD_HEIGHT} format=\"%lf\"" >> ${OUTPUT_NAME_PLT}; done
time gnuplot ${OUTPUT_NAME_PLT}
