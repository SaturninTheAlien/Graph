#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <optional>
#include <stdexcept>

#include <stack>
#include <queue>

// Uwaga! Kod powinien być odporny na błędy i każda z metod jeżeli zachodzi niebezpieczeństwo wywołania z niepoprawnymi parametrami powinna zgłaszac odpowiednie wyjątki!

// klasa reprezentująca graf skierowany oparty na MACIERZY SĄSIEDZTWA
// V - dane przechowywane przez wierzcholki
// E - dane przechowywane przez krawedzie (etykiety)
template <typename V, typename E>
class Graph
{
public:
    // iterator po wierzchołkach (rosnąco po id wierzchołków)
    class VerticesIterator: public std::iterator
            <std::input_iterator_tag,V>
    {
        // ...
        friend class Graph;
        std::size_t index;
        std::vector<V>* vecPtr;

        VerticesIterator(std::size_t index,std::vector<V>* vecPtr)
            :index(index), vecPtr(vecPtr)
        {
        }
    public:
        bool operator==(const VerticesIterator &vi2) const
        {
            return this->index==vi2.index&&this->vecPtr==vi2.vecPtr;
        }
        bool operator!=(const VerticesIterator &vi2) const
        {
            return !this->operator==(vi2);
        }

        VerticesIterator& operator++()
        {
            ++this->index;
            return *this;
        }
        VerticesIterator operator++(int)
        {
            VerticesIterator copy = *this;
            this->operator++();
            return copy;
        }

        operator bool()const
        {
            return this->index<this->vecPtr->size();
        }

        V& operator*() const
        {
            return (*this->vecPtr)[this->index];
        }
        V* operator->() const
        {
            return &this->operator*();
        }
        // zwraca id aktualnego wierzchołka
        std::size_t id() const
        {
            return this->index;
        }
    };

    // iterator po istniejących krawędziach
    class EdgesIterator: public std::iterator
            <std::input_iterator_tag,E>
    {
        friend class Graph;
        EdgesIterator(std::size_t y, std::size_t x,
                      std::vector<std::vector<std::optional<E>>>*vecPtr)
            :y(y),x(x), vecPtr(vecPtr)
        {

        }

        void validate();
        std::size_t y,x;
        std::vector<std::vector<std::optional<E>>>* vecPtr;
    public:
        bool operator==(const EdgesIterator &ei) const
        {
            return this->x==ei.x&&this->y==ei.y&&this->vecPtr==ei.vecPtr;
        }
        bool operator!=(const EdgesIterator &ei) const
        {
            return !(this->operator==(ei));
        }
        EdgesIterator& operator++();
        EdgesIterator operator++(int)
        {
            EdgesIterator copy = *this;
            this->operator++();
            return copy;
        }
        operator bool()const;
        E& operator*() const
        {
            return (*this->vecPtr)[y][x].value();
        }
        E* operator->() const
        {
            return &this->operator*();
        }
        // zwraca id wierzchołka startowego
        std::size_t v1id() const
        {
            return this->y;
        }
        // zwraca id wierzchołka koncowego
        std::size_t v2id() const
        {
            return this->x;
        }
    };

