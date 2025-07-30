#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    // TODO store the indegree for each vertex in the graph in the indegrees map
    for(auto it = graph.begin(); it != graph.end(); ++it) {
        indegrees[it->first] = 0;
    }

    for(auto it = graph.begin(); it!= graph.end(); it++) {
        const adjacency_list<T> &list = it->second;
        for(auto it2 = list.begin(); it2 != list.end(); it2++) {
            indegrees[it2->first] += 1;
        }
    }


}
