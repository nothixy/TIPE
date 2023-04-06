all: clean kruskal shortestpath algo1

kruskal:
	@echo "Building Kruskal"
	gcc -lm $$(sdl2-config --cflags --libs) kruskal.c -o kruskal

shortestpath:
	@echo "Building Shortest path"
	gcc -lm $$(sdl2-config --cflags --libs) shortestpath.c -o shortestpath

algo1:
	@echo "Building mixed algorithm"
	gcc -g -lm $$(sdl2-config --cflags --libs) algo1.c -o mixed

clean:
	@echo "Clean"
	rm -rf kruskal shortestpath mixed
