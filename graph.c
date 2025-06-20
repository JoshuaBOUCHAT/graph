#include "graph.h"
#include "heap.h"
#include "queue.h"

#include <cerrno>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
const double inifity_plus = 1.0 / 0.0;

graph *new_graph(bool oriented, int nb_vertices, int nb_edges, int weighted)
{

    graph *p_graph = malloc(sizeof(graph));
    if (!p_graph)
    {
        goto graph_alloc_error;
    }

    // Si le graphe n'est pas orienté, on double le nombre d'arêtes
    int total_nb_edges = nb_edges << !oriented;

    p_graph->edges = malloc(total_nb_edges * sizeof(edge_t));
    if (!p_graph->edges)
    {
        goto graph_alloc_edges;
    }

    p_graph->vertices = malloc(nb_vertices * sizeof(int));
    if (!p_graph->vertices)
    {
        goto graph_alloc_vertices;
    }
    p_graph->weighted = weighted;
    if (weighted)
    {
        p_graph->weights = malloc(total_nb_edges * sizeof(double));
        if (p_graph->weights == NULL)
        {
            goto graph_alloc_weigth;
        }
    }
    else
    {
        p_graph->weights = NULL;
    }

    for (int i = 0; i < nb_vertices; i++)
    {
        p_graph->vertices[i] = -1;
    }

    p_graph->oriented = oriented;
    p_graph->nb_edges = nb_edges;
    p_graph->nb_vertices = nb_vertices;

    printf("allocated ! \n");
    return p_graph;

graph_alloc_weigth:
    free(p_graph->vertices);
graph_alloc_vertices:
    free(p_graph->edges);
graph_alloc_edges:
    free(p_graph);
graph_alloc_error:
    printf("graph allocation failed!\n");
    return NULL;
}
void free_graph(graph *p_graph)
{
    if (p_graph->weighted)
    {
        free(p_graph->weights);
    }
    free(p_graph->edges);
    free(p_graph->vertices);
    free(p_graph);
}

void insert_single_edge(graph *p_graph, int source, int destination, int insert_index)
{
    edge_t *p_edge = p_graph->edges + insert_index;
    p_edge->vertex = destination;
    p_edge->next = -1;

    int index = p_graph->vertices[source];

    if (index == -1)
    {
        p_graph->vertices[source] = insert_index;
        return;
    }

    while ((p_graph->edges[index].next != -1))
    {
        index = p_graph->edges[index].next;
    }
    p_graph->edges[index].next = insert_index;
}
void insert_single_weighted_edge(graph *p_graph, int source, int destination, int insert_index,
                                 double weigth)
{
    insert_single_edge(p_graph, source, destination, insert_index);
    p_graph->weights[insert_index] = weigth;
}

