#include "capaDePresentacion.h"
#include <iostream>
#include <string>
#include <windows.h> // Para la función Sleep()

using namespace std;

void capaDePresentacion::mostrarTablero() {
    vector<vector<int>> t = negocio.getTablero();
    cout << "\n   A B C D E F G H\n";
    for (int i = 0; i < 8; i++) {
        cout << i + 1 << "  ";
        for (int j = 0; j < 8; j++) {
            if (t[i][j] == 0) cout << ". ";
            else if (t[i][j] == 1) cout << "N "; // Negras
            else cout << "B ";                   // Blancas
        }
        cout << " " << i + 1 << "\n";
    }
    cout << "   A B C D E F G H\n\n";
    
    int n, b;
    negocio.contFichas(n, b);
    cout << "Fichas -> Negras (N): " << n << " | Blancas (B): " << b << "\n";
}

void capaDePresentacion::turnoHumano(int jugador) {
    string entrada;
    bool jugadaHecha = false;

    while (!jugadaHecha) {
        cout << "Jugador " << (jugador == 1 ? "Negras (N)" : "Blancas (B)") << ", ingresa tu jugada (Ej: D3) o 'G' para guardar: ";
        cin >> entrada;

        if (entrada == "G" || entrada == "g") {
            datos.guardarPartida(negocio.getTablero(), negocio.getTurno());
            continue;
        }

        if (entrada.length() < 2) { cout << "Entrada invalida.\n"; continue; }

        int col = toupper(entrada[0]) - 'A';
        int fila = entrada[1] - '1';

        if (negocio.movValido(fila, col, jugador, true)) {
            jugadaHecha = true;
        } else {
            cout << "Movimiento invalido. Debes flanquear al menos una ficha rival.\n";
        }
    }
}

void capaDePresentacion::iniciarJuego() {
    bool ejecutando = true;

    while (ejecutando) {
        int opcion;
        cout << "\n=============================================\n";
        cout << "=== B I E N V E N I D O   A   R E V E R S I ===\n";
        cout << "=============================================\n";
        cout << "1. Jugador vs Jugador\n";
        cout << "2. Jugador vs PC\n";
        cout << "3. PC vs PC\n";
        cout << "4. Cargar Partida Guardada\n";
        cout << "5. Salir del Juego\n";
        cout << "Elige una opcion: ";
        cin >> opcion;

        if (opcion == 5) {
            cout << "\n¡Gracias por jugar Reversi! Saliendo del sistema...\n";
            ejecutando = false;
            break; 
        }

        int modoActual = opcion;
        int eleccionColor = 1; // Por defecto: Humano es Negras (1)

        if (opcion == 2) {
            cout << "\n--- Selecciona tu bando ---\n";
            cout << "1. Jugar con Negras (N) - Mueves primero\n";
            cout << "2. Jugar con Blancas (B) - PC mueve primero\n";
            cout << "Elige una opcion: ";
            cin >> eleccionColor;
        }

        if (opcion == 4) {
            vector<vector<int>> t(8, vector<int>(8));
            int turnoAnterior;
            if (datos.cargarPartida(t, turnoAnterior)) {
                negocio.setEstado(t, turnoAnterior);
                cout << "\n[SISTEMA] Partida reanudada correctamente en modo Jugador vs Jugador.\n";
                modoActual = 1; // Al cargar, se asume el control manual por defecto
            } else {
                cout << "\n[ERROR] No se encontro ninguna partida guardada.\n";
                continue;
            }
        } else if (opcion < 1 || opcion > 3) {
            cout << "\nOpcion no valida. Intenta de nuevo.\n";
            continue;
        }

        // Si es una partida nueva, inicializamos el tablero limpito
        if (opcion != 4) {
            negocio.iniTablero();
        }

        bool finDePartida = false;

        while (!finDePartida) {
            mostrarTablero();
            int turno = negocio.getTurno();

            if (!negocio.tieneMovimientos(turno)) {
                cout << "El jugador " << (turno == 1 ? "Negras" : "Blancas") << " no tiene movimientos validos. Pasa el turno.\n";
                negocio.cambiarTurno();
                if (!negocio.tieneMovimientos(negocio.getTurno())) {
                    finDePartida = true; // Fin si ninguno puede mover
                }
                continue;
            }

            // Evaluar si le toca a la IA o a un humano
            bool juegaPC = false;
            if (modoActual == 3) {
                juegaPC = true; // PC vs PC
            } else if (modoActual == 2) {
                if (eleccionColor == 1 && turno == 2) juegaPC = true; // Humano es Negro, PC es Blanco
                if (eleccionColor == 2 && turno == 1) juegaPC = true; // Humano es Blanco, PC es Negro
            }

            if (juegaPC) {
                cout << "La PC esta analizando el tablero...\n";
                Sleep(1000); // Pausa usando windows.h
                negocio.jugarPC(turno);
            } else {
                turnoHumano(turno);
            }

            negocio.cambiarTurno();
        }

        // Resultados finales antes de volver al menú
        mostrarTablero();
        cout << "\n=== F I N   D E L   J U E G O ===\n";
        int n, b;
        negocio.contFichas(n, b);
        if (n > b) cout << "¡Ganan las Negras con " << n << " fichas!\n";
        else if (b > n) cout << "¡Ganan las Blancas con " << b << " fichas!\n";
        else cout << "¡Es un Empate de " << n << " fichas!\n";
        
        cout << "\nPresiona cualquier letra y Enter para regresar al menu principal: ";
        string pausa;
        cin >> pausa;
    }
}