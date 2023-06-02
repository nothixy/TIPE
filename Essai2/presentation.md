---
author:
- Valentin FOULON - N° 29836
date: 2023-06-01
subtitle: Comment créer un réseau de transports publics dans une ville
  afin de minimiser les temps de déplacement ?
title: TIPE - La Ville
---

::: frame
:::

::: frame
Table des matières
:::

# Choix du thème

# Définitions

::: frame
Définitions

::: block
Définition Graphe : on note $G=(S, A)$ un graphe ayant pour sommets
l'ensemble $S$ et pour arêtes l'ensemble $A$. On désigne par $|A|$ et
$|S|$ les cardinaux respectifs de $A$ et $S$
:::

![image](graphe_complet){width="70%"}

::: exampleblock
Remarque On considère ici des graphes non orientés connexes : il n'y a
pas de point isolé dans le graphe
:::
:::

::: frame
Définitions

::: block
Définition Poids d'une arête : fonction
$d : A \rightarrow \mathbb{N}, (u, v) \rightarrowtail d(u, v)$ utilisée
pour les algorithmes de graphe. On utilisera ici la distance euclidienne
entre deux sommets
:::

On représente les villes par des graphes non orientés connexes. Les
centres d'intérêt sont des sommets de ce graphe. Pour chaque couple (u,
v) de sommets dans le graphe, si la distance entre u et v est inférieure
à une valeur donnée, il existe une arête entre u et v. On suppose que le
graphe ainsi créé reste connexe
:::

# Objectifs du TIPE

::: frame
Objectifs Etudier différents algorithmes pour créer un réseau de
transport public et les comparer

-   Les algorithmes doivent produire des résultats efficaces

-   Les algorithmes doivent être efficaces

    -   On espère ici avoir une complexité inférieure à du $O(n^2)$

-   Tous les points d'intérêt doivent être reliés entre eux sauf si
    aucune arête de longueur acceptable ne peut rendre le graphe connexe

-   La solution trouvée doit être réaliste
:::

# Conditions d'exploitation

::: frame
Conditions d'exploitation

-   On utilise l'exemple des métros car pas de contrainte de parcours

-   Les trains circulent à vitesse constante (proportionnalité entre
    distance et temps de trajet)

-   Le temps d'arrêt à une station est nul

-   Le temps d'attente à une station est nul

Ces suppositions ne sont pas réelles mais influent peu sur le résultat
:::

::: frame
Paramètres Les unités sont arbitraires

-   On utilise un plan de taille 100\*100

-   On souhaite des distances inférieures à 15

-   Pour les tests on positionne les points aléatoirement et on place au
    plus 1000 stations

Ces suppositions ne sont pas réelles mais influent peu sur le résultat
:::

# Première approche : algorithme de Dijkstra

::: frame
Dijkstra

::: block
Algorithme de Dijkstra Algorithme de plus court chemin dans un graphe
:::

Objectif : trouver le plus court chemin entre les bords haut et bas du
plan
:::

::: frame
Choix et utilisation de l'algorithme ![image](maze1){width="40%"}
![image](maze2){width="40%"}
:::

::: frame
Dijkstra

-   Complexité : $O(|A| log |A|)$

-   Trouve le meilleur résultat à chaque fois

-   Fonctionne car la fonction de poids est positive
:::

::: frame
Production de l'algorithme ![image](sp){width="50%"}

Cet algorithme crée des trajets avec peu de changements de direction
:::

::: frame
Mesure des résultats Les différents algorithmes employés ici permettent
de sauvegarder les graphes créés dans des fichiers. Un autre programme
peut ensuite calculer les informations que l'on souhaite sur ces graphes
: moyenne, médiane, longueur minimum, maximum
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](sp_r){width="90%"}

Nombre de chemins en fonction de leur longueur
:::

::: frame
Problèmes

-   Cet algorithme donne le plus court chemin entre deux points
    spécifiques du graphe, on peut ainsi l'appliquer sur chaque couple
    de sommets

-   La solution trouvée est alors optimale en terme de temps de trajet

-   La capacité de transport entre les deux points vaut $(|S| - 2) *$
    \"nombre de places dans une rame\"

-   Mais il reste deux problèmes

    -   Risque de lignes en double sur des portions de trajet

    -   Beaucoup de lignes à créer
:::

# Deuxième approche : algorithme de Kruskal

::: frame
Kruskal

::: block
Algorithme de Kruskal Algorithme qui trouve un arbre couvrant de poids
minimal dans un Graphe
:::

::: block
Définition Un arbre couvrant est un graphe $G'=(S, A')$ où
$A' \subset A$
:::

::: exampleblock
Remarque Un arbre couvrant de poids minimum est **un** arbre couvrant
dont la somme des poids des arêtes de $A'$ est la plus petite
:::
:::

