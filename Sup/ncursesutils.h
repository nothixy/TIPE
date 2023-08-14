#include <ncurses.h>
extern int get_train(int i, int j);
extern int** grid;
extern WINDOW* graphic;
extern int** stations;
extern int GREEN;
extern int RED;

void print_grid() {
	for (int i = 0; i < 31; i++) {
		for (int j = 0; j < 31; j++) {
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