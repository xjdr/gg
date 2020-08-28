#include "gtest/gtest.h"
#include "graph.h"
#include "absl/container/flat_hash_set.h"
#include <iostream>

class GraphQueryTest : public ::testing::Test {
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

TEST_F(GraphQueryTest, Author1Wrote) {
  auto subject = "author1";
  auto predicate = "/author/wrote/book";
  absl::flat_hash_set<absl::string_view> books = g.findObjectByPredicate(subject, predicate);

  EXPECT_TRUE(books.contains("book1"));
  EXPECT_TRUE(books.contains("book3"));
  EXPECT_FALSE(books.contains("book2"));
}

TEST_F(GraphQueryTest, AuthorsBooks1And3) {
  auto object1 = "book1";
  auto object2 = "book3";
  auto predicate = "/author/read/book";

  auto ob1 = g.findSubjectByPredicate(object1, predicate);
  auto ob3 = g.findSubjectByPredicate(object2, predicate);

  absl::flat_hash_set<absl::string_view> intersection = g.findIntersection(ob1, ob3);

  EXPECT_TRUE(intersection.contains("author2"));
  EXPECT_TRUE(intersection.contains("author3"));
  EXPECT_FALSE(intersection.contains("author1"));
}
