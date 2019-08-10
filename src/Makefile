# Copyright (c) 2004 Kurt Stutsman. All rights reserved.

ifndef TOPDIR
$(error TOPDIR must be defined)
endif

ifndef BOOST
$(error BOOST must be set to the base directory of the boost library)
endif

INCLUDES = -I ${TOPDIR}/include -I ${BOOST} -I . 

CPP_FILES = reply.cpp type_registry.cpp intvar.cpp realvar.cpp strvar.cpp op_map.cpp \
            op_package.cpp listvar.cpp compiler.cpp disassembler.cpp codevect.cpp \
            opcodes.cpp symbol_table.cpp bc_program.cpp string_utils.cpp \
            label_linker.cpp immediate.cpp flow_ctrl.cpp interpreter.cpp \
            exec_state.cpp task_frame.cpp builtin_registry.cpp \
            moove.tab.cpp parser_state.cpp expr_ast.cpp stmt_ast.cpp parser.cpp

DERIVED = lexer_symbols.hpp moove.tab.cpp

O_FILES = $(CPP_FILES:.cpp=.o)

DEPENDS = $(CPP_FILES:.cpp=.d)

ifndef CXXFLAGS

ifndef STRICT
CXXFLAGS = -pipe -g -fno-inline
else
CXXFLAGS = -pipe -g -fno-inline -Wall -pedantic
endif

endif

override CXXFLAGS += ${INCLUDES}

.PHONY: all
all: moove.a

lexer_symbols.hpp: lexer_symbols.hpp.gperf
	gperf -S 1 -t -D -C -L 'C++' -E -Z 'LexerSymbolTable' \
		lexer_symbols.hpp.gperf | \
		sed -e 's/strcmp/std::strcmp/g' > lexer_symbols.hpp

moove.tab.cpp: moove.y
	bison -o moove.tab.cpp moove.y

moove.a: ${O_FILES}
	ar rcs moove.a $?

.PHONY: clean
clean:
	rm -f ${O_FILES} ${DEPENDS} moove.a moove.tab.* lexer_symbols.hpp

%.o: %.cpp ${DERIVED}
	${CXX} -c ${CPPFLAGS} ${CXXFLAGS} $<

%.d: %.cpp ${DERIVED}
	set -e; \
	${CXX} -MM -MG ${CPPFLAGS} ${CXXFLAGS} $< > $@.$$$$; \
	sed -e 's#\($*\)\.o[ :]*#\1.o $@: #g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
    -include ${DEPENDS}
endif

