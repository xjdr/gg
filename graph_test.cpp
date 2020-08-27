#include "gtest/gtest.h"
#include "graph.h"

 class GraphTest : public ::testing::Test {
  protected:
   void SetUp() override {
     absl::string_view k = "key";
     absl::string_view v = "val";
     absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
     metadata[k] = v;

     g.addEdge("author1", "/author/wrote/book", "book1", 1.0, absl::Now(), metadata);
   }

   Graph g;
};

TEST_F(GraphTest, GetByStringId) {
  EXPECT_EQ("author1", g.at("author1")->id);
}

TEST_F(GraphTest, GetByMId) {
  EXPECT_EQ("author1", g.at(0)->id);
}

