#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ==========================================
// 1. CAPA DE DATOS
// ==========================================
class CapaDatos {
public:
    // Guarda el estado actual del tablero y de quién es el turno
    void guardarPartida(char tablero[8][8], char turnoActual) {
        ofstream archivo("partida_reversi.txt");
        if (archivo.is_open()) {
            archivo << turnoActual << endl;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    archivo << tablero[i][j];
                }
                archivo << endl;
            }
            archivo.close();
            cout << "Partida guardada exitosamente en la Capa de Datos.\n";
        } else {
            cout << "Error al guardar el archivo.\n";
        }
    }
};

// ==========================================
// 2. CAPA DE NEGOCIO
// ==========================================
class CapaNegocio {
private:
    char tablero[8][8];
    char turnoActual; // 'N' para Negras, 'B' para Blancas
    CapaDatos gestorDatos;

    // Direcciones para evaluar el flanqueo (8 direcciones posibles)
    int df[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

public:
    CapaNegocio() {
        inicializarJuego();
    }

    void inicializarJuego() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                tablero[i][j] = ' ';
            }
        }
        // Posiciones iniciales centrales cruzadas
        tablero[3][3] = 'B';
        tablero[3][4] = 'N';
        tablero[4][3] = 'N';
        tablero[4][4] = 'B';
        
        turnoActual = 'N'; // Las negras siempre inician
    }

    char getTurno() { return turnoActual; }
    
    char obtenerCasilla(int f, int c) { return tablero[f][c]; }

    void cambiarTurno() {
        turnoActual = (turnoActual == 'N') ? 'B' : 'N';
    }

    // Verifica si un movimiento encierra fichas enemigas en una dirección específica
    int evaluarFlanqueo(int f, int c, char jugador, int dirF, int dirC, bool aplicar) {
        char oponente = (jugador == 'N') ? 'B' : 'N';
        int filaActual = f + dirF;
        int colActual = c + dirC;
        int fichasAFlanquear = 0;

        // Avanzar mientras encontremos fichas del oponente
        while (filaActual >= 0 && filaActual < 8 && colActual >= 0 && colActual < 8 && tablero[filaActual][colActual] == oponente) {
            fichasAFlanquear++;
            filaActual += dirF;
            colActual += dirC;
        }

        // Si encontramos una ficha propia al final de la línea, el flanqueo es válido
        if (fichasAFlanquear > 0 && filaActual >= 0 && filaActual < 8 && colActual >= 0 && colActual < 8 && tablero[filaActual][colActual] == jugador) {
            if (aplicar) {
                // Voltear las fichas encerradas
                int fVoltear = f + dirF;
                int cVoltear = c + dirC;
                for (int i = 0; i < fichasAFlanquear; i++) {
                    tablero[fVoltear][cVoltear] = jugador;
                    fVoltear += dirF;
                    cVoltear += dirC;
                }
            }
            return fichasAFlanquear;
        }
        return 0;
    }

    // Comprueba si colocar una ficha en (f, c) es legal
    bool esMovimientoValido(int f, int c, char jugador) {
        if (f < 0 || f >= 8 || c < 0 || c >= 8 || tablero[f][c] != ' ') return false;

        int totalVolteadas = 0;
        for (int i = 0; i < 8; i++) {
            totalVolteadas += evaluarFlanqueo(f, c, jugador, df[i], dc[i], false);
        }
        return totalVolteadas > 0;
    }

    // Ejecuta el movimiento y voltea las fichas correspondientes
    bool aplicarMovimiento(int f, int c, char jugador) {
        if (!esMovimientoValido(f, c, jugador)) return false;

        tablero[f][c] = jugador;
        for (int i = 0; i < 8; i++) {
            evaluarFlanqueo(f, c, jugador, df[i], dc[i], true);
        }
        return true;
    }

    // Verifica si el jugador actual tiene al menos un movimiento válido en el tablero
    bool tieneMovimientosValidos(char jugador) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (esMovimientoValido(i, j, jugador)) return true;
            }
        }
        return false;
    }

    // Lógica básica de Computadora (Algoritmo Codicioso / Greedy)
    void jugarComputadora() {
        int maxVolteadas = -1;
        int mejorF = -1, mejorC = -1;

        // Analiza todo el tablero buscando la jugada que voltee más fichas
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (esMovimientoValido(i, j, turnoActual)) {
                    int volteadasAqui = 0;
                    for (int d = 0; d < 8; d++) {
                        volteadasAqui += evaluarFlanqueo(i, j, turnoActual, df[d], dc[d], false);
                    }
                    if (volteadasAqui > maxVolteadas) {
                        maxVolteadas = volteadasAqui;
                        mejorF = i;
                        mejorC = j;
                    }
                }
            }
        }

        if (mejorF != -1) {
            cout << "Computadora juega en: " << char(mejorC + 'A') << mejorF + 1 << endl;
            aplicarMovimiento(mejorF, mejorC, turnoActual);
        }
    }

    void contarFichas(int &negras, int &blancas) {
        negras = 0; blancas = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (tablero[i][j] == 'N') negras++;
                else if (tablero[i][j] == 'B') blancas++;
            }
        }
    }
    
    void guardarPartidaEnDatos() {
        gestorDatos.guardarPartida(tablero, turnoActual);
    }
};

