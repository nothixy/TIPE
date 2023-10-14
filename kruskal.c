#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include "json.h"
#include <assert.h>
#define GRIDSIZE 100
#define MAXLENGTH 15

int NMAX = 100;

typedef struct Station {
	int x;
	int y;
	char* name;
	int id;
} Station;

typedef struct Pile {
	int val;
	struct Pile* next;
} Pile;

typedef struct htbl {
	int val;
	int rg;
	struct htbl* parent;
} htbl;

typedef struct Solution {
	struct edge val;
	struct Solution* next;
} Solution;

double** tableau_distances;
double** graphe;
char** grid;
Station* stations;

// Add element to hashtable
void addElt(int val, htbl* table2) {
	table2->parent = table2;
	table2->val = val;
	table2->rg = 0;
}

// Initialize array of random locations
void initrandom() {
	for (int i = 1; i < NMAX - 1; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		l.x = random() % GRIDSIZE;
		l.y = random() % GRIDSIZE;
		l.id = i;
		stations[i] = l;
	}
	Station l0;
	asprintf(&l0.name, "Salut");
	l0.x = 0;
	l0.y = 0;
	l0.id = 0;
	stations[0] = l0;
	Station lnmax;
	asprintf(&lnmax.name, "Salut");
	lnmax.x = GRIDSIZE - 1;
	lnmax.y = GRIDSIZE - 1;
	lnmax.id = NMAX - 1;
	stations[NMAX - 1] = lnmax;
}

// Initialize array of locations from file
void initFromFile(FILE* fd) {
	for (int i = 0; i < NMAX; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		fscanf(fd, "%d %d %d", &l.id, &l.x, &l.y);
		stations[i] = l;
	}
}

int power(int number, int nth) {
	if (nth < 0) return 1 / power(number, -nth);
	if (nth == 0) return 1;
	return number * power(number, nth - 1);
}

// Euclidian distance between two locations
double distance(Station station1, Station station2) {
	if (station1.x == 0 && station1.y == 0) return (double) station2.y;
	if (station2.x == GRIDSIZE - 1 && station2.y == GRIDSIZE - 1) return (double) GRIDSIZE - station1.y;
	return sqrt((double) (power((station2.y - station1.y), 2) + power((station2.x - station1.x), 2)));
}

// Make adjacency matrix of locations
void maketab() {
	tableau_distances = malloc(NMAX * sizeof(double*));
	for (int i = 0; i < NMAX; i++) {
		tableau_distances[i] = malloc(NMAX * sizeof(double));
		for (int j = 0; j < i; j++) {
			double d = distance(stations[j], stations[i]);
			tableau_distances[i][j] = tableau_distances[j][i] = d;
		}
	}
}

// The two following function are used for the union-find data structure
htbl* find(htbl* val) {
	htbl* par = val->parent;
	if (par == val) return val;
	htbl* par2 = find(par);
	val->parent = par2;
	return par2;
}

void unite(htbl* t1, htbl* t2) {
	htbl* t1b = find(t1);
	htbl* t2b = find(t2);
	if (t2b == t1b) {
		return;
	}
	int t1r = t1b->rg;
	int t2r = t2b->rg;
	if (t1r > t2r) {
		t2b->parent = t1b;
	} else {
		t1b->parent = t2b;
		if (t1r == t2r) {
			t2b->rg++;
		}
	}
}

void swp(edge* a, edge* b) {
	edge tmp = *a;
	*a = *b;
	*b = tmp;
}

// Move elements smaller than pivot before it
// And elements bigger after it
int partition(Edgelist* el, int begin, int end) {
	double pivot = el->list[end].w;
	int i = begin - 1;
	for (int j = begin; j < end; j++) {
		if (el->list[j].w < pivot) {
			i++;
			swp(&el->list[i], &el->list[j]);
		}
	}
	swp(&el->list[i + 1], &el->list[end]);
	return i + 1;
}

// O(n*log(n)) average sorting algorithm
void quicksort(Edgelist* el, int begin, int end) {
	if (end < begin) return;
	int p = partition(el, begin, end);
	quicksort(el, begin, p - 1);
	quicksort(el, p + 1, end);
}

void appendIJ(int i, int j, edge* li, Station* list) {
	li[NMAX * i + j].s1 = i;
	li[NMAX * i + j].s2 = j;
	if (i < j) {
		float d = distance(list[i], list[j]);
		li[NMAX * i + j].w = d;
	} else {
		li[NMAX * i + j].w = INT_MAX;
	}
}

// Convert location array to an array of edges
Edgelist* cv_graph(Station* list) {
	Edgelist* edges = malloc(sizeof(Edgelist));
	edge* li = malloc(NMAX * NMAX * sizeof(edge));
	for (int i = 0; i < NMAX; i++) {
		for (int j = 0; j < NMAX; j++) {
			appendIJ(i, j, li, list);
		}
	}
	edges->s = NMAX * NMAX;
	edges->list = li;
	return edges;
}

// For all edges sorted by length, if there is no path
// linking its vertices, add it to the list
Solution* auxiliary(Edgelist* edges, htbl** table) {
	Solution* solution = NULL;
	for (int i = 0; i < edges->s; i++) {
		edge a = edges->list[i];
		htbl* v1 = find(table[a.s1]);
		htbl* v2 = find(table[a.s2]);
		if (v1 == v2) {
			continue;
		}
		unite(v1, v2);
		Solution* l2 = malloc(sizeof(Solution));
		l2->val = a;
		l2->next = solution;
		solution = l2;
	}
	for (int i = 0; i < edges->s; i++) {
		free(table[i]);
	}
	free(table);
	return solution;
}

