# Copyright (c) 2004 Kurt Stutsman. All rights reserved.
CXXFLAGS = -pipe -g -I ../../include

EXAMPLE_DIR = ../../test-scripts
EXAMPLE_SCRIPTS = ${EXAMPLE_DIR}/strcmp.moo ${EXAMPLE_DIR}/file_ext.moo

viewer: parser main.o graphviz_visitor.o
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o viewer main.o graphviz_visitor.o \
	       ../../src/moove_parser.a

parser:
	cd ../../src && make parser

examples: viewer ${EXAMPLE_SCRIPTS}
	for f in $?; do \
		base="$${f%.moo}"; \
		base="$${base##*/}"; \
		./viewer < "$$f" | dot -Tpng -o "$$base.png"; \
	done

clean:
	rm -f *.o *.exe