#include "union_find.h"

ufa *new_ufa(int size);
int find_ufa(ufa* p_ufa,int vertice);
void union_ufa(ufa* p_ufa,int vertice1,int vertice2);