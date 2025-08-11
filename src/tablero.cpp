#include "tablero.h"

Color coloresLado[4] = {
    {255, 50, 50, 255},    // lado 0
    {57, 255, 20, 255},    // lado 1
    {0, 191, 255, 255},    // lado 2
    {255, 0, 255, 255}     // lado 3
};

std::vector<Casilla> generarTablero(Vector3 offset, const std::vector<std::string>& nombres) {
    std::vector<Casilla> tablero;
    const int numCasillasPorLado = 6;           
    const int numCasillasTotales = numCasillasPorLado * 4;
    const float tamCasilla = 2.0f;
    const float alturaBase = 0.2f;

    tablero.reserve(numCasillasTotales);

    for (int i = 0; i < numCasillasPorLado; ++i) {
        Vector3 pos = { offset.x + i * tamCasilla, offset.y, offset.z + 0 * tamCasilla };
        Color col = (i == 0 || i == numCasillasPorLado - 1) ? WHITE : coloresLado[0];
        tablero.push_back({ pos, col, alturaBase, 0, nombres[(int)tablero.size()] });
    }

    for (int i = 0; i < numCasillasPorLado; ++i) {
        Vector3 pos = { offset.x + (numCasillasPorLado - 1) * tamCasilla, offset.y, offset.z + i * tamCasilla };
        Color col = (i == numCasillasPorLado - 1) ? WHITE : coloresLado[1];
        tablero.push_back({ pos, col, alturaBase, 0, nombres[(int)tablero.size()] });
    }

    for (int i = 0; i < numCasillasPorLado; ++i) {
        Vector3 pos = { offset.x + (numCasillasPorLado - 1 - i) * tamCasilla, offset.y, offset.z + (numCasillasPorLado - 1) * tamCasilla };
        Color col = (i == numCasillasPorLado - 1) ? WHITE : coloresLado[2];
        tablero.push_back({ pos, col, alturaBase, 0, nombres[(int)tablero.size()] });
    }

    for (int i = 0; i < numCasillasPorLado; ++i) {
        Vector3 pos = { offset.x + 0 * tamCasilla, offset.y, offset.z + (numCasillasPorLado - 1 - i) * tamCasilla };
        Color col = (i == 0) ? WHITE : coloresLado[3];
        tablero.push_back({ pos, col, alturaBase, 0, nombres[(int)tablero.size()] });
    }

    return tablero;
}

void dibujarTablero(const std::vector<Casilla>& tablero) {
    const float tamCasilla = 2.0f;
    for (const auto &c : tablero) {
        DrawCube(c.posicion, tamCasilla * 0.95f, c.altura, tamCasilla * 0.95f, c.color);
        DrawCubeWires(c.posicion, tamCasilla * 0.95f, c.altura, tamCasilla * 0.95f, RAYWHITE);
    }
}