    template<bool BFS>
    class FSIterator: public std::iterator
            <std::input_iterator_tag,V>
    {
        using collection_type = typename
        std::conditional<BFS,std::queue<std::size_t>,std::stack<std::size_t>>::type;

        // ...
        FSIterator(Graph* graphIn)
            :mGraph(graphIn),
              mVertexIndex(graphIn->nrOfVertices()),
              mVertexIndexStart(graphIn->nrOfVertices())
        {

        }

        FSIterator(Graph* graphIn,std::size_t vertexIndexIn)
            :mGraph(graphIn),
             mVertexIndex(vertexIndexIn),
             mVertexIndexStart(vertexIndexIn),
             mVisitedVertices(graphIn->nrOfVertices(),false)
        {
            this->mCollection.push(vertexIndexIn);
        }

    public:
        bool operator==(const FSIterator& dfsi) const
        {

            return this->mVertexIndex==dfsi.mVertexIndex && this->mGraph == dfsi.mGraph
                    &&this->mVertexIndexStart==dfsi.mVertexIndexStart;
        }
        bool operator!=(const FSIterator& dfsi) const
        {
            return !(*this==dfsi);
        }
        FSIterator& operator++();
        FSIterator operator++(int)
        {
            FSIterator copy = *this;
            this->operator++();
            return copy;
        }
        V& operator*() const
        {
            return this->mGraph->vertexData(mVertexIndex);
        }
        V* operator->() const
        {
            return &this->operator*();
        }
        operator bool() const
        {
            return this->mVertexIndex < this->mGraph->nrOfVertices();
        }
    private:
        friend class Graph;

        Graph* mGraph;
        std::size_t mVertexIndex;
        std::size_t mVertexIndexStart;

        std::vector<bool> mVisitedVertices;
        collection_type mCollection;

        std::size_t mCollectionTop()
        {
            if constexpr (BFS)
            {
                return this->mCollection.front();
            }
            else
            {
                return  this->mCollection.top();
            }
        }
    };

public:
    Graph()
        :mEdgesNumber(0)
    {

    }
    Graph(const Graph<V, E> &source) = default;
    Graph(Graph<V, E> &&source) = default;
    Graph& operator=(const Graph<V, E> &source) = default;
    Graph& operator=(Graph<V, E> &&source) = default;
    ~Graph() = default;

    // dodaje nowy wierzchołek z danymi przyjętymi w argumencie (wierzchołek powinien posiadać kopie danych) i zwraca "VerticesIterator" na nowo utworzony wierzchołek
    VerticesIterator insertVertex(const V &vertex_data);
    // dodaje nową krawędź między wierzchołkami o id "vertex1_id" i "vertex2_id" i zwraca "EdgesIterator" na nowo dodaną krawędź, oraz informację o tym czy została dodana nowa krawędź, czy nie
    // jeśli krawędź między podanymi wierzchołkami już istnieje to działanie funkcji zależy od ostatniego argumentu
    // jeśli ostatni argument przyjmuje wartość "true" to krawędź zostaje zastąpiona, jeśli "false" to nie
    std::pair<EdgesIterator, bool> insertEdge(std::size_t y, std::size_t x, const E &label = E(), bool replace = true);
    std::pair<EdgesIterator, bool> insertEdge(VerticesIterator vi1, VerticesIterator vi2,
                                              const E &label = E(), bool replace = true)
    {
        this->mCheckIterator(vi1);
        this->mCheckIterator(vi2);
        return this->insertEdge(vi1.index,vi2.index,label,replace);
    }


    // usuwa wierzchołek o podanym id i zwraca "VerticesIterator" na kolejny wierzchołek, lub to samo co "endVertices()" w przypadku usunięcia ostatniego wierzchołka, lub braku wierzchołka o podanym id
    VerticesIterator removeVertex(std::size_t vertex_id);
    VerticesIterator removeVertex(VerticesIterator vi)
    {
        this->mCheckIterator(vi);
        return this->removeVertex(vi.index);
    }

    // usuwa krawedź między dwoma wierzchołkami o podanych id i zwraca "EdgesIterator" na kolejną krawędź, lub to samo co "endEdges()" w przypadku usunięcia ostatniej krawedzi, lub braku krawędzi między wierzchołkami o podanych id
    EdgesIterator removeEdge(std::size_t y, std::size_t x);
    EdgesIterator removeEdge(VerticesIterator vi1,VerticesIterator vi2)
    {
        this->mCheckEdge(vi1);
        this->mCheckEdge(vi2);
        return this->removeEdge(vi1.index,vi2.index);
    }

    EdgesIterator removeEdge(EdgesIterator ei)
    {
        this->mCheckIterator(ei);
        return this->removeEdge(ei.x,ei.y);
    }

