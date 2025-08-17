#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>
#include "tablero.h"
#include "simulacion.h"
#include "matriz.h" 

const int screenWidth = 1000;
const int screenHeight = 700;

int main() {
    InitWindow(screenWidth, screenHeight, "MACROMONOPOLY");
    SetTargetFPS(60);

    Camera3D camara = {0};
    camara.position = {2.0f, 18.0f, 22.0f};
    camara.target   = {2.0f, 0.0f, 7.0f};
    camara.up       = {0.0f, 1.0f, 0.0f};
    camara.fovy     = 45.0f;
    camara.projection = CAMERA_PERSPECTIVE;

    std::vector<std::string> nombres1 = {
        "Salida","Baltic Av","Vacia","Ferrocarril","Vacia","Medit Av",
        "Carcel","Verm Av","Vacia","100 Pesos","Vacia","Conn Av",
        "Visita Carcel","Chari Av","Vacia","Fortuna 1","Vacia","States Av",
        "Parada Libre","James Av","Vacia","Fortuna 2","Vacia","Tenn Av"
    };

    std::vector<std::string> nombres2 = {
        "Visita Carcel","Park Place","Vacia","Fortuna 2","Vacia","Boardwalk",
        "Parada Libre","Kent Av","Vacia","Fortuna 1","Vacia","Indian Av",
        "Parada Libre","Atlan Av","Vacia","Tren Ligero","Vacia","Vent Av",
        "Carcel","Pacif Av","Vacia","Luz","Vacia","North Av"
    };

    Vector3 origen1 = {0.0f, 0.0f, 0.0f};
    Vector3 origen2 = {-6.0f, 0.0f, 4.0f};

    auto tablero1 = generarTablero(origen1, nombres1);
    auto tablero2 = generarTablero(origen2, nombres2);

    const int fortuna1_t1 = 15;
    const int fortuna2_t1 = 21;
    const int fortuna1_t2 = 9;
    const int fortuna2_t2 = 3;

    bool simulacionIniciada = false;
    Rectangle botonSimulacion = {screenWidth/2 - 150, screenHeight - 70, 300, 50};

    std::vector<std::string> creadores = {"Ángel", "Axel", "Esteban", "Julio", "Daniel"};

    static bool debugClick = false;

    while (!WindowShouldClose()) {
        // Cámara
        if (!simulacionIniciada) {
            static float angulo = 0.0f;
            angulo += 0.01f;
            camara.position.x = 2.0f + 20.0f * cos(angulo);
            camara.position.z = 7.0f + 20.0f * sin(angulo);
            camara.position.y = 18.0f;
            camara.target = {2.0f, 0.0f, 7.0f};
        } else {
            UpdateCamera(&camara, CAMERA_ORBITAL);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camara);

        float ladoTablero = 2.0f * (6 - 1);
        DrawCube({(origen1.x + origen2.x + ladoTablero)/2.0f, -0.2f, (origen1.z + origen2.z + ladoTablero)/2.0f},
                 (ladoTablero + 20.0f), 0.4f, (ladoTablero + 20.0f), DARKGRAY);

        dibujarTablero(tablero1);
        dibujarTablero(tablero2);

        if (simulacionIniciada) {
            dibujarSimulacion(tablero1, tablero2);
        }

        {
            Vector3 p1 = tablero1[fortuna1_t1].posicion;
            Vector3 p2 = tablero2[fortuna1_t2].posicion;
            DrawSphere({p1.x, p1.y + tablero1[fortuna1_t1].altura / 2.0f + 0.15f, p1.z}, 0.18f, BLACK);
            DrawSphere({p2.x, p2.y + tablero2[fortuna1_t2].altura / 2.0f + 0.15f, p2.z}, 0.18f, BLACK);
        }
        {
            Vector3 p1 = tablero1[fortuna2_t1].posicion;
            Vector3 p2 = tablero2[fortuna2_t2].posicion;
            DrawSphere({p1.x, p1.y + tablero1[fortuna2_t1].altura / 2.0f + 0.15f, p1.z}, 0.18f, BLACK);
            DrawSphere({p2.x, p2.y + tablero2[fortuna2_t2].altura / 2.0f + 0.15f, p2.z}, 0.18f, BLACK);
        }

        EndMode3D();

        DrawText("VERSION 0.12.04", 10, 10, 20, RAYWHITE);

        if (!simulacionIniciada) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

            const char* titulo = "MACROMONOPOLY";
            int anchoTitulo = MeasureText(titulo, 60);
            DrawText(titulo, screenWidth/2 - anchoTitulo/2, 50, 60, RAYWHITE);

            int yBase = 150;
            for (int i = 0; i < (int)creadores.size(); i++) {
                const char* nombre = creadores[i].c_str();
                int anchoNombre = MeasureText(nombre, 30);
                DrawText(nombre, screenWidth/2 - anchoNombre/2, yBase + i*40, 30, RAYWHITE);
            }

            DrawRectangleRec(botonSimulacion, RED);
            DrawRectangleLinesEx(botonSimulacion, 2, WHITE);

            const char* textoBoton = "CADENAS DE MARKOV";
            int anchoBoton = MeasureText(textoBoton, 20);
            DrawText(textoBoton, botonSimulacion.x + (botonSimulacion.width - anchoBoton)/2,
                     botonSimulacion.y + 12, 20, WHITE);

            Vector2 mousePos = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, botonSimulacion)) {
                simulacionIniciada = true;
                iniciarSimulacion(tablero1, tablero2);
                debugClick = true;
            }

            if (debugClick) {
                DrawText("CLICK DETECTADO!", 10, 60, 20, YELLOW);
            }
        }

        if (simulacionIniciada) {
            float deltaTime = GetFrameTime();
            actualizarSimulacion(deltaTime);

            DrawText("SIMULACION ACTIVA", 10, 40, 20, GREEN);

            if (simulacionTerminada()) {
                dibujarResumen();
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

