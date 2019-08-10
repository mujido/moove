# Copyright (c) 2004 Kurt Stutsman. All rights reserved.
CXXFLAGS = -pipe -g -fno-inline -I ../../include -DCHECK_EXCEPTS

all: parser main.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o syntax main.o ../../src/moove_parser.a

dmalloc: parser main.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o syntax main.o \
		/usr/share/doc/dmalloc/c++/dmallocc.cc ../../src/moove_parser.a

parser:
	cd ../../src && make parser

clean:
	rm -f *.o *.exe