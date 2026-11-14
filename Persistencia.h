/*
Módulo de Persistencia y Reportes por Corte de Control.
Almacena y recupera el estado de vuelos y reservas en archivos binarios (.dat).
Genera reportes estadísticos agrupados por aerolínea aplicando la técnica de corte de control.
nota:
- Los datos se guardan ordenados por aerolínea para permitir el procesamiento secuencial en un solo paso.

objetivo: Garantizar la persistencia de datos en disco mediante archivos binarios y emitir informes con corte de control.

nombre: Persistencia.h

estructura especial:
FILE *archivoVuelos, *archivoReservas;
tVuelo regVuelo;
tReserva regReserva;

estrategia: 
    grabarColaBinario
    guardarDatosBinarios
    cargarDatosBinarios
    corteDeControlAerolineas

nombre de funciones:
    grabarColaBinario();
    guardarDatosBinarios();
    cargarDatosBinarios();
    corteDeControlAerolineas();
*/

#include "PersistenciaRutas.h"

#define TITULO_CORTE "\n*** CORTE DE CONTROL POR AEROLINEA ***\n"

/* Declaración de prototipos */
void grabarColaBinario(FILE *, tColaReservas);
void guardarDatosBinarios(tNodoVuelo *, tColaReservas, tColaReservas);
void cargarDatosBinarios(tNodoVuelo **, tColaReservas *, tColaReservas *, tRedRutas *);
void corteDeControlAerolineas();

/* Implementación de funciones */

// Graba los nodos de una cola de reservas en un archivo binario
void grabarColaBinario(FILE *pArchivo, tColaReservas pCola) {
    tNodoReserva *aux = pCola.principio;
    while (aux != NULL) {
        fwrite(&aux->datos, sizeof(tReserva), 1, pArchivo);
        aux = aux->siguiente;
    }
}

// Guarda el estado del sistema (vuelos y reservas) en archivos binarios
void guardarDatosBinarios(tNodoVuelo *pListaVuelos, tColaReservas pConfirmadas, tColaReservas pEspera) {
    FILE *archivoVuelos = fopen("vuelos.dat", "wb");
    if (archivoVuelos == NULL) {
        return;
    }

    ordenarVuelos(pListaVuelos, 2);

    tNodoVuelo *auxV = pListaVuelos;
    while (auxV != NULL) {
        fwrite(&auxV->datos, sizeof(tVuelo), 1, archivoVuelos);
        auxV = auxV->siguiente;
    }
    fclose(archivoVuelos);

    FILE *archivoReservas = fopen("reservas.dat", "wb");
    if (archivoReservas == NULL) {
        return;
    }
    grabarColaBinario(archivoReservas, pConfirmadas);
    grabarColaBinario(archivoReservas, pEspera);
    fclose(archivoReservas);
}

// Carga los vuelos y reservas desde los archivos binarios
void cargarDatosBinarios(tNodoVuelo **pListaVuelos, tColaReservas *pConfirmadas, tColaReservas *pEspera, tRedRutas *pRedRutas) {
    FILE *archivoVuelos = fopen("vuelos.dat", "rb");
    if (archivoVuelos == NULL) {
        return;
    }

    liberarListaVuelos(pListaVuelos);
    liberarCola(pConfirmadas);
    liberarCola(pEspera);

    tVuelo regVuelo;
    fread(&regVuelo, sizeof(tVuelo), 1, archivoVuelos);
    while (!feof(archivoVuelos)) {
        insertarVuelo(pListaVuelos, regVuelo);
        agregarAeropuerto(pRedRutas, regVuelo.idDestino, regVuelo.destino);
        fread(&regVuelo, sizeof(tVuelo), 1, archivoVuelos);
    }
    fclose(archivoVuelos);

    FILE *archivoReservas = fopen("reservas.dat", "rb");
    if (archivoReservas != NULL) {
        tReserva regReserva;
        fread(&regReserva, sizeof(tReserva), 1, archivoReservas);
        while (!feof(archivoReservas)) {
            encolarReserva(regReserva.confirmada ? pConfirmadas : pEspera, regReserva);
            fread(&regReserva, sizeof(tReserva), 1, archivoReservas);
        }
        fclose(archivoReservas);
    }
}

// Emite informe agrupado por aerolínea aplicando corte de control
void corteDeControlAerolineas() {
    FILE *archivo = fopen("vuelos.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir vuelos.dat\n");
        return;
    }

    tVuelo reg;
    sstring aerolineaAnt;
    int totalAerolineas = 0, totalPasajeros = 0;

    printf(TITULO_CORTE);
    fread(&reg, sizeof(tVuelo), 1, archivo);

    while (!feof(archivo)) {
        strcpy(aerolineaAnt, reg.aerolinea);
        int cantVuelos = 0, pasajerosAerolinea = 0;

        while (!feof(archivo) && strcmp(reg.aerolinea, aerolineaAnt) == 0) {
            cantVuelos++;
            pasajerosAerolinea += reg.ocupados;
            fread(&reg, sizeof(tVuelo), 1, archivo);
        }

        printf("Aerolinea: %-s | Vuelos: %d | Pasajeros: %d\n",
               aerolineaAnt, cantVuelos, pasajerosAerolinea);
        totalAerolineas++;
        totalPasajeros += pasajerosAerolinea;
    }

    printf("Total de aerolineas: %d | Total pasajeros: %d\n", totalAerolineas, totalPasajeros);
    fclose(archivo);
}
