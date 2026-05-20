#ifndef CAPA_DE_DATOS_H
#define CAPA_DE_DATOS_H

#include <vector>
using namespace std;

class capaDeDatos {
public:
    void guardarPartida(const vector<vector<int>>& tablero, int turnoActual);
    bool cargarPartida(vector<vector<int>>& tablero, int& turnoActual);
};

#endif