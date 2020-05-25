#include <functional>
#include <iostream>
#include <cstdint>
#include <cmath>
#include "Graph.hpp"
#include "dijkstra.hpp"
#include "a_star.hpp"

using namespace std;

template<typename Iter>
void printDebugInsertInfo(const std::pair<Iter, bool>& insert_info)
{
    if(insert_info.first) { std::cout << "Dodano " << (insert_info.second ? "(zastepujac) " : "") << *insert_info.first << std::endl; }
    else
        std::cout << "Nie dodano" << std::endl;
}

template<typename Iter>
void printDebugRemoveInfo(const Iter& next)
{
    if(next)
        std::cout << "Usunięto, następny element to: " << *next << std::endl;
    else
        std::cout << "Nie usunięto, lub następny element to end()" << std::endl;
}

void graphTest()
{
    {
        Graph<std::string, double> g;
        {
            for(std::size_t i = 0u; i < 6u; ++i) { g.insertVertex("data " + std::to_string(i)); }

            for(std::size_t i = 0u; i < g.nrOfVertices(); ++i)
            {
                for(std::size_t j = 0u; j < g.nrOfVertices(); ++j)
                {
                    if((i + j) & 1u || i & 1u) { g.insertEdge(i, j, ((i != j) ? (i + j) / 2. : 1.)); }
                }
            }

            g.printNeighborhoodMatrix();
            std::cout << std::endl;

            printDebugRemoveInfo(g.removeEdge(0, 2));
            printDebugInsertInfo(g.insertEdge(0, 2, 4.));
            printDebugRemoveInfo(g.removeVertex(1));
            printDebugRemoveInfo(g.removeEdge(2, 2));
            printDebugRemoveInfo(g.removeEdge(2, 3));
            printDebugRemoveInfo(g.removeEdge(4, 3));
            printDebugRemoveInfo(g.removeVertex(8));
            std::cout << "Nr of vertices: " << g.nrOfVertices() << std::endl;
            std::cout << "Nr of edges: " << g.nrOfEdges() << std::endl;
            std::cout << std::endl;
            g.printNeighborhoodMatrix();
            std::cout << std::endl;
            std::cout << "Vertices data:" << std::endl;
            for(auto v_it = g.beginVertices(); v_it != g.endVertices(); ++v_it) { std::cout << *v_it << ", "; }
            std::cout << std::endl << std::endl;
            std::cout << "Edges data:" << std::endl;
            for(auto e_it = g.beginEdges(); e_it != g.endEdges(); ++e_it) { std::cout << *e_it << ", "; }
            std::cout << std::endl << std::endl;

            std::cout << "DFS(1):" << std::endl;
            for(auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) { std::cout << *dfs_it << ", "; }
            std::cout << std::endl << std::endl;
            std::cout << "BFS(1):" << std::endl;
            for(auto bfs_it = g.beginBFS(1); bfs_it != g.endBFS(); ++bfs_it) { std::cout << *bfs_it << ", "; }
            std::cout << std::endl << std::endl;

            auto [shortest_path_distance, shortest_path] = dijkstra<std::string, double>(g, 3u, 0u, [](const double& e) -> double { return e; });
            std::cout << "Distance from 3 to 0: " << shortest_path_distance << std::endl;
            std::cout << "Path from 3 to 0:" << std::endl;
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;

            std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 3u, 1u, [](const double& e) -> double { return e; });
            std::cout << "Distance from 3 to 1: " << shortest_path_distance << std::endl;
            std::cout << "Path from 3 to 1:" << std::endl;
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;

            std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 1u, 3u, [](const double& e) -> double { return e; });
            std::cout << "Distance from 1 to 3: " << shortest_path_distance << std::endl;
            std::cout << "Path from 1 to 3:" << std::endl;
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;

            std::vector<Graph<std::string, double>> vg, vg2;
            vg.resize(1000);
            for(auto& e : vg)
            {
                e = g;
                e.insertVertex("data x");
            }
            vg2.resize(1000);
            for(std::size_t i = 0u; i < vg.size(); ++i) { vg2[i] = std::move(vg[i]); }
            for(auto& e : vg2) { e.removeVertex(2); }
            vg = vg2;
            vg2.clear();
            vg.front().insertEdge(0, 4);

            g = std::move(vg.front());
        }
        g.printNeighborhoodMatrix();

        std::cout << std::endl;
        std::cout << "Vertices data:" << std::endl;
        for(auto v_it = g.beginVertices(); v_it != g.endVertices(); ++v_it) { std::cout << *v_it << ", "; }
        std::cout << std::endl << std::endl;
        std::cout << "Edges data:" << std::endl;
        for(auto e_it = g.beginEdges(); e_it != g.endEdges(); ++e_it) { std::cout << *e_it << ", "; }
        std::cout << std::endl << std::endl;

        std::cout << "DFS(1):" << std::endl;
        //DFS<std::string, double>(g, 1, [](const std::string& v) -> void { std::cout << v << ", "; });
        for(auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) { std::cout << *dfs_it << ", "; }
        std::cout << std::endl;
        std::cout << "BFS(1):" << std::endl;
        //BFS<std::string, double>(g, 1, [](const std::string& v) -> void { std::cout << v << ", "; });
        for(auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) { std::cout << *dfs_it << ", "; }
        std::cout << std::endl;

        std::cout << "DFS(1):" << std::endl;
        for(auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) { std::cout << *dfs_it << ", "; }
        std::cout << std::endl;
        std::cout << "BFS(1):" << std::endl;
        for(auto bfs_it = g.beginBFS(1); bfs_it != g.endBFS(); ++bfs_it) { std::cout << *bfs_it << ", "; }
        std::cout << std::endl;

        std::cout << std::endl;
        printDebugInsertInfo(g.insertEdge(1, 2, 2));
        printDebugInsertInfo(g.insertEdge(1, 4, 2));
        printDebugInsertInfo(g.insertEdge(2, 1, 2));
        printDebugInsertInfo(g.insertEdge(2, 4, 2));
        printDebugInsertInfo(g.insertEdge(4, 1, 2));
        printDebugInsertInfo(g.insertEdge(4, 2, 2));
        std::cout << std::endl;
        g.printNeighborhoodMatrix();
        std::cout << std::endl;

        std::cout << "DFS(1):" << std::endl;
        for(auto dfs_it = g.beginDFS(1); dfs_it != g.endDFS(); ++dfs_it) { std::cout << *dfs_it << ", "; }
        std::cout << std::endl;
        std::cout << "BFS(1):" << std::endl;
        for(auto bfs_it = g.beginBFS(1); bfs_it != g.endBFS(); ++bfs_it) { std::cout << *bfs_it << ", "; }
        std::cout << std::endl;
        std::cout << std::endl;

        auto [shortest_path_distance, shortest_path] = dijkstra<std::string, double>(g, 2u, 4u, [](const double& e) -> double { return e; });
        std::cout << "Distance from 2 to 4: " << shortest_path_distance << std::endl;
        std::cout << "Path from 2 to 4:" << std::endl;
        for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
        std::cout << std::endl;

        std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 1u, 0u, [](const double& e) -> double { return e; });
        std::cout << "Distance from 1 to 0: " << shortest_path_distance << std::endl;
        std::cout << "Path from 1 to 0:" << std::endl;
        for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
        std::cout << std::endl;

        std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 3u, 0u, [](const double& e) -> double { return e; });
        std::cout << "Distance from 3 to 0: " << shortest_path_distance << std::endl;
        std::cout << "Path from 3 to 0:" << std::endl;
        for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
        std::cout << std::endl;

        std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 3u, 1u, [](const double& e) -> double { return e; });
        std::cout << "Distance from 3 to 1: " << shortest_path_distance << std::endl;
        std::cout << "Path from 3 to 1:" << std::endl;
        for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
        std::cout << std::endl;

        std::tie(shortest_path_distance, shortest_path) = dijkstra<std::string, double>(g, 1u, 3u, [](const double& e) -> double { return e; });
        std::cout << "Distance from 1 to 3: " << shortest_path_distance << std::endl;
        std::cout << "Path from 1 to 3:" << std::endl;
        for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
        std::cout << std::endl;

        std::cout << std::endl;
    }

    {
        Graph<std::pair<float, float>, double> g;

        constexpr std::size_t grid_size = 16u;
        const auto sqrt_2 = std::sqrt(2.);

        auto zero_heuristics = [](const Graph<std::pair<float, float>, double>& graph, std::size_t current_vertex_id, std::size_t end_vertex_id) -> double { return 0.; };

        auto manhattan_heuristics = [](const Graph<std::pair<float, float>, double>& graph, std::size_t current_vertex_id, std::size_t end_vertex_id) -> double {
            const auto& v1_data = graph.vertexData(current_vertex_id);
            const auto& v2_data = graph.vertexData(end_vertex_id);
            return std::abs(v1_data.first - v2_data.first) + std::abs(v1_data.second - v2_data.second);
        };

        auto euclidean_heuristics = [](const Graph<std::pair<float, float>, double>& graph, std::size_t current_vertex_id, std::size_t end_vertex_id) -> double {
            const auto& v1_data = graph.vertexData(current_vertex_id);
            const auto& v2_data = graph.vertexData(end_vertex_id);
            return std::sqrt(std::pow(v2_data.first - v1_data.first, 2u) + std::pow(v2_data.second - v1_data.second, 2u));
        };//

        for(std::size_t i = 0u; i < grid_size; ++i)
        {
            for(std::size_t j = 0u; j < grid_size; ++j) { g.insertVertex(std::make_pair(i, j)); }
        }
        for(std::size_t i = 0u; i < grid_size; ++i)
        {
            for(std::size_t j = 0u; j < grid_size - 1u; ++j)
            {
                if(j < grid_size - 1u)
                {
                    g.insertEdge(i * grid_size + j, i * grid_size + j + 1u, 1.);
                    g.insertEdge(i * grid_size + j + 1u, i * grid_size + j, 1.);
                }
                if(i < grid_size - 1u)
                {
                    g.insertEdge(i * grid_size + j, (i + 1u) * grid_size + j, 1.);
                    g.insertEdge((i + 1u) * grid_size + j, i * grid_size + j, 1.);
                }
                if(i < grid_size - 1u && j < grid_size - 1u)
                {
                    g.insertEdge(i * grid_size + j, (i + 1u) * grid_size + j + 1u, sqrt_2);
                    g.insertEdge((i + 1u) * grid_size + j + 1u, i * grid_size + j, sqrt_2);
                    g.insertEdge(i * grid_size + j + 1u, (i + 1u) * grid_size + j, sqrt_2);
                    g.insertEdge((i + 1u) * grid_size + j, i * grid_size + j + 1u, sqrt_2);
                }
            }
        }

        for(std::size_t j = 1u; j < grid_size - 1u; ++j)
        {
            g.removeVertex(std::find(g.beginVertices(), g.endVertices(), std::make_pair(static_cast<float>(j), grid_size / 2.f)).id());
        }

        auto start_data = std::make_pair(grid_size / 2.f, 1.f);
        auto end_data = std::make_pair(grid_size / 2.f + 1.f, grid_size - 1.f);
        auto start_it = std::find(g.beginVertices(), g.endVertices(), start_data);
        auto end_it = std::find(g.beginVertices(), g.endVertices(), end_data);
        if(start_it != g.endVertices() && end_it != g.endVertices())
        {
            auto [shortest_path_distance, shortest_path] = dijkstra<std::pair<float, float>, double>(g, start_it.id(), end_it.id(), [](const double& e) -> double { return e; });
            std::cout << "Dijkstra results:" << std::endl;
            std::cout << "\tDistance: " << shortest_path_distance << std::endl;
            std::cout << "\tPath (ids): ";
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;
            std::cout << "\tPath (data): ";
            for(auto& v_id : shortest_path)
            {
                std::cout << "[" << g.vertexData(v_id).first << ", " << g.vertexData(v_id).second << "]"
                          << ", ";
            }
            std::cout << std::endl;

            std::tie(shortest_path_distance, shortest_path) = astar<std::pair<float, float>, double>(g, start_it.id(), end_it.id(), zero_heuristics, [](const double& e) -> double { return e; });
            std::cout << "AStar (zero) results:" << std::endl;
            std::cout << "\tDistance: " << shortest_path_distance << std::endl;
            std::cout << "\tPath (ids): ";
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;
            std::cout << "\tPath (data): ";
            for(auto& v_id : shortest_path)
            {
                std::cout << "[" << g.vertexData(v_id).first << ", " << g.vertexData(v_id).second << "]"
                          << ", ";
            }
            std::cout << std::endl;

            std::tie(shortest_path_distance, shortest_path) = astar<std::pair<float, float>, double>(g, start_it.id(), end_it.id(), manhattan_heuristics, [](const double& e) -> double { return e; });
            std::cout << "AStar (manhattan) results:" << std::endl;
            std::cout << "\tDistance: " << shortest_path_distance << std::endl;
            std::cout << "\tPath (ids): ";
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;
            std::cout << "\tPath (data): ";
            for(auto& v_id : shortest_path)
            {
                std::cout << "[" << g.vertexData(v_id).first << ", " << g.vertexData(v_id).second << "]"
                          << ", ";
            }
            std::cout << std::endl;

            std::tie(shortest_path_distance, shortest_path) = astar<std::pair<float, float>, double>(g, start_it.id(), end_it.id(), euclidean_heuristics, [](const double& e) -> double { return e; });
            std::cout << "AStar (euclidean) results:" << std::endl;
            std::cout << "\tDistance: " << shortest_path_distance << std::endl;
            std::cout << "\tPath (ids): ";
            for(auto& v_id : shortest_path) { std::cout << v_id << ", "; }
            std::cout << std::endl;
            std::cout << "\tPath (data): ";
            for(auto& v_id : shortest_path)
            {
                std::cout << "[" << g.vertexData(v_id).first << ", " << g.vertexData(v_id).second << "]"
                          << ", ";
            }

            std::cout<<"\n\nStartID:"<<start_it.id()<<std::endl;
            std::cout<<"EndID:"<<end_it.id()<<std::endl;

            std::cout << std::endl;
        }//*/
    }
}
