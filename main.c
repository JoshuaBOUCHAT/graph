#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main()
{
  graph *p_graph = from_file("exemple.grf");
  if (p_graph == NULL)
  {
    printf("Graph initialisation failed !\n");
    return EXIT_FAILURE;
  }
  display_graph(p_graph);
  int size;
  int *path = shortest_path(p_graph, 0, 9, &size, NULL);
  for (int i = 0; i < size; i++)
  {
    printf("%d ", path[i]);
  }
  putchar('\n');
  free(path);
  free_graph(p_graph);
}
