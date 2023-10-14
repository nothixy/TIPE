#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include <assert.h>
#include "json.h"
#define GRIDSIZE 100
#define MAXLENGTH 15
#define K 13
#define SDL_RED 255, 0, 0
#define SDL_FUCHSIA 255, 0, 127
#define SDL_MAGENTA 255, 0, 255
#define SDL_MAUVE 127, 0, 255
#define SDL_BLUE 0, 0, 255
#define SDL_CYAN 0, 127, 255
#define SDL_TURQUOISE 0, 255, 255
#define SDL_TEAL 0, 255, 127
#define SDL_GREEN 0, 255, 0
#define SDL_CHARTREUSE 127, 255, 0
#define SDL_YELLOW 255, 255, 0
#define SDL_ORANGE 255, 127, 0
#define SDL_BLACK 0, 0, 0
#define SDL_GREY 127, 127, 0
#define SDL_WHITE 255, 255, 255
#define SDL_COLOR_COUNT 15
#define SDL_COLORS (int[]) {SDL_RED, SDL_FUCHSIA, SDL_MAGENTA, SDL_MAUVE, SDL_BLUE, SDL_CYAN, SDL_TURQUOISE, SDL_GREEN, SDL_CHARTREUSE, SDL_YELLOW, SDL_ORANGE, SDL_BLACK, SDL_GREY, SDL_WHITE}
#define RED 0
#define FUCHSIA 1
#define MAGENTA 2
#define MAUVE 3
#define BLUE 4
#define CYAN 5
#define TURQUOISE 6
#define GREEN 7
#define CHARTREUSE 8
#define YELLOW 9
#define ORANGE 10
#define BLACK 11
#define GREY 12
#define WHITE 13
#define GET_SDL_COLOR(X) SDL_COLORS[3 * X], SDL_COLORS[3 * X + 1], SDL_COLORS[3 * X + 2]

int NMAX = 3000;

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

typedef struct Couple {
    double x0;
    double y0;
	int id;
} Couple;

typedef struct StackCouple {
    Couple val;
    struct StackCouple* next;
} StackCouple;

typedef struct LineStack {
    Couple line;
    struct LineStack* next;
} LineStack;

typedef struct Solution {
	edge val;
	struct Solution* next;
} Solution;

Station* stations;
SDL_Renderer *renderer = NULL;
int sigma[13];
int depth = 0;

// Initialize array of random locations
void initrandom(Station* stations) {
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
void initFromFile(Station* stations, FILE* fd) {
	for (int i = 0; i < NMAX; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		fscanf(fd, "%d %d", &l.x, &l.y);
		l.id = i;
		stations[i] = l;
	}
}

// If the random location chosen in the next function is already used, choose another one
void redoIfExists(Station* stations, StackCouple** sets, int* r, int i) {
	for (int j = 0; j < i; j++) {
		if (stations[*r].x == sets[j]->val.x0 && stations[*r].y == sets[j]->val.y0) {
			*r = random() % NMAX;
			j = 0;
		}
	}
}

// Place K random locations to an array
void getKRandom(Station* stations, StackCouple** sets) {
	for (int i = 0; i < K; i++) {
		int r = random() % NMAX;
		redoIfExists(stations, sets, &r, i);
		sets[i] = malloc(sizeof(StackCouple));
		sets[i]->val.x0 = stations[r].x;
		sets[i]->val.y0 = stations[r].y;
		sets[i]->val.id = r;
		sets[i]->next = NULL;
	}
}

// Euclidian distance
double distance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Add an element to a stack
void push(Stack* s, int val) {
	Stack* new = malloc(sizeof(Stack));
	new->val = val;
	new->next = s;
	s = new;
}

// Add a Couple to a stack
void push_alt(StackCouple** s, float x0, float y0, int r) {
	StackCouple* new = malloc(sizeof(StackCouple));
	new->val.x0 = x0;
	new->val.y0 = y0;
	new->val.id = r;
	new->next = NULL;
	if (*s == NULL) {
		*s = new;
	} else {
		StackCouple* set = *s;
		while (set->next != NULL) {
			set = set->next;
		}
		set->next = new;
	}
}

// Pop last element from a stack
int pop(Stack* s) {
	int val = s->val;
	s = s->next;
	return val;
}

void freeSet(StackCouple* s) {
	if (s != NULL) {
		freeSet(s->next);
		free(s);
	}
}

// Classify all stations into the set where its centroid is the closest
void classify(Station* stations, StackCouple* sets[], int count) {
	for (int i = 0; i < count; i++) {
		double min = INT_MAX;
		int minset = 0;
		for (int j = 0; j < K; j++) {
			double d = distance(stations[i].x, stations[i].y, sets[j]->val.x0, sets[j]->val.y0);
			if (d >= min) continue;
			min = d;
			minset = j;
		}
		push_alt(&sets[minset], stations[i].x, stations[i].y, i);
	}
}

// Get the new center of all sets
void getSetAvg(StackCouple* sets[]) {
	for (int i = 0; i < K; i++) {
		double sumX = 0;
		double sumY = 0;
		int count = 0;
		StackCouple* s = sets[i];
		s = s->next;
		while (s != NULL) {
			sumX += s->val.x0;
			sumY += s->val.y0;
			count++;
			s = s->next;
		}
		assert(count != 0);
		sumX /= count;
		sumY /= count;
		freeSet(sets[i]->next);
		sets[i]->next = NULL;
		sets[i]->val.x0 = sumX;
		sets[i]->val.y0 = sumY;
	}
}

// Initialize a permutation
void initSigma() {
	for (int i = 0; i < K; i++) sigma[i] = i;
	for (int i = K - 1; i >= 1; i--) {
		int j = rand() % i;
		int s = sigma[i];
		sigma[i] = sigma[j];
		sigma[j] = s;
	}
}

void handleSDLReturn(StackCouple* sets[], Station** sub) {
	getSetAvg(sets);
	classify(sub[0], sets, NMAX);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	for (int j = 0; j < K; j++) {
		if(0 != SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(sigma[j]), 255)) {
			return;
		}
		StackCouple* s = sets[j];
		while (s != NULL) {
			int x = s->val.x0;
			int y = s->val.y0;
			SDL_RenderDrawPoint(renderer, 10 * x, 10 * y);
			SDL_RenderDrawPoint(renderer, 10 * x + 1, 10 * y + 1);
			SDL_RenderDrawPoint(renderer, 10 * x - 1, 10 * y - 1);
			SDL_RenderDrawPoint(renderer, 10 * x + 1, 10 * y - 1);
			SDL_RenderDrawPoint(renderer, 10 * x - 1, 10 * y + 1);
			s = s->next;
		}
	}
	SDL_RenderPresent(renderer);
}

