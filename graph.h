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

class Graph {
 private:
  std::vector<std::shared_ptr<Edge>> db;
  absl::flat_hash_map<const absl::string_view, uint64_t> id_index;
  absl::flat_hash_map<const uint64_t, std::shared_ptr<Node>> mid_index;

  uint64_t getMid(absl::string_view id);
  uint64_t addNode(absl::string_view id, absl::string_view type, const std::vector<std::shared_ptr<Edge>>& ev);

 public:
  std::shared_ptr<Node> at(absl::string_view id);
  std::shared_ptr<Node> at(uint64_t mid);

  void addEdge(absl::string_view subject,
               absl::string_view predicate,
               absl::string_view object,
               double weight,
               absl::Time timestamp,
               const absl::flat_hash_map<absl::string_view, absl::string_view> &metadata);
};
