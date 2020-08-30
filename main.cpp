#include "graph.h"

#include <random>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <jemalloc/jemalloc.h>


const static int TEST_SIZE = 3000000;

int main(int argc, char **argv) {
  FLAGS_logtostderr = true;
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  Graph g;

  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> node_distribution(0, TEST_SIZE / (6 + 7));
  std::uniform_int_distribution<> pred_distribution(0, 6);

  auto t = absl::Now();
  std::string k = "key";
  std::string v = "val";
  absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
  metadata[k] = v;

  for (int kI = 0; kI < TEST_SIZE; ++kI) {
    auto sub = absl::StrCat("author", node_distribution(generator));
    auto pred = absl::StrCat("/author/", pred_distribution(generator), "/book");
    auto obj = absl::StrCat("book", node_distribution(generator));

    g.addEdge(sub, pred, obj, 1.0, t, metadata);

    if (kI % 1000000 == 0) {
      LOG(INFO) << sub;
      LOG(INFO) << pred;
      LOG(INFO) << obj;

      LOG(INFO) << "Processed: " << kI << " messages";
    }
  }

  malloc_stats_print(nullptr, nullptr, nullptr);


  return 0;
}