    // zwraca true jeśli istnieje krawędź między wierzchołkami o podanych id, false w przeciwnym razie
    // O(1)
    bool edgeExist(std::size_t y, std::size_t x) const;
    bool edgeExist(VerticesIterator vi1,VerticesIterator vi2)const
    {
        this->mCheckIterator(vi1);
        this->mCheckIterator(vi2);
        return edgeExist(vi1.index,vi2.index);
    }

    // zwraca ilość wierzchołków w grafie
    // O(1)
    std::size_t nrOfVertices() const
    {
        return this->mVertices.size();
    }
    // zwraca ilość krawędzi w grafie
    // O(1)
    std::size_t nrOfEdges() const
    {
        return this->mEdgesNumber;
    }
    // drukuje macierz sąsiedztwa na konsoli (debug)
    void printNeighborhoodMatrix() const;
    // zwraca "VerticesIterator" do wierzchołka o podanym id, lub to samo co "endVertices()" w przypadku braku wierzchołka o podanym id
    VerticesIterator vertex(std::size_t vertex_id)
    {
        return vertex_id < this->mVertices.size()
                ? VerticesIterator(vertex_id,&this->mVertices)
                : this->endVertices();
    }
    // zwraca referencję do danych wierzchołka o podanym id
    const V& vertexData(std::size_t vertex_id) const
    {
        this->mCheckVertex(vertex_id);
        return this->mVertices[vertex_id];
    }
    // zwraca referencję do danych wierzchołka o podanym id
    V& vertexData(std::size_t vertex_id)
    {
        this->mCheckVertex(vertex_id);
        return this->mVertices[vertex_id];
    }
    // zwraca "EdgesIterator" do krawędzi pomiędzy wierzchołkami o podanych id, lub to samo co "endEdges()" w przypadku braku krawędzi między wierzchołkami o podanych id
    EdgesIterator edge(std::size_t y, std::size_t x)
    {
        return this->edgeExist(y,x)
                ?EdgesIterator(y,x,&this->mEdges):this->endEdges();
    }
    // zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o podanych id
    const E& edgeLabel(std::size_t y, std::size_t x) const
    {
        this->mCheckEdge(y,x);
        return this->mEdges[y][x].value();
    }
    // zwraca referencję do danych (etykiety) krawędzi pomiędzy wierzchołkami o podanych id
    E& edgeLabel(std::size_t y, std::size_t x)
    {
        this->mCheckEdge(y,x);
        return this->mEdges[y][x].value();
    }

    VerticesIterator begin() { return beginVertices(); }
    VerticesIterator end() { return endVertices(); }
    // zwraca "VerticesIterator" na pierwszy wierzchołek (o najmniejszym id)
    VerticesIterator beginVertices()
    {
        return VerticesIterator(0, &this->mVertices);
    }
    // zwraca "VerticesIterator" "za ostatni" wierzchołek
    VerticesIterator endVertices()
    {
        return VerticesIterator(this->mVertices.size(),&this->mVertices);
    }
    // zwraca "EdgesIterator" na pierwszą krawędz
    EdgesIterator beginEdges()
    {
        EdgesIterator it(0,0,&this->mEdges);
        it.validate();
        return it;
    }
    // zwraca "EdgesIterator" "za ostatnią" krawędz
    EdgesIterator endEdges()
    {
        return EdgesIterator(this->mVertices.size(),0,&this->mEdges);
    }

    FSIterator<false> beginDFS(std::size_t vertexIndex)
    {
        return FSIterator<false>(this,vertexIndex);
    }

    FSIterator<false> endDFS()
    {
        return FSIterator<false>(this);
    }

    FSIterator<true> beginBFS(std::size_t vertexIndex)
    {
        return FSIterator<true>(this,vertexIndex);
    }

    FSIterator<true> endBFS()
    {
        return FSIterator<true>(this);
    }

    void clear();
private:
    std::vector<V> mVertices;
    std::vector<std::vector<std::optional<E>>> mEdges;
    std::size_t mEdgesNumber;

