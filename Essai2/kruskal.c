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
int NMAX = 1000;
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
void addElt(int val, htbl* table2) {
    table2->parent = table2;
    table2->val = val;
    table2->rg = 0;
}
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
void initFromFile(FILE* fd) {
    for (int i = 0; i < NMAX; i++) {
        Station l;
        asprintf(&l.name, "Salut");
        // int y;
        fscanf(fd, "%d %d %d", &l.id, &l.x, &l.y);
        stations[i] = l;
    }
}
int power(int number, int nth) {
	if (nth < 0) return 1 / power(number, -nth);
	if (nth == 0) return 1;
	return number * power(number, nth - 1);
}
double distance(Station station1, Station station2) {
	if (station1.x == 0 && station1.y == 0) return (double) station2.y;
	if (station2.x == GRIDSIZE - 1 && station2.y == GRIDSIZE - 1) return (double) GRIDSIZE - station1.y;
	return sqrt((double) (power((station2.y - station1.y), 2) + power((station2.x - station1.x), 2)));
}
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
    if (t2b != t1b) {
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
}
void swp(edge* a, edge* b) {
    edge tmp = *a;
    *a = *b;
    *b = tmp;
}
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
void quicksort(Edgelist* el, int begin, int end) {
    if (end < begin) return;
    int p = partition(el, begin, end);
    quicksort(el, begin, p - 1);
    quicksort(el, p + 1, end);
}
Edgelist* cv_graph(Station* list) {
    Edgelist* edges = malloc(sizeof(Edgelist));
    edge* li = malloc(NMAX * NMAX * sizeof(edge));
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < NMAX; j++) {
            li[NMAX * i + j].s1 = i;
            li[NMAX * i + j].s2 = j;
            if (i < j) {
                float d = distance(list[i], list[j]);
                printf("%f\n", d);
                li[NMAX * i + j].w = d;
            } else {
                li[NMAX * i + j].w = INT_MAX;
            }
        }
    }
    edges->s = NMAX * NMAX;
    edges->list = li;
    return edges;
}
Solution* auxiliary(Edgelist* edges, htbl** table) {
    Solution* solution = NULL;
    for (int i = 0; i < edges->s; i++) {
        edge a = edges->list[i];
        htbl* v1 = find(table[a.s1]);
        htbl* v2 = find(table[a.s2]);
        if (v1 != v2) {
            unite(v1, v2);
            Solution* l2 = malloc(sizeof(Solution));
            l2->val = a;
            l2->next = solution;
            solution = l2;
        }
    }
    return solution;
}
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
Solution* copyEdgeList(Solution* li1) {
    if (li1 == NULL) return NULL;
    Solution* s2 = malloc(sizeof(Solution));
    s2->val = li1->val;
    s2->next = copyEdgeList(li1->next);
    return s2;
}
void freeEdgeList(Solution* sln) {
    if (sln == NULL) return;
    if (sln->next != NULL) freeEdgeList(sln->next);
    free(sln);
}
void print_solution(Solution* sl1) {
    Solution* sln = copyEdgeList(sl1);
    double ttl = 0;
    double maxl = 0;
    double minl = INT_MAX;
    double maxdep = 0;
    while(sln != NULL) {
        int s1 = sln->val.s1;
        int s2 = sln->val.s2;
        int w = sln->val.w;
        ttl += w;
        if (w > maxl) maxl = w;
        if (w < minl) minl = w;
        sln = sln->next;
    }
    freeEdgeList(sln);
}
int edgeListLength(Solution* s1) {
    Solution* sln = copyEdgeList(s1);
    if (sln == NULL) {
        free(sln);
        return 0;
    }
    int ttl = 1 + edgeListLength(sln->next);
    freeEdgeList(sln);
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
    freeEdgeList(sln2);
    return sln;
}
void print_edgelist(Edgelist* c) {
    for (int i = 0; i < c->s; i++) {
        printf("%d %d %lf\n", c->list[i].s1, c->list[i].s2, c->list[i].w);
    }
    printf("\n");
}
Solution* mapToS(Solution* sln) {
    Solution* t = copyEdgeList(sln);
    Solution* s = t;
    while (s != NULL) {
        s->val.s1 = stations[s->val.s1].id;
        s->val.s2 = stations[s->val.s2].id;
        s = s->next;
    }
    return t;
}
int main(int argc, char* argv[]) {
    Solution* res = malloc(sizeof(Solution));
    srand(time(NULL));
    if (argc >= 2) {
        FILE* fd;
        fd = fopen(argv[1], "r");
        fscanf(fd, "%d", &NMAX);
        stations = malloc(NMAX * sizeof(Station));
        initFromFile(fd);
        fclose(fd);
    } else {
        stations = malloc(NMAX * sizeof(Station));
        initrandom();
    }
    Edgelist* c = cv_graph(stations);
    res = kruskal(c);
    Solution* w = mapToS(res);
    Edgelist* sol = convert(w);
    writeEdgeListArray(sol, "kruskal.json", "kruskal_raw.txt");
	printf("\n");
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
	    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("Kruskal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    if(0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int x, y;
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
	if(0 != SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
    SDL_Event ev;
	int quit = 0;
	while (quit != 2) {
		while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_KEYUP:
                    if (quit == 0) {
                        while (res != NULL) {
                            SDL_RenderDrawLine(renderer, stations[res->val.s1].x * 10, stations[res->val.s1].y * 10, stations[res->val.s2].x * 10, stations[res->val.s2].y * 10);
                            printf("%d->%d:%lf\n", res->val.s1, res->val.s2, res->val.w);
                            res = res->next;
                        }
                        SDL_RenderPresent(renderer);
                        quit = 1;
                    } else {
                        quit = 2;
                    }
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
