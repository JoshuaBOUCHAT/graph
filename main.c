#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main()
{
    graph *p_graph = from_file("weighted.grf");
    if (p_graph == NULL)
    {
        printf("Graph initialisation failed !\n");
        return EXIT_FAILURE;
    }

    display_graph(p_graph);
    int size;
    double distance;
    int *path = shortest_path(p_graph, 0, 15, &size, &distance);
    if (path==NULL)
    {
        free_graph(p_graph);
    }
    printf("The total distance to go from 0 to 15 is :%lf",distance);
    for (int i = 0; i < size; i++)
    {
        printf("%d ", path[i]);
    }
    
    putchar('\n');
    free(path);
    free_graph(p_graph);
}
