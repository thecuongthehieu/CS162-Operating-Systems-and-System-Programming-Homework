#!/usr/bin/env bash

BUILD=.
EXAMPLES=../../examples
OUTPUT=cs162proj.dsk

LOADER=$BUILD/loader.bin
KERNEL=$BUILD/kernel.bin

make -C $EXAMPLES

rm -f $OUTPUT
pintos -v -k --make-disk $OUTPUT --kernel=$KERNEL --loader $LOADER --qemu --filesys-size=4 --align=full -- -q  -f

PREFIX=
if [ "$1" == "--simple" ]
then
	# If someone has really only done Project 1, then their root directory
	# can contain at most 16 entries. So we have to use them wisely. Two
	# spaces are used by file1.txt and file2.txt. I'm reserving two more for
	# one music demo and the contest, should they choose to do it. That leaves
	# 12 spaces for binaries. I'm including ls, mkdir, and pwd so students can
	# experience that aspect of it if they have their Project 3 and choose to
	# start with the simple shell.
	rm -f $EXAMPLES/auld
	rm -f $EXAMPLES/bubsort
	rm -f $EXAMPLES/fappend
	rm -f $EXAMPLES/fcreate
	rm -f $EXAMPLES/lineup
	rm -f $EXAMPLES/matmult
	rm -f $EXAMPLES/mcat
	rm -f $EXAMPLES/mcp
	rm -f $EXAMPLES/railroad
	rm -f $EXAMPLES/recursor
	rm -f $EXAMPLES/rmrec
	rm -f $EXAMPLES/yankee
else
	PREFIX=/bin/
	pintos -v -k --qemu --disk $OUTPUT -p $EXAMPLES/mkdir -a mkdir -- -q run "mkdir bin"
	pintos -v -k --qemu --disk $OUTPUT -p $EXAMPLES/rm -a rm -- -q run "rm mkdir rm"
fi

COPY_FILES_CMD="pintos -v -k --qemu --disk $OUTPUT"
for file in $(ls $EXAMPLES)
do
	if [ -f $EXAMPLES/$file ] && [ -x $EXAMPLES/$file ]
	then
		 COPY_FILES_CMD+=" -p $EXAMPLES/$file -a $PREFIX$file"
	fi
done

# Add some regular files too
echo -e "I need to study for the CS 162 final. Do you?" > file1.txt
echo -e "I need to study for the CS 186 final. Do you?" > file2.txt
COPY_FILES_CMD+=" -p ./file1.txt -a file1.txt"
COPY_FILES_CMD+=" -p ./file2.txt -a file2.txt"

COPY_FILES_CMD+=" -- -q"

eval $COPY_FILES_CMD
rm -f file1.txt file2.txt

# This last part is necessary when booting in VMWare or on real hardware
pintos-set-cmdline $OUTPUT -- -q run ${PREFIX}shell