::: frame
Choix et utilisation de l'algorithme
:::

::: frame
Kruskal

-   Complexité : $O(|A| log |A|)$

-   Algorithme correct : le sous-graphe renvoyé est un arbre couvrant de
    poids minimum

-   Fonctionne ici car la fonction de poids est positive
:::

::: frame
Production de l'algorithme ![image](k){width="50%"}

Chaque sommet est relié à tous les autres par un chemin
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](k_r){width="90%"}

Une grande partie des chemins a une longueur faible
:::

::: frame
Problèmes

-   Cet algorithme fournit un unique chemin entre chaque couple de
    sommets

-   Certains chemins semblent donc absurdes et beaucoup trop longs entre
    certains couples de sommets

-   La capacité de transport entre les deux points est le nombre de
    places dans une rame

-   On peut alors ajouter des arêtes avec l'algorithme de Dijkstra vu
    avant

-   Beaucoup de lignes différentes à créer
:::

# Troisième algorithme

::: frame
Résumé de ce qu'on a vu Les deux algorithmes vu précédemment ne
suffisent pas à répondre à la question. De plus les villes se
développent en agglomérations
:::

::: frame
Idée Utiliser l'algorithme des K-moyennes afin de regrouper des centres
d'intérêt entre eux, relier les groupes entre eux puis relier les
centres d'intérêt dans ces groupes
:::

::: frame
K-moyennes

::: block
K-moyennes Algorithme de regroupement de points en K groupes appelés
clusters afin de minimiser la distance entre les points d'un même groupe
:::

::: exampleblock
Remarque On parle ici de l'algorithme de Lloyd, ou algorithme naïf
:::
:::

::: frame
Choix et utilisation de l'algorithme
:::

::: frame
K-moyennes

-   Complexité : $O(|S| K i)$, i étant le nombre d'itérations avant
    d'obtenir une solution

-   L'algorithme produit une solution souvent proche de l'optimale

::: alertblock
Attention L'algorithme ne converge pas nécessairement vers une solution
pour certaines combinaisons de jeux de données, valeurs de K, positions
initiales des centroïdes
:::
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](m0){width="50%"}

1e itération
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](m1){width="50%"}

2e itération
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](m2){width="50%"}

3e itération
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](m3){width="50%"}

4e itération
:::

::: frame
Résultat sur un jeu de données aléatoire ![image](m4){width="50%"}

Après plusieurs itérations, convergence
:::

::: frame
Utilisation On applique l'algorithme des K-moyennes sur la ville en
donnant à l'algorithme une valeur de K cohérente avec ce que l'on veut.
On peut ensuite appliquer l'un des deux algorithmes précédents sur
chaque cluster puis relier les cluster entre eux.

-   Si on considère des quartiers d'une ville, on peut relier les
    clusters directement entre eux

-   Si on considère une ville et son agglomération, on relie chaque
    cluster au cluster de la ville principale

Complexité totale de l'algorithme : $O(K (|S| i + |A| log |A|))$
:::

::: frame
Avantages

-   Réduction des valeurs des longueurs maximales de chemins trouvés
    précédemment

-   Réduction de la complexité du problème en créant le réseau sur des
    ensembles plus petits

-   Adaptation à des villes dont les centres d'intérêt ne sont pas
    répartis de façon homogène dans l'espace, ou des agglomérations
:::

::: frame
Résultat de l'algorithme final
:::

# Codes

::: frame
shortestpath.c

