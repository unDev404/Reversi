#include "capaDeDatos.h"
#include <iostream>
#include <fstream>

using namespace std;

void capaDeDatos::guardarPartida(const vector<vector<int>>& tablero, int turnoActual) {
    ofstream archivo("reversi_guardado.txt");
    if (archivo.is_open()) {
        archivo << turnoActual << endl;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                archivo << tablero[i][j] << " ";
            }
            archivo << endl;
        }
        archivo.close();
        cout << "\n[SISTEMA] Partida guardada exitosamente en reversi_guardado.txt\n";
    }
}

bool capaDeDatos::cargarPartida(vector<vector<int>>& tablero, int& turnoActual) {
    ifstream archivo("reversi_guardado.txt");
    if (archivo.is_open()) {
        archivo >> turnoActual;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                archivo >> tablero[i][j];
            }
        }
        archivo.close();
        return true;
    }
    return false;
}