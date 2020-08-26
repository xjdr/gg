#include "graph.h"

int main() {
  Graph g;

  for (int kI = 0; kI < 110000000; ++kI) {
    g.addEdge(kI,kI+1,kI+2,kI+3, kI+4);
  }

  return 0;
}
