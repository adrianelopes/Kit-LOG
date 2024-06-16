#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>


using namespace std;



typedef struct Solution{
 vector <int> sequence;
 double cost;

};

typedef struct InsertionInfo{
int noInserido;
int arestaRemovida;
double custo;
};


vector <InsertionInfo> calcularCustoInsercao(Solution& s, vector<int>& CL, Data& data){

int m = (s.sequence.size()-1)*CL.size();

vector <InsertionInfo> custoInsercao(m);
int l = 0;
for (int a = 0; a < s.sequence.size() - 1; a++){
int i = s.sequence[a];
int j = s.sequence[a + 1];
for (auto k: CL){
    custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(j, k) - data.getDistance(i, j);
    custoInsercao[l].noInserido = k;
    custoInsercao[l].arestaRemovida = a;
    l++;
}

}

return custoInsercao;

}



vector<int> escolher3NosAleatorios(Solution& s, vector <int>& CL){
    
s.sequence = {1};

for(int i=0; i<3; i++){
int add = rand () % (CL.size());

s.sequence.push_back(add);

CL.erase(CL.begin() + add);
}


s.sequence.push_back(1);
return s.sequence;

}


/*vector<int> nosRestantes(Solution s, vector <int> cl){

    vector<int>CL;
    int tam = s.sequence.size();
    int ta = cl.size();

    for(int i=1; i<ta; i++){
     

     for(int j=1; j<tam; j++ ){

        if (s.sequence[j] != i){
           CL.push_back(i);
        }

     }

    }

    return CL;

}*/

bool compararCusto(const InsertionInfo &primeiro, const InsertionInfo &segundo) {
  return primeiro.custo < segundo.custo;
}


void ordenarEmOrdemCrescente (vector <InsertionInfo>& c){
sort(c.begin(), c.end(), compararCusto);
}

void inserirNaSolucao(Solution& s, int no, int aresta){
auto position = aresta + 1;
s.sequence.insert(s.sequence.begin() + position, no);
std:: vector <int>:: iterator it;
it = find(s.sequence.begin(), s.sequence.end(), no);
s.sequence.erase(it);

}

Solution Construcao(Data& data){

Solution s;

int t = data.getDimension()-1;
vector <int> CL(t);

for(int i=0, k=2; i<t, i++; k++){
CL[i] = k;
}

s.sequence = escolher3NosAleatorios(s, CL);
//CL = nosRestantes(s, CL);

while (!CL.empty()){
    vector <InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, data);
    ordenarEmOrdemCrescente (custoInsercao);
    double alpha = (double) rand() / 1;
    int selecionado = rand() % ((int)ceil (alpha * custoInsercao.size()));
    int no = custoInsercao[selecionado].noInserido;
    int aresta = custoInsercao[selecionado].arestaRemovida;
    inserirNaSolucao(s, custoInsercao[selecionado].noInserido, custoInsercao[selecionado].arestaRemovida);
   // CL = nosRestantes(s, CL);
    
}

return s;

}


bool Swap (Solution *s, Data& data){

double bestDelta = 0;
int best_i, best_j;
for(int i = 1; i < s->sequence.size() - 1; i++){
int vi = s->sequence[i];
int vi_next = s->sequence [i + 1];
int vi_prev = s->sequence [i - 1];

for(int j= i + 1; j < s->sequence.size() - 1; j++){
int vj = s->sequence[j];
int vj_next = s->sequence[j+1];
int vj_prev = s->sequence [j - 1];
double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) + data.getDistance(vi_prev, vj)  + 
data.getDistance(vj, vi_next) - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next) + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next); 

if(delta < bestDelta){
    bestDelta = delta;
    best_i = i;
    best_j = j;
}
}


}

if(bestDelta < 0){
swap(s->sequence[best_i], s->sequence[best_j]);
s->cost = s->cost + bestDelta;
return true;

}

return false;

}


