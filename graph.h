#include <utility>
#include <vector>
#include <cstdint>
#include <cstring>
#include <absl/container/flat_hash_map.h>
#include <memory>

struct Edge {
  // Constructor needed until C++2020
  Edge(unsigned long mid,
       unsigned long timestamp,
       double weight,
       unsigned long subject,
       unsigned long predicate,
       unsigned long object)
      : mid(mid), timestamp(timestamp), weight(weight), subject(subject), predicate(predicate), object(object) {

  }
  uint64_t mid;
  uint64_t timestamp;
  double weight;
  uint64_t subject;
  uint64_t predicate;
  uint64_t object;
};

struct Node {
  // Constructor needed until C++2020
  Node(uint64_t mid,
       std::vector<std::shared_ptr<Edge>> edges) : mid(mid), edges(std::move(edges)) {

  }
  uint64_t mid;
  std::vector<std::shared_ptr<Edge>> edges;
};

struct Graph {
  std::vector<std::shared_ptr<Edge>> db;
  //std::vector<std::shared_ptr<Node>> nodes;
  absl::flat_hash_map<uint64_t, std::shared_ptr<Node>> index;
  void addEdge(uint64_t subject, uint64_t predicate, uint64_t object, double weight, uint64_t timestamp) {
    size_t mid = db.size();
    auto e = std::make_shared<Edge>(mid, timestamp, weight, subject, predicate, object);
    db.push_back(e);

    if (index.contains(subject)) {
      index[subject]->edges.emplace_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      auto n = std::make_shared<Node>(subject, ev);
      //nodes.push_back(n);
      index[subject] = n;
    }

    if (index.contains(predicate)) {
      index[predicate]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      auto n = std::make_shared<Node>(subject, ev);
      //nodes.push_back(n);
      index[predicate] = n;
    }

    if (index.contains(object)) {
      index[object]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      auto n = std::make_shared<Node>(subject, ev);
      //nodes.push_back(n);
      index[object] = n;
    }
  }
};
