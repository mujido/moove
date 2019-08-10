TOPDIR = ${PWD}
export TOPDIR

all: moove

moove:
	cd src && make

tags:
	ctags-exuberant -e -R

clean:
	cd src && make clean

