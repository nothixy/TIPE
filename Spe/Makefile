all: clean kruskal shortestpath algo1

kruskal:
	@echo "Building Kruskal"
	gcc -g -lm $$(sdl2-config --cflags --libs) kruskal.c -o kruskal

shortestpath:
	@echo "Building Shortest path"
	gcc -g -lm $$(sdl2-config --cflags --libs) shortestpath.c -o shortestpath

algo1:
	@echo "Building k-average algorithm"
	gcc -g -lm $$(sdl2-config --cflags --libs) kavg.c -o kavg

clean:
	@echo "Clean"
	rm -rf kruskal shortestpath kavg
