#include "main.h"

int offset;

/**
 * run_bdj - wrapper function for running bidirectional dijkstra from u to v
 * @graph: graph with vertices and edges
 * @u: source vertex u
 * @v: target vertex v
 * @n: number of vertices in graph
 * @m: number of edges in graph
 * Calls Bidirectional Dijkstra's algorithm, and measures the spent RAM and CPU time
 */
struct sssp_res *run_bdj (struct graph *graph, int s, int t, int n, int m)
{
	struct sssp_res *bdj;
	double cpu_time_spent = 0.0, avg_ins = 0.0, avg_dec = 0.0, avg_ext = 0.0;
	clock_t begin = clock();
	bdj = bidirectional_dijkstra (graph, s-offset, t-offset);
	clock_t end = clock();
	if (bdj == NULL) {
		perror ("No path (u, v) in djkstra_opt_alg could be found\n");
		exit (-1);
	}
	cpu_time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	avg_ext = bdj->avg_extract_min_time /
		bdj->num_extract_min / MS;
	avg_dec = bdj->avg_decrease_key_time /
		bdj->num_decrease_key / MS;
	avg_ins = bdj->avg_min_heap_insert_time = bdj->avg_min_heap_insert_time /
		bdj->visited_nodes / MS;
	bdj->memory_consump = get_vm_peak ();
	bdj->avg_extract_min_time = avg_ext;
	bdj->avg_decrease_key_time = avg_dec;
	bdj->avg_min_heap_insert_time = avg_ins;
	bdj->dist_time = cpu_time_spent;
	bdj->visited_nodes_ratio = (double)bdj->visited_nodes/(double)(2*n);
	bdj->visited_edges_ratio = (double)bdj->visited_edges/(double)(2*m);

	bdj->query_times = 1;

	printf ("Result of Bidirectional Dijkstra (%d, %d) = %d\n", s, t, bdj->dist);
	printf ("vertices n=%d, edges m=%d in graph\n", n, m);
	printf ("Visiting ratio of vertices = %f, edges = %f\n",
			bdj->visited_nodes_ratio,
			bdj->visited_edges_ratio);
	printf ("%d extract-min operations. Avg time pr. operation: %.10f ms\n",
			bdj->num_extract_min, bdj->avg_extract_min_time);
	printf ("%d decrease-key operations. Avg time pr. operation: %.10f ms\n",
			bdj->num_decrease_key, bdj->avg_decrease_key_time);
	printf ("%d min-heap-insert operations. Avg time pr. operation: %.10f ms\n",
			bdj->visited_nodes, bdj->avg_min_heap_insert_time);
	printf ("Time spent on running Bidirectional Dijkstra (%d, %d) = %f sec\n", s, t, bdj->dist_time);
	printf ("Algorithm was executed %d times\n", bdj->query_times);
	printf ("Memory usage of Bidirectional Dijkstra = %d KB\n", bdj->memory_consump);

	return bdj;
}

int main (int argc, char *argv[])
{
	if ((argc-1) < MIN_REQUIRED || (((argc-1) % MIN_REQUIRED) != 0)) {
		printf ("No input and output arguments or input/output does not match!\n");
		printf ("Number of arguments is %d\n\n", argc-1);
		printf ("Interface should be:\n");
		printf ("./bin/main <DIMACS SSSP input file> <source vertex> <target vertex>\n");
		return EXIT_FAILURE;
	} else {
			const char *fname_read = argv[1];
			offset = read_offset_in_file (fname_read);
			const int s = atoi(argv[2]);
			const int t = atoi(argv[3]);
			struct graph_data *gd = read_vertices_and_edges (fname_read);
			if (s > gd->n || t > gd->n) {
				printf ("Source vertex u or/and target vertex v is/are invalid\n");
				printf ("Please consider a valid vertex that is <= n\n\n");
				return EXIT_FAILURE;
			}
			struct graph *graph = init_graph (gd->n);
			read_from_file (graph, fname_read);
			struct sssp_res *bdj = run_bdj (graph, s, t, gd->n, gd->m);
			bdj = bdj;
	}
	return EXIT_SUCCESS;
}
