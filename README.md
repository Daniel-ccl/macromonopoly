
Este proyecto contiene la simulación de Monopoly usando cadenas de Markov.  
Se incluyen todos los archivos de C++ para compilar el proyecto, el código en Python para generar la matriz de transición 
y documentación matemática

## Contenido

- `src/` - Código fuente en C++ (`.cpp` y `.h`)
- `python/` - Código Python para generar la matriz de Markov
- `doc/` - Documentación y notas
  - `Abstract.pdf` - Explicación 
  - `Raylib.txt` - Cómo instalar Raylib en Windows
- `logo.ico` - Icono del proyecto
- `recursos.rc` - Archivo de recursos para el icono
- `build/` - Carpeta vacía para compilar el ejecutable

## Requisitos para Windows

Para compilar y ejecutar el proyecto en Windows es necesario tener Raylib. Hay dos alternativas:

1. **Usando Visual Studio**  
   - Descargar la versión Windows de Raylib desde la página oficial: [https://www.raylib.com/](https://www.raylib.com/)  
   - Extraer el contenido del ZIP y configurar Visual Studio para incluir los headers y librerías.

2. **Usando MinGW**  
   - Instalar MinGW y `x86_64-w64-mingw32-g++`  
   - Compilar con las instrucciones de la sección `Compilación` en `Instrucciones_Raylib.txt`

## Compilación

Para MinGW, los pasos típicos serían:

```bash
x86_64-w64-mingw32-g++ -c src/main.cpp -o build/main.o -I./src
x86_64-w64-mingw32-g++ -c src/simulacion.cpp -o build/simulacion.o -I./src
x86_64-w64-mingw32-g++ -c src/tablero.cpp -o build/tablero.o -I./src
x86_64-w64-mingw32-windres recursos.rc -o build/recursos.o
x86_64-w64-mingw32-g++ build/main.o build/simulacion.o build/tablero.o build/recursos.o -o build/Monopoly.exe -L./ -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

YEP YEP