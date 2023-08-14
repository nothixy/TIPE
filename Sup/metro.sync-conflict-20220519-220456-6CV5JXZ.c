#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <sys/ioctl.h>
#include <locale.h>
#include <pthread.h>
#include "vector.h"

#define SEATS 1000

#define ROME 0
#define LA_FOURCHE 1
#define BLANCHE 2
#define PLACE_DE_CLICHY 3
#define LIEGE 4

#define DUREE_TRAJET 30
#define ARRET_STATION 15

#define DEFAULT 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CYAN 5

char* stations[5] = {"Rome", "La Fourche", "Blanche", "Place de clichy", "Liège"};
int grid[31][31] = {-1};
//int ligne1[3] = {0, 1, 4};
//int ligne2[3] = {2, 1, 3};
bool verbose = false;

int station_count = 5;

struct train {
	bool backwards;
	int ligne;
	int destination;
	int passagers;
	int station_actuelle;
	bool deplacement;
	int prochain_depart;
	int color;
};

typedef struct train train;

struct personne {
	int station_actuelle;
	int destination;
	int attente;
	int nom;
};
typedef struct personne personne;

struct gare {
	int personnes_a_quai;
	char* nom;
	int attentes[2];
	int affluence;
};
typedef struct gare gare;

struct ligne {
	int* stations;
	int* interval_st;
	int len;
};
typedef struct ligne ligne;

ligne ligne1;
ligne ligne2;
ligne* lignes;

struct winsize w;

personne personnes[1000];
train trains[10];
gare gares[5];
int rame[1000] = {0};
int lcount;
int p_offset[3] = {0, 0, 0};
int w_offset = 2;
int ccount;

WINDOW* w_gares;
WINDOW* attente;
WINDOW* w_personnes;
WINDOW* graphic;

pthread_t ti;


int indexOf(int n, int* array, int len);

void print_personnes() {
	for (int i = 0; i < lcount - 2; i++) {
		if (w_offset == 2 && i == 0) {
			wattron(w_personnes, COLOR_PAIR(RED));
		}
		mvwprintw(w_personnes, i + 1, 1, "%d -> %d", personnes[i + p_offset[2]].nom, personnes[i + p_offset[2]].attente);
		if (w_offset == 2 && i == 0) {
			wattroff(w_personnes, COLOR_PAIR(RED));
		}
	}
	wrefresh(w_personnes);
}

void print_gare() {
	for (int i = 0; i < lcount - 2; i++) {
		if (w_offset == 1 && i == p_offset[1]) {
			wattron(attente, COLOR_PAIR(RED));
		}
		mvwprintw(w_personnes, i + 1, 1, "%d -> %d", personnes[i + p_offset[2]].nom, personnes[i + p_offset[2]].attente);
		if (w_offset == 1 && i == p_offset[1]) {
			wattroff(attente, COLOR_PAIR(RED));
		}
	}
	wrefresh(w_personnes);
}

void print_stations() {
	for (int i = 0; i < 5; i++) {
		if (w_offset == 0 && i == p_offset[0]) {
			wattron(w_gares, COLOR_PAIR(RED));
		}
		mvwprintw(w_gares, i + 1, 1, "%s", stations[i]);
		if (w_offset == 0 && i == p_offset[0]) {
			wattroff(w_gares, COLOR_PAIR(RED));
		}
	}
	wrefresh(w_gares);
}

void print_grid();

void print_status() {
	system("clear");
	for (int i = 0; i < 10; i++) {
		printf("\033[32mTrain #%d\033[0m\n", i);
		printf("Destination : %s\n", stations[trains[i].destination]);
		printf("Ligne : %d\n", trains[i].ligne);
		printf("Passagers : %d\n", trains[i].passagers);
		printf("Station actuelle : %s\n", stations[trains[i].station_actuelle]);
		printf("Déplacement : %s\n", trains[i].deplacement ? "TRUE" : "FALSE");
		printf("Prochain départ : %d s\n", trains[i].prochain_depart);
		float percentage_done = 1.0 - (float) ((trains[i].prochain_depart > ARRET_STATION ? trains[i].prochain_depart - ARRET_STATION : 0)) / ARRET_STATION;
		printf("Pourcentage trajet %lf\n", percentage_done);
		printf("Backwards : %s\n", trains[i].backwards ? "TRUE" : "FALSE");
	}
}

