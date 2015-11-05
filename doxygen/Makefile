# Copyright (c) 2004 Kurt Stutsman. All rights reserved.
examples: viewer file_ext.png
	mv ../test-scripts/*.png ./

viewer:
	cd ../examples/ast_viewer && make

%.png: ../test-scripts/%.moo
	../examples/ast_viewer/viewer < '$<' | dot -Tpng -o '$@'

clean:
	rm -rf html latex *.png
