#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "json.h"
// #define NMAX 30
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

void initrandom();
void initFromFile(FILE* fd);
int power(int number, int nth);
void append(Solution* s, struct edge next);
double distance(Station station1, Station station2, bool horiz);
void maketab(bool horiz, float** graphe);
int hasValue(int x, int y);
void makeGraph();
int minimum(int len, float tab[], bool processed[]);
void dijkstra(int initial, Stack** stack, bool w);
void freeStack(Stack* p);
Solution* copyEdgeList(Solution* li1);
Stack* copyStack(Stack* li1);
int edgeListLength(Solution* s1);
Edgelist* convert(Solution* s1);
int main(int argc, char* argv[]);

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

int power(int number, int nth) {
	if (nth < 0) return 1 / power(number, -nth);
	if (nth == 0) return 1;
	return number * power(number, nth - 1);
}

void append(Solution* s, struct edge next) {
	while (s != NULL && s->next != NULL) {
		s = s->next;
	}
	s->next = malloc(sizeof(Solution));
	s->next->val = next;
	s->next->next = NULL;
}

double distance(Station station1, Station station2, bool horiz) {
	// printf("Pos1 : %d %d\n", station1.x, station1.y);
	// printf("Pos2 : %d %d\n", station2.x, station2.y);
	// if (station2.x == 0 && station2.y == 0) return (double) station1.y;
	// if (!horiz) {
		if (station1.x == 0 && station1.y == 0) return (double) station2.y;
		if (station2.x == GRIDSIZE - 1 && station2.y == GRIDSIZE - 1) return (double) GRIDSIZE - station1.y;
	// } else {
		// if (station1.y == 0 && station1.x == 0) return (double) station2.x;
		// if (station2.y == GRIDSIZE - 1 && station2.x == GRIDSIZE - 1) return (double) GRIDSIZE - station1.x;
	// }
	// if (station1.x == GRIDSIZE - 1 && station1.y == GRIDSIZE - 1) return (double) GRIDSIZE - station2.x;
	return sqrt((double) (power((station2.y - station1.y), 2) + power((station2.x - station1.x), 2)));
}

void maketab(bool horiz, float** graphe) {
	tableau_distances = malloc(NMAX * sizeof(float*));
	for (int i = 0; i < NMAX; i++) {
		tableau_distances[i] = malloc(NMAX * sizeof(float));
		for (int j = 0; j < i; j++) {
			// printf("%d %d\n", i, j);
			float d = distance(stations[j], stations[i], horiz);
			// printf("%d->%d:%f\n", i, j, d);
			tableau_distances[i][j] = tableau_distances[j][i] = d;
		}
	}
}

int hasValue(int x, int y) {
	for (int i = 0; i < NMAX; i++) {
		if (stations[i].x == x && stations[i].y == y) return i;
	}
	return -1;
}

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

void freeStack(Stack* p) {
	if (p == NULL) return;
	freeStack(p->next);
	free(p);
}

Solution* copyEdgeList(Solution* li1) {
    if (li1 == NULL) return NULL;
    Solution* s2 = malloc(sizeof(Solution));
    s2->val = li1->val;
    s2->next = copyEdgeList(li1->next);
    return s2;
}

Stack* copyStack(Stack* li1) {
    if (li1 == NULL) return NULL;
    Stack* s2 = malloc(sizeof(Stack));
    s2->val = li1->val;
    s2->next = copyStack(li1->next);
    return s2;
}

int edgeListLength(Solution* s1) {
    Solution* sln = copyEdgeList(s1);
    if (sln == NULL) {
        free(sln);
        return 0;
    }
    int ttl = 1 + edgeListLength(sln->next);
    free(sln);
    return ttl;
}

Edgelist* convert(Solution* s1) {
    Edgelist* sln = malloc(sizeof(Edgelist));
    sln->s = edgeListLength(s1);
    sln->list = malloc(sln->s * sizeof(edge));
    Solution* sln2 = copyEdgeList(s1);
    for (int i = 0; i < sln->s; i++) {
        sln->list[i] = sln2->val;
        sln2 = sln2->next;
    }
    free(sln2);
    return sln;
}

