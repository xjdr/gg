#include <vector>
#include <cstdint>
#include <memory>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/strings/string_view.h>

enum Field {
  SUBJECT,
  PREDICATE,
  OBJECT,
};

struct Edge {
  // Constructor needed until C++2020
  Edge(/*const uint64_t mid,*/
      const uint64_t subject,
      const uint64_t predicate,
      const uint64_t object,
      double weight,
      const absl::Time timestamp,
      absl::flat_hash_map<absl::string_view, absl::string_view> metadata)
      : /*mid(mid),*/
      subject(subject),
      predicate(predicate),
      object(object),
      weight(weight),
      timestamp(timestamp),
      metadata(std::move(metadata)) {

  }
  //const uint64_t mid;
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
  //std::vector<std::shared_ptr<Edge>> db;
  absl::flat_hash_map<const absl::string_view, const uint64_t> id_index;
  absl::flat_hash_map<const uint64_t, const std::shared_ptr<Node>> mid_index;

  uint64_t getMid(absl::string_view id);
  uint64_t addNode(absl::string_view id, absl::string_view type, const std::vector<std::shared_ptr<Edge>> &ev);

 public:
  absl::optional<std::shared_ptr<Node>> at(absl::string_view id);
  absl::optional<std::shared_ptr<Node>> at(uint64_t mid);

  void addEdge(absl::string_view subject,
               absl::string_view predicate,
               absl::string_view object,
               double weight,
               absl::Time timestamp,
               const absl::flat_hash_map<absl::string_view, absl::string_view> &metadata);

  absl::flat_hash_set<absl::string_view> findObjectByPredicate(absl::string_view subject, absl::string_view predicate);

  absl::flat_hash_set<absl::string_view> findSubjectByPredicate(absl::string_view object, absl::string_view predicate);

  absl::flat_hash_set<absl::string_view> findIntersection(const absl::flat_hash_set<absl::string_view> &set1,
                                                          const absl::flat_hash_set<absl::string_view> &set2);

  absl::flat_hash_set<uint64_t> findIntersection(const absl::flat_hash_set<uint64_t> &set1,
                                                 const absl::flat_hash_set<uint64_t> &set2);

  absl::flat_hash_set<absl::string_view> connectedComponents(absl::string_view seed);

  absl::flat_hash_set<absl::string_view> getNodesByType(uint64_t seed_mid, absl::string_view type);

  absl::flat_hash_set<std::shared_ptr<Edge>> egoNet(absl::string_view ego);

  void personaClusters(const absl::flat_hash_set<std::shared_ptr<Edge>> &ego_net,
                                                                    uint64_t ego_mid);

  absl::flat_hash_set<absl::string_view> CommonNeighbors(absl::string_view nA, absl::string_view nB);

//  absl::flat_hash_set<absl::flat_hash_set<absl::string_view>> LabelPropCluster();
//
//  absl::flat_hash_set<absl::flat_hash_set<absl::string_view>> ModularityCluster();

  double JaccardCoefficient(absl::string_view nA, absl::string_view nB);

  double AdamicAdar(absl::string_view nA, absl::string_view nB);

};
