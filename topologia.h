/*
Módulo de Topología y Red de Rutas Aéreas.
Modela la interconexión entre aeropuertos comerciales mediante una matriz de adyacencia y pesos bidimensional.
nota:
- Los vértices representan aeropuertos y las aristas almacenan distancias en kilómetros y tiempos de vuelo en minutos.

objetivo: Gestionar la red de destinos y rutas aéreas utilizando una red de rutas ponderada bidireccional.

nombre: topologia.h

estructura especial:

typedef int tVertice;

typedef struct {
    int distanciaKm;
    int duracionMin;
} tPesoRuta;

typedef struct {
    tVertice origen;
    tVertice destino;
    tPesoRuta peso;
} tAristaRuta;

typedef bool tConjuntoVertices[R];
typedef tPesoRuta tMatrizPesos[R][R];

typedef struct {
    tConjuntoVertices vertices;
    sstring nombres[R];
    tMatrizPesos pesos;
} tRedRutas;

estrategia: 

    cargarRutaManual
    menuDeRutas
    inicializarRedRutas
    existeAeropuerto
    existeRuta
    agregarAeropuerto
    agregarRuta
    maximoAeropuerto
    mostrarMatrizAdyacencia
    mostrarMatrizPesos
    mostrarConexiones


nombre de funciones:
    cargarRutaManual();
    menuDeRutas();
    inicializarRedRutas();
    existeAeropuerto();
    existeRuta();
    agregarAeropuerto();
    agregarRuta();
    maximoAeropuerto();
    mostrarMatrizAdyacencia();
    mostrarMatrizPesos();
    mostrarConexiones();

*/

#include "Gestion.h"

/*matriz fija de R=15 aeropuertos como en pasar a lista si supera 100 */
#define R 15
#define TITULO_ADYACENCIA "\n*** MATRIZ DE ADYACENCIA ***\n"
#define TITULO_PESOS "\n*** MATRIZ DE PESOS (Distancia km, Duracion min) ***\n"

/* Declaración de tipos de datos personalizados */

typedef int tVertice;

typedef struct {
    int distanciaKm;
    int duracionMin;
} tPesoRuta;

typedef struct {
    tVertice origen;
    tVertice destino;
    tPesoRuta peso;
} tAristaRuta;

typedef bool tConjuntoVertices[R];
typedef tPesoRuta tMatrizPesos[R][R];

typedef struct {
    tConjuntoVertices vertices;
    sstring nombres[R];
    tMatrizPesos pesos;
} tRedRutas;

/* Declaración de prototipos */
void cargarRutaManual(tRedRutas *);
void menuDeRutas(tRedRutas *);
void inicializarRedRutas(tRedRutas *);
bool existeAeropuerto(tRedRutas, tVertice);
bool existeRuta(tRedRutas, tVertice, tVertice);
bool agregarAeropuerto(tRedRutas *, tVertice, sstring);
bool agregarRuta(tRedRutas *, tAristaRuta);
int maximoAeropuerto(tRedRutas);
void mostrarMatrizAdyacencia(tRedRutas);
void mostrarMatrizPesos(tRedRutas);
void mostrarConexiones(tRedRutas);

/* Implementación de funciones */
// Submenu interactivo para operar con la red de rutas y destinos
void menuDeRutas(tRedRutas *pRedRutas) {
    int subopcion;
    printf("\n1. Matriz de Pesos\n");
    printf("2. Matriz de Adyacencia\n");
    printf("3. Consultar conexiones\n");
    printf("4. Agregar ruta\n");
    printf("Opcion: ");
    fflush(stdin);
    scanf("%d", &subopcion);

    switch (subopcion) {
        case 1: 
            mostrarMatrizPesos(*pRedRutas); 
            break;
        case 2: 
            mostrarMatrizAdyacencia(*pRedRutas); 
            break;
        case 3: 
            mostrarConexiones(*pRedRutas); 
            break;
        case 4: 
            cargarRutaManual(pRedRutas); 
            break;
        default: printf("Opcion invalida.\n");
    }
}
// Inicializa la matriz de aristas/pesos en cero y desactiva todos los aeropuertos
void inicializarRedRutas(tRedRutas *pRedRutas) {
    int x, y;
    for (x = 0; x < R; x++) {
        pRedRutas->vertices[x] = false;
        strcpy(pRedRutas->nombres[x], "");
        for (y = 0; y < R; y++) {
            pRedRutas->pesos[x][y].distanciaKm = 0;
            pRedRutas->pesos[x][y].duracionMin = 0;
        }
    }
}

// Comprueba si un vertice esta dentro del rango permitido y activo en la red de rutas
bool existeAeropuerto(tRedRutas pRedRutas, tVertice pVertice) {
    return (pVertice >= 0 && pVertice < R && pRedRutas.vertices[pVertice]);
}