``` {.objectivec language="C"}
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
		stations[i] = l;
	}
	Station l0;
	asprintf(&l0.name, "Salut");
	l0.x = 0;
	l0.y = 0;
	stations[0] = l0;
	Station lnmax;
	asprintf(&lnmax.name, "Salut");
	lnmax.x = GRIDSIZE - 1;
	lnmax.y = GRIDSIZE - 1;
	stations[NMAX - 1] = lnmax;
}

void initFromFile(FILE* fd) {
	fscanf(fd, "%d", &NMAX);
	stations = malloc(NMAX * sizeof(Station));
	for (int i = 0; i < NMAX; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		fscanf(fd, "%d %d", &l.x, &l.y);
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
			printf("%d ", pre);
			if (i == NMAX - 1) {
				Stack* new = malloc(sizeof(Stack));
				new->val = pre;
				new->next = *stack;
				*stack = new;
			}
			pre = prev[pre];
		}
		printf ("0\n");
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
	grid = malloc(GRIDSIZE * sizeof(char*));
	for (int i = 0; i < GRIDSIZE; i++) {
		grid[i] = malloc(GRIDSIZE * sizeof(char));
	}
	srand(time(NULL));
	if (argc >= 2) {
		FILE* fd;
		fd = fopen(argv[1], "r");
		initFromFile(fd);
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
	printf("\n");
	for (int i = 0; i < GRIDSIZE; i++) {
	 	for (int j = 0; j < GRIDSIZE; j++) {
	 		int x = hasValue(i, j);
			if (x != -1) printf("%d", x); else printf(" ");
		}
		printf("\n");
	}
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
		if (i == 0) stations[i].x = stations[1].x;
		if (i == NMAX - 1) stations[i].x = stations[i - 1].x;
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
						SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, 0, stations[liste->val].x * 10, stations[liste->val].y * 10);
						// edge t = {.s1 = liste->val, .s2 = liste->next->val, .w = distance(stations[liste->val], stations[liste->next->val], false)};
						// append(sln, t); 
						// stations[liste->val].x = 10000 * GRIDSIZE;
						// stations[liste->val].y = 10000 * GRIDSIZE;
						Solution* sln = malloc(sizeof(Solution));
						printf("__sln__\n");
						printf("0 %d\n", liste->val);
						printf("d %d\n", (int) stations[liste->val].y);
						edge t1;
						t1.s1 = 0;
						t1.s2 = liste->val;
						t1.w = (double) stations[liste->val].y;
						sln->val = t1;
						sln->next = NULL;
						// printf("No problem\n");
						// append(sln, t1);
						// printf("No problem v2\n");
						while (liste != NULL && liste->next != NULL && liste->next->next != NULL && liste->next->next->next != NULL) {
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
						t2.s2 = 29;
						t2.w = (double) (GRIDSIZE - 1) - stations[liste->val].y;
						append(sln, t2);
						SDL_RenderDrawLine(renderer, stations[liste->val].x * 10, stations[liste->val].y * 10, stations[liste->val].x * 10, (GRIDSIZE - 1) * 10);
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
```
:::

::: frame
kruskal.c

