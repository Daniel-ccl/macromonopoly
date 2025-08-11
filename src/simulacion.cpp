#include "simulacion.h"
#include <cstdlib>
#include <iostream>  // para debug

static std::vector<Jugador> jugadores;
static int turnosTotales = 100;
static int turnoActual = 0;
static bool finalizada = false;
static float tiempoAcumulado = 0.0f;
static const float intervaloMovimiento = 0.1f;
static std::vector<std::vector<int>> visitas;
static const std::vector<Casilla>* refTablero1 = nullptr;
static const std::vector<Casilla>* refTablero2 = nullptr;

void iniciarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2) {
    refTablero1 = &t1;
    refTablero2 = &t2;
    jugadores.clear();
    jugadores.push_back({ YELLOW, 0, 0, 0, {} });
    jugadores.push_back({ PURPLE, 0, 0, 0, {} });

    visitas = std::vector<std::vector<int>>(2, std::vector<int>(t1.size(), 0));

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
    int dado = (std::rand() % 6) + 1;

    // Avanza posición en tablero 1 (puedes extender para usar tablero2 si quieres)
    jugadores[jugadorTurno].posicion = (jugadores[jugadorTurno].posicion + dado) % refTablero1->size();

    visitas[jugadorTurno][jugadores[jugadorTurno].posicion]++;

    std::cout << "Turno: " << turnoActual << " Jugador: " << jugadorTurno 
              << " Posición: " << jugadores[jugadorTurno].posicion << "\n";

    if (std::rand() % 5 == 0) {
        jugadores[jugadorTurno].propiedades++;
        jugadores[jugadorTurno].propiedadesCompradas.push_back(jugadores[jugadorTurno].posicion);
    }

    if (std::rand() % 10 == 0) {
        jugadores[jugadorTurno].casas++;
    }

    turnoActual++;
    if (turnoActual >= turnosTotales) {
        finalizada = true;
        std::cout << "Simulación finalizada.\n";
    }
}

void dibujarSimulacion(const std::vector<Casilla>& t1, const std::vector<Casilla>& t2) {
    // Dibuja torres en tablero 1
    for (int i = 0; i < (int)t1.size(); ++i) {
        int totalVisitas = visitas[0][i] + visitas[1][i];
        if (totalVisitas > 0) {
            Vector3 pos = t1[i].posicion;
            float alturaTorre = 0.1f + totalVisitas * 0.03f;
            DrawCube({ pos.x, pos.y + alturaTorre / 2.0f, pos.z }, 1.5f, alturaTorre, 1.5f, DARKBLUE);
        }
    }

    // Dibuja jugadores en tablero 1
    for (int j = 0; j < 2; j++) {
        Vector3 pos = t1[jugadores[j].posicion].posicion;
        pos.y += t1[0].altura / 2.0f + 0.3f;
        DrawSphere(pos, 0.3f, jugadores[j].color);

        // Texto con posición para debug
        DrawText(TextFormat("Jugador %d Pos %d", j+1, jugadores[j].posicion), 20, 20 + j*20, 20, jugadores[j].color);
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