void insert_edge(graph *p_graph, int source, int destination, int insert_index)
{
    insert_single_edge(p_graph, source, destination, insert_index);
    if (!p_graph->oriented)
    {
        insert_single_edge(p_graph, destination, source, insert_index + 1);
    }
}
void insert_weighted_edge(graph *p_graph, int source, int destination, int insert_index,
                          double weigth)
{
    insert_single_weighted_edge(p_graph, source, destination, insert_index, weigth);
    if (!p_graph->oriented)
    {
        insert_single_weighted_edge(p_graph, destination, source, insert_index + 1, weigth);
    }
}
graph *init_graph_from_file(FILE *file, char *line, int line_size)
{
    if (!fgets(line, line_size, file) || strcmp(line, "#GRAPH#\n") != 0)
    {
        return NULL;
    }
    int nb_vertices = -1;
    int nb_edges = -1;
    int oriented = -1;
    int weighted = -1;
    bool edges_section_found = false;

    while ((fgets(line, line_size, file)))
    {
        if (sscanf(line, "NB_VERTICES %d\n", &nb_vertices))
            continue;
        if (sscanf(line, "NB_EDGES %d\n", &nb_edges))
            continue;
        if (sscanf(line, "ORIENTED %d\n", &oriented))
            continue;
        if (sscanf(line, "WEIGHTED %d\n", &weighted))
            continue;
        if (strcmp(line, "#EDGES#\n") == 0)
        {
            edges_section_found = true;
            break;
        }
    }
    if (!edges_section_found || nb_vertices <= 0 || nb_edges <= 0 ||
        (oriented != 0 && oriented != 1) || (weighted != 0 && weighted != 1))
    {
        printf("Wrong graph variable !\n");
        return NULL;
    }
    // now every variable is define we allocate and return

    return new_graph(oriented, nb_vertices, nb_edges, weighted);
}
bool parse_edges(graph *p_graph, FILE *file, char *line, int line_size)
{

    for (int i = 0; i < p_graph->nb_edges; i++)
    {
        if (fgets(line, line_size, file) == NULL)
        {
            printf("nombre de ligne insufisante !\n");
            return false;
        }

        int source, destination, nb_parsed;
        if (p_graph->weighted)
        {
            double weigth;
            nb_parsed = sscanf(line, "%d;%d;%lf\n", &source, &destination, &weigth);
            if (nb_parsed != 3)
            {
                printf("La lecture d'une arrète à échouer paramètre attendu: 3 obtenable: %i !\n",
                       nb_parsed);
                return false;
            }
            // ici on multiplie par deux l'index d'insertion si c'est un graph non
            // orienté car on ajoute les arrète 2 par 2
            insert_weighted_edge(p_graph, source, destination, i << (!p_graph->oriented), weigth);
        }
        else
        {
            nb_parsed = sscanf(line, "%d;%d\n", &source, &destination);
            if (nb_parsed != 2)
            {
                printf("La lecture d'une arrète à échouer paramètre attendu: 3 obtenable: %i !\n",
                       nb_parsed);
                return false;
            }
            // ici on multiplie par deux l'index d'insertion si c'est un graph non
            // orienté car on ajoute les arrète 2 par 2
            insert_edge(p_graph, source, destination, i << (!p_graph->oriented));
        }
    }
    return true;
}

graph *from_file(const char *filename)
{
    FILE *file;
    if (((file = fopen(filename, "r")) == NULL))
    {
        return NULL;
    }
    char line[BUFFER_SIZE];

    graph *p_graph = init_graph_from_file(file, line, sizeof(line));
    if (p_graph == NULL)
    {
        fclose(file);
        return NULL;
    }

    // une erreur est survenue dans la lecture
    int is_parsing_success = parse_edges(p_graph, file, line, sizeof(line));
    fclose(file);

    if (!is_parsing_success)
    {
        free_graph(p_graph);
        return NULL;
    }

    return p_graph;
}
void display_graph(graph *p_graph)
{
    if (!p_graph || !p_graph->vertices || !p_graph->edges)
    {
        printf("Graphe invalide.\n");
        return;
    }
    bool weithed = p_graph->weighted;
    for (int i = 0; i < p_graph->nb_vertices; i++)
    {
        int index = p_graph->vertices[i];
        if (index == -1)
        {
            printf("%d -> /\n", i);
            continue;
        }
        printf("%d -> ", i);
        while (index != -1)
        {
            edge_t e = p_graph->edges[index];
            if (weithed)
            {
                printf("%d(%lf) ", e.vertex, p_graph->weights[index]);
            }
            else
            {
                printf("%d ", e.vertex);
            }

            index = e.next;
        }
        putchar('\n');
    }
}
int *bfs(graph *p_graph, int first_vertex)
{
    int nb_vertices = p_graph->nb_vertices;

    queue queue;
    if (queue_with_capacity(&queue, nb_vertices) == NULL)
    {
        return NULL;
    }

    int *parent = malloc(nb_vertices * sizeof(int));
    if (parent == NULL)
    {
        free_queue(&queue);
        return NULL;
    }

    for (int i = 0; i < nb_vertices; i++)
    {
        parent[i] = -1;
    }
    parent[first_vertex] = first_vertex;

    enqueue(&queue, first_vertex);
    while (!is_queue_empty(&queue))
    {
        int current = dequeue(&queue);
        int index = p_graph->vertices[current];
        while (index != -1)
        {
            edge_t edge = p_graph->edges[index];
            if (parent[edge.vertex] == -1)
            {
                parent[edge.vertex] = current;
                enqueue(&queue, edge.vertex);
            }
            index = edge.next;
        }
    }
    return parent;
}

