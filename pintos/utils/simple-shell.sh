PROGRAMS="cat \
cmp \
cp \
echo \
hex-dump \
insult \
ls \
mkdir \
pwd \
rm \
halt \
shell"
EXAMPLES="../../examples"
CMDLINE="pintos --filesys-size=4 $@"
CMDLINE_END="-f -q run 'shell'"

make -C $EXAMPLES

for PROGRAM in $PROGRAMS; do
    CMDLINE+=" -p $EXAMPLES/$PROGRAM -a $PROGRAM"
done
rm -f file1.txt file2.txt
echo -e "I need to study for the CS 162 final. Do you?" > file1.txt
echo -e "I need to study for the CS 186 final. Do you?" > file2.txt
CMDLINE+=" -p ./file1.txt -a file1.txt"
CMDLINE+=" -p ./file2.txt -a file2.txt"
CMDLINE+=" -- $CMDLINE_END"
echo $CMDLINE
eval $CMDLINE
rm -f file1.txt file2.txt
