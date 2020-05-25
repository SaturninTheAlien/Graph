#pragma once
#include "Graph.hpp"
#include <functional>
#include <limits>
#include <limits.h>
#include <optional>
#include <algorithm>

template<typename V, typename E>
std::pair<double, std::vector<std::size_t>> dijkstra(Graph<V, E>& graph, std::size_t start_idx, std::size_t end_idx,
         std::function<double(const E&)> getEdgeLength =
         [](const E&edge)->double{return edge;})
{

    constexpr double MAX_DOUBLE_VALUE = std::numeric_limits<double>::max();
    const std::size_t verticesNumber = graph.nrOfVertices();

    if(start_idx>=verticesNumber|| end_idx>=verticesNumber)
    {
        std::size_t var1 = std::max(start_idx,end_idx);
        throw std::runtime_error("[Dijikstra algorithm] Incorrect vertex index: "
                                 +std::to_string(var1));
    }

    class DijikstraNode
    {
    public:
        DijikstraNode()
            :visited(false),distance(MAX_DOUBLE_VALUE)
        {

        }
        bool visited;
        double distance;
        std::optional<std::size_t > precursor;
    };

    std::vector<DijikstraNode> nodes(verticesNumber);
    nodes[start_idx].distance = 0;
    std::size_t currentVertex = start_idx;

    while(true)
    {
        if(currentVertex == end_idx)break;
        else if(currentVertex >= verticesNumber)
        {
            return std::make_pair(MAX_DOUBLE_VALUE,std::vector<std::size_t>());
        }

        nodes[currentVertex].visited = true;

        std::size_t nextVertex = verticesNumber;
        double var1 = MAX_DOUBLE_VALUE;

        for(std::size_t i=0;i<verticesNumber;++i)
        {
            if(!nodes[i].visited)
            {
                if(graph.edgeExist(currentVertex,i))
                {
                    double newDistance = nodes[currentVertex].distance +
                            getEdgeLength(graph.edgeLabel(currentVertex,i));
                    if(newDistance < nodes[i].distance)
                    {
                        nodes[i].distance = newDistance;
                        nodes[i].precursor = currentVertex;
                    }
                }

                if(nodes[i].distance < var1)
                {
                    var1 = nodes[i].distance;
                    nextVertex = i;
                }
            }
        }

        currentVertex = nextVertex;
    }

    std::vector<std::size_t> result;
    currentVertex = end_idx;

    do
    {
        result.push_back(currentVertex);
        if(nodes[currentVertex].precursor.has_value())
        {
            currentVertex = nodes[currentVertex].precursor.value();
        }
        else
        {
            return std::make_pair(MAX_DOUBLE_VALUE,std::vector<std::size_t>());
        }

    }while(currentVertex!=start_idx);
    result.push_back(start_idx);

    std::reverse(result.begin(),result.end());
    return std::make_pair(nodes[end_idx].distance,result);

}


template<typename V,typename E>
std::vector<std::size_t> dijkstra_old(Graph<V,E>&g,
                                   std::size_t begin,
                                   std::size_t end,
                                   const std::function<std::size_t(const E&)>metric
                                   = [](const E&edge)->unsigned int{return edge;})
{
    const std::size_t verticesNumber = g.nrOfVertices();

    if(begin>=verticesNumber|| end>=verticesNumber)
    {
        std::size_t var1 = std::max(begin,end);
        throw std::runtime_error("[Dijikstra algorithm] Incorrect vertex index: "
                                 +std::to_string(var1));
    }

    class DijikstraNode
    {
    public:
        DijikstraNode()
            :visited(false),distance(UINT_MAX)
        {

        }
        bool visited;
        unsigned int distance;
        std::optional<std::size_t > precursor;
    };

    std::vector<DijikstraNode> nodes(verticesNumber);

    nodes[begin].distance = 0;
    std::size_t currentVertex = begin;

    while(true)
    {
        if(currentVertex == end)break;
        else if(currentVertex >= verticesNumber)
        {
            std::cout<<"[Dijikstra] No path exists."<<std::endl;
            return std::vector<std::size_t>();
        }

        nodes[currentVertex].visited = true;

        std::size_t nextVertex = verticesNumber;
        unsigned int var1 = UINT_MAX;

        for(std::size_t i=0;i<verticesNumber;++i)
        {
            if(!nodes[i].visited)
            {
                if(g.edgeExist(currentVertex,i))
                {
                    unsigned int newDistance = nodes[currentVertex].distance +
                            metric(g.edgeLabel(currentVertex,i));
                    if(newDistance < nodes[i].distance)
                    {
                        nodes[i].distance = newDistance;
                        nodes[i].precursor = currentVertex;
                    }
                }

                if(nodes[i].distance < var1)
                {
                    var1 = nodes[i].distance;
                    nextVertex = i;
                }
            }
        }

        currentVertex = nextVertex;
    }

    std::vector<std::size_t> result;


    currentVertex = end;
    do
    {
        result.push_back(currentVertex);
        if(nodes[currentVertex].precursor.has_value())
        {
            currentVertex = nodes[currentVertex].precursor.value();
        }
        else
        {
            throw std::runtime_error("[Dijikstra algorithm] Error, no precursor");
        }

    }while(currentVertex!=begin);
    result.push_back(begin);
    std::reverse(result.begin(),result.end());
    return result;
}
