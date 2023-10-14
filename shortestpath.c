#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "json.h"
#define GRIDSIZE 100
#define MAXLENGTH 15

int NMAX = 1000;

typedef struct Station {
	int x;
	int y;
	char* name;
	int id;
} Station;

typedef struct Stack {
	int val;
	struct Stack* next;
} Stack;

typedef struct Solution {
	struct edge val;
	struct Solution* next;
} Solution;

float** tableau_distances;
float** graphe;
float** graphe2;
char** grid;
Station* stations;

// Initialize a random array of locations
void initrandom() {
	stations = malloc(NMAX * sizeof(Station));
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

// Initialize an array of locations from file
void initFromFile(FILE* fd) {
	fscanf(fd, "%d", &NMAX);
	stations = malloc(NMAX * sizeof(Station));
	for (int i = 0; i < NMAX; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		fscanf(fd, "%d %d %d", &l.id, &l.x, &l.y);
		stations[i] = l;
	}
}

// Could have been better but works as is
int power(int number, int nth) {
	if (nth < 0) return 1 / power(number, -nth);
	if (nth == 0) return 1;
	return number * power(number, nth - 1);
}

// Append a node to a solution
void append(Solution* s, struct edge next) {
	while (s != NULL && s->next != NULL) {
		s = s->next;
	}
	s->next = malloc(sizeof(Solution));
	s->next->val = next;
	s->next->next = NULL;
}

// Euclidian distance between two locations (checks if at the border)
double distance(Station station1, Station station2) {
	if (station1.x == 0 && station1.y == 0) return (double) station2.y;
	if (station2.x == GRIDSIZE - 1 && station2.y == GRIDSIZE - 1) return (double) GRIDSIZE - station1.y;
	return sqrt((double) (power((station2.y - station1.y), 2) + power((station2.x - station1.x), 2)));
}

// Make an adjacency matrix from an array of locations
void maketab(float** graphe) {
	tableau_distances = malloc(NMAX * sizeof(float*));
	for (int i = 0; i < NMAX; i++) {
		tableau_distances[i] = malloc(NMAX * sizeof(float));
		for (int j = 0; j < i; j++) {
			float d = distance(stations[j], stations[i]);
			tableau_distances[i][j] = tableau_distances[j][i] = d;
		}
	}
}

// If distance between two locations is greater than MAXLENGTH, remove the corresponding edge
void makeGraph() {
	graphe = malloc(NMAX * sizeof(float*));
	for (int i = 0; i < NMAX; i++) {
		graphe[i] = malloc(NMAX * sizeof(float));
		graphe[i][i] = 0;
		for (int j = 0; j < i; j++) {
			if (tableau_distances[i][j] < MAXLENGTH)
				graphe[i][j] = graphe[j][i] = tableau_distances[i][j];
			else
				graphe[i][j] = graphe[j][i] = MAXLENGTH * NMAX;
		}
	}
}

// Returns index of the minimum value of an array
int minimum(int len, float tab[], bool processed[]) {
	float min = MAXLENGTH * NMAX;
	int min_index;
	for (int i = 0; i < len; i++) {
		if (!processed[i] && tab[i] <= min) {
			min = tab[i];
			min_index = i;
		}
	}
	return min_index;
}

void freeStack(Stack* p) {
	if (p == NULL) return;
	if (p->next != NULL) freeStack(p->next);
	free(p);
}

// Classic Dijkstra function
void dijkstra(int initial, Stack** stack, bool w) {
	float dist[NMAX];
	int prev[NMAX];
	bool processed[NMAX];
	for (int i = 0; i < NMAX; i++) {
		dist[i] = MAXLENGTH * NMAX;
		prev[i] = 0;
		processed[i] = false;
	}
	dist[initial] = 0;
	for (int i = 0; i < NMAX; i++) {
		int u = minimum(NMAX, dist, processed);
		processed[u] = true;
		for (int v = 0; v < NMAX; v++) {
			if (!processed[v] && dist[u] + graphe[u][v] < dist[v] && graphe[u][v] != MAXLENGTH * NMAX && dist[u] != MAXLENGTH * NMAX) {
				dist[v] = dist[u] + graphe[u][v];
				prev[v] = u;
			}
		}
	}
	for (int i = 0; i < NMAX; i++) {
		int pre = i;
		while (pre != 0) {
			if (i == NMAX - 1) {
				Stack* new = malloc(sizeof(Stack));
				new->val = pre;
				new->next = *stack;
				*stack = new;
			}
			pre = prev[pre];
		}
	}
}

// Duplicate an Solution
Solution* copyEdgeList(Solution* li1) {
    if (li1 == NULL) return NULL;
    Solution* s2 = malloc(sizeof(Solution));
    s2->val = li1->val;
    s2->next = copyEdgeList(li1->next);
    return s2;
}

// Duplicate a stack
Stack* copyStack(Stack* li1) {
    if (li1 == NULL) return NULL;
    Stack* s2 = malloc(sizeof(Stack));
    s2->val = li1->val;
    s2->next = copyStack(li1->next);
    return s2;
}

void freeSolution(Solution* sln) {
    if (sln == NULL) return;
    if (sln->next != NULL) freeSolution(sln->next);
    free(sln);
}

int edgeListLength(Solution* s1) {
    Solution* sln = copyEdgeList(s1);
    if (sln == NULL) {
        return 0;
    }
    int ttl = 1 + edgeListLength(sln->next);
    freeSolution(sln);
    return ttl;
}

// Convert a solution to an EdgeList
Edgelist* convert(Solution* s1) {
    Edgelist* el = malloc(sizeof(Edgelist));
	if (el == NULL) {
		return NULL;
	}
    el->s = edgeListLength(s1);
	if (el->s == 0) {
		free(el);
		return NULL;
	}
    el->list = malloc(el->s * sizeof(edge));
	if (el->list == NULL) {
		free(el);
		return NULL;
	}
    for (int i = 0; i < el->s; i++) {
        el->list[i] = s1->val;
        s1 = s1->next;
    }
    return el;
}

void freeEdgeList(Edgelist* el) {
	if (el == NULL) return;
	if (el->list != NULL)
		free(el->list);
	free(el);
}

void handleKeyUp(Stack* liste, int* quit, int argc, bool* w, SDL_Renderer* renderer) {
	if (*quit != 0) {
		*quit = 2;
		w = false;
		return;
	}
	if (argc < 2)
		SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, 0, stations[liste->val].x * 10, stations[liste->val].y * 10);
	else
		SDL_RenderDrawLine(renderer, stations[0].x * 10, stations[0].y * 10, stations[liste->val].x * 10, stations[liste->val].y * 10);
	Solution* sln = malloc(sizeof(Solution));
	edge t1;
	t1.s1 = 0;
	t1.s2 = liste->val;
	t1.w = (double) stations[liste->val].y;
	sln->val = t1;
	sln->next = NULL;
	// Ugly looping through edges to display them
	while (liste != NULL && liste->next != NULL && liste->next->next != NULL) {
		SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, stations[liste->val].y * 10, stations[liste->next->val].x * 10, stations[liste->next->val].y * 10);
		double d = distance(stations[liste->val], stations[liste->next->val]);
		int s1 = liste->val;
		int s2 = liste->next->val;
		edge t;
		t.w = d;
		t.s1 = s1;
		t.s2 = s2;
		append(sln, t);
		liste = liste->next;
	}
	edge t2;
	t2.s1 = liste->val;
	t2.s2 = NMAX - 1;
	t2.w = (double) (GRIDSIZE - 1) - stations[liste->val].y;
	append(sln, t2);
	if (argc < 2) SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, stations[liste->val].y * 10, stations[liste->val].x * 10, (GRIDSIZE - 1) * 10);
	SDL_RenderPresent(renderer);
	*quit = 1;
	Edgelist* el = convert(sln);
	freeSolution(sln);
	// Write data to a json file
	writeEdgeListArray(el, "shortestpath.json", "shortestpath_raw.txt");
	freeEdgeList(el);
	w = false;
}