int main(int argc, char* argv[]) {
	SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("Shortest path", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
    if(NULL == window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
	if(0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
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
	// for (int i = 0; i < NMAX; i++) {
	// 	printf("%d %d %d\n", i, stations[i].x, stations[i].y);
	// }
	maketab(false, graphe);
	// memcpy(graphe, graphe2, NMAX * NMAX * sizeof(float));
	// for (int i = 0; i < NMAX; i++) {
	// 	for (int j = 0; j < NMAX; j++) {
	// 		printf("%f\t", tableau_distances[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");
	// for (int i = 0; i < GRIDSIZE; i++) {
	//  	for (int j = 0; j < GRIDSIZE; j++) {
	//  		int x = hasValue(i, j);
	// 		if (x != -1) printf("%d", x); else printf(" ");
	// 	}
	// 	printf("\n");
	// }
	makeGraph();
	// maketab(true, graphe2);
	// makeGraph(graphe2);
	// for (int i = 0; i < NMAX; i++) {
	// 	for (int j = 0; j < NMAX; j++) {
	// 		printf("%f\t", graphe[i][j]);
	// 	}
	// 	printf("\n");
	// }
	Stack* liste = malloc(sizeof(Stack));
	liste->val = -1;
	liste->next = NULL;
	int x, y;
	dijkstra(0, &liste, true);
	// Stack* restant = malloc(sizeof(Stack));
	// restant->val = -1;
	// while (restant->val != -1) {
	// 	for (int i = 0; i < NMAX; i++) {
	// 		Stack* k = copyStack(liste);
	// 		bool append = false;
	// 		while (k != NULL) {
	// 			if (k->val == i) append = true;
	// 			k = k->next;
	// 		}
	// 		if (append) {
	// 			Stack* new = malloc(sizeof(Stack));
	// 			new->val = i;
	// 			new->next = restant;
	// 			restant = new;
	// 		}
	// 	}
	// }
	SDL_GetWindowSize(window, &x, &y);
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
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
	SDL_Event ev;
	bool w = true;
	int quit = 0;
	while (quit != 2) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_KEYUP:
				// case SDL_KEYDOWN:
					if (quit == 0) {
						if (argc < 2) 
							SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, 0, stations[liste->val].x * 10, stations[liste->val].y * 10);
						else
							SDL_RenderDrawLine(renderer, stations[0].x * 10, stations[0].y * 10, stations[liste->val].x * 10, stations[liste->val].y * 10);
						// edge t = {.s1 = liste->val, .s2 = liste->next->val, .w = distance(stations[liste->val], stations[liste->next->val], false)};
						// append(sln, t); 
						// stations[liste->val].x = 10000 * GRIDSIZE;
						// stations[liste->val].y = 10000 * GRIDSIZE;
						Solution* sln = malloc(sizeof(Solution));
						printf("__sln__\n");
						printf("0 %d\n", liste->val);
						// printf("d %d\n", (int) stations[liste->val].y);
						edge t1;
						t1.s1 = 0;
						t1.s2 = liste->val;
						t1.w = (double) stations[liste->val].y;
						sln->val = t1;
						sln->next = NULL;
						// printf("No problem\n");
						// append(sln, t1);
						// printf("No problem v2\n");
						while (liste != NULL && liste->next != NULL && liste->next->next != NULL) {
							// printf("Hello\n");
							// printf("%d %d\n", liste->val, liste->next->val);
							SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, stations[liste->val].y * 10, stations[liste->next->val].x * 10, stations[liste->next->val].y * 10);
							printf("%d %d\n", liste->val, liste->next->val);
							double d = distance(stations[liste->val], stations[liste->next->val], false);
							int s1 = liste->val;
							int s2 = liste->next->val;
							edge t;
							t.w = d;
							t.s1 = s1;
							t.s2 = s2;
							append(sln, t);
							// printf("Removing %d\n", liste->val);
							// stations[liste->val].x = 100 * GRIDSIZE;
							// stations[liste->val].y = 100 * GRIDSIZE;
							liste = liste->next;
							// printf("Hello2\n");
						}
						edge t2;
						t2.s1 = liste->val;
						t2.s2 = NMAX - 1;
						t2.w = (double) (GRIDSIZE - 1) - stations[liste->val].y;
						append(sln, t2);
						if (argc < 2) SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, stations[liste->val].y * 10, stations[liste->val].x * 10, (GRIDSIZE - 1) * 10);
						// printf("Removing %d\n", liste->val);
						// stations[liste->val].x = 100 * GRIDSIZE;
						// stations[liste->val].y = 100 * GRIDSIZE;
						// printf("works\n");
						SDL_RenderPresent(renderer);
						// SDL_Delay(1000);
						quit = 1;
						Edgelist* el = convert(sln);
						writeEdgeListArray(el, "shortestpath.json", "shortestpath_raw.txt");
						// maketab(false, graphe);
						// makeGraph(&graphe);
						// dijkstra(0, &liste, false);
					} else {
						quit = 2;
					}
					w = false;
					break;
				case SDL_QUIT:
					quit = 2;
					break;
				default:
					break;
			}
		}
	}
	Quit:
		if(NULL != renderer)
			SDL_DestroyRenderer(renderer);
		if(NULL != window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}
