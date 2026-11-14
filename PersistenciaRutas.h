/*
funcion de Persistencia en Texto para la Red de Rutas Aereas.
Almacena y recupera las conexiones y distancias entre aeropuertos en un archivo de texto plano (.txt).
nota:
- Permite la lectura, edición e inspección externa de la red de conexiones sin herramientas binarias.

objetivo: Garantizar la persistencia de las aristas y ponderaciones de la red de rutas en un archivo de texto secuencial.

nombre: PersistenciaRutas.h

estructura especial:
    typedef char tlinea[MAX] ;
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
#define MAXL 50

typedef char tlinea[MAX] ;

/* Declaración de prototipos */
void cargarRutasTexto(tRedRutas *);
void guardarRutasTexto(tRedRutas);

/* Implementación de funciones */

// Carga las conexiones de la red aérea y nombres de aeropuertos desde rutas.txt
void cargarRutasTexto(tRedRutas *pRedRutas) {
    FILE *archivo = fopen("rutas.txt", "r");
    if (archivo == NULL) {
        return;
    }

    tlinea linea;
    tAristaRuta arco;
    sstring nomOrigen, nomDestino;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (linea[0] == '#' || linea[0] == '\n') {
            continue;
        }
        if (sscanf(linea, "%d;%49[^;];%d;%49[^;];%d;%d",
                   &arco.origen, nomOrigen,
                   &arco.destino, nomDestino,
                   &arco.peso.distanciaKm, &arco.peso.duracionMin) == 6) {
            if (!existeAeropuerto(*pRedRutas, arco.origen)) {
                agregarAeropuerto(pRedRutas, arco.origen, nomOrigen);
            }
            if (!existeAeropuerto(*pRedRutas, arco.destino)) {
                agregarAeropuerto(pRedRutas, arco.destino, nomDestino);
            }
            agregarRuta(pRedRutas, arco);
        }
    }
    fclose(archivo);
}

// Guarda las aristas activas de la red aérea en formato texto en rutas.txt
void guardarRutasTexto(tRedRutas pRedRutas) {
    FILE *archivo = fopen("rutas.txt", "w");
    if (archivo == NULL) {
        return;
    }

    fprintf(archivo, "# Red de Rutas Aereas - AeroGestion\n");
    fprintf(archivo, "# OrigenID;OrigenNombre;DestinoID;DestinoNombre;DistanciaKm;DuracionMin\n");

    int i, j;
    for (i = 0; i < N; i++) {
        for (j = i + 1; j < N; j++) {
            if (pRedRutas.pesos[i][j].distanciaKm > 0) {
                fprintf(archivo, "%d;%s;%d;%s;%d;%d\n",
                        i, pRedRutas.nombres[i],
                        j, pRedRutas.nombres[j],
                        pRedRutas.pesos[i][j].distanciaKm,
                        pRedRutas.pesos[i][j].duracionMin);
            }
        }
    }
    fclose(archivo);
}
