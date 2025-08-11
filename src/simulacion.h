#ifndef SIMULACION_H
#define SIMULACION_H

#include <vector>
#include <string>
#include "raylib.h"
#include "tablero.h"

struct Jugador {
    Color color;
    int posicion;
    int propiedades;
    int casas;
    std::vector<int> propiedadesCompradas;
};

void iniciarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2);
void actualizarSimulacion(float deltaTime);
void dibujarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2);
bool simulacionTerminada();
void dibujarResumen();

#endif