void handleSDLKeyUp(SDL_Event ev, int* done, StackCouple* sets[], Station** sub) {
	switch(ev.key.keysym.sym) {
		case SDLK_ESCAPE: {
			*done = 2;
			break;
		}
		case SDLK_RETURN: {
			handleSDLReturn(sets, sub);
			break;
		}
	}
}

void handleSDLEvent(SDL_Event ev, int* done, StackCouple* sets[], Station** sub) {
	switch (ev.type) {
		case SDL_KEYUP: {
			handleSDLKeyUp(ev, done, sets, sub);
			break;
		}
		case SDL_QUIT: {
			*done = 2;
			break;
		}
		default: {
			break;
		}
	}
}

void loop(Station** sub, StackCouple* sets[], int* len, int count, int savedepth, char* outScheme, bool export) {
	int done = 0;
	SDL_Event ev;
	if(SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(WHITE), 255) != 0) {
		return;
	}
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < len[i]; j++) {
			int x = sub[i][j].x;
			int y = sub[i][j].y;
			SDL_RenderDrawPoint(renderer, 10 * x, 10 * y);
			SDL_RenderDrawPoint(renderer, 10 * x + 1, 10 * y + 1);
			SDL_RenderDrawPoint(renderer, 10 * x - 1, 10 * y - 1);
			SDL_RenderDrawPoint(renderer, 10 * x + 1, 10 * y - 1);
			SDL_RenderDrawPoint(renderer, 10 * x - 1, 10 * y + 1);
		}
	}
	SDL_RenderPresent(renderer);
	done = 0;
	while (done != 2) {
		while (SDL_PollEvent(&ev)) {
			handleSDLEvent(ev, &done, sets, sub);
		}
	}
}

void freeStackCouple(StackCouple* s) {
	if (s == NULL) return;
	if (s->next != NULL) freeStackCouple(s->next);
	free(s);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	initSigma();
	int x, y;
	SDL_Window *window = NULL;
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        goto Quit;
    }
    window = SDL_CreateWindow("Algo1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
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
	StackCouple** sets = malloc(K * sizeof(StackCouple*));
	if (sets == NULL) return 2;
	Station* stations;
	if (argc >= 3) {
		FILE* fd;
		fd = fopen(argv[2], "r");
		fscanf(fd, "%d", &NMAX);
		stations = malloc(NMAX * sizeof(Station));
		if (stations == NULL) {
			free(sets);
			return 2;
		}
		initFromFile(stations, fd);
	} else {
		stations = malloc(NMAX * sizeof(Station));
		if (stations == NULL) {
			free(sets);
			return 2;
		}
		initrandom(stations);
	}
	getKRandom(stations, sets);
	classify(stations, sets, NMAX);
	int done = 0;
	int quit = 0;
	int firstrun = 0;
	int c[1] = {NMAX};
	if (argc >= 2) {
		loop(&stations, sets, c, 1, 0, argv[1], true);
	} else {
		loop(&stations, sets, c, 1, 0, NULL, false);
	}
	Quit:
		if (sets != NULL) {
			for (int i = 0; i < K; i++) freeStackCouple(sets[i]);
			free(sets);
		}
		if (stations != NULL) {
			for (int i = 0; i < NMAX; i++) free(stations[i].name);
			free(stations);
		}
		return 0;
}