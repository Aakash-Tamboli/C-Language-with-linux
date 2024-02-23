gcc -c *.c -I ../include
ar rcs ../lib/libtmterm.a *.o
rm *.o
