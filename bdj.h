#ifndef SSP_H
#define SSP_H

#include "graph.h"
#include "minheap.h"

struct sssp_res *bidirectional_dijkstra (struct graph *gf, int u, int v);

#endif
