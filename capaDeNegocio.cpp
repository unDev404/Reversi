#include "capaDeNegocio.h"
#include <iostream>

using namespace std;

capaDeNegocio::capaDeNegocio() {
    tablero = vector<vector<int>>(8, vector<int>(8, 0));
    iniTablero();
}

void capaDeNegocio::iniTablero() {
    for(int i = 0; i < 8; i++) fill(tablero[i].begin(), tablero[i].end(), 0);
    // Blanco
    tablero[3][3] = 2; 
    tablero[4][4] = 2; 
    // Negro
    tablero[3][4] = 1; 
    tablero[4][3] = 1; 
    
    turnoActual = 1;   // Las negras inician siempre
}

int capaDeNegocio::getTurno() { return turnoActual; }

void capaDeNegocio::cambiarTurno() { turnoActual = (turnoActual == 1) ? 2 : 1; }

vector<vector<int>> capaDeNegocio::getTablero() { return tablero; }

void capaDeNegocio::setEstado(vector<vector<int>> t, int turno) { tablero = t; turnoActual = turno; }

bool capaDeNegocio::movValido(int fila, int col, int jugador, bool aplicar) {
    if (fila < 0 || fila >= 8 || col < 0 || col >= 8 || tablero[fila][col] != 0) return false;

    bool jugadaValida = false;
    int oponente = (jugador == 1) ? 2 : 1;

    for (int dir = 0; dir < 8; dir++) {
        int x = fila + dx[dir];
        int y = col + dy[dir];
        bool encontroOponente = false;

        while (x >= 0 && x < 8 && y >= 0 && y < 8 && tablero[x][y] == oponente) {
            encontroOponente = true;
            x += dx[dir];
            y += dy[dir];
        }

        if (encontroOponente && x >= 0 && x < 8 && y >= 0 && y < 8 && tablero[x][y] == jugador) {
            jugadaValida = true;
            if (aplicar) {
                int voltearX = fila + dx[dir];
                int voltearY = col + dy[dir];
                while (voltearX != x || voltearY != y) {
                    tablero[voltearX][voltearY] = jugador;
                    voltearX += dx[dir];
                    voltearY += dy[dir];
                }
            }
        }
    }
    if (jugadaValida && aplicar) tablero[fila][col] = jugador;
    return jugadaValida;
}

bool capaDeNegocio::tieneMovimientos(int jugador) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (movValido(i, j, jugador)) return true;
        }
    }
    return false;
}

void capaDeNegocio::contFichas(int& negras, int& blancas) {
    negras = 0; blancas = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tablero[i][j] == 1) negras++;
            else if (tablero[i][j] == 2) blancas++;
        }
    }
}

void capaDeNegocio::jugarPC(int jugador) {
    int mejorFila = -1, mejorCol = -1;
    int maxVolteadas = -1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (movValido(i, j, jugador)) {
                vector<vector<int>> tableroTemp = tablero;
                movValido(i, j, jugador, true);
                
                int nNegras, nBlancas;
                contFichas(nNegras, nBlancas);
                int ganancia = (jugador == 1) ? nNegras : nBlancas;

                if (ganancia > maxVolteadas) {
                    maxVolteadas = ganancia;
                    mejorFila = i;
                    mejorCol = j;
                }
                tablero = tableroTemp;
            }
        }
    }

    if (mejorFila != -1) {
        movValido(mejorFila, mejorCol, jugador, true);
        cout << "La PC jugo en la posicion: " << char('A' + mejorCol) << mejorFila + 1 << "\n";
    }
}