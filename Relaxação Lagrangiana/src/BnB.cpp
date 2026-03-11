#include "BnB.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;
/*
Roteiro:
1.Uma função para verificar se a melhor árvore é um tour;
2.Uma função para proibir os arcos associados ao vértice de maior grau
3.A função do branch and bound
OBS: Colocar o vetor de penalizadores na estrutura
*/

// Criar função que:
// Usa o dual lagrangiano
// Verifica se a melhor árvore obtida é um tour
// Se não for: encontra o vértice com maior grau, proibe os arcos associados a esse vértice nos nós filhos

void printNo(long long nodeCount, long long left, long long treeSz, double bestInteger, double bestBound, long long iterCount, double gap, vector<pair<int, int>> &forbidden_arcs)
{

    cout << setw(8) << nodeCount
         << setw(8) << left
         << setw(12) << treeSz
         << setw(14) << fixed << setprecision(2) << bestInteger
         << setw(14) << fixed << setprecision(2) << bestBound
         << setw(10) << iterCount
         << setw(10) << fixed << setprecision(2) << gap
         //<< setw(10) << fixed << setprecision(2) << forbidden_arcs.size()
         //<< setw(10) << fixed << "(" << setprecision(2) << forbidden_arcs.back().first << ", " << forbidden_arcs.back().second << ")"
         << "\n";
}
// Cada vértice ter grau 2 e todos os vértices estarem em um único ciclo
bool isTour(const vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++)
    {
        if (adj[i].size() != 2)
            return false;
    }

    int visitedCount = 0;
    int curr = 0; // Começamos pelo vértice 0
    int prev = -1;

    // Percorre o ciclo seguindo as adjacências
    while (!visited[curr])
    {
        visited[curr] = true;
        visitedCount++;

        // Como o grau é 2, cada nó tem dois vizinhos.
        // Escolhemos o vizinho que não é o nó de onde viemos (prev).
        int next = -1;
        for (int neighbor : adj[curr])
        {
            if (neighbor != prev)
            {
                next = neighbor;
                break;
            }
        }

        if (next == -1)
            break; // Não deve acontecer se grau == 2

        prev = curr;
        curr = next;
    }

    // A solução é viável se visitamos todos os nós antes de voltar ao início
    return (visitedCount == n);
}

AuxNode verificaTour(Data *data, vector<double> &lambda, double upper_bound, double **cost, const vector<pair<int, int>> &forbidden_arcs)
{
    AuxNode aux;
    Tree tree = Subgradient(data, cost, upper_bound, lambda, forbidden_arcs);

    aux.cost = tree.cost;
    aux.lambda = tree.lambda;
    int n = data->getDimension();
    bool allDegreesTwo = true;
    int maiorgrau = -1;
    int maiorno = 0;

    for (int i = 0; i < n; i++)
    {
        int grau = tree.listAdj[i].size();
        if (grau != 2)
            allDegreesTwo = false;

        if (grau > maiorgrau)
        {
            maiorgrau = grau;
            maiorno = i;
        }
    }

    // Só chama a função de conectividade se a condição básica de grau for aceita
    if (allDegreesTwo)
    {
        aux.tour = isTour(tree.listAdj);
    }
    else
    {
        aux.tour = false;
    }

    aux.degreeAdj = tree.listAdj[maiorno];
    aux.v = maiorno;
    return aux;
}

void updateNode(Node *node, Data *data, double &upper_bound, double **cost, vector<double> &lambda)
{
    for (int i = 0; i < data->getDimension(); i++)
    {
        for (int j = 0; j < data->getDimension(); j++)
        {
            if (i == j)
            {
                cost[i][j] = 9999999;
            }
            else
            {
                cost[i][j] = data->getDistance(i + 1, j + 1);
            }
        }
    }

    // // Proibindo os arcos
    // for (auto aresta : node->forbidden_arcs)
    // {
    //     cost[aresta.first][aresta.second] = 9999999;
    //     cost[aresta.second][aresta.first] = 9999999;
    // }

    AuxNode aux = verificaTour(data, lambda, upper_bound, cost, node->forbidden_arcs);

    node->lambda = aux.lambda;
    node->lower_bound = aux.cost;
    node->highestDegreeAdj = aux.degreeAdj;
    node->highestDegreeNode = aux.v;
    node->feasible = aux.tour; // verificar viabilidade
}

