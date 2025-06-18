#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct
{
    int vertex;
    int next;

} edge_t;

typedef struct
{
    int nb_vertices;
    int nb_edges;
    /// contain the edges (allocation)
    edge_t *edges;
    /// point to the first edge_t related to edge[i]
    int *vertices;

    double *weights;
    bool weighted;

    bool oriented;

} graph;

/// return the graph pointer if every things is ok return NULL if an
/// error occurs
graph *from_file(const char *filename);
void free_graph(graph *p_graph);
void display_graph(graph *p_graph);

/// retourn le tableau de précédence
int *bfs(graph *p_graph, int first_vertex);

int *shortest_path(graph *p_graph, int src, int dest, int *size, double *);

graph *minimum_spaning(graph *p_graph);

#endif