void handleEvent(SDL_Event ev, Stack* liste, int* quit, int argc, bool* w, SDL_Renderer* renderer) {
	switch (ev.type) {
		case SDL_KEYUP:
			handleKeyUp(liste, quit, argc, w, renderer);
			break;
		case SDL_QUIT:
			*quit = 2;
			break;
		default:
			break;
	}
}

int main(int argc, char* argv[]) {
	SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        goto Quit;
    }
    window = SDL_CreateWindow("Shortest path", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
    if(NULL == window) {
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer) {
        goto Quit;
    }
	if(0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
        goto Quit;
    }
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	grid = malloc(GRIDSIZE * sizeof(char*));
	for (int i = 0; i < GRIDSIZE; i++) {
		grid[i] = malloc(GRIDSIZE * sizeof(char));
	}
	srand(time(NULL));
	if (argc >= 2) {
		FILE* fd;
		fd = fopen(argv[1], "r");
		initFromFile(fd);
		fclose(fd);
	} else {
		initrandom();
	}
	maketab(graphe);
	makeGraph();
	Stack* liste = malloc(sizeof(Stack));
	liste->val = -1;
	liste->next = NULL;
	int x, y;
	dijkstra(0, &liste, true);
	SDL_GetWindowSize(window, &x, &y);
	// Draw all the locations
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
	bool w = true;
	int quit = 0;
	while (quit != 2) {
		while(SDL_PollEvent(&ev)) {
			handleEvent(ev, liste, &quit, argc, &w, renderer);
		}
	}
	Quit:
		freeStack(liste);
		if(NULL != renderer)
			SDL_DestroyRenderer(renderer);
		if(NULL != window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}
