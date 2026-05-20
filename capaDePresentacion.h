#ifndef CAPA_DE_PRESENTACION_H
#define CAPA_DE_PRESENTACION_H

#include "capaDeNegocio.h"
#include "capaDeDatos.h"

class capaDePresentacion {
private:
    capaDeNegocio negocio;
    capaDeDatos datos;

    void mostrarTablero();
    void turnoHumano(int jugador);

public:
    void iniciarJuego();
};

#endif