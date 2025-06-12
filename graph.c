#include "graph.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

graph *new_graph(bool oriented, int nb_vertices, int nb_edges) {
  graph *p_graph = malloc(sizeof(graph));
  if (!p_graph) {
    goto graph_alloc_error;
  }

  // Si le graphe n'est pas orienté, on double le nombre d'arêtes
  int total_nb_edges = nb_edges << !oriented;

  p_graph->edges = malloc(total_nb_edges * sizeof(edge_t));
  if (!p_graph->edges) {
    goto graph_alloc_edges;
  }

  p_graph->vertices = malloc(nb_vertices * sizeof(int *));
  if (!p_graph->vertices) {
    goto graph_alloc_vertices;
  }

  p_graph->oriented = oriented;
  p_graph->nb_edges = nb_edges;
  p_graph->nb_vertices = nb_vertices;
  return p_graph;

graph_alloc_vertices:
  free(p_graph->edges);

graph_alloc_edges:
  free(p_graph);

graph_alloc_error:
  return NULL;
}

void insert_single_edge(graph *p_graph, int source, int destination,
                        int insert_index) {
  edge_t *p_edge = p_graph->edges + insert_index;
  p_edge->vertex = destination;
  p_edge->next = -1;

  edge_t *current = p_graph->edges + p_graph->vertices[source];
  if (current == NULL) {
    p_graph->vertices[source] = insert_index;
    return;
  }

  while ((current->next != -1)) {
    current = p_graph->edges + current->next;
  }
  current->next = insert_index;
}
void insert_edge(graph *p_graph, int source, int destination,
                 int insert_index) {
  insert_single_edge(p_graph, source, destination, insert_index);
  if (!p_graph->oriented) {
    insert_single_edge(p_graph, source, destination, insert_index + 1);
  }
}

graph *from_file(const char *filename) {
  FILE *file;
  if (((file = fopen(filename, "r")) == NULL)) {
    goto file_error;
  }
  char line[BUFFER_SIZE];
  int byte_read = -1;

  // on s'assure que le fichier est bien un graph via une balise
  if (!fgets(line, sizeof(line), file) || sscanf(line, "#GRAPH#")) {
    goto reading_error;
  }
  int nb_vertices = -1;
  int nb_edges = -1;
  int oriented = -1;
  bool edges_section_found = false;

  while ((fgets(line, sizeof(line), file))) {
    if (sscanf(line, "NB_VERTICE %d\n", &nb_vertices))
      continue;
    if (sscanf(line, "NB_EDGES %d\n", &nb_edges))
      continue;
    if (sscanf(line, "NB_VERTICE %d\n", &nb_vertices))
      continue;
    if (sscanf(line, "#EDGES#\n")) {
      edges_section_found = true;
      break;
    }
  }
  if (!edges_section_found || nb_vertices <= 0 || nb_edges <= 0 ||
      (oriented != 0 && oriented != 1)) {
    goto reading_error;
  }
  // now every variable is define we allocate and then fill

  graph *p_graph = new_graph(oriented, nb_vertices, nb_edges);
  if (p_graph == NULL) {
    goto reading_error;
  }

  for (int i = 0; i < nb_edges; i++) {
    if (fgets(line, sizeof(line), file) == NULL) {
      goto after_graph_alloc_error;
    }
    int source, destination;
    if (sscanf(line, "%d;%d\n", &source, &destination) != 2) {
      goto after_graph_alloc_error;
    }
    // ici on multiplie par deux l'index d'insertion si c'est un graph non
    // orienté car on ajoute les arrète 2 par 2
    insert_edge(p_graph, source, destination, i << (!p_graph->oriented));
  }

  return p_graph;

after_graph_alloc_error:
  free(p_graph);
reading_error:
  fclose(file);
file_error:
  return NULL;
}

void free_graph(graph *p_graph) {
  free(p_graph->edges);
  free(p_graph->vertices);
  free(p_graph);
}