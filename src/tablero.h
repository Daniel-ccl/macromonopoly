#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include <string>
#include "raylib.h"

extern Color coloresLado[4];

struct Casilla {
    Vector3 posicion;
    Color color;
    float altura;
    int visitas;
    std::string nombre;
};

std::vector<Casilla> generarTablero(Vector3 offset, const std::vector<std::string>& nombres);

void dibujarTablero(const std::vector<Casilla>& tablero);

#endif // TABLERO_H