void reverse_array(int *arr, int len)
{
    if (arr == NULL || len < 2)
    {
        return;
    }
    int idx = 0;
    int idy = len - 1;
    while (idx < idy)
    {
        int temp = arr[idx];
        arr[idx++] = arr[idy];
        arr[idy--] = temp;
    }
}
int *non_weighted_shortest_path(graph *p_graph, int src, int dest, int *size)
{
    int *parents = bfs(p_graph, src);
    if (parents == NULL)
    {
        return NULL;
    }
    // Gére le cas ou la destination n'as pas été atteite
    if (parents[dest] == -1)
    {
        free(parents);
        *size = -1;
        return NULL;
    }
    int *stack = malloc(sizeof(int) * p_graph->nb_vertices);
    if (stack == NULL)
    {
        free(parents);
        return NULL;
    }
    int stack_index = 0;
    int current = dest;
    // tant que le courant n'est pas la racine du bfs
    while (current != parents[current] && current != -1)
    {
        stack[stack_index++] = current;
        current = parents[current];
    }
    // push the parent node
    stack[stack_index++] = current;

    free(parents);

    // reverse the array to have src ->dest instead of dest-> src
    reverse_array(stack, stack_index);
    *size = stack_index;
    return stack;
}
int *dijkstra(graph *p_graph, int src, int dest, int *size, double *total_weight);

/// size * is used to return the length of path and total_weight is used when the graph is weighted
int *shortest_path(graph *p_graph, int src, int dest, int *size, double *total_weight)
{
    if (!p_graph->weighted)
    {
        return non_weighted_shortest_path(p_graph, src, dest, size);
    }
    else
    {
        return dijkstra(p_graph, src, dest, size, total_weight);
    }
}

int *reconstruct_path(graph *p_graph, heap_node *parent_distances, int dest, int *size)
{
    int *stack = malloc(sizeof(int) * p_graph->nb_vertices);
    if (stack == NULL)
    {
        return NULL;
    }
    int stack_index = 0;
    int current = dest;

    // tant que le courant n'est pas la racine du parcours
    while (current != parent_distances[current].index && current != -1)
    {
        stack[stack_index++] = current;
        current = parent_distances[current].index;
    }
    // push the parent node
    stack[stack_index++] = current;
    reverse_array(stack, stack_index);
    *size = stack_index;
    return stack;
}
heap_node *init_parent_distances(int nb_vertices, int src)
{
    heap_node *array = malloc(sizeof(heap_node) * nb_vertices);
    if (array == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < nb_vertices; i++)
    {
        array[i].distance = inifity_plus;
        array[i].index = -1;
    }
    array[src].distance = 0.0F;
    array[src].index = src;
    return array;
}
void update_neighbors(graph *p_graph, int edge_index, heap_node *parent_distances, heap *p_heap,
                      heap_node current_node)
{
    edge_t current_edge;
    while (edge_index != -1)
    {
        current_edge = p_graph->edges[edge_index];
        double edge_weight = p_graph->weights[edge_index];
        double temp_dist = edge_weight + current_node.distance;
        if (temp_dist < parent_distances[current_edge.vertex].distance)
        {
            parent_distances[current_edge.vertex].distance = temp_dist;
            parent_distances[current_edge.vertex].index = current_node.index;
            push_from_value(p_heap, current_edge.vertex, temp_dist);
        }
        edge_index = current_edge.next;
    }
}

