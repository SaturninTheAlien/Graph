#pragma once
#include "Graph.hpp"
#include <functional>
#include <limits>
#include <optional>
#include <algorithm>
#include <set>
#include <map>

template<typename V, typename E>
std::pair<double, std::vector<std::size_t>>
astar(Graph<V, E>& graph, std::size_t start_idx, std::size_t end_idx,
      std::function<double(const Graph<V, E>&, std::size_t actual_vertex_id, std::size_t end_vertex_id)>
      heuristics, std::function<double(const E&)> getEdgeLength = nullptr)
{

    constexpr double MAX_DOUBLE_VALUE = std::numeric_limits<double>::max();
    const std::size_t verticesNumber = graph.nrOfVertices();

    if(start_idx>=verticesNumber|| end_idx>=verticesNumber)
    {
        std::size_t var1 = std::max(start_idx,end_idx);
        throw std::runtime_error("[Dijikstra algorithm] Incorrect vertex index: "
                                 +std::to_string(var1));
    }


    std::set<std::size_t> openSet;
    openSet.insert(start_idx);

    std::map<std::size_t,std::size_t> cameFrom;

    std::vector<double> gScore(verticesNumber, MAX_DOUBLE_VALUE);
    gScore[start_idx] = 0;
    std::vector<double> fScore(verticesNumber, MAX_DOUBLE_VALUE);
    fScore[start_idx] = heuristics(graph, start_idx, end_idx);

    while(!openSet.empty())
    {
        std::size_t current_node = verticesNumber;
        double var1 = MAX_DOUBLE_VALUE;

        for(const std::size_t& node:openSet)
        {
            if(fScore[node] < var1)
            {
                var1 = fScore[node];
                current_node = node;
            }
        }

        if(current_node == end_idx)
        {
            std::vector<std::size_t> path;
            path.push_back(current_node);
            while(current_node!=start_idx)
            {
                auto it = cameFrom.find(current_node);
                if(it==cameFrom.end())
                {
                    std::cout<<"No path 3"<<std::endl;
                    return std::make_pair(0,std::vector<std::size_t>());
                }
                current_node = it->second;
                path.push_back(current_node);
            }
            std::reverse(path.begin(),path.end());

            return std::make_pair(gScore[end_idx], path);
        }
        else if(current_node >= verticesNumber)
        {
            std::cout<<"No path 1"<<std::endl;
            return std::make_pair(0,std::vector<std::size_t>());
        }
        openSet.erase(openSet.find(current_node));

        for(std::size_t neighbor = 0; neighbor < verticesNumber;++neighbor)
        {
            if( neighbor!= current_node && graph.edgeExist(current_node,neighbor))
            {
                double tentative_gScore = gScore[current_node] +
                        getEdgeLength(graph.edgeLabel(current_node,neighbor));

                if(tentative_gScore < gScore[neighbor])
                {
                    cameFrom[neighbor] = current_node;
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = gScore[neighbor] + heuristics(graph,neighbor,end_idx);

                    openSet.insert(neighbor);
                }
            }
        }
    }

    //std::cout<<"No path 2"<<std::endl;
    return std::make_pair(0,std::vector<std::size_t>());
}
