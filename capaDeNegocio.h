#ifndef CAPA_DE_NEGOCIO_H
#define CAPA_DE_NEGOCIO_H

#include <vector>
using namespace std;

class capaDeNegocio {
private:
    vector<vector<int>> tablero;
    int turnoActual; // 1 = Negras, 2 = Blancas
    int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[8] = {0, 0, 1, -1, 1, -1, 1, -1};

public:
    capaDeNegocio();
    void iniTablero();
    int getTurno();
    void cambiarTurno();
    vector<vector<int>> getTablero();
    void setEstado(vector<vector<int>> t, int turno);
    bool movValido(int fila, int col, int jugador, bool aplicar = false);
    bool tieneMovimientos(int jugador);
    void contFichas(int& negras, int& blancas);
    void jugarPC(int jugador);
};

#endif