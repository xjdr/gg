#include "gtest/gtest.h"
#include "graph.h"

 TEST(GraphTest, basic) {
   Graph g;
  absl::string_view k = "key";
  absl::string_view v = "val";
  absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
  metadata[k] = v;

  g.addEdge(1,2,3,4.0, absl::Now(), metadata);

   EXPECT_EQ(1,g.db.at(0)->subject);
   EXPECT_EQ(g.db.at(0)->subject, g.index[1]->edges.at(0)->subject);
   EXPECT_EQ(1, g.db[0]->subject);
   EXPECT_EQ(2, g.db[0]->predicate);
   EXPECT_EQ(3, g.db[0]->object);

 }

// TEST(GraphTest, loadTest) {
//   Graph g;

//   for (int kI = 0; kI < 100000000; ++kI) {
//     g.addEdge(kI,kI+1,kI+2,kI+3, kI+4);
//   }
// }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
