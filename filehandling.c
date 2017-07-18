#include "filehandling.h"

FILE *file;

/**
 * parse_line - parsing line from /proc/self/status
 * @line: the line being passed
 * Parsing the line for memory reading as a string and
 * return the memory consumption as an integer
 */
int parse_line (char *line)
{
	// This assumes that a digit will be found
	// and the line ends in " Kb".
	int i = strlen (line);
	const char* p = line;
	while (*p < '0' || *p > '9')
		p++;
	line[i-3] = '\0';
	i = atoi(p);
	return i;
}

/**
 * get_vm_peak - peak virtual memory use
 * Note: Works only on Linux and value is in kilobytes
 */
int get_vm_peak ()
{
	file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];

	while (fgets(line, 128, file) != NULL) {
		if (strncmp(line, "VmPeak:", 7) == 0) {
			result = parse_line(line);
			break;
		}
	}
	if (fclose(file)) {
		printf("Error closing file.");
		exit (EXIT_FAILURE);
	}
	return result;
}

/**
 * read_offset_in_file - reading from the input file whether 0 or 1 indexed
 * @fname: The input file name
 * iterates through all to check whether there are some vertices in between
 * that have lowest vertex id. The vertex id indicates the index
 */
int read_offset_in_file (const char *fname)
{
	char t;
	int u, v, w, i, offset;

	i = 0;
	offset = (int) INFINITY;
	file = fopen (fname, "r");

	if (file == NULL)
		exit (EXIT_FAILURE);

	while (!feof(file)) {
		int num_match = fscanf (file, "%c %d %d %d\n", &t, &u, &v, &w);
		if (num_match == 4 && (i % 2) == 0 && t == 'a') {
			offset = u < offset ? u : offset;
			offset = v < offset ? v : offset;
		}
		i++;
	}

	if (fclose(file)) {
		printf("Error closing file.");
		exit (EXIT_FAILURE);
	}

	return offset;
}

/**
 * read_from_file - reading node & edges from the input graph file
 * @graph: the graph to be filled with content
 * @fname: the file name to be read
 * Reading the graph in the file. The function is tailored for
 * the DIMACS ssp graph format (.gr)
 * http://www.diag.uniroma1.it/challenge9/format.shtml
 */
void read_from_file (struct graph *graph, const char *fname)
{
	char t;
	int u, v, w, i;

	i = 0;
	file = fopen (fname, "r");

	if (file == NULL)
		exit (EXIT_FAILURE);

	while (!feof(file)) {
		int num_match = fscanf (file, "%c %d %d %d\n", &t, &u, &v, &w);
		if (num_match == 4 && (i % 2) == 0 && t == 'a') {
			// If e.g 1 is used in input,
			// subtract the offset such that begin is 0
			add_edges (graph, u-offset, v-offset, w);
		}
		i++;
	}

	if (fclose(file)) {
		printf("Error closing file.");
		exit (EXIT_FAILURE);
	}

	return;
}

/**
 * read_vertices_and_edges - reading number of vertices and edges
 * @fname: the file name to be read
 * In DIMACS-like files, p stands for the problem line, means it is unique and
 * must appear as the first non-comment line.
 * In the p-line number of nodes and the number of edges can be read.
 */
struct graph_data *read_vertices_and_edges (const char *fname)
{
	file = fopen (fname, "r");
	char p;
	char t[256];
	int n, m;

	if (file == NULL)
		exit (EXIT_FAILURE);

	while (!feof(file)) {
		int no_match = fscanf (file, "%s %s %d %d\n", &p, t, &n, &m);
		if (no_match == 4 && p == 'p') {
			break;
		}
	}

	if (fclose(file)) {
		printf("Error closing file in count_vertices.");
		exit (EXIT_FAILURE);
	}

	struct graph_data *gd = malloc (sizeof (struct graph_data));
	gd->n = n;
	gd->m = m;

	return gd;
}
