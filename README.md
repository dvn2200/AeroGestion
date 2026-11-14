# ✈️ AeroGestión — Sistema de Gestión de Vuelos y Pasajeros

[![Asignatura](https://img.shields.io/badge/Asignatura-AED2-blue.svg)](#)
[![Metodología](https://img.shields.io/badge/Metodología-Programacion%20por%20Pares-orange.svg)](#)
[![Estado](https://img.shields.io/badge/Estado-En%20Desarrollo-green.svg)](#)

> **AeroGestión** es un sistema desarrollado para **Algoritmos y Estructuras de Datos II (AED2)**. Su propósito es simular y optimizar la administración de vuelos, aerolíneas, pasajeros y rutas comerciales en un aeropuerto, aplicando estructuras de datos avanzadas, técnicas algorítmicas y persistencia de datos.

---

## Tabla de Contenidos

1. [Introducción](#-introducción)
2. [Características Principales](#-características-principales)
3. [Estructuras de Datos y Técnicas](#-estructuras-de-datos-y-técnicas)
4. [Arquitectura y Diseño](#-arquitectura-y-diseño)
5. [Metodología de Trabajo](#-metodología-de-trabajo)
6. [Instrucciones Básicas](#-instrucciones-básicas)

---

## Introducción

El proyecto surge de la descomposición modular de la operativa diaria de un aeropuerto comercial. El objetivo es resolver la logística de asignación de vuelos, el control estricto del orden de llegada en la reserva de pasajes, el cálculo y consulta de rutas aéreas entre destinos, y el almacenamiento seguro de la información mediante archivos.

---

## Características Principales

 **Registro de Aerolíneas y Vuelos:** Gestión de código de vuelo, aerolínea operadora, destino, duración estimada y cupo máximo de pasajeros.
 **Gestión de Reservas y Cancelaciones:** Sistema de reservas en orden de llegada con control de aforo máximo y liberación automática de cupos ante cancelaciones.
 **Red de Conexiones Aéreas:** Modelado de conexiones entre aeropuertos con distancias en kilómetros y tiempos de vuelo.
 **Búsqueda y Ordenamiento Eficiente:** Búsqueda rápida de vuelos por código y ordenamiento de itinerarios.
 **Persistencia de Datos:** Almacenamiento y recuperación en archivos binarios y de texto para conservar el estado del sistema.

---

## Estructuras de Datos y Técnicas

| Componente / Estructura | Aplicación en el Proyecto | Justificación de Diseño |
| :--- | :--- | :--- |
| **Listas Enlazadas** | Registro de vuelos y aerolíneas | Permiten un manejo dinámico y flexible de registros sin tamaño fijo previo en memoria. |
| **Colas (FIFO)** | Sistema de reservas de pasajeros | Garantizan equidad procesando las solicitudes estrictamente por orden de llegada. |
| **Red de Rutas (Ponderada)** | Red de aeropuertos y rutas | Los nodos representan aeropuertos/destinos y las aristas almacenan distancias (km) y tiempos de viaje. |
| **Algoritmos de Búsqueda** | Búsqueda por código de vuelo | Localización rápida y precisa de vuelos y pasajeros dentro del sistema. |
| **Algoritmos de Ordenamiento** | Listados de vuelos y destinos | Organización de los vuelos según criterios de horario, destino o aerolínea. |
| **TADs (Tipos Abstractos)** | Modelado de Entidades | Modularización y encapsulamiento del comportamiento de vuelos, reservas y rutas. |
| **Persistencia Binaria (.dat)** | Archivos del sistema | Almacenamiento confiable de vuelos y reservas al iniciar y modificar datos. |

---

## Control de flujo

```mermaid
flowchart TD
    subgraph UI["AeroGestion.c (Interfaz y Menú)"]
        M[Menú Principal CLI]
    end

    subgraph TADs["Gestion.h (Estructuras Dinámicas)"]
        LV[Lista Enlazada de Vuelos]
        CF[Cola FIFO Reservas Confirmadas]
        CE[Cola FIFO Lista de Espera]
    end

    subgraph Topo["topologia.h (Red Aérea)"]
        G[Red Ponderada de Rutas y Destinos]
        G --> P["Matriz de Pesos (Distancia km / Duración min)"]
    end

    subgraph Logica["Negocioyop.h (Lógica y Algoritmos)"]
        ORD["Ordenamiento (Burbuja por Código/Aerolínea/Destino/Duración)"]
        BUS["Búsqueda Secuencial y Binaria"]
        RES{"¿Capacidad disponible en Vuelo?"}
        CANC["Cancelación por DNI"]
        PROM["Promoción Automática FIFO"]
    end

    subgraph Persist["Persistencia.h (Archivos Binarios)"]
        DAT1[("vuelos.dat")]
        DAT2[("reservas.dat")]
        CORTE["Corte de Control por Aerolínea"]
    end

    M -->|1. Registrar Vuelo| LV
    M -->|1. Conectar Destino| G
    M -->|2. Ordenar / Listar| ORD
    ORD --> LV
    M -->|3. Buscar Vuelo| BUS
    BUS --> LV

    M -->|4. Solicitar Reserva| RES
    RES -- "Cupo disponible" --> CF
    RES -- "Vuelo lleno" --> CE

    M -->|5. Cancelar Reserva| CANC
    CANC -->|Libera Asiento| LV
    CANC -.->|Desencola Espera| PROM
    PROM -.->|Ocupa Vacante| CF

    LV -->|Auto-guardado| DAT1
    CF -->|Auto-guardado| DAT2
    CE -->|Auto-guardado| DAT2

    DAT1 -->|Auto-carga al inicio| LV
    DAT2 -->|Auto-carga al inicio| CF
    DAT2 -->|Auto-carga al inicio| CE

    DAT1 -->|8. Informe Agrupado| CORTE
```

---

## Metodología de Trabajo

* **Programación de a Pares (Pair Programming):** Desarrollo colaborativo continuo para maximizar la calidad del código, el diseño de algoritmos y la detección temprana de errores.
* **Descomposición Modular:** Abordaje sistemático de subproblemas individuales hacia la integración del sistema global.
* **Reflexión y Aprendizaje:** Documentación de decisiones técnicas y lecciones aprendidas a lo largo de las distintas etapas de la asignatura.

---

## Instrucciones Básicas

### Prerrequisitos
* Compilador / Entorno de desarrollo acorde al lenguaje del proyecto (e.g. GCC/DevC++/Visual Studio para C/C++ o el entorno configurado para AED2).

### Ejecución
1. Clonar o descargar el repositorio:
   ```bash
   git clone https://github.com/tu-usuario/AeroGestion.git
   cd AeroGestion
   ```
2. Compilar o ejecutar el proyecto según la configuración de tu entorno.
3. Seguir las opciones del menú interactivo para:
   - Cargar y consultar vuelos.
   - Solicitar y cancelar reservas.
   - Consultar la red de destinos y conexiones disponibles.
   - Guardar o cargar datos desde disco.

---

> [!NOTE]
> Proyecto integrador para **Algoritmos y Estructuras de Datos II**.