int *dijkstra(graph *p_graph, int src, int dest, int *size, double *total_weight)
{
    int nb_vertices = p_graph->nb_vertices;
    heap_node *parent_distances = init_parent_distances(nb_vertices, src);
    if (parent_distances == NULL)
    {
        printf("Impossible d'initialiser parent_distances dans dijkstra !\n");
        return NULL;
    }
    heap heap;
    if (heap_with_capacity(&heap, nb_vertices) == NULL)
    {
        printf("heap allocation failed !\n");
        free(parent_distances);
        return NULL;
    }
    push_from_value(&heap, src, 0.0F);
    heap_node current_node;
    while (!is_heap_empty(&heap))
    {
        current_node = pop(&heap);
        if (current_node.index == dest)
        {
            break;
        }
        if (current_node.distance > parent_distances[current_node.index].distance)
        {
            continue;
        }
        int edge_index = p_graph->vertices[current_node.index];
        update_neighbors(p_graph, edge_index, parent_distances, &heap, current_node);
    }
    if (total_weight)
    {
        *total_weight = parent_distances[dest].distance;
    }

    // Cas ou aucun chemin permet de relié les deux sommet
    if (parent_distances[dest].index == -1)
    {
        *size = -1;
        free(parent_distances);
        free_heap(&heap);
        return NULL;
    }
    int *path = reconstruct_path(p_graph, parent_distances, dest, size);

    free(parent_distances);
    free_heap(&heap);

    return path;
}

typedef struct
{
    int src;
    int dest;
    double weight;
} kruskal_edge;

int sort_kruskal_edge(const void *p_edge1, const void *p_edge2)
{
    const kruskal_edge *edge1 = (const kruskal_edge *)p_edge1;
    const kruskal_edge *edge2 = (const kruskal_edge *)p_edge2;

    return (edge1->weight > edge2->weight) - (edge1->weight < edge2->weight);
}

kruskal_edge *get_edges(graph *p_graph)
{

    // optional check to be 100% safe
    if (!p_graph->vertices || !p_graph->edges || !p_graph->weights)
    {
        return NULL;
    }
    kruskal_edge *edges = malloc(p_graph->nb_edges * sizeof(kruskal_edge));
    if (edges == NULL)
    {
        perror("Allocation failed in get_graph_edges when allocating edges \n");
        return NULL;
    }
    int edges_index = 0;
    for (int i = 0; i < p_graph->nb_vertices; i++)
    {
        int index = p_graph->vertices[i];
        while (index != -1)
        {
            edge_t edge = p_graph->edges[index];

            kruskal_edge to_insert;
            to_insert.src = i;
            to_insert.dest = edge.vertex;
            to_insert.weight = p_graph->weights[index];
            edges[edges_index++] = to_insert;

            index = edge.next;
        }
    }
    if (edges_index != p_graph->nb_edges)
    {
        printf("Number of edges inside the graph: %d ne corespond pas au nombre indiqué %d !\n",
               edges_index, p_graph->nb_edges);
        free(edges);
        return NULL;
    }
    return edges;
}
static inline kruskal_edge *get_sorted_edges(graph *p_graph)
{
    kruskal_edge* edges=get_edges(p_graph);
    if (edges)
    {
        qsort(edges, sizeof(kruskal_edge) * p_graph->nb_edges, p_graph->nb_edges, sort_kruskal_edge);
    }
    return edges;
}


/// @brief assum that the graph is weighted and not oriented
/// @param p_graph
/// @return NULL on error and a new allocated graph represententing the minimal spaning sub-tree of
/// the the param
graph *kruskal(graph *p_graph)
{
    // on allou le nouveau graph
    // pour conect N sommet on à besoins de N-1 arrete
    graph *p_minimal = new_graph(false, p_graph->nb_vertices, p_graph->nb_edges, true);
    if (p_minimal == NULL)
    {
        printf("Impossible d'initialiser le graph couvrant de poids minimal !\n");
        return NULL;
    }
    kruskal_edge *sorted_edges = get_sorted_edges(p_graph);
    if (sorted_edges == NULL)
    {
        free_graph(p_minimal);
        return NULL;
    }
    
}

graph *minimum_spaning(graph *p_graph)
{
    if (p_graph == NULL)
    {
        printf("Passing null graph in minimum_spaning !\n");
        return NULL;
    }
    if (!p_graph->weighted)
    {
        perror("calling minimum_spaning on non weighted graph \n");
        return NULL;
    }

    if (p_graph->oriented)
    {
        // TODO
        perror("Message not implemented");
        exit(EXIT_FAILURE);
    }
    return kruskal(p_graph);
}