``` {.objectivec language="C"}
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include "json.h"
#include <execinfo.h>
#include <assert.h>
// #define NMAX 1000
#define GRIDSIZE 100
#define MAXLENGTH 15
int NMAX = 1000;

typedef struct Station {
	int x;
	int y;
	char* name;
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

void addElt(int val, htbl* table2);
void initrandom();
void initFromFile(FILE* fd);
int power(int number, int nth);
double distance(Station station1, Station station2);
void maketab();
htbl* find(htbl* val);
void unite(htbl* t1, htbl* t2);
void swp(edge* a, edge* b);
int partition(Edgelist* el, int begin, int end);
void quicksort(Edgelist* el, int begin, int end);
Edgelist* cv_graph(Station* list);
Solution* auxiliary(Edgelist* edges, htbl** table);
Solution* kruskal(Edgelist* edges);
Solution* copyEdgeList(Solution* li1);
void freeEdgeList(Solution* sln);
void print_solution(Solution* sl1);
int edgeListLength(Solution* s1);
Edgelist* convert(Solution* s1);
void print_edgelist(Edgelist* c);
int main(int argc, char* argv[]);

void addElt(int val, htbl* table2) {
    // table2->parent = malloc(sizeof(htbl*));
    // assert(table2->parent != NULL);
    table2->parent = table2;
    table2->val = val;
    table2->rg = 0;
    // printf("Pass %d\n", val);
}

void initrandom() {
	// stations = malloc(NMAX * sizeof(Station));
	for (int i = 1; i < NMAX - 1; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		l.x = random() % GRIDSIZE;
		l.y = random() % GRIDSIZE;
		stations[i] = l;
	}
	Station l0;
	asprintf(&l0.name, "Salut");
	l0.x = 0;
	l0.y = 0;
	stations[0] = l0;
	Station lnmax;
	asprintf(&lnmax.name, "Salut");
	lnmax.x = GRIDSIZE - 1;
	lnmax.y = GRIDSIZE - 1;
	stations[NMAX - 1] = lnmax;
}

void initFromFile(FILE* fd) {
    // fscanf(fd, "%d", &NMAX);
    // stations = malloc(NMAX * sizeof(Station));
    for (int i = 0; i < NMAX; i++) {
        Station l;
        asprintf(&l.name, "Salut");
        fscanf(fd, "%d %d", &l.x, &l.y);
        stations[i] = l;
    }
}

int power(int number, int nth) {
	if (nth < 0) return 1 / power(number, -nth);
	if (nth == 0) return 1;
	return number * power(number, nth - 1);
}

double distance(Station station1, Station station2) {
	// printf("Pos1 : %d %d\n", station1.x, station1.y);
	// printf("Pos2 : %d %d\n", station2.x, station2.y);
	if (station1.x == 0 && station1.y == 0) return (double) station2.y;
	// if (station2.x == 0 && station2.y == 0) return (double) station1.y;
	if (station2.x == GRIDSIZE - 1 && station2.y == GRIDSIZE - 1) return (double) GRIDSIZE - station1.y;
	// if (station1.x == GRIDSIZE - 1 && station1.y == GRIDSIZE - 1) return (double) GRIDSIZE - station2.x;
	return sqrt((double) (power((station2.y - station1.y), 2) + power((station2.x - station1.x), 2)));
}

void maketab() {
	tableau_distances = malloc(NMAX * sizeof(double*));
	for (int i = 0; i < NMAX; i++) {
		tableau_distances[i] = malloc(NMAX * sizeof(double));
		for (int j = 0; j < i; j++) {
			// printf("%d %d\n", i, j);
			double d = distance(stations[j], stations[i]);
			// printf("%d->%d:%d\n", i, j, d);
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
    // printf("%d %d\n", (float) sizeof(el->list) / sizeof(edge), end);
    double pivot = el->list[end].w;
    // printf("Pass\n");
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
    // printf("%d\n", NMAX);
    edge* li = malloc(NMAX * NMAX * sizeof(edge));
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < NMAX; j++) {
            li[NMAX * i + j].s1 = i;
            li[NMAX * i + j].s2 = j;
            if (i != j) {
                li[NMAX * i + j].w = distance(list[i], list[j]); 
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
        // printf("Bp %d\n", i);
        edge a = edges->list[i];
        htbl* v1 = find(table[a.s1]);
        htbl* v2 = find(table[a.s2]);
        // printf("Bp %d\n", i);
        if (v1 != v2) {
            unite(v1, v2);
            // printf("%d %d\n", a.s1, a.s2);
            Solution* l2 = malloc(sizeof(Solution));
            // Pile* l = malloc(sizeof(Pile));
            // l2->val = malloc(sizeof(edge));
            l2->val = a;
            // printf("%d\n", l2->val.w);
            // l->val = a.w;
            l2->next = solution;
            // l->next = solution;
            solution = l2;
        }
    }
    return solution;
}

Solution* kruskal(Edgelist* edges) {
    int x = edges->s;
    // printf("%d\n", x);
    htbl** table = malloc(x * sizeof(htbl*));
    assert(table != NULL);
    // printf("Malloc done\n");
    for (int i = 0; i < x; i++) {
        table[i] = malloc(sizeof(htbl));
        // assert(table[i] != NULL);
        // printf("%x\n", table[i]->parent);
        // table[i]->parent = malloc(sizeof(htbl));
        // printf("Add %d\n", i);
        addElt(i, table[i]);
        // printf("Added %d\n", i);
    }
    // printf("Bp\n");
    // quicksort(edges, 0, x);
    return auxiliary(edges, table);
}

// void print_edgelist(Edgelist* s) {
//     printf("Edgelist\n");
//     for (int i = 0; i < s->s; i++) {
//         printf("%d->%d : %d\n", s->list[i].s1, s->list[i].s2, s->list[i].w);
//     }
//     printf("EndEdgelist\n");
// }

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
        printf("%d %d : %f\n", s1, s2, w);
        ttl += w;
        if (w > maxl) maxl = w;
        if (w < minl) minl = w;
        sln = sln->next;
    }
    // printf("%f %f %f %f\n", ttl, maxl, minl, maxdep);
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

int main(int argc, char* argv[]) {
    Solution* res = malloc(sizeof(Solution));
    srand(time(NULL));
    if (argc >= 2) {
        FILE* fd;
        fd = fopen(argv[1], "r");
        fscanf(fd, "%d", &NMAX);
        stations = malloc(NMAX * sizeof(Station));
        initFromFile(fd);
    } else {
        stations = malloc(NMAX * sizeof(Station));
        initrandom();
    }
	// for (int i = 0; i < NMAX; i++) {
	// 	printf("%d %d %d\n", i, stations[i].x, stations[i].y);
	// }
	// maketab();
	// for (int i = 0; i < NMAX; i++) {
	// 	for (int j = 0; j < NMAX; j++) {
	// 		printf("%f\t", tableau_distances[i][j]);
	// 	}
	// 	printf("\n");
	// }
        // printf("%d %d\n", begin, end);
    Edgelist* c = cv_graph(stations);
        // print_edgelist(c);
    // printf("%d\n", c->s);
    quicksort(c, 0, c->s - 1);
    for (int i = 0; i < c->s; i++) {
        printf("%d %d : %f\n", c->list[i].s1, c->list[i].s2, c->list[i].w);
    }
    // print_edgelist(c);
    res = kruskal(c);
    Edgelist* sol = convert(res);
    writeEdgeListArray(sol, "kruskal.json", "kruskal_raw.txt");
    // while (res != NULL && res->next != NULL) {
    //     printf("%d %d %d\n", res->val.s1, res->val.s2, res->val.w);
    //     res = res->next;
    // }
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
    int x, y;
	SDL_GetWindowSize(window, &x, &y);
	for (int i = 0; i < NMAX; i++) {
		if (i == 0) stations[i].x = stations[1].x;
		if (i == NMAX - 1) stations[i].x = stations[i - 1].x;
		SDL_RenderDrawPoint(renderer, stations[i].x * 10, stations[i].y * 10);
        SDL_RenderDrawPoint(renderer, stations[i].x * 10 + 1, stations[i].y * 10 + 1);
        SDL_RenderDrawPoint(renderer, stations[i].x * 10 + 1, stations[i].y * 10 - 1);
        SDL_RenderDrawPoint(renderer, stations[i].x * 10 - 1, stations[i].y * 10 + 1);
        SDL_RenderDrawPoint(renderer, stations[i].x * 10 - 1, stations[i].y * 10 - 1);	
		// SDL_Delay(500);
	}
    SDL_RenderPresent(renderer);
	SDL_Delay(500);
	if(0 != SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
	// for (int i = 0; i < GRIDSIZE; i++) {
	//  	for (int j = 0; j < GRIDSIZE; j++) {
	//  		int x = hasValue(i, j);
	// 		if (x != -1) printf("%d", x); else printf(" ");
	// 	}
	// 	printf("\n");
	// }
	// makeGraph();
    // print_solution(res);
    SDL_Event ev;
	int quit = 0;
	while (quit != 2) {
		while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_KEYUP:
                // case SDL_KEYDOWN:
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
```
:::