    void mCheckVertex(size_t vertex_id)const;
    void mCheckEdge(size_t y, size_t x)const;
    void mCheckIterator(const VerticesIterator& vi)const;
    void mCheckIterator(const EdgesIterator& vi)const;
};

template<typename V, typename E>
typename Graph<V,E>::EdgesIterator& Graph<V,E>::EdgesIterator::operator++()
{
    std::size_t verticesNumber = this->vecPtr->size();
    do
    {
        ++x;
        if(x>=verticesNumber)
        {
            x=0;
            ++y;
        }
    }while(y<verticesNumber && !(*this->vecPtr)[y][x].has_value());

    return *this;
}

template<typename V, typename E>
void Graph<V,E>::EdgesIterator::validate()
{
    std::size_t verticesNumber = this->vecPtr->size();
    while(y<verticesNumber && !(*this->vecPtr)[y][x].has_value())
    {
        ++x;
        if(x>=verticesNumber)
        {
            x=0;
            ++y;
        }
    }
}

template<typename V, typename E>
Graph<V,E>::EdgesIterator::operator bool()const
{
    std::size_t verticesNumber = this->vecPtr->size();

    if(this->x<verticesNumber&&this->y<verticesNumber)
    {
        return (*this->vecPtr)[y][x].has_value();
    }
    return false;
}

template<typename V, typename E>
typename Graph<V,E>::VerticesIterator Graph<V,E>::insertVertex(const V&vertexData)
{
    size_t index = this->mVertices.size();
    this->mVertices.push_back(vertexData);

    size_t s1 = index+1;
    this->mEdges.resize(s1);
    for(std::vector<std::optional<E>>& v1: this->mEdges)
    {
        v1.resize(s1);
    }
    return VerticesIterator(s1,&this->mVertices);
}

template<typename V, typename E>
std::pair<typename Graph<V,E>::EdgesIterator, bool> Graph<V,E>::
insertEdge(std::size_t y, std::size_t x, const E &label,bool replace)
{
    std::size_t verticesNumber = this->mVertices.size();

    if(y<verticesNumber&&x<verticesNumber)
    {
        if(this->mEdges[y][x].has_value())
        {
            if(!replace)return std::make_pair(
                        EdgesIterator(y,x,&this->mEdges),false);
        }
        else
        {
            ++this->mEdgesNumber;
        }

        this->mEdges[y][x] = label;
        return std::make_pair(EdgesIterator(y,x,&this->mEdges),true);
    }

    return std::make_pair(this->endEdges(),false);
}

template<typename V,typename E>
typename Graph<V,E>::VerticesIterator Graph<V,E>::removeVertex(std::size_t vertex_id)
{
    if(vertex_id < this->mVertices.size())
    {
        this->mVertices.erase(this->mVertices.begin()+vertex_id);

        for(std::optional<E>& op: this->mEdges[vertex_id])
        {
            if(op.has_value()) --this->mEdgesNumber;
        }
        this->mEdges.erase(this->mEdges.begin()+vertex_id);

        for(std::vector<std::optional<E>>& v1:this->mEdges)
        {
            if(v1[vertex_id].has_value()) --this->mEdgesNumber;
            v1.erase(v1.begin()+vertex_id);
        }

        return VerticesIterator(vertex_id,&this->mVertices);
    }
    return this->endVertices();
}


template<typename V,typename E>
typename Graph<V,E>::EdgesIterator Graph<V,E>::removeEdge(std::size_t y, std::size_t x)
{
    if(this->edgeExist(y,x))
    {
        --this->mEdgesNumber;
        this->mEdges[y][x].reset();

        EdgesIterator iter(y,x,&this->mEdges);
        iter.validate();
        return iter;
    }
    return this->endEdges();
}


template<typename V,typename E>
bool Graph<V,E>::edgeExist(std::size_t y, std::size_t x)const
{
    if(y<this->mVertices.size()&&x<this->mVertices.size())
    {
        return this->mEdges[y][x].has_value();
    }
    return false;
}