double branch_and_bound(Data *data, double upper_bound, int tipo)
{
    double **cost = new double *[data->getDimension()];
    for (int i = 0; i < data->getDimension(); i++)
    {
        cost[i] = new double[data->getDimension()];
        for (int j = 0; j < data->getDimension(); j++)
        {
            if (i == j)
            {
                cost[i][j] = 9999999;
            }
            else
            {
                cost[i][j] = data->getDistance(i + 1, j + 1);
            }
        }
    }

    Node root;
    vector<double> lambda(data->getDimension(), 0.0);
    root.lambda = lambda;
    root.forbidden_arcs = {};
    updateNode(&root, data, upper_bound, cost, root.lambda);
    long long nodeCount = 0;             // Nós processados
    long long iterCount = 0;             // Contador de cortes/interações
    double bestInteger = upper_bound;    // Melhor solução inteira até agora
    double bestBound = root.lower_bound; // Melhor limite inferior

    if (tipo == 3)
    {
        priority_queue<Node> tree;
        tree.push(root);

        while (!tree.empty())
        {
            Node node = tree.top();
            tree.pop();
            nodeCount++;
            iterCount++;

            // Cortar os nós com valores piores que o upper_bound
            if (node.lower_bound >= upper_bound)
            {
                continue;
            }

            if (node.feasible)
            {
                upper_bound = min(upper_bound, node.lower_bound);
                // atualiza melhor inteiro
                if (node.lower_bound < bestInteger)
                {
                    bestInteger = node.lower_bound;
                }
            }

            // atualiza melhor bound
            bestBound = (!tree.empty() ? tree.top().lower_bound : node.lower_bound);

            if (nodeCount % 100 == 0)
            {
                long long left = (long long)tree.size();
                long long treeSz = (long long)tree.size();
                double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
                // printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap, node.forbidden_arcs);
            }

            if (bestInteger == bestBound)
            {
                long long left = (long long)tree.size();
                long long treeSz = (long long)tree.size();
                double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
                // printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap, node.forbidden_arcs);

                break;
            }

            if (!node.feasible)
            {
                for (int i = 0; i < node.highestDegreeAdj.size(); i++)
                {
                    Node son;

                    son.forbidden_arcs = node.forbidden_arcs;
                    pair<int, int> forbidden_arc = {
                        node.highestDegreeNode,
                        node.highestDegreeAdj[i]};
                    son.forbidden_arcs.push_back(forbidden_arc);
                    son.lambda = node.lambda;
                    updateNode(&son, data, upper_bound, cost, son.lambda);
                    if (son.lower_bound < upper_bound)
                    {
                        tree.push(son);
                    }
                }
            }
        }
    }

    else
    {
        deque<Node> tree;
        tree.push_back(root);
        Node node;

        while (!tree.empty())
        {
            if (tipo == 1)
            {
                // DFS
                node = tree.back();
                tree.pop_back();
            }
            else
            {
                // BFS
                node = tree.front();
                tree.pop_front();
            }
            nodeCount++;
            iterCount++;

            // Cortar os nós com valores piores que o upper_bound
            if (node.lower_bound > upper_bound)
            {
                continue;
            }

            if (node.feasible)
            {
                upper_bound = min(upper_bound, node.lower_bound);
                if (node.lower_bound < bestInteger)
                {
                    bestInteger = node.lower_bound;
                }
            }

            // atualiza melhor bound
            if (!tree.empty())
            {
                double bestLb = tree.front().lower_bound;
                for (const auto &n : tree)
                    bestLb = min(bestLb, n.lower_bound);
                bestBound = bestLb;
            }
            if (nodeCount % 100 == 0)
            {
                long long left = (long long)tree.size();
                long long treeSz = (long long)tree.size();
                double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
                // printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap, node.forbidden_arcs);
            }

            if (bestInteger == bestBound)
            {
                long long left = (long long)tree.size();
                long long treeSz = (long long)tree.size();
                double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
                // printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap, node.forbidden_arcs);
                break;
            }

            if (!node.feasible)
            {
                for (int i = 0; i < node.highestDegreeAdj.size(); i++)
                {
                    Node son;
                    son.forbidden_arcs = node.forbidden_arcs;
                    pair<int, int> forbidden_arc = {
                        node.highestDegreeNode,
                        node.highestDegreeAdj[i]};
                    son.forbidden_arcs.push_back(forbidden_arc);
                    son.lambda = node.lambda;
                    updateNode(&son, data, upper_bound, cost, son.lambda);
                    if (son.lower_bound < upper_bound)
                    {
                        tree.push_back(son);
                    }
                }
            }
        }
    }

    for (int i = 0; i < data->getDimension(); i++)
        delete[] cost[i];
    delete[] cost;
    return upper_bound;
}
