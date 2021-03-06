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

  EXPECT_TRUE(n.contains("book1"));
  EXPECT_TRUE(n.contains("book2"));
  EXPECT_TRUE(n.contains("book3"));

  EXPECT_FALSE(n.contains("author3"));
}

TEST_F(GraphAlgoTest, JaccardCoefficient) {
  double jc = g.JaccardCoefficient("author1", "author2");
  EXPECT_DOUBLE_EQ(0.75, jc);
}

TEST_F(GraphAlgoTest, AdamicAdar) {
  double aa = g.AdamicAdar("author1", "author2");
  EXPECT_DOUBLE_EQ(  2.2957285153592331, aa);
}
