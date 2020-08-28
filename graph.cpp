#include "graph.h"

#include <iostream>

uint64_t Graph::getMid(const absl::string_view id) {
  if (id_index.contains(id)) {
    return id_index[id];
  } else {
    return mid_index.size();
  }
}

absl::optional<std::shared_ptr<Node>> Graph::at(const absl::string_view id) {
  absl::optional<std::shared_ptr<Node>> opt;

  if (id_index.contains(id)) {
    auto mid = id_index[id];
    if (mid_index.contains(mid)) {
      opt.emplace(mid_index[mid]);
    }
  }

  return opt;
}

absl::optional<std::shared_ptr<Node>> Graph::at(const uint64_t mid) {
  absl::optional<std::shared_ptr<Node>> opt;

  if (mid_index.contains(mid)) {
    opt.emplace(mid_index[mid]);
  }

  return opt;
}

uint64_t Graph::addNode(const absl::string_view id,
                        const absl::string_view type,
                        const std::vector<std::shared_ptr<Edge>> &ev) {
  const absl::Span span = absl::Span<float>(nullptr, 128);
  const uint64_t mid = getMid(id);
  const std::shared_ptr<Node> n = std::make_shared<Node>(mid, id, type, ev, span);
  auto ok = mid_index.emplace(mid, n);

  if (ok.second) {
    id_index.emplace(id, mid);
    return mid;
  } else {
    return -1;
  }
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

absl::flat_hash_set<absl::string_view> Graph::findObjectByPredicate(absl::string_view subject,
                                                                    absl::string_view predicate) {
  absl::flat_hash_set<absl::string_view> objects;
  auto edges = at(subject).value()->edges;

  for (auto &e : edges) {
    auto p_opt = at(predicate);
    if (p_opt.has_value()) {
      auto mid = p_opt.value()->mid;
      if (e->predicate == mid) {
        objects.insert(at(e->object).value()->id);
      }
    }
  }

  return objects;
}

absl::flat_hash_set<absl::string_view> Graph::findSubjectByPredicate(absl::string_view object,
                                                                     absl::string_view predicate) {
  absl::flat_hash_set<absl::string_view> subjects;
  auto edges = at(object).value()->edges;

  for (auto &e : edges) {
    auto p_opt = at(predicate);
    if (p_opt.has_value()) {
      auto mid = p_opt.value()->mid;
      if (e->predicate == mid) {
        subjects.insert(at(e->subject).value()->id);
      }
    }
  }

  return subjects;
}

absl::flat_hash_set<absl::string_view> Graph::findIntersection(const absl::flat_hash_set<absl::string_view>& set1,
                                                               const absl::flat_hash_set<absl::string_view>& set2) {
  if (set1.size() > set2.size()) {
    return findIntersection(set2, set1);
  }
  absl::flat_hash_set<absl::string_view> intersection;

  for (auto &n : set1) {
    if (set2.contains(n)) {
      intersection.emplace(n);
    }
  }

  return intersection;
}

absl::flat_hash_set<absl::string_view> Graph::connectedComponents(absl::string_view seed) {
  absl::flat_hash_set<absl::string_view> ids;
  return ids;
}

absl::flat_hash_set<absl::string_view> Graph::personaCluster(absl::string_view seed) {
  absl::flat_hash_set<absl::string_view> ids;
  return ids;
}

absl::flat_hash_set<absl::string_view> Graph::CommonNeighbors(absl::string_view nA, absl::string_view nB) {
  absl::flat_hash_set<absl::string_view> ids;

  if (id_index.contains(nA) && id_index.contains(nB)) {
    auto nA_ = mid_index[id_index[nA]];
    auto nB_ = mid_index[id_index[nB]];

    if (nA_->edges.size() > nB_->edges.size()) {
      return CommonNeighbors(nB, nA);
    }
    
    for (auto &eA : nA_->edges) {
      // Subjects match on common objects
      if (eA->subject == mid_index[id_index[nA]]->mid) {
        for (auto &eB : nB_->edges) {
          if (eB->subject == mid_index[id_index[nB]]->mid) {
            if (eA->object == eB->object) {
              ids.emplace(mid_index[eA->object]->id);
            }
          }
        }
      }
      // Objects match on common subjects
      if (eA->object == mid_index[id_index[nA]]->mid) {
        for (auto &eB : nB_->edges) {
          if (eB->object == mid_index[id_index[nB]]->mid) {
            if (eA->subject == eB->subject) {
              std::cout << mid_index[eA->subject]->id << std::endl;
              ids.emplace(mid_index[eA->subject]->id);
            }
          }
        }
      }
    }
  }
  return ids;
}

double Graph::JaccardCoefficient(absl::string_view nA, absl::string_view nB) {
  auto intersection = CommonNeighbors(nA, nB);
  auto union_ = (mid_index[id_index[nA]]->edges.size() + mid_index[id_index[nB]]->edges.size()) - intersection.size();

  return (double) 1 / union_ * intersection.size();
}

double Graph::AdamicAdar(absl::string_view nA, absl::string_view nB) {
  auto intersection = CommonNeighbors(nA, nB);
  double val = 0;

  for (auto &id : intersection) {
    auto opt = at(id);

    if (opt.has_value()) {
      val += 1 / std::sqrt(std::log2(opt.value()->edges.size()));
    }
  }

  return val;
}



