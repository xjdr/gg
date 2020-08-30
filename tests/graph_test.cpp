#include "gtest/gtest.h"
#include "graph.h"

 class GraphTest : public ::testing::Test {
 protected:
   void SetUp() override {
     absl::string_view k = "key";
     absl::string_view v = "val";
     metadata[k] = v;

     g.addEdge("author1", "/author/wrote/book", "book1", 1.0, absl::Now(), metadata);
     g.addEdge("author2", "/author/wrote/book", "book2", 1.0, absl::Now(), metadata);
     g.addEdge("author3", "/author/wrote/book", "book3", 1.0, absl::Now(), metadata);
   }

   Graph g;
   absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
 };

TEST_F(GraphTest, BasicInsert) {
  std::string sub = "subject";
  const char * pred = &"predicate" ['\0'];
  absl::string_view obj = "object";

  g.addEdge(sub, pred, obj, 1.0, absl::Now(), metadata);

  EXPECT_EQ("subject", g.at("subject").value()->id);
  EXPECT_EQ("predicate", g.at("predicate").value()->id);
  EXPECT_EQ("object", g.at("object").value()->id);
}

TEST_F(GraphTest, GetByStringId) {
  EXPECT_EQ("author1", g.at("author1").value()->id);
  EXPECT_EQ("author2", g.at("author2").value()->id);
  EXPECT_EQ("author3", g.at("author3").value()->id);
}

TEST_F(GraphTest, GetByMId) {
  EXPECT_EQ("author1", g.at(0).value()->id);
  EXPECT_EQ("author2", g.at(3).value()->id);
  EXPECT_EQ("author3", g.at(5).value()->id);
}