// ==========================================
// 3. CAPA DE PRESENTACIÓN
// ==========================================
class CapaPresentacion {
private:
    CapaNegocio negocio;

public:
    void mostrarTablero() {
        cout << "\n  A B C D E F G H\n";
        for (int i = 0; i < 8; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; j++) {
                char casilla = negocio.obtenerCasilla(i, j);
                if (casilla == ' ') cout << ". ";
                else cout << casilla << " ";
            }
            cout << i + 1 << "\n";
        }
        cout << "  A B C D E F G H\n\n";
    }

    void jugar() {
        int modo;
        cout << "Seleccione modo de juego:\n1. Humano vs Humano\n2. Humano vs Computadora\n3. Computadora vs Computadora\nOpcion: ";
        cin >> modo;

        bool juegoTerminado = false;

        while (!juegoTerminado) {
            mostrarTablero();
            char turnoActual = negocio.getTurno();
            
            cout << "Turno de: " << (turnoActual == 'N' ? "Negras (N)" : "Blancas (B)") << endl;

            if (!negocio.tieneMovimientosValidos(turnoActual)) {
                cout << "No hay movimientos validos. Se cede el turno.\n";
                negocio.cambiarTurno();
                if (!negocio.tieneMovimientosValidos(negocio.getTurno())) {
                    cout << "Ningun jugador tiene movimientos. Fin del juego.\n";
                    juegoTerminado = true;
                }
                continue;
            }

            bool esComputadora = (modo == 3) || (modo == 2 && turnoActual == 'B');

            if (esComputadora) {
                negocio.jugarComputadora();
            } else {
                string entrada;
                cout << "Ingrese movimiento (ej. D3) o 'G' para guardar: ";
                cin >> entrada;

                if (entrada == "G" || entrada == "g") {
                    negocio.guardarPartidaEnDatos();
                    continue; // Repite el turno
                }

                if (entrada.length() < 2) continue;

                int col = toupper(entrada[0]) - 'A';
                int fila = entrada[1] - '1';

                if (!negocio.aplicarMovimiento(fila, col, turnoActual)) {
                    cout << "Movimiento invalido. Intente de nuevo.\n";
                    continue; // Repite el turno sin cambiar de jugador
                }
            }

            negocio.cambiarTurno();
        }

        // Fin de partida
        mostrarTablero();
        int negras, blancas;
        negocio.contarFichas(negras, blancas);
        cout << "Resultado final -> Negras: " << negras << " | Blancas: " << blancas << endl;
        if (negras > blancas) cout << "Ganador: Negras!\n";
        else if (blancas > negras) cout << "Ganador: Blancas!\n";
        else cout << "Empate!\n";
    }
};

int main() {
    CapaPresentacion juego;
    juego.jugar();
    return 0;
}