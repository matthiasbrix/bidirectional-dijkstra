#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "graph.h"
#include <time.h>

int get_vm_peak ();
void read_from_file (struct graph *graph, const char *fname);
int read_offset_in_file (const char *fname);
struct graph_data *read_vertices_and_edges (const char *fname);


#endif
