#include "gtest/gtest.h"
#include "graph.h"
#include "absl/container/flat_hash_set.h"
#include <iostream>

class GraphAlgoTest : public ::testing::Test {
 protected:
  void SetUp() override {
    absl::string_view k = "key";
    absl::string_view v = "val";
    absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
    metadata[k] = v;

    g.addEdge("author1", "/author/wrote/book", "book1", 1.0, absl::Now(), metadata);
    g.addEdge("author2", "/author/wrote/book", "book2", 1.0, absl::Now(), metadata);
    g.addEdge("author1", "/author/wrote/book", "book3", 1.0, absl::Now(), metadata);

    g.addEdge("author1", "/author/read/book", "book2", 1.0, absl::Now(), metadata);
    g.addEdge("author1", "/author/read/book", "book3", 1.0, absl::Now(), metadata);

    g.addEdge("author2", "/author/read/book", "book3", 1.0, absl::Now(), metadata);
    g.addEdge("author2", "/author/read/book", "book1", 1.0, absl::Now(), metadata);

    g.addEdge("author3", "/author/read/book", "book1", 1.0, absl::Now(), metadata);
    g.addEdge("author3", "/author/read/book", "book2", 1.0, absl::Now(), metadata);
    g.addEdge("author3", "/author/read/book", "book3", 1.0, absl::Now(), metadata);

  }

  Graph g;
};

TEST_F(GraphAlgoTest, CommonNeighbors) {
  auto n = g.CommonNeighbors("author1", "author2");

  for(auto& n_ : n) {
    std::cout << n_ << std::endl;
  }
}

TEST_F(GraphAlgoTest, JaccardCoefficient) {
  double jc = g.JaccardCoefficient("author1", "author2");

  std::cout << jc << std::endl;
}

TEST_F(GraphAlgoTest, AdamicAdar) {
  double aa = g.AdamicAdar("author1", "author2");

  std::cout << aa << std::endl;
}