::: frame
kavg.c

``` {.objectivec language="C"}
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
// #define NMAX 1000
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
#define SDL_WHITE 255, 255, 255
#define SDL_GREY 127, 127, 0
#define SDL_BLACK 0, 0, 0
#define SDL_COLOR_COUNT 15
#define SDL_COLORS (int[]) {SDL_RED, SDL_FUCHSIA, SDL_MAGENTA, SDL_MAUVE, SDL_BLUE, SDL_CYAN, SDL_TURQUOISE, SDL_GREEN, SDL_CHARTREUSE, SDL_YELLOW, SDL_ORANGE, SDL_WHITE, SDL_GREY, SDL_BLACK}
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
#define WHITE 11
#define GREY 12
#define BLACK 13
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

void pr_gr();
void pr_dn();
void initrandom(Station* stations);
void initFromFile(Station* stations, FILE* fd);
void getKRandom(Station* stations, StackCouple** sets);
double distance(float x1, float y1, float x2, float y2);
void push(Stack* s, int val);
void push_alt(StackCouple** s, float x0, float y0, int r);
int pop(Stack* s);
void classify(Station* stations, StackCouple* sets[], int count);
void calculateAvg(StackCouple*** sets);
void freeSet(StackCouple* s);
void getSetAvg(StackCouple* sets[]);
void alt_printf(int r, int g, int b);
void linkPoints();
void initSigma();
int length(StackCouple* set);
Station* subset(StackCouple* set);
int loop(Station** sub, StackCouple* sets[], int* len, int count, int savedepth, char* outScheme, bool export);
int main(int argc, char* argv[]);

void pr_gr() {
	printf("\033[32m");
}

void pr_dn() {
	printf("\033[0m\n");
}

void initrandom(Station* stations) {
	// stations = malloc(NMAX * sizeof(Station));
	// printf("Allocate %d stations\n", NMAX);
	for (int i = 1; i < NMAX - 1; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		l.x = random() % GRIDSIZE;
		l.y = random() % GRIDSIZE;
		l.id = i;
		// printf("%d %d\n", l.x, l.y);
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
	// pr_gr();
	// printf("Done");
	// pr_dn();
}

void initFromFile(Station* stations, FILE* fd) {
	// fscanf(fd, "%d", &NMAX);
	// stations = malloc(NMAX * sizeof(Station));
	for (int i = 0; i < NMAX; i++) {
		Station l;
		asprintf(&l.name, "Salut");
		fscanf(fd, "%d %d", &l.x, &l.y);
		stations[i] = l;
	}
}

void getKRandom(Station* stations, StackCouple** sets) {
	// printf("Get %d random\n", K);
	for (int i = 0; i < K; i++) {
		int r = random() % NMAX;
		for (int j = 0; j < i; j++) {
			if (stations[r].x == sets[j]->val.x0 && stations[r].y == sets[j]->val.y0) {
				r = random() % NMAX;
				j = 0;
			}
		}
		sets[i] = malloc(sizeof(StackCouple));
		sets[i]->val.x0 = stations[r].x;
		sets[i]->val.y0 = stations[r].y;
		sets[i]->val.id = r;
		// printf("Pos %d : %f %f, supposed %f %f\n", i, sets[i]->val.x0, sets[i]->val.y0, stations[r].x, stations[r].y);
		sets[i]->next = NULL;
		// pr_gr();
		// printf("Done %d", i);
		// pr_dn();
	}
}

double distance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void push(Stack* s, int val) {
	Stack* new = malloc(sizeof(Stack));
	new->val = val;
	new->next = s;
	s = new;
}

void push_alt(StackCouple** s, float x0, float y0, int r) {
	StackCouple* new = malloc(sizeof(StackCouple));
	new->val.x0 = x0;
	new->val.y0 = y0;
	new->val.id = r;
	new->next = NULL;
	// new->next = *s;
	// *s = new;
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

int pop(Stack* s) {
	int val = s->val;
	s = s->next;
	return val;
}

void classify(Station* stations, StackCouple* sets[], int count) {
	// printf("Classify %d stations into %d groups\n", NMAX, K);
	for (int i = 0; i < count; i++) {
		double min = INT_MAX;
		int minset = 0;
		for (int j = 0; j < K; j++) {
			double d = distance(stations[i].x, stations[i].y, sets[j]->val.x0, sets[j]->val.y0);
			if (d < min) {
				min = d;
				minset = j;
			}
		}
		push_alt(&sets[minset], stations[i].x, stations[i].y, i);
		// pr_gr();
		// printf("Done %d -> %d", i, minset);
		// pr_dn();
	}
}

// void calculateAvg(StackCouple*** sets) {
// 	Couple* tab = malloc(K * sizeof(Couple));
// 	for (int i = 0; i < K; i++) {
// 		int count = 0;
// 		StackCouple* n = (*sets)[i]->next;
// 		n = n->next;
// 		while (n->next != NULL) {
// 			tab[i].x0 += n->val.x0;
// 			tab[i].y0 += n->val.y0;
// 			count++;
// 			n = n->next;
// 		}
// 		// assert(count != 0);
// 		tab[i].x0 /= count;
// 		tab[i].y0 /= count;
// 	}
// }

// void freeSet(StackCouple* s) {
//    StackCouple* tmp;
//    while (s != NULL) {
//        tmp = s;
//        s = s->next;
// 	   free(tmp);
//     }
// }

void freeSet(StackCouple* s) {
	if (s != NULL) {
		freeSet(s->next);
		free(s);
	}
}


void getSetAvg(StackCouple* sets[]) {
	for (int i = 0; i < K; i++) {
		double sumX = 0;
		double sumY = 0;
		int count = 0;
		// printf("%d\n", i);
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
		// Couple c;
		// c.x0 = sumX;
		// c.y0 = sumY;
		// sets[i] = malloc(sizeof(StackCouple));
		// sets[i]->val = malloc(sizeof(Couple));
		// sets[i]->next = NULL;
		freeSet(sets[i]->next);
		sets[i]->next = NULL;
		sets[i]->val.x0 = sumX;
		sets[i]->val.y0 = sumY;
		// printf("Pos %d : %f %f\n", i, sumX, sumY);
	}
}

void alt_printf(int r, int g, int b) {
	printf("%d %d %d\n", r, g, b);
}

void linkPoints() {
	Solution* s = malloc(sizeof(Solution));
	for (int i = 0; i < K; i++) {
		
	}
}

void initSigma() {
	for (int i = 0; i < 13; i++) sigma[i] = i;
	for (int i = 13 - 1; i >= 1; i--) {
		int j = rand() % i;
		int s = sigma[i];
		sigma[i] = sigma[j];
		sigma[j] = s;
	}
}

int length(StackCouple* set) {
	int c = 0;
	while (set != NULL) c++;
	return c;
}

Station* subset(StackCouple* set) {
	int len = length(set);
	Station* st = malloc(len * sizeof(Station));
	int c = 0;
	while (set != NULL) {
		st[c].id = set->val.id;
		st[c].x = set->val.x0;
		st[c].y = set->val.y0;
		c++;
		set = set->next;
	}
	return st;
}

int loop(Station** sub, StackCouple* sets[], int* len, int count, int savedepth, char* outScheme, bool export) {
	// int quit = 0;
	int done = 0;
	SDL_Event ev;
	
		// printf("Enter loop\n");
		if(0 != SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(BLACK), 255)) {
			fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
			return 1;
			// goto Quit;
		}
		SDL_RenderClear(renderer);
		if(0 != SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(WHITE), 255)) {
			fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
			return 1;
		}
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < len[i]; j++) {
				// printf("%d\n", sub[j].x);
				int x = sub[i][j].x;
				int y = sub[i][j].y;
				SDL_RenderDrawPoint(renderer, 10 * x, 10 * y);
				SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y + 2);
				SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y - 2);
				SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y - 2);
				SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y + 2);
			}
		}
		SDL_RenderPresent(renderer);
		done = 0;
		while (done != 2) {
			while (SDL_PollEvent(&ev)) {
				switch (ev.type) {
					case SDL_KEYDOWN: {
						switch(ev.key.keysym.sym) {
							case SDLK_ESCAPE: {
								done = 2;
								break;
							}
							case SDLK_SPACE: {
									if (export) {
										FILE* fd;
										for (int j = 0; j < K; j++) {
											char* outFile;
											asprintf(&outFile, "%s%d.txt", outScheme, j);
											fd = fopen(outFile, "w");
											StackCouple* s = sets[j];
											int count = 0;
											while (s != NULL) {
												count++;
												s = s->next;
											}
											fprintf(fd, "%d\n", count);
											s = sets[j];
											while (s != NULL) {
												int x = s->val.x0;
												int y = s->val.y0;
												fprintf(fd, "%d %d\n", x, y);
												s = s->next;
											}
											fclose(fd);
										}
										char* outFile;
										asprintf(&outFile, "%s%d.txt", outScheme, K);
										fd = fopen(outFile, "w");
										fprintf(fd, "%d\n", K);
										for (int j = 0; j < K; j++) {
											int x = sets[j]->val.x0;
											int y = sets[j]->val.y0;
											fprintf(fd, "%d %d\n", x, y);
										}
										fclose(fd);
									}
									break;
								}
							case SDLK_RETURN: {
								// StackCouple** setsw = malloc(K * sizeof(StackCouple*));
									getSetAvg(sets);
									classify(sub[0], sets, NMAX);
									SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
									SDL_RenderClear(renderer);
									SDL_RenderPresent(renderer);
									for (int j = 0; j < K; j++) {
										if(0 != SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(sigma[j]), 255)) {
											fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
											return 1;
										}
										StackCouple* s = sets[j];
										// SDL_RenderDrawLine(renderer, 10 * (s->val.x0 - 1), 10 * (s->val.y0 - 1), 10 * (s->val.x0 + 1), 10 * (s->val.y0 + 1));
										// printf("\e[32mPos %d : %f %f\e[0m\n", i, s->val.x0, s->val.y0);
										// s = s->next;
										while (s != NULL) {
											int x = s->val.x0;
											int y = s->val.y0;
											// printf("%d %d\n", x, y);
											SDL_RenderDrawPoint(renderer, 10 * x, 10 * y);
											SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y + 2);
											SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y - 2);
											SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y - 2);
											SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y + 2);
											s = s->next;
										}
									}
								SDL_RenderPresent(renderer);
								break;
							}
						}
						break;
					}
					case SDL_QUIT: {
						return 0;
						break;
					}
					default: {
						break;
					}
				}
			}
		}
	
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	initSigma();
	int x, y;
	SDL_Window *window = NULL;
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("Algo1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (GRIDSIZE - 1) * 10, (GRIDSIZE - 1) * 10, SDL_WINDOW_SHOWN);
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
    // StackCouple** sets = malloc(K * sizeof(Stack*));
	StackCouple** sets = malloc(K * sizeof(StackCouple*));
	// Station* stations = malloc(NMAX * sizeof(Station));
	Station* stations;
	if (argc >= 3) {
		FILE* fd;
		fd = fopen(argv[2], "r");
		fscanf(fd, "%d", &NMAX);
		stations = malloc(NMAX * sizeof(Station));
		initFromFile(stations, fd);
	} else {
		stations = malloc(NMAX * sizeof(Station));
		initrandom(stations);
	}
	// for (int i = 0; i < NMAX; i++) {
	// 	printf("%d %d\n", stations[i].x, stations[i].y);
	// }
	getKRandom(stations, sets);
	// for (int i = 0; i < K; i++) {
	// 	printf("%f %f\n", sets[i]->val.x0, sets[i]->val.y0);
	// }
	// for (int i = 0; i < K; i++) {
	// 	printf("Set %d\n", i);
	// 	StackCouple* s = sets[i];
	// 	while (s != NULL) {
	// 		printf("%f %f\n", s->val.x0, s->val.y0);
	// 		s = s->next;
	// 	}
	// }
	classify(stations, sets, NMAX);
	// SDL_GetWindowSize(window, &x, &y);
	// for (int i = 0; i < NMAX; i++) {
	// 	if (i == 0) stations[i].x = stations[1].x;
	// 	if (i == NMAX - 1) stations[i].x = stations[i - 1].x;
	// 	SDL_RenderDrawPoint(renderer, stations[i].x * 10, stations[i].y * 10);	
	// }
	// for (int i = 0; i < K; i++) {
	// 	// alt_printf(GET_SDL_COLOR(i));
	// 	if(0 != SDL_SetRenderDrawColor(renderer, GET_SDL_COLOR(i), 255)) {
	// 		fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
	// 		goto Quit;
	// 	}
	// 	StackCouple* s = sets[i];
	// 	// SDL_RenderDrawLine(renderer, 10 * (s->val.x0 - 1), 10 * (s->val.y0 - 1), 10 * (s->val.x0 + 1), 10 * (s->val.y0 + 1));
	// 	// printf("\e[32mPos %d : %f %f\e[0m\n", i, s->val.x0, s->val.y0);
	// 	// s = s->next;
	// 	while (s != NULL) {
	// 		int x = s->val.x0;
	// 		int y = s->val.y0;
	// 		printf("%d %d | ", x, y);
	// 		// alt_printf(GET_SDL_COLOR(i));
	// 		SDL_RenderDrawPoint(renderer, 10 * x, 10 * y);
	// 		SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y + 2);
	// 		SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y - 2);
	// 		SDL_RenderDrawPoint(renderer, 10 * x + 2, 10 * y - 2);
	// 		SDL_RenderDrawPoint(renderer, 10 * x - 2, 10 * y + 2);
	// 		s = s->next;
	// 	}
	// 	printf("\n");
	// }
	// SDL_RenderPresent(renderer);
	int done = 0;
	int quit = 0;
	int firstrun = 0;
	int c[1] = {NMAX};
	if (argc >= 2) {
		return loop(&stations, sets, c, 1, 0, argv[1], true);
	} else {
		return loop(&stations, sets, c, 1, 0, NULL, false);
	}
	Quit:
		return 0;
	// TODO: relier tous les points comme il faut
}
```
:::

