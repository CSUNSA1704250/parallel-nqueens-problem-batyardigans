image:
	g++ -o bin/files src/files.cpp
	./bin/files
	dot files/graph.dot -Tpng -o files/graph.png