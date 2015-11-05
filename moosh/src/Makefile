ifndef MOOVE
$(error MOOVE must be set to the base directory of the MOOVE library)
endif

ifndef BOOST
$(error BOOST must be set to the base directory of the Boost library)
endif

INCLUDE_DIRS = -I ../include -I ${MOOVE}/include -I ${BOOST}

moosh_CPP_FILES = msg_handler.cpp moosh_builtins.cpp moosh.cpp
moosh_O_FILES = $(moosh_CPP_FILES:.cpp=.o)

disasm_CPP_FILES = msg_handler.cpp disasm.cpp
disasm_O_FILES = $(disasm_CPP_FILES:.cpp=.o)

CPP_FILES = ${moosh_CPP_FILES} ${disasm_CPP_FILES}
DEPENDS = $(CPP_FILES:.cpp=.d)

ifndef CXXFLAGS
  ifndef STRICT
    CXXFLAGS = -pipe -g -fno-inline
  else
    CXXFLAGS = -pipe -g -fno-inline -Wall -pedantic
  endif
endif

.PHONY: all
all: moosh disasm

moosh: ${moosh_O_FILES} ${MOOVE}/src/moove.a
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -o moosh $^

disasm: ${disasm_O_FILES} ${MOOVE}/src/moove.a
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -o disasm $^

.PHONY: clean
clean:
	rm -f ${DEPENDS} *.o moosh disasm

%.o: %.cpp
	${CXX} -c ${CPPFLAGS} ${CXXFLAGS} ${INCLUDE_DIRS} $<

%.d: %.cpp
	set -e; \
	${CXX} -MM -MG ${CPPFLAGS} ${CXXFLAGS} ${INCLUDE_DIRS} $< > $@.$$$$; \
	sed -e 's#\($*\)\.o[ :]*#\1.o $@: #g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
    include ${DEPENDS}
endif
