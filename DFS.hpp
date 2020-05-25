#pragma once

#include "Graph.hpp"
#include <functional>
#include <stack>
#include <queue>

template<typename V,typename E>
void DFS(const Graph<V,E>&g,std::size_t startID,
         std::function<void(const V&) > f)
{
    std::size_t verticesNumber = g.nrOfVertices();
    if(startID>=verticesNumber)
        throw std::runtime_error(std::string("[DFS] Incorrect startID:")
                                 +std::to_string(startID));

    std::vector<bool> visitedVertices(verticesNumber,false);

    std::stack<std::size_t> s1;
    s1.push(startID);

    while(!s1.empty())
    {
        std::size_t vId = s1.top();
        visitedVertices[vId] = true;
        f(g.vertexData(vId));
        s1.pop();

        std::size_t iter = verticesNumber;
        while(iter>0)
        {
            --iter;
            if(!visitedVertices[iter])
            {
                if(g.edgeExist(vId,iter))
                {
                    s1.push(iter);
                }
            }
        }
    }
}

template<typename V,typename E>
void BFS(const Graph<V,E>&g,std::size_t startID,
         std::function<void(const V&) > f)
{
    std::size_t verticesNumber = g.nrOfVertices();
    if(startID>=verticesNumber)
        throw std::runtime_error(std::string("[BFS] Incorrect startID:")
                                 +std::to_string(startID));

    std::vector<bool> visitedVertices(verticesNumber,false);
    visitedVertices.resize(verticesNumber);

    std::queue<std::size_t> q1;
    q1.push(startID);

    while(!q1.empty())
    {
        std::size_t vId = q1.front();
        visitedVertices[vId] = true;
        f(g.vertexData(vId));
        q1.pop();

        for(std::size_t iter=0; iter < verticesNumber; ++iter)
        {
            if(!visitedVertices[iter])
            {
                if(g.edgeExist(vId,iter))
                {
                    q1.push(iter);
                }
            }
        }
    }
}
