#include "graph.h"

uint64_t Graph::getMid(const absl::string_view id) {
  return 0;
}

uint64_t Graph::addNode(const absl::string_view id, const absl::string_view type, const std::vector<std::shared_ptr<Edge>> &ev) {
  const absl::Span span = absl::Span<float>(nullptr, 128);
  auto mid = getMid(id);
  auto n = std::make_shared<Node>(mid, id, type, ev, span);
  auto ok = mid_index.try_emplace(mid, n);

  if (ok.second) {
    id_index.try_emplace(id, mid);
    return mid;
  } else {
    return -1;
  }
}

std::shared_ptr<Node> Graph::at(const absl::string_view id) {
  return mid_index[id_index[id]];
}
std::shared_ptr<Node> Graph::at(const uint64_t mid) {
  return mid_index[mid];
}

void Graph::addEdge(const absl::string_view subject,
             const absl::string_view predicate,
             const absl::string_view object,
             const double weight,
             const absl::Time timestamp,
             const absl::flat_hash_map<absl::string_view, absl::string_view> &metadata) {
  size_t mid = db.size();

  uint64_t s_mid;
  uint64_t p_mid;
  uint64_t o_mid;

  if (id_index.contains(subject)) {
    s_mid = id_index[subject];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    s_mid = addNode(subject, "", ev);
  }

  if (id_index.contains(predicate)) {
    p_mid = id_index[predicate];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    p_mid = addNode(predicate, "", ev);
  }

  if (id_index.contains(object)) {
    o_mid = id_index[object];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    o_mid = addNode(object, "", ev);
  }

  auto e = std::make_shared<Edge>(mid, s_mid, p_mid, o_mid, weight, timestamp, metadata);

  db.push_back(e);
  mid_index[id_index[subject]]->edges.emplace_back(e);
  mid_index[id_index[predicate]]->edges.emplace_back(e);
  mid_index[id_index[object]]->edges.emplace_back(e);
}

