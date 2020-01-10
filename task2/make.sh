#!/usr/bin/bash
clear

if [ -d "./.rush" ]; then
	rm -rf ./.rush
	echo "$0: renew ./rush folder."
fi
mkdir ./.rush

cd ./utils/
gcc touch.c -o ../.rush/touch
gcc cp.c -o ../.rush/cp
gcc ls.c -o ../.rush/ls
gcc mkdir.c -o ../.rush/mkdir
gcc cat.c -o ../.rush/cat
gcc xargs.c -o ../.rush/xargs
gcc mv.c -o ../.rush/mv
gcc ln.c -o ../.rush/ln
gcc rm.c -o ../.rush/rm
gcc more.c -o ../.rush/more

cd ../src/
gcc rush.c read.c command.c info.c buildin.c buildout.c handle.c fileIO.c operator.c -lreadline -o ../rush

cd ..
./rush
