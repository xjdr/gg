#include <utility>
#include <vector>
#include <cstdint>
#include <memory>
#include <absl/container/flat_hash_map.h>
#include <absl/strings/string_view.h>

struct Edge {
  // Constructor needed until C++2020
  Edge(unsigned long mid,
       unsigned long subject,
       unsigned long predicate,
       unsigned long object,
       double weight,
       absl::Time timestamp,
       absl::flat_hash_map<absl::string_view, absl::string_view> metadata)
        : mid(mid), subject(subject), predicate(predicate), object(object), weight(weight),  timestamp(timestamp), metadata(std::move(metadata)){

  }
  uint64_t mid;
  uint64_t subject;
  uint64_t predicate;
  uint64_t object;
  double weight;
  absl::Time timestamp;
  absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
};

struct Node {
  // Constructor needed until C++2020
  Node(uint64_t mid,
       absl::string_view id,
  absl::string_view type,
       std::vector<std::shared_ptr<Edge>>  edges,
       absl::Span<float> memory) : mid(mid), id(id), type(type), edges(std::move(edges)), memory(memory) {

  }
  uint64_t mid;
  absl::string_view id;
  absl::string_view type;
  std::vector<std::shared_ptr<Edge>> edges;
  absl::Span<float> memory;
};

struct Graph {
  std::vector<std::shared_ptr<Edge>> db;
  absl::flat_hash_map<uint64_t, std::shared_ptr<Node>> index;

  void addEdge(uint64_t subject, uint64_t predicate, uint64_t object, double weight, absl::Time timestamp, absl::flat_hash_map<absl::string_view, absl::string_view> metadata) {
    size_t mid = db.size();
    auto e = std::make_shared<Edge>(mid, subject, predicate, object, weight, timestamp, metadata);
    db.push_back(e);

    if (index.contains(subject)) {
      index[subject]->edges.emplace_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr,128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[subject] = n;
    }

    if (index.contains(predicate)) {
      index[predicate]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr,128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[predicate] = n;
    }

    if (index.contains(object)) {
      index[object]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr,128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[object] = n;
    }
  }
};