void* updatetime() {
	while (true) {
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		lcount = w.ws_row;
		ccount = w.ws_col;
		wresize(w_gares, lcount, 30);
		wresize(attente, lcount, 30);
		wresize(w_personnes, lcount, 30);
		wresize(graphic, lcount, ccount - 90);
		box(w_gares, 0, 0);
		box(attente, 0, 0);
		box(w_personnes, 0, 0);
		box(graphic, 0, 0);
		mvwprintw(w_gares, 0, 1, "Gares");
		mvwprintw(attente, 0, 1, "Attente");
		mvwprintw(w_personnes, 0, 1, "Personnes à quai");
		mvwprintw(graphic, 0, 1, "Graphic");
		refresh();
		wrefresh(w_gares);
		wrefresh(attente);
		wrefresh(w_personnes);
		wrefresh(graphic);
		sleep(1);
		for (int i = 0; i < 1000; i++) {
			personnes[i].attente++;
		}
		for (int i = 0; i < 10; i++) {
			if (trains[i].prochain_depart != 0) {
				trains[i].prochain_depart--;
				if (trains[i].prochain_depart <= 15) {
					trains[i].deplacement = false;
				} else {
					trains[i].deplacement = true;
				}
			} else {
				trains[i].prochain_depart = DUREE_TRAJET;
				if (trains[i].backwards) {
					trains[i].station_actuelle = lignes[trains[i].ligne].stations[indexOf(trains[i].station_actuelle, lignes[trains[i].ligne].stations, 3) - 1];
					if (trains[i].station_actuelle == trains[i].destination && !trains[i].deplacement) {
						trains[i].destination = lignes[trains[i].ligne].stations[2 - indexOf(trains[i].destination, lignes[trains[i].ligne].stations, 3)];;
						trains[i].backwards = !(trains[i].backwards);
					}
				} else {
					trains[i].station_actuelle = lignes[trains[i].ligne].stations[indexOf(trains[i].station_actuelle, lignes[trains[i].ligne].stations, 3) + 1];
					if (trains[i].station_actuelle == trains[i].destination && !trains[i].deplacement) {
						trains[i].destination = lignes[trains[i].ligne].stations[2 - indexOf(trains[i].destination, lignes[trains[i].ligne].stations, 3)];;
						trains[i].backwards = !(trains[i].backwards);
					}
				}
			}
		}
		for (int i = 0; i < 5; i++) {
			int h = random() % 100;
			if (h == gares[i].affluence) {
				gares[i].personnes_a_quai++;
			}
		}
		print_personnes();
		print_grid();
		if (verbose) {
			print_status();
		}
	}
}

void getinput() {
	cbreak();
	noecho();
	int c;
	while (true) {
		c = getch();
		switch(c) {
			case 'A':
			case KEY_UP:
				if (p_offset[w_offset] != 0) p_offset[w_offset]--;
				break;
			case 'B':
			case KEY_DOWN:
				switch (w_offset) {
					case 2:
						if (p_offset[w_offset] != 1000 - lcount + 2) p_offset[w_offset]++;
						break;
					case 0:
						if (p_offset[w_offset] != 4) p_offset[w_offset]++;
						break;
				}
				break;
			case 'D':
			case KEY_LEFT:
				if (w_offset != 0) w_offset--;
				break;
			case 'C':
			case KEY_RIGHT:
				if (w_offset != 2) w_offset++;
				break;
			case '/':
				nocbreak();
				// Do sth
				break;
			case 'q':
				pthread_cancel(ti);
				system("reset");
				exit(0);
				break;
			default:
				break;
		}
		print_personnes();
		print_stations();
	}
}

void define_stations() {
	for (int i = 0; i < 5; i++) {
		gares[i].nom = malloc(strlen(stations[i]));
		char* r = stations[i];
		strcpy(gares[i].nom, r);
	}
}

void creer_station_sur_lignes(ligne* lines, int len, int index) {
	for (int i = 0; i < len; i++) {
		lines[i].len++;
		lines[i].stations = realloc(lines[i].stations, lines[i].len * sizeof(int));
		lines[i].stations[index] = station_count;
	}
	station_count++;
}

int get_train(int x, int y) {
	int train_number = -1;
	for (int k=0; k < 10; k++) {
		int train_x = 0;
		int train_y = 0;
		float percentage_done = 1.0 - (float) ((trains[k].prochain_depart > ARRET_STATION ? trains[k].prochain_depart - ARRET_STATION : 0)) / ARRET_STATION;
		switch (trains[k].ligne) {
			case 0:
				train_x = 15;
				train_y = indexOf(trains[k].station_actuelle, lignes[trains[k].ligne].stations, 3);
				if (trains[k].backwards) {
					if (x == train_x && y == train_y * 15 - (int) (percentage_done * 15)) {
						train_number = k;
					}
				} else {
					if (x == train_x && y == train_y * 15 + (int) (percentage_done * 15)) {
						train_number = k;
					}
				}
				break;
			case 1:
				train_y = 15;
				train_x = indexOf(trains[k].station_actuelle, lignes[trains[k].ligne].stations, 3);
				if (trains[k].backwards) {
					if (x == train_x * 15 - (int) (percentage_done * 15) && y == train_y) {
						train_number = k;
					}
				} else {
					if (x == train_x * 15 + (int) (percentage_done * 15) && y == train_y) {
						train_number = k;
					}
				}
				break;
			default:
				break;
		}
	}
	return train_number;
}

