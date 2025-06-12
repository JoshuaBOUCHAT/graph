#include <stdio.h>

#include "graph.h"

int main() {
  graph *p_graph = from_file("exemple.grf");
  display_graph(p_graph);
  free_graph(p_graph);
}