bool TwoOpt(Solution *s, Data& data){
double bestDelta = 0;
int best_i, best_j;

for(int i = 1; i<s->sequence.size() - 1; i++){

int vi= s->sequence[i];
int vi_next = s->sequence[i-1];
int vi_prev = s->sequence [i - 1];

for(int j= i + 1; j < s->sequence.size() - 1; j++){
int vj = s->sequence[j];
int vj_next = s->sequence[j+1];
int vj_prev = s->sequence [j - 1];
double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj)  + 
data.getDistance(vi, vj_next);

if(delta < bestDelta){
    bestDelta = delta;
    best_i = i;
    best_j = j;
}
}

}

if(bestDelta < 0){
reverse(s->sequence[best_i], s->sequence[best_j]);
s->cost = s->cost + bestDelta;
return true;

}

return false;


}


bool OrOpt (Solution *s, int n, Data& data){
double bestDelta = 0;
int best_i, best_j;
double delta;

for(int i = 1; i<s->sequence.size() - 1; i++){

int vi= s->sequence[i];
int vi_next = s->sequence[i+n];
int vi_prev = s->sequence [i - 1];

/*Reinsertion: {4} bloco de 1 elemento, frist=4; last=4
  OR-OPT-2:  {4,10} bloco de 2 elementos, frist=4; last=10
  OT-OPT-3:  {4,10,3} bloco de 3 elementos, frist=4; last=3
  Desse forma atribuimos que o índice do elemento last é dado por i+(n-1)*/

int last = s->sequence[i+(n-1)];

for(int j= i + n; j < s->sequence.size() - 1; j++){
int vj = s->sequence[j];
int vj_next = s->sequence[j+1];
int vj_prev = s->sequence [j - 1];

double delta = - data.getDistance(vi_prev, vi) - data.getDistance(last, vi_next) - data.getDistance(vj_prev, vj) + data.getDistance(vi_prev, vi_next)  + 
data.getDistance(vj_prev, vi) + data.getDistance(last, vj);


if(delta < bestDelta){
    bestDelta = delta;
    best_i = i;
    best_j = j;
}

}

}

if(bestDelta < 0){

for(int rep=1; rep<=n; rep++){

int position = best_j - 1;
int vertice = s->sequence[best_i];
s->sequence.erase(s->sequence.begin() + best_i);
s->sequence.insert(s->sequence.begin() + position, vertice);

}
s->cost = s->cost + bestDelta;
return true;

}

return false;

}

void BuscaLocal(Solution *s, Data& data){

vector <int> NL = {1,2,3,4,5};
bool improved = false;

while (NL.empty()== false){

int n = rand() % NL.size();

switch (NL[n]){
case 1:
improved = Swap(s, data);
break;

case 2:
improved = TwoOpt(s, data);
break;

case 3:
improved = OrOpt(s, 1, data); //Reinsertion
break;

case 4:
improved = OrOpt (s, 2, data); //  Or-opt2
break;

case 5:
improved = OrOpt (s, 3, data); // Or-opt3
break;
}

if(improved){
    NL ={1, 2, 3, 4, 5};

} else{
    NL.erase(NL.begin() + n);
}

}

}


Solution ILS (int maxIter, int maxIterIls, Data& data){

Solution bestOfAll;
bestOfAll.cost = INFINITE;
for(int i=0; i< maxIter; i++){
    Solution s = Construcao(data);
    Solution best = s;

    int iterIls = 0;

    while(iterIls <= maxIterIls){
        BuscaLocal(&s, data);
        if(s.cost < best.cost){
            best = s;
            iterIls = 0;
        }
        
        //s = Pertubacao(best);
        iterIls++;
    }
    if(best.cost < bestOfAll.cost){
        bestOfAll = best;
    }

}

return bestOfAll;

}



int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    int maxIter, maxIterIls;

    if(n >= 150){
        maxIterIls = n/2;

    } else {
      maxIterIls = n;
    }

    maxIter = 50;

    Solution final = ILS (maxIter,maxIterIls, data);


   /* cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;*/

    return 0;
}
