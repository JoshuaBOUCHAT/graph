#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef struct {
    int *set;
    int size;
}union_find_array;


typedef union_find_array ufa;
ufa *new_ufa(int size);
int find_ufa(ufa* p_ufa,int vertice);
void union_ufa(ufa* p_ufa,int vertice1,int vertice2);

#endif