// Determina si existe una conexion directa con peso positivo entre dos aeropuertos
bool existeRuta(tRedRutas pRedRutas, tVertice pOrigen, tVertice pDestino) {
    return (existeAeropuerto(pRedRutas, pOrigen) &&
            existeAeropuerto(pRedRutas, pDestino) &&
            pRedRutas.pesos[pOrigen][pDestino].distanciaKm > 0);
}
// Permite ingresar manualmente una nueva conexion entre aeropuertos
void cargarRutaManual(tRedRutas *pRedRutas) {
    tAristaRuta arco;
    sstring nombre;

    printf("ID Aeropuerto origen (0 a 14): ");
    fflush(stdin);
    scanf("%d", &arco.origen);
    if (!existeAeropuerto(*pRedRutas, arco.origen)) {
        printf("Nombre del aeropuerto origen: ");
        fflush(stdin);
        scanf(" %[^\n]", nombre);
        agregarAeropuerto(pRedRutas, arco.origen, nombre);
    }

    printf("ID Aeropuerto destino (0 a 14): ");
    fflush(stdin);
    scanf("%d", &arco.destino);
    if (!existeAeropuerto(*pRedRutas, arco.destino)) {
        printf("Nombre del aeropuerto destino: ");
        fflush(stdin);
        scanf(" %[^\n]", nombre);
        agregarAeropuerto(pRedRutas, arco.destino, nombre);
    }

    printf("Distancia km: ");
    fflush(stdin);
    scanf("%d", &arco.peso.distanciaKm);
    printf("Duracion min: ");
    fflush(stdin);
    scanf("%d", &arco.peso.duracionMin);

    if (agregarRuta(pRedRutas, arco)) {
        printf("Ruta registrada correctamente.\n");
    } else {
        printf("Error al registrar la ruta.\n");
    }
}

// Registra un vertice activo asignandole el nombre del aeropuerto
bool agregarAeropuerto(tRedRutas *pRedRutas, tVertice pVertice, sstring pNombre) {
    if (pVertice < 0 || pVertice >= R) {
        return false;
    }
    pRedRutas->vertices[pVertice] = true;
    strcpy(pRedRutas->nombres[pVertice], pNombre);
    return true;
}

// Inserta una arista bidireccional con distancia y duracion entre dos vertices
bool agregarRuta(tRedRutas *pRedRutas, tAristaRuta pArista) {
    if (!existeAeropuerto(*pRedRutas, pArista.origen) || !existeAeropuerto(*pRedRutas, pArista.destino)) {
        return false;
    }
    pRedRutas->pesos[pArista.origen][pArista.destino] = pArista.peso;
    pRedRutas->pesos[pArista.destino][pArista.origen] = pArista.peso;
    return true;
}

// Retorna el indice del vertice activo con mayor numeracion en la red de rutas
int maximoAeropuerto(tRedRutas pRedRutas) {
    int x, max = -1;
    for (x = 0; x < R; x++) {
        if (pRedRutas.vertices[x]) {
            max = x;
        }
    }
    return max;
}

// Muestra la matriz de adyacencia (1 si hay ruta directa, 0 si no existe)
void mostrarMatrizAdyacencia(tRedRutas pRedRutas) {
    int x, y;
    int maxV = maximoAeropuerto(pRedRutas);
    if (maxV == -1) {
        printf("La red de rutas esta vacia.\n");
        return;
    }
    printf("%s\t", TITULO_ADYACENCIA);
    for (y = 0; y <= maxV; y++) {
        printf("%d\t", y);
    }
    printf("\n");
    for (x = 0; x <= maxV; x++) {
        printf("%d\t", x);
        for (y = 0; y <= maxV; y++) {
            printf("%d\t", existeRuta(pRedRutas, x, y) ? 1 : 0);
        }
        printf("(%s)\n", pRedRutas.nombres[x]);
    }
}

// Imprime la matriz con distancias en km y duracion estimada entre aeropuertos
void mostrarMatrizPesos(tRedRutas pRedRutas) {
    int x, y;
    int maxV = maximoAeropuerto(pRedRutas);
    if (maxV == -1) {
        printf("La red de rutas esta vacia.\n");
        return;
    }
    printf("%s      ", TITULO_PESOS);
    for (y = 0; y <= maxV; y++) {
        printf("    [%2d]       ", y);
    }
    printf("\n");
    for (x = 0; x <= maxV; x++) {
        printf("[%2d]  ", x);
        for (y = 0; y <= maxV; y++) {
            if (existeRuta(pRedRutas, x, y)) {
                printf("(%4dkm,%3dm)  ", pRedRutas.pesos[x][y].distanciaKm, pRedRutas.pesos[x][y].duracionMin);
            } else {
                printf("  (  - ,  - )  ");
            }
        }
        printf("%s\n", pRedRutas.nombres[x]);
    }
}

// Lista los vuelos directos que parten desde un aeropuerto origen especifico
void mostrarConexiones(tRedRutas pRedRutas) {
    int maxV = maximoAeropuerto(pRedRutas);
    int origen, d;
    printf("Aeropuerto origen (0 a 14): ");
    fflush(stdin);
    scanf("%d", &origen);

    if (!existeAeropuerto(pRedRutas, origen)) {
        printf("No existe el aeropuerto %d.\n", origen);
        return;
    }

    printf("\n*** Conexiones directas desde [%d] %s ***\n", origen, pRedRutas.nombres[origen]);
    for (d = 0; d <= maxV; d++) {
        if (existeRuta(pRedRutas, origen, d)) {
            printf(" -> [%d] %s | Distancia: %d km | Duracion: %d min\n",
                   d, pRedRutas.nombres[d], pRedRutas.pesos[origen][d].distanciaKm, pRedRutas.pesos[origen][d].duracionMin);
        }
    }
}