// Append all indexes to an array of hashtables,
// Sort the edges by length
// Apply the auxiliary function
Solution* kruskal(Edgelist* edges) {
	int x = edges->s;
	htbl** table = malloc(x * sizeof(htbl*));
	assert(table != NULL);
	for (int i = 0; i < x; i++) {
		table[i] = malloc(sizeof(htbl));
		addElt(i, table[i]);
	}
	quicksort(edges, 0, edges->s - 1);
	return auxiliary(edges, table);
}

// Duplicate a Solution
Solution* copySolution(Solution* li1) {
	if (li1 == NULL) return NULL;
	Solution* s2 = malloc(sizeof(Solution));
	s2->val = li1->val;
	s2->next = copySolution(li1->next);
	return s2;
}

void freeSolution(Solution* sln) {
	if (sln == NULL) return;
	if (sln->next != NULL) freeSolution(sln->next);
	free(sln);
}

void freeEdgeList(Edgelist* el) {
	if (el == NULL) return;
	if (el->list != NULL) free(el->list);
	free(el);
}

int solutionLength(Solution* s1) {
	Solution* sln = copySolution(s1);
	if (sln == NULL) {
		return 0;
	}
	int ttl = 1 + solutionLength(sln->next);
	freeSolution(sln);
	return ttl;
}

// Convert a linked list of edges to an array of edges
Edgelist* convert(Solution* s1) {
	Edgelist* sln = malloc(sizeof(Edgelist));
	sln->s = solutionLength(s1);
	sln->list = malloc(sln->s * sizeof(edge));
	for (int i = 0; i < sln->s; i++) {
		sln->list[i] = s1->val;
		s1 = s1->next;
	}
	return sln;
}

void moveValues(Solution* s) {
	while (s != NULL) {
		s->val.s1 = stations[s->val.s1].id;
		s->val.s2 = stations[s->val.s2].id;
		s = s->next;
	}
}

// Map edge vertices to their station ID
Solution* mapToS(Solution* sln) {
	Solution* t = copySolution(sln);
	moveValues(t);
	return t;
}

// Draw all edges
void handleKeyUp(int* quit, Solution* res, SDL_Renderer* renderer) {
	if (*quit == 0) {
		while (res != NULL) {
			SDL_RenderDrawLine(renderer, stations[res->val.s1].x * 10, stations[res->val.s1].y * 10, stations[res->val.s2].x * 10, stations[res->val.s2].y * 10);
			res = res->next;
		}
		SDL_RenderPresent(renderer);
		*quit = 1;
	} else {
		*quit = 2;
	}
}

void handleEvent(SDL_Event ev, int* quit, Solution* res, SDL_Renderer* renderer) {
	switch (ev.type) {
		case SDL_KEYUP:
			handleKeyUp(quit, res, renderer);
			break;
		case SDL_QUIT:
			*quit = 2;
			break;
		default:
			break;
	}
}

int main(int argc, char* argv[]) {
	Solution* res = NULL;
	srand(time(NULL));
	if (argc >= 2) {
		FILE* fd;
		fd = fopen(argv[1], "r");
		fscanf(fd, "%d", &NMAX);
		stations = malloc(NMAX * sizeof(Station));
		if (stations == NULL) {
			freeSolution(res);
			fclose(fd);
			return 2;
		}
		initFromFile(fd);
		fclose(fd);
	} else {
		stations = malloc(NMAX * sizeof(Station));
		if (stations == NULL) {
			freeSolution(res);
			return 2;
		}
		initrandom();
	}
	// Convert the array of locations to an array of edges
	Edgelist* c = cv_graph(stations);
	// Apply Kruskal on it
	res = kruskal(c);
	Solution* w = mapToS(res);
	Edgelist* sol = convert(w);
	// Save the result to a file
	writeEdgeListArray(sol, "kruskal.json", "kruskal_raw.txt");
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		goto Quit;
	}
	window = SDL_CreateWindow("Kruskal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		goto Quit;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		goto Quit;
	}
	if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {
		goto Quit;
	}
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	int x, y;
	SDL_GetWindowSize(window, &x, &y);
	// Draw all vertices
	for (int i = 0; i < NMAX; i++) {
		if (argc < 2) {
			if (i == 0) stations[i].x = stations[1].x;
			if (i == NMAX - 1) stations[i].x = stations[i - 1].x;
		}
		SDL_RenderDrawPoint(renderer, stations[i].x * 10, stations[i].y * 10);
		SDL_RenderDrawPoint(renderer, stations[i].x * 10 + 1, stations[i].y * 10 + 1);
		SDL_RenderDrawPoint(renderer, stations[i].x * 10 + 1, stations[i].y * 10 - 1);
		SDL_RenderDrawPoint(renderer, stations[i].x * 10 - 1, stations[i].y * 10 + 1);
		SDL_RenderDrawPoint(renderer, stations[i].x * 10 - 1, stations[i].y * 10 - 1);	
	}
	SDL_RenderPresent(renderer);
	SDL_Delay(500);
	if(0 != SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255)) {
		goto Quit;
	}
	SDL_Event ev;
	int quit = 0;
	while (quit != 2) {
		while (SDL_PollEvent(&ev)) {
			handleEvent(ev, &quit, res, renderer);
		}
	}
	Quit:
		// Free alloc'd memory
		if (res != NULL) freeSolution(res);
		if (stations != NULL) {
			for (int i = 0; i < NMAX; i++) free(stations[i].name);
			free(stations);
		}
		if (w != NULL) freeSolution(w);
		if (sol != NULL) freeEdgeList(sol);
		if (c != NULL) freeEdgeList(c);
		if(NULL != renderer)
			SDL_DestroyRenderer(renderer);
		if(NULL != window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}
