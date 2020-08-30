#include "graph.h"

#include <iostream>
#include <absl/strings/str_split.h>

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
    //TODO(xjdr): WTF?!?!?!?!?!?
    //id_index.try_emplace(id, mid);
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
  //size_t mid = db.size();

  uint64_t s_mid;
  uint64_t p_mid;
  uint64_t o_mid;

  std::vector<absl::string_view> types = absl::StrSplit(predicate, '/');

  if (id_index.contains(subject)) {
    s_mid = id_index[subject];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    s_mid = addNode(subject, types[0], ev);
    if (s_mid == -1U) {
      //TODO(xjdr): Do something, node insertion failed.
    }
  }

  if (id_index.contains(predicate)) {
    p_mid = id_index[predicate];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    p_mid = addNode(predicate, "predicate", ev);
    if (p_mid == -1U) {
      //TODO(xjdr): Do something, node insertion failed.
    }
  }

  if (id_index.contains(object)) {
    o_mid = id_index[object];
  } else {
    std::vector<std::shared_ptr<Edge>> ev{};
    o_mid = addNode(object, types[types.size() - 1], ev);
    if (o_mid == -1U) {
      //TODO(xjdr): Do something, node insertion failed.
    }
  }

  auto e = std::make_shared<Edge>(s_mid, p_mid, o_mid, weight, timestamp, metadata);

  //db.push_back(e);
  mid_index[s_mid]->edges.emplace_back(e);
  mid_index[p_mid]->edges.emplace_back(e);
  mid_index[o_mid]->edges.emplace_back(e);
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

absl::flat_hash_set<absl::string_view> Graph::findIntersection(const absl::flat_hash_set<absl::string_view> &set1,
                                                               const absl::flat_hash_set<absl::string_view> &set2) {
  // THIS IS SLOW / FIX LATER
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

absl::flat_hash_set<uint64_t> Graph::findIntersection(const absl::flat_hash_set<uint64_t> &set1,
                                                      const absl::flat_hash_set<uint64_t> &set2) {
  // THIS IS SLOW / FIX LATER
  if (set1.size() > set2.size()) {
    return findIntersection(set2, set1);
  }

  absl::flat_hash_set<uint64_t> intersection;

  for (auto &n : set1) {
    if (set2.contains(n)) {
      intersection.emplace(n);
    }
  }

  return intersection;
}

absl::flat_hash_set<absl::string_view> Graph::getNodesByType(uint64_t seed_mid, absl::string_view type) {
  absl::flat_hash_set<absl::string_view> nodes;

  auto s_opt = at(seed_mid);

  if (s_opt.has_value()) {
    auto n = s_opt.value();
    for (auto &e : n->edges) {
      auto sub_opt = at(e->subject);
      auto pred_opt = at(e->predicate);
      auto obj_opt = at(e->object);

      if (sub_opt.has_value()) {
        auto sub = sub_opt.value();
        if (sub->type.compare(type) == 0) {
          nodes.emplace(sub->id);
        }
      }

      if (pred_opt.has_value()) {
        auto pred = pred_opt.value();
        if (pred->type.compare(type) == 0) {
          nodes.emplace(pred->id);
        }
      }

      if (obj_opt.has_value()) {
        auto obj = obj_opt.value();
        if (obj->type.compare(type) == 0) {
          nodes.emplace(obj->id);
        }
      }
    }
  }

  return nodes;
}

absl::flat_hash_set<absl::string_view> Graph::connectedComponents(absl::string_view seed) {
  absl::flat_hash_set<absl::string_view> ids;
  return ids;
}

absl::flat_hash_set<std::shared_ptr<Edge>> Graph::egoNet(absl::string_view ego_id) {
  absl::flat_hash_set<uint64_t> mids;
  absl::flat_hash_set<std::shared_ptr<Edge>> edges;
  absl::flat_hash_map<uint64_t, absl::flat_hash_set<uint64_t>> mid_map;
  absl::flat_hash_map<std::pair<uint64_t, uint64_t>, absl::flat_hash_set<uint64_t>> intersection_map;

  auto ego_opt = at(ego_id);
  if (ego_opt.has_value()) {
    auto ego = ego_opt.value();

    for (auto &e : ego->edges) {
      edges.emplace(e);
      if (e->subject == ego->mid) {
        mids.emplace(e->object);
      } else if (e->object == ego->mid) {
        mids.emplace(e->subject);
      }
    }

    for (auto &m : mids) {
      auto m_opt = at(m);
      if (m_opt.has_value()) {
        auto m_node = m_opt.value();
        for (auto &e : m_node->edges) {
          if (e->subject == m_node->mid) {
            mid_map.emplace(m_node->mid, e->object);
          } else if (e->object == m_node->mid) {
            mid_map.emplace(m_node->mid, e->subject);
          }
        }
      }
    }

    for (auto &p : mid_map) {
      for (auto &px : mid_map) {
        if (p.first != px.first) {
          auto intersection = findIntersection(p.second, px.second);
          intersection_map.emplace(std::pair<uint64_t, uint64_t>(p.first, px.first), intersection);
        }
      }
    }

    for (auto &p : intersection_map) {
      auto p1_opt = at(p.first.first);
      auto p2_opt = at(p.first.second);

      if (p1_opt.has_value() && p2_opt.has_value()) {
        auto p1 = p1_opt.value();
        auto p2 = p2_opt.value();

        for (auto &e1 : p1->edges) {
          if (e1->subject == p1->mid) {
            if (p.second.contains(e1->object)) {
              edges.emplace(e1);
            }
          }
        }

        for (auto &e2 : p2->edges) {
          if (e2->subject == p2->mid) {
            if (p.second.contains(e2->object)) {
              edges.emplace(e2);
            }
          }
        }

      }
    }
  }

  return edges;
}

void Graph::personaClusters(const absl::flat_hash_set<std::shared_ptr<
    Edge>> &ego_net, uint64_t ego_mid) {
  absl::flat_hash_set<uint64_t> cluster_mids;
  absl::flat_hash_set<std::shared_ptr<Edge>> ego_edges;
  absl::flat_hash_set<std::shared_ptr<Edge>> edge_union;
  absl::flat_hash_map<uint64_t, absl::flat_hash_set<std::shared_ptr<Edge>>> clusters;

  auto e_opt = at(ego_mid);

  if (e_opt.has_value()) {
    auto ego = e_opt.value();
    for (auto &e : ego->edges) {
      ego_edges.emplace(e);
    }

    for (auto &e : ego_net) {
      if (!ego_edges.contains(e)) {
        cluster_mids.emplace(e->subject);
        cluster_mids.emplace(e->object);
        edge_union.emplace(e);
      }
    }
  }

  // This is where we call a clustering algo?

  //return clusters;
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

//absl::flat_hash_set<absl::flat_hash_set<absl::string_view>> Graph::LabelPropCluster() {
//  // semi sync label prop algo from https://arxiv.org/pdf/1103.4550.pdf
//  return absl::flat_hash_set<absl::flat_hash_set<absl::string_view>>();
//}
//absl::flat_hash_set<absl::flat_hash_set<absl::string_view>> Graph::ModularityCluster() {
//  // Based on CNM Modularity algo https://arxiv.org/pdf/cond-mat/0408187.pdf
//  return absl::flat_hash_set<absl::flat_hash_set<absl::string_view>>();
//}