template<typename V,typename E>
void Graph<V,E>::mCheckEdge(size_t y, size_t x)const
{
    if(!this->edgeExist(y,x))
    {
        std::ostringstream os;
        os<<"[Graph] Edge["<<y<<"]["<<x<<"] does not exist!";
        throw std::runtime_error(os.str());
    }
}

template<typename V,typename E>
void Graph<V,E>::clear()
{
    this->mEdges.clear();
    this->mVertices.clear();
    this->mEdgesNumber=0;
}

template<typename V,typename E>
void Graph<V,E>::mCheckVertex(size_t vertex_id)const
{
    if(vertex_id>=this->mVertices.size())
    {
        std::ostringstream os;
        os<<"[Graph] Vertex["<<vertex_id<<"] does not exist!";
        throw std::runtime_error(os.str());
    }
}
template<typename V,typename E>
void Graph<V,E>::mCheckIterator(const VerticesIterator& vi)const
{
    if(vi.vecPtr!=&this->mVertices)
    {
        throw std::runtime_error("[Graph] Incorrect iterator");
    }
}

template<typename V,typename E>
void Graph<V,E>::mCheckIterator(const EdgesIterator& ei)const
{
    if(ei.vecPtr != &this->mEdges)
    {
        throw std::runtime_error("[Graph] Incorrect iterator");
    }
}

template<typename V,typename E>
void Graph<V,E>::printNeighborhoodMatrix()const
{
    std::size_t x,y;
    std::size_t vertices_number = this->nrOfVertices();

    std::cout<<"NrOfVertices:"<<vertices_number<<std::endl;
    std::cout<<"NrOfEdges:"<<mEdgesNumber<<std::endl;

    {
        std::size_t width = this->mEdges.size();
        std::size_t height = width>0?this->mEdges[0].size():0;

        if(width!=vertices_number || height!=vertices_number)
        {
            std::cout<<"[Incorrect matrix]"<<std::endl;
            std::cout<<"Width:"<<width<<std::endl;
            std::cout<<"Height:"<<height<<std::endl;

            return;
        }
    }

    if(vertices_number==0)
    {
        std::cout<<"+\n";
        return;
    }

    std::cout<<"\n";

    for(y=0;y<vertices_number;++y)
    {
        for(x=0;x<vertices_number;++x)
        {
            std::cout<<"+---";
        }
        std::cout<<"+\n";
        for(x=0;x<vertices_number;++x)
        {
            std::cout<<"| "<<this->mEdges[y][x].has_value()<<" ";
        }
        std::cout<<"|\n";
    }
    for(x=0;x<vertices_number;++x)
    {
        std::cout<<"+---";
    }
    std::cout<<"+\n\n";
}

template<typename V,typename E>
template<bool BFS>
typename Graph<V,E>::template FSIterator<BFS>&  Graph<V,E>::FSIterator<BFS>::operator++()
{
    if(!this->mCollection.empty())
    {
        std::size_t verticesNumber = this->mGraph->nrOfVertices();
        std::size_t vId = mCollectionTop();
        this->mVisitedVertices[vId] = true;
        this->mCollection.pop();


        if constexpr (BFS)
        {
            for(std::size_t iter=0; iter < verticesNumber; ++iter)
            {
                if(!mVisitedVertices[iter])
                {
                    if(this->mGraph->edgeExist(vId,iter))
                    {
                        this->mVisitedVertices[iter] = true;
                        this->mCollection.push(iter);
                    }
                }
            }
        }
        else
        {
            std::size_t iter = verticesNumber;
            while(iter>0)
            {
                --iter;
                if(!mVisitedVertices[iter])
                {
                    if(this->mGraph->edgeExist(vId,iter))
                    {
                        this->mVisitedVertices[iter] = true;
                        this->mCollection.push(iter);
                    }
                }
            }
        }
    }

    if(this->mCollection.empty())
    {
        this->mVertexIndex = this->mGraph->nrOfVertices();;
        this->mVertexIndexStart = mVertexIndex;
    }
    else
    {
        this->mVertexIndex = mCollectionTop();
    }

    return *this;
}
