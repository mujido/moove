# Copyright (c) 2004 Kurt Stutsman. All rights reserved.
variant_test.exe: moove_lib
	g++ -g -I ../include -ansi -pedantic -o variant_test.exe variant_test.cpp ../src/moove.a

moove_lib:
	cd ../src && make

clean:
	cd ../src && make clean
	rm -f test.exe test.o
