#include "simulacion.h"
#include <cstdlib>
#include <iostream>

static std::vector<Jugador> jugadores;
static int turnosTotales = 846;
static int turnoActual = 0;
static bool finalizada = false;
static float tiempoAcumulado = 0.0f;
static const float intervaloMovimiento = 0.1f;
static std::vector<std::vector<int>> visitas;
static const std::vector<Casilla>* refTablero1 = nullptr;
static const std::vector<Casilla>* refTablero2 = nullptr;
static std::vector<int> tableroJugador;

const Color COLOR_PURPURA = {128, 0, 128, 255};
static std::vector<int> casillasMultiplicador = {5, 13, 21, 30};
static float multiplicadorVisitas = 3.0f;

void iniciarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2) {
    refTablero1 = &t1;
    refTablero2 = &t2;

    jugadores.clear();
    jugadores.push_back({YELLOW, 0, 0, 0, {}});
    jugadores.push_back({PURPLE, 0, 0, 0, {}});

    visitas = std::vector<std::vector<int>>(2, std::vector<int>(t1.size(), 0));
    tableroJugador = std::vector<int>(2, 0);

    turnoActual = 0;
    tiempoAcumulado = 0.0f;
    finalizada = false;
}

void actualizarSimulacion(float deltaTime) {
    if (finalizada) return;

    tiempoAcumulado += deltaTime;
    if (tiempoAcumulado < intervaloMovimiento) return;
    tiempoAcumulado = 0.0f;

    int jugadorTurno = turnoActual % 2;
    int dado1 = (std::rand() % 6) + 1;
    int dado2 = (std::rand() % 6) + 1;
    int sumaDados = dado1 + dado2;
    bool pares = (dado1 == dado2);

    int tableroActual = tableroJugador[jugadorTurno];
    const std::vector<Casilla>* tableroActualRef = (tableroActual == 0) ? refTablero1 : refTablero2;

    jugadores[jugadorTurno].posicion = (jugadores[jugadorTurno].posicion + sumaDados) % tableroActualRef->size();

    std::string nombreCasilla = (*tableroActualRef)[jugadores[jugadorTurno].posicion].nombre;
    bool esFortuna = (nombreCasilla.find("Fortuna") != std::string::npos);

    if (esFortuna && pares) {
        tableroJugador[jugadorTurno] = 1 - tableroActual;
        jugadores[jugadorTurno].posicion %= ((tableroJugador[jugadorTurno] == 0) ? refTablero1->size() : refTablero2->size());
    }

    int visitaIndex = jugadores[jugadorTurno].posicion;
    float factor = 1.0f;
    if (tableroActual == 0) {
        for (int c : casillasMultiplicador) {
            if (visitaIndex == c) {
                factor = multiplicadorVisitas;
                break;
            }
        }
    }

    visitas[jugadorTurno][visitaIndex] += static_cast<int>(factor);

    std::cout << "Turno: " << turnoActual << " Jugador: " << jugadorTurno
              << " Tablero: " << tableroActual << " Posición: " << visitaIndex
              << " Dados: " << dado1 << "," << dado2
              << " Cambio tablero: " << (esFortuna && pares ? "SI" : "NO") << std::endl;

    turnoActual++;
    if (turnoActual >= turnosTotales) {
        finalizada = true;
        std::cout << "Simulación finalizada.\n";
    }
}

void dibujarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2) {
    for (int i = 0; i < (int)t1.size(); ++i) {
        int totalVisitas = visitas[0][i] + visitas[1][i];
        if (totalVisitas > 0) {
            Vector3 pos = t1[i].posicion;
            float alturaTorre = 0.1f + totalVisitas * 0.03f;
            DrawCube({pos.x, pos.y + alturaTorre / 2.0f, pos.z}, 1.5f, alturaTorre, 1.5f, DARKBLUE);
        }
    }

    for (int i = 0; i < (int)t2.size(); ++i) {
        int totalVisitas = visitas[0][i] + visitas[1][i];
        if (totalVisitas > 0) {
            Vector3 pos = t2[i].posicion;
            float alturaTorre = 0.1f + totalVisitas * 0.03f;
            DrawCube({pos.x, pos.y + alturaTorre / 2.0f, pos.z}, 1.5f, alturaTorre, 1.5f, DARKPURPLE);
        }
    }

    for (int j = 0; j < 2; j++) {
        const std::vector<Casilla>* tableroRef = (tableroJugador[j] == 0) ? &t1 : &t2;
        Vector3 pos = (*tableroRef)[jugadores[j].posicion].posicion;
        pos.y += (*tableroRef)[0].altura / 2.0f + 0.3f;
        DrawSphere(pos, 0.3f, jugadores[j].color);

        DrawText(TextFormat("Jugador %d Tablero %d Pos %d", j+1, tableroJugador[j], jugadores[j].posicion),
                 20, 20 + j*20, 20, jugadores[j].color);
    }
}

bool simulacionTerminada() {
    return finalizada;
}

void dibujarResumen() {
    DrawRectangle(100, 100, 400, 300, Fade(BLACK, 0.8f));
    DrawText("Resumen de la simulacion", 120, 120, 20, RAYWHITE);

    for (int i = 0; i < (int)jugadores.size(); ++i) {
        DrawText(TextFormat("Jugador %d: Propiedades %d Casas %d", i+1, jugadores[i].propiedades, jugadores[i].casas),
                 120, 160 + i * 40, 20, jugadores[i].color);
    }
}

