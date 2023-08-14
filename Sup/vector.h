#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define str(ELT) #ELT
#define fmt(ELT) _Generic((ELT), int : "%d", float : "%f", char : "%c", signed char : "%c", unsigned char : "%c", short : "%hi", unsigned short : "%hu", unsigned int : "%u", long int : "%ld", long unsigned int : "%lu", long long : "%lld", unsigned long long : "%llu", double : "%lf", long double : "%Lf")
#define type(ELT) _Generic((ELT), int : (int) 0, float : (float) 0.0, char : (char) 'a', signed char : (signed char) 'a', short : (short) 0, unsigned short : (unsigned short) 0, long : (long) 0, unsigned long : (unsigned long) 0, long long : (long long) 0, unsigned long long : (unsigned long long) 0, double : (double) 0.0, long double : (long double) 0.0)
#define Vector(TYPE) struct {TYPE* tab; int len;}
#define push(VEC, ELT) \
	({ \
		VEC.len++; \
		realloc(VEC.tab, sizeof(VEC) + sizeof(ELT)); \
		VEC.tab[VEC.len - 1] = ELT; \
	})
#define insert(VEC, ELT, INDEX) \
	({ \
		VEC.len++; \
		realloc(VEC.tab, VEC.len * sizeof(ELT)); \
		for (int i = VEC.len - 1; i > INDEX; i--) { \
			VEC.tab[i] = VEC.tab[i - 1]; \
		}; \
		VEC.tab[INDEX] = ELT; \
	})
#define print_vector(VEC) \
	({ \
		printf("Vector : %s\n", str(VEC)); \
		printf("Size : %d\n", VEC.len); \
		printf("Elements : "); \
		for (int i = 0; i < VEC.len; i++) { \
			printf(fmt(nth(VEC, i)), nth(VEC, i)); \
			printf(" "); \
		} \
		printf("\n"); \
	})
#define nth(VEC, N) VEC.tab[N]
#define delete_index(VEC, INDEX) \
	({ \
		for (int i = INDEX; i < VEC.len; i++) { \
			VEC.tab[i] = VEC.tab[i + 1]; \
		} \
		if (VEC.len > 0) { \
			VEC.len--; \
			realloc(VEC.tab, VEC.len * sizeof(VEC.tab[0])); \
		} else { \
			realloc(VEC.tab, 0); \
		} \
	})
#define indexOfVector(ELT, VEC) \
	({ \
		int x = -1; \
		for (int i = 0; i < VEC.len; i++) { \
			if (VEC.tab[i] == ELT) { \
				x = i; \
				break; \
			} \
		} \
		x; \
	})
#define delete_element(VEC, ELT) \
	({delete_index(VEC, index_of(VEC, ELT));})
#define set(VEC, INDEX, ELT) \
	({ \
		if (VEC.len >= INDEX) { \
			VEC.tab[INDEX] = ELT; \
		} \
	})
#define init_vector(VEC, TYPE, LEN) \
	({ \
		VEC.tab = malloc(LEN * sizeof(TYPE)); \
		VEC.len = LEN; \
	})
#define merge(VEC1, TYPE1, VEC2, TYPE2) \
	({ \
		if (TYPE1 == TYPE2) { \
			Vector(TYPE1) newV; \
			newV.len = VEC1.len + VEC2.len; \
			int j = 0; \
			for (int i = 0; i < VEC1.len; i++) { \
				newV.tab[j] = VEC1.tab[i]; \
				j++; \
			} \
			for (int i = 0; i < VEC2.len; i++) { \
				newV.tab[j] = VEC2.tab[i]; \
				j++; \
			} \
			return newV; \
		} \
	})
#define iter(VEC, F) \
	({ \
		for (int i = 0; i < VEC.len; i++) { \
			F(VEC.tab[i]); \
		} \
	})
#define exists(VEC, E) \
	({ \
		bool found = false; \
		for (int i = 0; i < VEC.len; i++) { \
			if (VEC.tab[i == E]) found = true; \
		} \
		found; \
	})
#define len(VEC) \
	({VEC.len;})