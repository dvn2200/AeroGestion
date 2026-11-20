/*
funcion de Persistencia en Texto para la Red de Rutas Aereas.
Almacena y recupera las conexiones y distancias entre aeropuertos en un archivo de texto plano (.txt).
nota:
- Permite la lectura, edición e inspección externa de la red de conexiones sin herramientas binarias.

objetivo: Garantizar la persistencia de las aristas y ponderaciones de la red de rutas en un archivo de texto secuencial.

nombre: PersistenciaRutas.h

estructura especial:
    FILE *archivoRutas;
    tAristaRuta regArista;

estrategia: 
    cargarRutasTexto
    guardarRutasTexto

nombre de funciones:
    cargarRutasTexto();
    guardarRutasTexto();
*/

#include "Negocioyop.h"

/* Declaración de prototipos */
void cargarRutasTexto(tRedRutas *);
void guardarRutasTexto(tRedRutas);

/* Implementación de funciones */

// Carga las conexiones de la red aérea y nombres de aeropuertos desde rutas.txt
void cargarRutasTexto(tRedRutas *pRedRutas) {
    tAristaRuta arco;
    sstring nomOrigen, nomDestino;
    
    FILE *archivo = fopen("rutas.txt", "r");
    if (archivo == NULL) {
        printf("\nerror al abrir el archivo\n");
        return;
    }

    while (fscanf(archivo, "%d;%[^;];%d;%[^;];%d;%d\n",
                  &arco.origen, nomOrigen,
                  &arco.destino, nomDestino,
                  &arco.peso.distanciaKm, &arco.peso.duracionMin) == 6) {
        agregarAeropuerto(pRedRutas, arco.origen, nomOrigen);
        agregarAeropuerto(pRedRutas, arco.destino, nomDestino);
        agregarRuta(pRedRutas, arco);
    }
    fclose(archivo);
}

// Guarda las aristas activas de la red aérea en formato texto en rutas.txt
void guardarRutasTexto(tRedRutas pRedRutas) {
    int x, y;
    
    FILE *archivo = fopen("rutas.txt", "w");
    if (archivo == NULL) {
        printf("\nerror al abrir el archivo\n");
        return;
    }

    for (x = 0; x < R; x++) {
        for (y = x + 1; y < R; y++) {
            if (pRedRutas.pesos[x][y].distanciaKm > 0) {
                fprintf(archivo, "%d;%s;%d;%s;%d;%d\n",
                        x, pRedRutas.nombres[x],
                        y, pRedRutas.nombres[y],
                        pRedRutas.pesos[x][y].distanciaKm,
                        pRedRutas.pesos[x][y].duracionMin);
            }
        }
    }
    fclose(archivo);
}
