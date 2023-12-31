#include <algorithm>
#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
int N;
int cantSubfilas;
int cantSubcol;
vector<vector<int>> subfilas; // Reconstruyo el tablero pero cada casillero tiene la fila a la que pertenece. Para saber las intersecciones con las columnas.
vector<vector<int>> digrafo; // Los enlaces de subfilas a subcol
vector<vector<int>> tablero;
int hueco = -1;
//EK:
int INF = 1e6;
int n;
vector<vector<int>> capacity;
vector<vector<int>> adj;


int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(n);
    int new_flow;

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}

int main(){
    int c;
    cin >> c;
    for(int i = 0; i < c; i++){
        cin >> N;
        cantSubfilas = 0;
        cantSubcol = 0;
        subfilas = vector<vector<int>>(N, vector<int>(N));

        // Nos guardarmos la matriz y despues la recorrermos tomando en cuenta que en el cambio 0 a 1 hay una nueva fila
        tablero = vector<vector<int>>(N, vector<int>(N));
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                cin >> tablero[j][k];
            }
        }

        for(int j = 0; j < N; j++){
            if(tablero[j][0] == 1){
                subfilas[j][0] = hueco;
            }else{
                subfilas[j][0] = cantSubfilas;
            }
            for(int k = 1; k < N; k++){
                if(tablero[j][k] == 1){
                    subfilas[j][k] = hueco;
                    if(tablero[j][k-1] == 0){
                        cantSubfilas++;
                    }
                }else{
                    subfilas[j][k] = cantSubfilas;
                }
            }
            if(tablero[j][N-1] != 1){
                cantSubfilas++;
            }
        }

        digrafo = vector<vector<int>>(cantSubfilas);
        int num = cantSubfilas; // Para id de columna

        // Vemos las intersecciones entre filas y col
        for(int j = 0; j < N; j++){

            if(tablero[0][j] != 1){
                digrafo[subfilas[0][j]].push_back(num);
            }

            for(int k = 1; k < N; k++){
                if(tablero[k][j] == 1){
                    if(tablero[k-1][j] == 0){
                        num++;
                    }
                }else{
                    digrafo[subfilas[k][j]].push_back(num);
                };
            }

            if(tablero[N-1][j] != 1){
                num++;
            }
        }

        cantSubcol = num - cantSubfilas;
        int cantNodos = cantSubfilas + cantSubcol + 2;
        int source = cantNodos - 2;
        int sink = cantNodos - 1;

        adj = vector<vector<int>>(cantNodos);
        capacity = vector<vector<int>>(cantNodos, vector<int>(cantNodos, 0));

        // Lleno capacity y adj
        for(int subfila = 0; subfila < cantSubfilas; subfila++){
            for(auto subcol : digrafo[subfila]){
                capacity[subfila][subcol] = 1;
                adj[subfila].push_back(subcol);
                adj[subcol].push_back(subfila);
            }
            // Une el source con las subfilas
            capacity[source][subfila] = 1;
            adj[source].push_back(subfila);
            adj[subfila].push_back(source);
        }

        // Falta unir las subcolumnas al sink
        for(int subcol = cantSubfilas; subcol < cantNodos - 2; subcol++){
            capacity[subcol][sink] = 1;
            adj[sink].push_back(subcol);
            adj[subcol].push_back(sink);
        }

        n = cantNodos;
        cout << maxflow(source, sink) << endl;
    }
    return 0;
}



