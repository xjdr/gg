#include <iostream>

#include "graph.h"

int main() {
  Graph g;

  for (int kI = 0; kI < 35000000; ++kI) {
    absl::string_view k = "key";
    absl::string_view v = "val";
    absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
    metadata[k] = v;
    g.addEdge(kI,kI+1,kI+2,4.0, absl::Now(), metadata);

    if (kI % 1000000 == 0 ) {
      std::cout << "Processed: " << kI << " messages" << std::endl;
    }
  }

  return 0;
}
