
import numpy as np

N_TABLEROS = 2
N_CASILLAS = 24
N_DOBLES = 3  
N_ESTADOS = N_TABLEROS * N_CASILLAS * N_DOBLES
TOLERANCIA = 1e-6  

CARCEL_T1, CARCEL_T2 = 6, 8
FORTUNA_T1 = [15, 21]
FORTUNA_T2 = [17, 23]  

def generar_matriz():
    P = np.zeros((N_ESTADOS, N_ESTADOS), dtype=np.float64)
    
    for estado_idx in range(N_ESTADOS):
        if estado_idx % 10 == 0:
            print(f"Generando estado {estado_idx}/{N_ESTADOS}...")
        
        tablero = estado_idx // (N_CASILLAS * N_DOBLES)
        casilla = (estado_idx % (N_CASILLAS * N_DOBLES)) // N_DOBLES
        dobles = estado_idx % N_DOBLES
        
        for d1 in range(1, 7):
            for d2 in range(1, 7):
                prob = 1.0 / 36.0
                es_par = (d1 == d2)
                suma = d1 + d2
                
                nuevo_tablero = tablero
                nueva_casilla = (casilla + suma) % N_CASILLAS
                nuevos_dobles = dobles + 1 if es_par else 0
                
                if es_par and dobles == 2:
                    nueva_casilla = CARCEL_T1 if tablero == 0 else CARCEL_T2
                    nuevos_dobles = 0
                
                if (nuevo_tablero == 0 and nueva_casilla in FORTUNA_T1) or \
                   (nuevo_tablero == 1 and nueva_casilla in FORTUNA_T2):
                    if es_par:
                        nuevo_tablero = 1 - tablero
                        nueva_casilla = {15:23, 21:17, 17:21, 23:15}[nueva_casilla]
                
                nuevo_estado_idx = (nuevo_tablero * N_CASILLAS * N_DOBLES) + \
                                   (nueva_casilla * N_DOBLES) + \
                                   nuevos_dobles
                P[estado_idx, nuevo_estado_idx] += prob
    
    return P

def normalizar_matriz(P):
    for i in range(P.shape[0]):
        suma_fila = np.sum(P[i])
        if not np.isclose(suma_fila, 1.0, atol=TOLERANCIA):
            P[i] /= suma_fila
    return P

def validar_matriz(P):
    for i in range(P.shape[0]):
        suma = np.sum(P[i])
        if not np.isclose(suma, 1.0, atol=TOLERANCIA):
            raise ValueError(f"Fila {i} suma {suma:.10f} (debería ser 1.0)")
    print("✓ Todas las filas suman ≈1.0 dentro de la tolerancia.")

def export_to_header(P, filename="matriz.h"):
    print(f"Escribiendo matriz en {filename}...")  # Debug
    with open(filename, 'w') as f:
        f.write("#pragma once\n\n#include <array>\n\n")
        f.write("constexpr std::array<std::array<float, 144>, 144> MATRIZ = {{\n")
        
        for i in range(144):
            line = "    {{" + ", ".join(f"{x:.8f}f" for x in P[i]) + "}},\n"
            f.write(line)
            if i % 20 == 0:  # Flush 
                f.flush()
                print(f"Progreso: {i+1}/144 filas")  # Debug
        
        f.write("}};\n")
    print(f"✓ {filename} escrito completamente")  # Debug


if __name__ == "__main__":
    print("Generando matriz de transición...")
    P = generar_matriz()
    print("Normalizando matriz...")
    P = normalizar_matriz(P)
    print("Validando matriz...")
    validar_matriz(P)
    
    np.save("matriz_transicion.npy", P)
    export_to_header(P)
    print("✓ Matriz guardada en 'matriz_transicion.npy' y 'matriz.h'")





