#ifndef GRAPH
#define GRAPH

#include <stdbool.h>

typedef struct _egde_t {
  int vertex;
  int next;

} edge_t;

typedef struct graph {
  int nb_vertices;
  int nb_edges;
  /// contain the edges (allocation)
  edge_t *edges;
  /// point to the first edge_t related to edge[i]
  int *vertices;

  bool oriented;

} graph;

/// return the graph pointer if every things is ok return NULL if an
/// error occurs
graph *from_file(const char *filename);
void free_graph(graph *p_graph);

#endif