::: frame
graph.py

``` {.python language="Python"}
import json
import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Nom de fichier requis")
    sys.exit(1)

f = open(sys.argv[1], "r")
data = json.load(f)
X = np.array([])
for x in data['data']:
    X = np.append(X, [x])

# def mu(array):
#     m = 0
#     for x in array:
#         # print(x['w'])
#         m += x['w']
#     return m / len(array)

# def sigma(array):
#     s = 0
#     mu_val = mu(array)
#     for elt in array:
#         x = elt['w'] - mu_val
#         s += x**2
#     s /= len(array)
#     return s**(1/2)

# Calculate path length between two nodes
def calculatePath(node1, node2, array):
    print("Calculating path between", node1, "and", node2)
    # print(array)
    path = 0
    current = node1
    while current != node2:
        # print("Current node:", current)
        # print("Path:", path)
        found = False
        for elt in array:
            if elt['s1'] == current:
                print("Found edge", elt['s1'], elt['s2'], "with weight", elt['w'])
                path += elt['w']
                current = elt['s2']
                found = True
                break
        if not found:
            # print("No path found")
            return None
    return path

def maxPathLength(array):
    max = 0
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                if path > max:
                    max = path
    return max

def minPathLength(array):
    min = -1
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                if path < min or min == -1:
                    min = path
    return min

def avgPathLength(array):
    sum = 0
    cnt = 0
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                cnt += 1
                sum += path
    if cnt != 0:
        return sum / cnt
    return 0

def medPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            if edge1 != edge2:
                path = calculatePath(edge1, edge2, array)
                if path != None and path != 0:
                    paths.append(path)
    return np.median(paths)

def sigmaPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                paths.append(path)
    return np.std(paths)

def getAllPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                paths.append(path)
    return np.array(paths)

Y = np.array(list(map(lambda elt : elt['w'], X)))
ya = Y.mean()
print("avgEdge =", ya)
ys = Y.std()
print("stdEdge =", ys)
yM = Y.max()
print("maxEdge =", yM)
ym = Y.min()
print("minEdge =", ym)
yS = np.median(Y)
print("medEdge =", yS)
xa = avgPathLength(X)
print("avgPath =", xa)
xs = sigmaPathLength(X)
print("stdPath =", xs)
xM = maxPathLength(X)
print("maxPath =", xM)
xm = minPathLength(X)
print("minPath =", xm)
xS = medPathLength(X)
print("medPath =", xS)


plt.hist(Y, bins=100, color='b')
plt.axvline(x=ya, color='blue', label='AvgEdge = '+str(ya))
plt.axvline(x=yS, color='red', label='MedEdge = '+str(yS))
plt.axvline(x=yM, color='green', label='MaxEdge = '+str(yM))
plt.axvline(x=ym, color='black', label='MinEdge = '+str(ym))
plt.legend(loc='lower right')
plt.show()
plt.hist(getAllPathLength(X), bins=100, color='r')
plt.axvline(x=xa, color='blue', label='AvgPath = '+str(xa))
plt.axvline(x=xS, color='red', label='MedPath = '+str(xS))
plt.axvline(x=xM, color='green', label='MaxPath = '+str(xM))
plt.axvline(x=xm, color='black', label='MinPath = '+str(xm))
plt.legend(loc='lower right')
plt.show()
```
:::

# Conclusion
