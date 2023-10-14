#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
    int s1;
    int s2;
    double w;
} edge;

typedef struct Edgelist {
    int s;
    edge* list;
} Edgelist;

int writeEdgeListArray(Edgelist* li1, char* filename, char* filename_raw) {
	Edgelist* li = li1;
    int err = 0;
    FILE* fd;
    FILE* fd_raw;
    err = ((fd = fopen(filename, "w")) != NULL ? 0 : 1);
    err = ((fd_raw = fopen(filename_raw, "w")) != NULL ? 0 : 1);
    err += fprintf(fd, "{\n\"size\": %d,\n\"data\": [\n", li->s);
    err += fprintf(fd_raw, "%d\n", li->s);
    for (int i = 0; i < li->s - 1; i++) {
        err += fprintf(fd_raw, "%d %d %lf\n", li->list[i].s1, li->list[i].s2, li->list[i].w);
        err += fprintf(fd, "{\n\"s1\": %d,\n\"s2\": %d,\n\"w\": %lf\n},\n", li->list[i].s1, li->list[i].s2, li->list[i].w);
    }
    err += fprintf(fd_raw, "%d %d %lf\n", li->list[li->s - 1].s1, li->list[li->s - 1].s2, li->list[li->s - 1].w);
    err += fprintf(fd, "{\n\"s1\": %d,\n\"s2\": %d,\n\"w\": %lf\n}\n", li->list[li->s - 1].s1, li->list[li->s - 1].s2, li->list[li->s - 1].w);
    err += fprintf(fd, "]\n}\n");
    fclose(fd_raw);
    fclose(fd);
    return err;
}

Edgelist* getVars(char* filename) {
    int err = 0;
    FILE* fd;
    err += ((fd = fopen(filename, "w")) != NULL ? 0 : 1);
    Edgelist* li = (Edgelist*) malloc(sizeof(Edgelist));
    fscanf(fd, "%d", &li->s);
    for (int i = 0; i < li->s; i++) {
        fscanf(fd, "%d %d %lf", &li->list[i].s1, &li->list[i].s2, &li->list[i].w);
    }
    fclose(fd);
    return li;
}

int writeRawValue() {
    return 0;
}