void print_grid() {
	int maxi, maxj;
	//getmaxyx(graphic, maxi, maxj);
	maxi = maxj = 31;
	for (int i = 0; i < maxi; i++) {
		for (int j = 0; j < maxj; j++) {
			int train = get_train(i, j);
			if (train == -1) {
				if (grid[i][j] == -1) {
					if (i == 15) {
						mvwprintw(graphic, i + 1, j + 1, "%c", '-');
					} else if (j == 15) {
						mvwprintw(graphic, i + 1, j + 1, "%c", '|');
					} else {
						mvwprintw(graphic, i + 1, j + 1, "%c", ' ');
					}
				} else {
					wattron(graphic, COLOR_PAIR(RED));
					mvwprintw(graphic, i + 1, j + 1, "%c", stations[grid[i][j]][0]);
					wattroff(graphic, COLOR_PAIR(RED));
				}
			} else {
				wattron(graphic, COLOR_PAIR(GREEN));
				mvwprintw(graphic, i + 1, j + 1, "%d", train);
				wattroff(graphic, COLOR_PAIR(GREEN));
			}
		}
	}
	refresh();
	wrefresh(graphic);
}

void calculate_directions(personne voyageur) {
	int depart = voyageur.station_actuelle;
	int arrivee = voyageur.destination;
}

int main(int argc, char* argv[]) {
	lignes = malloc(2 * sizeof(ligne));
	ligne1.len = 3;
	ligne1.stations = malloc(3 * sizeof(int));
	ligne1.stations[0] = 0;
	ligne1.stations[1] = 1;
	ligne1.stations[2] = 4;
	ligne1.interval_st = malloc(2 * sizeof(int));
	ligne1.interval_st[0] = ligne1.interval_st[1] = 15;
	ligne2.len = 3;
	ligne2.stations = malloc(3 * sizeof(int));
	ligne2.stations[0] = 2;
	ligne2.stations[1] = 1;
	ligne2.stations[2] = 3;
	ligne2.interval_st = malloc(3 * sizeof(int));
	ligne2.interval_st[0] = ligne2.interval_st[1] = 15;
	lignes[0] = ligne1;
	lignes[1] = ligne2;
	srand(time(NULL));
	setlocale(LC_ALL, "");
	if (argc > 1 && strcmp(argv[1], "-v") == 0) {
		verbose = true;
	} else {
		initscr();
	}
	start_color();
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	lcount = w.ws_row;
	ccount = w.ws_col;
	w_gares = newwin(lcount, 30, 0, 0);
	attente = newwin(lcount, 30, 0, 30);
	w_personnes = newwin(lcount, 30, 0, 60);
	graphic = newwin(lcount, ccount - 90, 0, 90);
	box(w_gares, 0, 0);
	box(attente, 0, 0);
	box(w_personnes, 0, 0);
	box(graphic, 0, 0);
	mvwprintw(w_gares, 0, 1, "Gares");
	mvwprintw(attente, 0, 1, "Attente");
	mvwprintw(w_personnes, 0, 1, "Personnes à quai");
	mvwprintw(graphic, 0, 1, "Graphic");
	refresh();
	wrefresh(w_gares);
	wrefresh(attente);
	wrefresh(w_personnes);
	wrefresh(graphic);
	for (int i = 0; i < 31; i++) {
		for (int j = 0; j < 31; j++) {
			grid[i][j] = -1;
		}
	}
	grid[0][15] = 0;
	grid[15][15] = 1;
	grid[15][0] = 2;
	grid[15][30] = 3;
	grid[30][15] = 4;
	for (int i = 0; i < 1000; i++) {
		personnes[i].nom = rand() % (10000000 - 1000000) + 1000000;
		personnes[i].destination = rand() % 5;
		personnes[i].attente = 0;
	}
	for (int i = 0; i < 10; i++) {
		trains[i].ligne = rand() % 2;
		int st_dest = (int) ((rand() % 2) * 2);
		trains[i].destination = lignes[trains[i].ligne].stations[st_dest];
		trains[i].backwards = (st_dest == 0);
		trains[i].passagers = 0;
		trains[i].station_actuelle = lignes[trains[i].ligne].stations[rand() % 3];
		trains[i].deplacement = true;
		trains[i].prochain_depart = rand() % DUREE_TRAJET;
	}
	for (int i = 0; i < 5; i++) {
		gares[i].personnes_a_quai = 0;
		gares[i].affluence = rand() % 100;
	}
	define_stations();
	print_stations();
	print_personnes();
	print_grid();
	pthread_create(&ti, NULL, updatetime, NULL);
	getinput();
	pthread_join(ti, NULL);
	return 0;
	Vector(int) s;
	init_vector(s, int, 10);
}

