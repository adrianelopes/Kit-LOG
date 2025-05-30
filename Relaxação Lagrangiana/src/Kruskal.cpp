#include "Kruskal.h"

Kruskal::Kruskal(vvi dist) // Recebe um vector de vector
{
    for (int i = 0; i < dist.size(); ++i)
    {
        for (int j = i + 1; j < dist[i].size(); ++j)
        {
            graph.push(make_pair(-dist[i][j], make_pair(i, j)));
        }
    }
}

void Kruskal::initDisjoint(int n)
{
    pset.resize(n);
    for (int i = 0; i < n; ++i)
    {
        pset[i] = i;
    }
}

int Kruskal::findSet(int i)
{
    return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j)
{
    pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j)
{
    return (findSet(i) == findSet(j)) ? true : false;
}

vii Kruskal::getEdges()
{
    return edges;
}

double Kruskal::MST(int nodes)
{
    initDisjoint(nodes);

    edges = vector<vector<int>>(nodes);

    double cost = 0;

    while (!graph.empty())
    {
        pair<double, ii> p = graph.top();
        graph.pop();

        if (!isSameSet(p.second.first, p.second.second))
        {
            edges[p.second.second].push_back(p.second.first + 1);
            edges[p.second.first].push_back(p.second.second + 1);
            cost += (-p.first);
            unionSet(p.second.first, p.second.second);
        }
    }

    return cost;
}