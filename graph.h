#include <utility>
#include <vector>
#include <cstdint>
#include <memory>
#include <absl/container/flat_hash_map.h>
#include <absl/strings/string_view.h>

struct Edge {
  // Constructor needed until C++2020
  Edge(const uint64_t mid,
       const uint64_t subject,
       const uint64_t predicate,
       const uint64_t object,
       double weight,
       const absl::Time timestamp,
       absl::flat_hash_map<absl::string_view, absl::string_view> metadata)
      : mid(mid),
        subject(subject),
        predicate(predicate),
        object(object),
        weight(weight),
        timestamp(timestamp),
        metadata(std::move(metadata)) {

  }
  const uint64_t mid;
  const uint64_t subject;
  const uint64_t predicate;
  const uint64_t object;
  double weight;
  const absl::Time timestamp;
  const absl::flat_hash_map<absl::string_view, absl::string_view> metadata;
};

struct Node {
  // Constructor needed until C++2020
  Node(const uint64_t mid,
       const absl::string_view id,
       const absl::string_view type,
       std::vector<std::shared_ptr<Edge>> edges,
       const absl::Span<float> memory) : mid(mid), id(id), type(type), edges(std::move(edges)), memory(memory) {

  }
  const uint64_t mid;
  const absl::string_view id;
  const absl::string_view type;
  std::vector<std::shared_ptr<Edge>> edges;
  const absl::Span<float> memory;
};

struct Graph {
  std::vector<std::shared_ptr<Edge>> db;
  absl::flat_hash_map<const uint64_t, std::shared_ptr<Node>> index;

  void addEdge(const uint64_t subject,
               const uint64_t predicate,
               const uint64_t object,
               const double weight,
               const absl::Time timestamp,
               const absl::flat_hash_map<absl::string_view, absl::string_view> &metadata) {
    size_t mid = db.size();
    auto e = std::make_shared<Edge>(mid, subject, predicate, object, weight, timestamp, metadata);
    db.push_back(e);

    if (index.contains(subject)) {
      index[subject]->edges.emplace_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr, 128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[subject] = n;
    }

    if (index.contains(predicate)) {
      index[predicate]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr, 128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[predicate] = n;
    }

    if (index.contains(object)) {
      index[object]->edges.push_back(e);
    } else {
      std::vector<std::shared_ptr<Edge>> ev{e};
      absl::Span span = absl::Span<float>(nullptr, 128);
      auto n = std::make_shared<Node>(subject, "id", "node", ev, span);
      index[object] = n;
    }
  }
};
