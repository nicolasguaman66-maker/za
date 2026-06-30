
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"


void mostrarMenu(void) {
    printf("\n========================================\n");
    printf(" SISTEMA INTEGRAL DE GESTION Y PREDICCION\n");
    printf(" DE CONTAMINACION DEL AIRE\n");
    printf("========================================\n");
    printf("1. Gestion de zonas urbanas\n");
    printf("2. Registrar dato diario de contaminacion\n");
    printf("3. Monitoreo de contaminacion actual\n");
    printf("4. Prediccion de niveles futuros (24h)\n");
    printf("5. Promedio historico (30 dias) vs OMS\n");
    printf("6. Alertas y recomendaciones de mitigacion\n");
    printf("7. Exportar reporte a archivo\n");
    printf("8. Editar parametros de contaminacion y clima\n");
    printf("9. Salir del sistema y guardar datos\n");
    printf("========================================\n");
}

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int leerEntero(const char *mensaje, int min, int max) {
    int valor, resultadoScanf, valido = 0;
    do {
        printf("%s", mensaje);
        resultadoScanf = scanf("%d", &valor);
        if (resultadoScanf == EOF) { printf("\nFin de entrada. Cerrando.\n"); exit(0); }
        if (resultadoScanf != 1) { printf("Entrada invalida.\n"); limpiarBuffer(); continue; }
        limpiarBuffer();
        if (valor < min || valor > max) printf("El valor debe estar entre %d y %d.\n", min, max);
        else valido = 1;
    } while (!valido);
    return valor;
}

float leerFlotante(const char *mensaje) {
    float valor; int resultadoScanf, valido = 0;
    do {
        printf("%s", mensaje);
        resultadoScanf = scanf("%f", &valor);
        if (resultadoScanf == EOF) { printf("\nFin de entrada. Cerrando.\n"); exit(0); }
        if (resultadoScanf != 1) { printf("Entrada invalida.\n"); limpiarBuffer(); continue; }
        limpiarBuffer();
        if (valor < 0) printf("El valor no puede ser negativo.\n");
        else valido = 1;
    } while (!valido);
    return valor;
}

float leerFlotanteRango(const char *mensaje, float min, float max) {
    float valor;
    int resultadoScanf, valido = 0;
    do {
        printf("%s", mensaje);
        resultadoScanf = scanf("%f", &valor);
        if (resultadoScanf == EOF) { printf("\nFin de entrada. Cerrando.\n"); exit(0); }
        if (resultadoScanf != 1) {
            printf("Entrada invalida. Ingrese un numero decimal.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        if (valor < min || valor > max) {
            printf("[ERROR CLIMATICO] El valor ingresado (%.1f) esta fuera del rango logico permitido (%.1f a %.1f).\n", valor, min, max);
        } else {
            valido = 1;
        }
    } while (!valido);
    return valor;
}

void inicializarZonas(Zona *zonas) {
    int i, j;
    for (i = 0; i < MAX_ZONAS; i++) {
        (zonas + i)->nombre[0] = '\0';
        (zonas + i)->diasRegistrados = 0;
        for (j = 0; j < DIAS_HISTORICO; j++) {
            (zonas + i)->historico[j].dia = 0;
            (zonas + i)->historico[j].co2 = 0.0f;
            (zonas + i)->historico[j].so2 = 0.0f;
            (zonas + i)->historico[j].no2 = 0.0f;
            (zonas + i)->historico[j].pm25 = 0.0f;
            (zonas + i)->historico[j].temperatura = 0.0f;
            (zonas + i)->historico[j].viento = 0.0f;
            (zonas + i)->historico[j].humedad = 0.0f;
        }
    }
}

int zonaTieneNombre(const Zona *zona) {
    return zona->nombre[0] != '\0';
}

int todasLasZonasTienenNombre(const Zona *zonas, int totalZonas) {
    int i;
    for (i = 0; i < totalZonas; i++) {
        if (!zonaTieneNombre(zonas + i)) return 0;
    }
    return 1;
}

int nameYaExiste(const Zona *zonas, int totalZonas, const char *nombre, int indiceExcluir) {
    return nombreYaExiste(zonas, totalZonas, nombre, indiceExcluir);
}

int nombreYaExiste(const Zona *zonas, int totalZonas, const char *nombre, int indiceExcluir) {
    int i, j;
    for (i = 0; i < totalZonas; i++) {
        if (i == indiceExcluir) continue;
        if (!zonaTieneNombre(zonas + i)) continue;
        j = 0;
        while ((zonas + i)->nombre[j] != '\0' && nombre[j] != '\0') {
            if (tolower((unsigned char)(zonas + i)->nombre[j]) != tolower((unsigned char)nombre[j])) break;
            j++;
        }
        if ((zonas + i)->nombre[j] == '\0' && nombre[j] == '\0') return 1;
    }
    return 0;
}

void gestionarNombresZonas(Zona *zonas, int *totalZonas) {
    char nombreTemp[MAX_NOMBRE];
    int continuarIngresando = 1;

    do {
        if (*totalZonas >= MAX_ZONAS) {
            printf("\nSe ha alcanzado el limite maximo del sistema (%d zonas).\n", MAX_ZONAS);
            break;
        }

        int nombreValido = 0;
        int indiceNuevaZona = *totalZonas;

        printf("\n--- REGISTRAR NUEVA ZONA (Zona Actual: %d) ---\n", indiceNuevaZona + 1);

        while (!nombreValido) {
            printf("Ingrese el nombre para la nueva zona: ");
            if (fgets(nombreTemp, MAX_NOMBRE, stdin) == NULL) {
                printf("No se pudo leer el nombre. Intente de nuevo.\n");
                continue;
            }

            size_t len = strlen(nombreTemp);
            if (len > 0 && nombreTemp[len - 1] == '\n') {
                nombreTemp[len - 1] = '\0';
            }

            if (nombreTemp[0] == '\0') {
                printf("El nombre no puede estar vacio. Intente de nuevo.\n");
                continue;
            }

            if (nombreYaExiste(zonas, *totalZonas, nombreTemp, indiceNuevaZona)) {
                printf("Ya existe una zona con ese nombre. Ingrese uno diferente.\n");
                continue;
            }

            nombreValido = 1;
        }

        strncpy((zonas + indiceNuevaZona)->nombre, nombreTemp, MAX_NOMBRE - 1);
        (zonas + indiceNuevaZona)->nombre[MAX_NOMBRE - 1] = '\0';
        (zonas + indiceNuevaZona)->diasRegistrados = 0;

        printf("Zona %d registrada exitosamente como \"%s\"!\n", indiceNuevaZona + 1, (zonas + indiceNuevaZona)->nombre);
        (*totalZonas)++;

        continuarIngresando = leerEntero("\n Desea ingresar otra zona? (1: Si / 0: No): ", 0, 1);

    } while (continuarIngresando);
}

void editarNombreZona(Zona *zonas, int totalZonas) {
    int idxZona;
    char nombreTemp[MAX_NOMBRE];
    int nombreValido = 0;

    printf("\n--- EDITAR NOMBRE DE ZONA EXISTENTE ---\n");
    listarZonas(zonas, totalZonas);

    idxZona = leerEntero("\nSeleccione el numero de zona que desea editar: ", 1, totalZonas) - 1;

    printf("Nombre actual: \"%s\"\n", (zonas + idxZona)->nombre);

    while (!nombreValido) {
        printf("Ingrese el nuevo nombre para esta zona: ");
        if (fgets(nombreTemp, MAX_NOMBRE, stdin) == NULL) {
            printf("No se pudo leer el nombre. Intente de nuevo.\n");
            continue;
        }

        size_t len = strlen(nombreTemp);
        if (len > 0 && nombreTemp[len - 1] == '\n') {
            nombreTemp[len - 1] = '\0';
        }

        if (nombreTemp[0] == '\0') {
            printf("El nombre no puede estar vacio. Intente de nuevo.\n");
            continue;
        }

        if (nombreYaExiste(zonas, totalZonas, nombreTemp, idxZona)) {
            printf("Ya existe otra zona con ese nombre. Ingrese uno diferente.\n");
            continue;
        }

        nombreValido = 1;
    }

    strncpy((zonas + idxZona)->nombre, nombreTemp, MAX_NOMBRE - 1);
    (zonas + idxZona)->nombre[MAX_NOMBRE - 1] = '\0';

    printf("¡Zona %d actualizada con exito! Nuevo nombre: \"%s\"\n", idxZona + 1, (zonas + idxZona)->nombre);
}

void eliminarZona(Zona *zonas, int *totalZonas) {
    int idxZona, i, j, confirmacion;

    if (*totalZonas == 0) {
        printf("\nNo hay zonas registradas para eliminar.\n");
        return;
    }

    printf("\n--- ELIMINAR ZONA ---\n");
    listarZonas(zonas, *totalZonas);

    idxZona = leerEntero("\nSeleccione el numero de zona a eliminar: ", 1, *totalZonas) - 1;

    printf("\nZona seleccionada : \"%s\"\n", (zonas + idxZona)->nombre);
    printf("Dias registrados  : %d\n",      (zonas + idxZona)->diasRegistrados);
    printf("\nATENCION: Se eliminaran la zona y TODOS sus datos historicos.\n");
    confirmacion = leerEntero("¿Confirma la eliminacion? (1: Si / 0: No): ", 0, 1);

    if (!confirmacion) {
        printf("Eliminacion cancelada.\n");
        return;
    }

    for (i = idxZona; i < *totalZonas - 1; i++) {
        zonas[i] = zonas[i + 1];
    }

    i = *totalZonas - 1;
    zonas[i].nombre[0]      = '\0';
    zonas[i].diasRegistrados = 0;
    for (j = 0; j < DIAS_HISTORICO; j++) {
        zonas[i].historico[j].dia         = 0;
        zonas[i].historico[j].co2         = 0.0f;
        zonas[i].historico[j].so2         = 0.0f;
        zonas[i].historico[j].no2         = 0.0f;
        zonas[i].historico[j].pm25        = 0.0f;
        zonas[i].historico[j].temperatura = 0.0f;
        zonas[i].historico[j].viento      = 0.0f;
        zonas[i].historico[j].humedad     = 0.0f;
    }

    (*totalZonas)--;

    printf("\nZona eliminada correctamente. Zonas restantes: %d\n", *totalZonas);
}

void listarZonas(const Zona *zonas, int totalZonas) {
    int i;

    printf("\n");
    printf("+----+--------------------------+------------+\n");
    printf("|       ZONAS URBANAS REGISTRADAS (%2d)       |\n", totalZonas);
    printf("+----+--------------------------+------------+\n");
    printf("| ID | Nombre                   | Registrado |\n");
    printf("+----+--------------------------+------------+\n");

    if (totalZonas == 0) {
        printf("| %-42s |\n", "No existen zonas registradas.");
    } else {
        for (i = 0; i < totalZonas; i++) {
            printf("| %2d | %-24s | %2d / %-2d    |\n",
                   i + 1,
                   zonas[i].nombre,
                   zonas[i].diasRegistrados,
                   DIAS_HISTORICO);
        }
    }

    printf("+----+--------------------------+------------+\n");
}

void registrarDatoDiario(Zona *zonas, int totalZonas) {
    int idxZona, idxDia;
    Zona *z;
    RegistroDiario nuevo;

    listarZonas(zonas, totalZonas);
    idxZona = leerEntero("\nSeleccione el numero de zona: ", 1, totalZonas) - 1;
    z = zonas + idxZona;

    if (z->diasRegistrados < DIAS_HISTORICO) {
        idxDia = z->diasRegistrados;
        z->diasRegistrados++;
    } else {
        idxDia = 0;
        printf("Aviso: el historico de %s esta lleno (30 dias). Se sobrescribira el dia 1.\n", z->nombre);
    }

    printf("\n--- Registro de datos para %s (dia %d) ---\n", z->nombre, idxDia + 1);
    nuevo.dia  = idxDia + 1;
    nuevo.co2  = leerFlotanteRango("CO2 (ppm): ", MIN_CO2, MAX_CO2);
    nuevo.so2  = leerFlotanteRango("SO2 (ug/m3): ", MIN_SO2, MAX_SO2);
    nuevo.no2  = leerFlotanteRango("NO2 (ug/m3): ", MIN_NO2, MAX_NO2);
    nuevo.pm25 = leerFlotanteRango("PM2.5 (ug/m3): ", MIN_PM25, MAX_PM25);

    nuevo.temperatura = leerFlotanteRango("Temperatura (C): ",       MIN_TEMP,    MAX_TEMP);
    nuevo.viento      = leerFlotanteRango("Velocidad del viento (km/h): ", MIN_VIENTO, MAX_VIENTO);
    nuevo.humedad     = leerFlotanteRango("Humedad (%%): ",            MIN_HUMEDAD, MAX_HUMEDAD);

    z->historico[idxDia] = nuevo;
    printf("\nDato registrado correctamente en %s.\n", z->nombre);
}
void mostrarHistoricoZona(const Zona *zonas, int totalZonas) {
    int idxZona, i;
    const Zona *z;

    listarZonas(zonas, totalZonas);
    idxZona = leerEntero("\nSeleccione el numero de zona: ", 1, totalZonas) - 1;
    z = zonas + idxZona;

    printf("\n");
    printf("+-----+----------+----------+----------+----------+-----------+-----------+-----------+\n");
    printf("| HISTORICO DE LA ZONA: %-61s |\n", z->nombre);
    printf("+-----+----------+----------+----------+----------+-----------+-----------+-----------+\n");
    printf("| Dia |   CO2    |   SO2    |   NO2    |  PM2.5   |  Temp(C)  |  Viento   | Humedad   |\n");
    printf("+-----+----------+----------+----------+----------+-----------+-----------+-----------+\n");

    if (z->diasRegistrados == 0) {
        printf("| %-83s |\n", "Esta zona aun no tiene datos registrados.");
    } else {
        for (i = 0; i < z->diasRegistrados; i++) {
            const RegistroDiario *r = &z->historico[i];
            printf("| %3d | %8.2f | %8.2f | %8.2f | %8.2f | %9.1f | %9.1f | %9.1f |\n",
                   r->dia,
                   r->co2, r->so2, r->no2, r->pm25,
                   r->temperatura, r->viento, r->humedad);
        }
    }

    printf("+-----+----------+----------+----------+----------+-----------+-----------+-----------+\n");
}

/* ------------------------------------------------------------------
   MONITOREO ACTUAL
   ------------------------------------------------------------------ */

int superaLimites(const RegistroDiario *r) {
    return (r->co2 > LIMITE_CO2 || r->so2 > LIMITE_SO2 || r->no2 > LIMITE_NO2 || r->pm25 > LIMITE_PM25);
}

void monitoreoActual(const Zona *zonas, int totalZonas) {
    int i;

    printf("\n");
    printf("+----+------------------------+---------+---------+---------+---------+---------------------+\n");
    printf("|              MONITOREO ACTUAL DE LA CALIDAD DEL AIRE                                      |\n");
    printf("+----+------------------------+---------+---------+---------+---------+---------------------+\n");
    printf("| ID | Zona                   |   CO2   |   SO2   |   NO2   |  PM2.5  | Excesos OMS         |\n");
    printf("+----+------------------------+---------+---------+---------+---------+---------------------+\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = &zonas[i];

        if (z->diasRegistrados == 0) {
            printf("| %2d | %-22s | %7s | %7s | %7s | %7s | %-19s |\n",
                   i + 1, z->nombre, "--", "--", "--", "--", "SIN DATOS");
        } else {
            const RegistroDiario *r = &z->historico[z->diasRegistrados - 1];
            
            // Construimos una cadena con los contaminantes que superaron el limite
            char excesos[50] = "";
            if (r->co2 > LIMITE_CO2)   strcat(excesos, "CO2 ");
            if (r->so2 > LIMITE_SO2)   strcat(excesos, "SO2 ");
            if (r->no2 > LIMITE_NO2)   strcat(excesos, "NO2 ");
            if (r->pm25 > LIMITE_PM25) strcat(excesos, "PM2.5 ");
            
            if (strlen(excesos) == 0) {
                strcpy(excesos, "NINGUNO (NORMAL)");
            }

            printf("| %2d | %-22s | %7.1f | %7.1f | %7.1f | %7.1f | %-19s |\n",
                   i + 1, z->nombre,
                   r->co2, r->so2, r->no2, r->pm25,
                   excesos);
        }
    }

    printf("+----+------------------------+---------+---------+---------+---------+---------------------+\n");
    printf("\nLimites OMS Actuales: CO2: %.1f | SO2: %.1f | NO2: %.1f | PM2.5: %.1f\n", 
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
}

float promedioPonderado(const float valores[], int n) {
    int i;
    float sumaPonderada = 0.0f, sumaPesos = 0.0f;
    if (n <= 0) return 0.0f;
    for (i = 0; i < n; i++) {
        float peso = (float)(i + 1);
        sumaPonderada += valores[i] * peso;
        sumaPesos += peso;
    }
    return sumaPonderada / sumaPesos;
}

Prediccion predecirZona(const Zona *zona) {
    Prediccion pred = {0.0f, 0.0f, 0.0f, 0.0f, 0};
    float co2vals[DIAS_HISTORICO], so2vals[DIAS_HISTORICO],
          no2vals[DIAS_HISTORICO], pm25vals[DIAS_HISTORICO];
    int n = zona->diasRegistrados, i;
    float factorClima = 1.0f;

    if (n == 0) return pred;

    for (i = 0; i < n; i++) {
        co2vals[i]  = zona->historico[i].co2;
        so2vals[i]  = zona->historico[i].so2;
        no2vals[i]  = zona->historico[i].no2;
        pm25vals[i] = zona->historico[i].pm25;
    }

    pred.co2  = promedioPonderado(co2vals,  n);
    pred.so2  = promedioPonderado(so2vals,  n);
    pred.no2  = promedioPonderado(no2vals,  n);
    pred.pm25 = promedioPonderado(pm25vals, n);

    const RegistroDiario *ultimo = &zona->historico[n - 1];
    factorClima = 1.0f + (ultimo->humedad / 200.0f) - (ultimo->viento / 100.0f);
    if (factorClima < 0.5f) factorClima = 0.5f;

    pred.co2  *= factorClima;
    pred.so2  *= factorClima;
    pred.no2  *= factorClima;
    pred.pm25 *= factorClima;
    pred.generaAlerta = (pred.co2 > LIMITE_CO2 || pred.so2 > LIMITE_SO2 ||
                         pred.no2 > LIMITE_NO2  || pred.pm25 > LIMITE_PM25);

    return pred;
}

void prediccionTodasLasZonas(const Zona *zonas, int totalZonas) {
    int i;

    printf("\n");
    printf("+----+------------------------+---------+---------+---------+---------+-------------------+\n");
    printf("|                    PREDICCION DE CONTAMINACION (24 HORAS)                               |\n");
    printf("+----+------------------------+---------+---------+---------+---------+-------------------+\n");
    printf("| ID | Zona                   |   CO2   |   SO2   |   NO2   |  PM2.5  | Estado previsto   |\n");
    printf("+----+------------------------+---------+---------+---------+---------+-------------------+\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = &zonas[i];

        if (z->diasRegistrados == 0) {
            printf("| %2d | %-22s | %7s | %7s | %7s | %7s | %-17s |\n",
                   i + 1, z->nombre, "--", "--", "--", "--", "SIN HISTORICO");
        } else {
            Prediccion p = predecirZona(z);
            /* ── último registro (datos actuales) ── */
            const RegistroDiario *actual = &z->historico[z->diasRegistrados - 1];

            /* ── diferencias: prediccion - actual ── */
            float difCO2  = p.co2  - actual->co2;
            float difSO2  = p.so2  - actual->so2;
            float difNO2  = p.no2  - actual->no2;
            float difPM25 = p.pm25 - actual->pm25;

            /* fila: valores actuales */
            printf("| %2d | %-22s | %7.1f | %7.1f | %7.1f | %7.1f | %-17s |\n",
                   i + 1, z->nombre,
                   actual->co2, actual->so2, actual->no2, actual->pm25,
                   "ACTUAL");

            /* fila: valores predichos */
            printf("|    | %-22s | %7.1f | %7.1f | %7.1f | %7.1f | %-17s |\n",
                   "(prediccion 24h)",
                   p.co2, p.so2, p.no2, p.pm25,
                   p.generaAlerta ? "ALERTA PREVENTIVA" : "NORMAL");

            /* fila: diferencia con signo */
            printf("|    | %-22s | %+7.1f | %+7.1f | %+7.1f | %+7.1f | %-17s |\n",
                   "diferencia",
                   difCO2, difSO2, difNO2, difPM25,
                   difCO2 > 0 || difSO2 > 0 || difNO2 > 0 || difPM25 > 0
                       ? "SUBE" : "BAJA o IGUAL");

            printf("+----+------------------------+---------+---------+---------+---------+-------------------+\n");
        }
    }

    printf("\n");
    printf("NORMAL            : La prediccion se encuentra dentro de los limites OMS.\n");
    printf("ALERTA PREVENTIVA : Se estima que uno o mas contaminantes superaran el limite.\n");
    printf("SIN HISTORICO     : No existen datos suficientes para realizar la prediccion.\n");
}


void emitirAlerta(const char *nombreZona, const char *contaminante, float valor, float limite) {
    (void)valor; (void)limite;
    printf("   >> ALERTA: %s presenta %s por encima del limite aceptable.\n", nombreZona, contaminante);
}

void mostrarAlertasYRecomendaciones(const Zona *zonas, int totalZonas) {
    int i;
    int huboAlertas = 0;

    printf("\n");
    printf("+----+------------------------+-------+-------+-------+-------+-------------------+\n");
    printf("|                    ALERTAS Y RECOMENDACIONES                                    |\n");
    printf("+----+------------------------+-------+-------+-------+-------+-------------------+\n");
    printf("| ID | Zona                   |  CO2  |  SO2  |  NO2  | PM2.5 | Estado            |\n");
    printf("+----+------------------------+-------+-------+-------+-------+-------------------+\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = zonas + i;

        if (z->diasRegistrados == 0) {
            printf("| %2d | %-22s | %-5s | %-5s | %-5s | %-5s | %-17s |\n",
                   i + 1, z->nombre, "--", "--", "--", "--", "SIN DATOS");
            continue;
        }

        Prediccion p = predecirZona(z);
        int co2Alerta  = p.co2  > LIMITE_CO2;
        int so2Alerta  = p.so2  > LIMITE_SO2;
        int no2Alerta  = p.no2  > LIMITE_NO2;
        int pm25Alerta = p.pm25 > LIMITE_PM25;

        if (p.generaAlerta) huboAlertas = 1;

        printf("| %2d | %-22s | %-5s | %-5s | %-5s | %-5s | %-17s |\n",
               i + 1, z->nombre,
               co2Alerta  ? "SI" : "NO",
               so2Alerta  ? "SI" : "NO",
               no2Alerta  ? "SI" : "NO",
               pm25Alerta ? "SI" : "NO",
               p.generaAlerta ? "ALERTA" : "NORMAL");
    }

    printf("+----+------------------------+-------+-------+-------+-------+-------------------+\n");

    if (!huboAlertas) {
        printf("\nNo existen alertas activas en ninguna zona.\n");
        return;
    }

    printf("\n==================== RECOMENDACIONES ====================\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = zonas + i;
        if (z->diasRegistrados == 0) continue;

        Prediccion p = predecirZona(z);
        if (!p.generaAlerta) continue;

        printf("\nZona: %s\n", z->nombre);
        if (p.co2  > LIMITE_CO2)  printf(" * Reducir el trafico vehicular en horas pico.\n");
        if (p.so2  > LIMITE_SO2)  printf(" * Disminuir temporalmente las emisiones industriales.\n");
        if (p.no2  > LIMITE_NO2)  printf(" * Restringir la circulacion de vehiculos pesados.\n");
        if (p.pm25 > LIMITE_PM25) printf(" * Evitar actividades al aire libre y utilizar mascarilla.\n");
    }
}

/* ------------------------------------------------------------------
   PROMEDIO HISTORICO 30 DIAS VS OMS
   ------------------------------------------------------------------ */

void promedioHistorico30Dias(const Zona *zonas, int totalZonas) {
    int i, j;

    printf("\n");
    printf("+----+------------------------+----------+----------+----------+----------+-------------------+\n");
    printf("|             PROMEDIO HISTORICO (ULTIMOS 30 DIAS) VS LIMITES OMS                             |\n");
    printf("+----+------------------------+----------+----------+----------+----------+-------------------+\n");
    printf("| ID | Zona                   |   CO2    |   SO2    |   NO2    |  PM2.5   | Resultado         |\n");
    printf("+----+------------------------+----------+----------+----------+----------+-------------------+\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = zonas + i;

        if (z->diasRegistrados == 0) {
            printf("| %2d | %-22s | %8s | %8s | %8s | %8s | %-17s |\n",
                   i + 1, z->nombre, "--", "--", "--", "--", "SIN DATOS");
            continue;
        }

        float sumaCO2 = 0, sumaSO2 = 0, sumaNO2 = 0, sumaPM25 = 0;
        for (j = 0; j < z->diasRegistrados; j++) {
            sumaCO2  += z->historico[j].co2;
            sumaSO2  += z->historico[j].so2;
            sumaNO2  += z->historico[j].no2;
            sumaPM25 += z->historico[j].pm25;
        }

        float promCO2  = sumaCO2  / z->diasRegistrados;
        float promSO2  = sumaSO2  / z->diasRegistrados;
        float promNO2  = sumaNO2  / z->diasRegistrados;
        float promPM25 = sumaPM25 / z->diasRegistrados;

        int alerta = (promCO2 > LIMITE_CO2 || promSO2 > LIMITE_SO2 ||
                      promNO2 > LIMITE_NO2  || promPM25 > LIMITE_PM25);

        printf("| %2d | %-22s | %8.2f | %8.2f | %8.2f | %8.2f | %-17s |\n",
               i + 1, z->nombre,
               promCO2, promSO2, promNO2, promPM25,
               alerta ? "EXCEDE OMS" : "DENTRO DEL LIMITE");
    }

    printf("+----+------------------------+----------+----------+----------+----------+-------------------+\n");
    printf("\n");
    printf("Limites OMS: CO2 = %.0f ppm | SO2 = %.0f ug/m3 | NO2 = %.0f ug/m3 | PM2.5 = %.0f ug/m3\n",
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
}

/* ------------------------------------------------------------------
   PERSISTENCIA Y REPORTES
   ------------------------------------------------------------------ */

void guardarDatos(const Zona *zonas, int totalZonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "w");
    int i, j;
    if (f == NULL) { printf("Error al guardar archivo.\n"); return; }

    fprintf(f, "%d\n", totalZonas);
    for (i = 0; i < totalZonas; i++) {
        const Zona *z = zonas + i;
        fprintf(f, "%s;%d\n", z->nombre, z->diasRegistrados);
        for (j = 0; j < z->diasRegistrados; j++) {
            const RegistroDiario *r = &z->historico[j];
            fprintf(f, "%d;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                    r->dia, r->co2, r->so2, r->no2, r->pm25,
                    r->temperatura, r->viento, r->humedad);
        }
    }
    fclose(f);
    printf("Datos guardados correctamente en %s.\n", ARCHIVO_DATOS);
}

void cargarDatos(Zona *zonas, int *totalZonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "r");
    int i, j, zonasA_Cargar = 0;
    char nombreLeido[MAX_NOMBRE];
    int dias;

    if (f == NULL) {
        printf("No se encontro archivo previo. Iniciando en 0 zonas.\n");
        *totalZonas = 0;
        return;
    }

    if (fscanf(f, "%d\n", &zonasA_Cargar) != 1) {
        *totalZonas = 0;
        fclose(f);
        return;
    }

    for (i = 0; i < zonasA_Cargar; i++) {
        if (i >= MAX_ZONAS) break;
        Zona *z = zonas + i;

        if (fscanf(f, "%49[^;];%d\n", nombreLeido, &dias) != 2) break;

        strncpy(z->nombre, nombreLeido, MAX_NOMBRE - 1);
        z->nombre[MAX_NOMBRE - 1] = '\0';
        z->diasRegistrados = dias;

        for (j = 0; j < dias; j++) {
            RegistroDiario *r = &z->historico[j];
            if (fscanf(f, "%d;%f;%f;%f;%f;%f;%f;%f\n",
                   &r->dia, &r->co2, &r->so2, &r->no2, &r->pm25,
                   &r->temperatura, &r->viento, &r->humedad) != 8) break;
        }
    }
    *totalZonas = i;
    fclose(f);
    printf("Datos cargados de %d zonas desde %s.\n", *totalZonas, ARCHIVO_DATOS);
}

void exportarReporte(const Zona *zonas, int totalZonas) {
    FILE *f = fopen(ARCHIVO_REPORTE, "w");
    int i, j;

    if (f == NULL) { printf("Error al exportar el reporte.\n"); return; }

    fprintf(f, "\n");
    fprintf(f, "===================================================================================\n");
    fprintf(f, "       SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION DEL AIRE\n");
    fprintf(f, "===================================================================================\n\n");

    fprintf(f, "+----+------------------------+------------------+-------------------+\n");
    fprintf(f, "| ID | Zona                   | Dias Registrados | Estado General    |\n");
    fprintf(f, "+----+------------------------+------------------+-------------------+\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = &zonas[i];
        if (z->diasRegistrados == 0) {
            fprintf(f, "| %2d | %-22s | %-16s | %-17s |\n",
                    i + 1, z->nombre, "0", "SIN DATOS");
        } else {
            Prediccion p = predecirZona(z);
            fprintf(f, "| %2d | %-22s | %-16d | %-17s |\n",
                    i + 1, z->nombre, z->diasRegistrados,
                    p.generaAlerta ? "ALERTA PREVENTIVA" : "NORMAL");
        }
    }

    fprintf(f, "+----+------------------------+------------------+-------------------+\n\n");

    for (i = 0; i < totalZonas; i++) {
        const Zona *z = &zonas[i];

        fprintf(f, "Zona: %s\n", z->nombre);
        fprintf(f, "+-----+----------+----------+----------+----------+----------+----------+----------+\n");
        fprintf(f, "| Dia |   CO2    |   SO2    |   NO2    |  PM2.5   | Temp     | Viento   | Humedad  |\n");
        fprintf(f, "+-----+----------+----------+----------+----------+----------+----------+----------+\n");

        if (z->diasRegistrados == 0) {
            fprintf(f, "| %-77s |\n", "Sin datos registrados.");
        } else {
            for (j = 0; j < z->diasRegistrados; j++) {
                const RegistroDiario *r = &z->historico[j];
                fprintf(f, "| %3d | %8.2f | %8.2f | %8.2f | %8.2f | %8.1f | %8.1f | %8.1f |\n",
                        r->dia,
                        r->co2, r->so2, r->no2, r->pm25,
                        r->temperatura, r->viento, r->humedad);
            }

            Prediccion p = predecirZona(z);
            fprintf(f, "+-----+----------+----------+----------+----------+----------+----------+----------+\n");
            fprintf(f, "\nPrediccion 24 horas: CO2=%.2f | SO2=%.2f | NO2=%.2f | PM2.5=%.2f\n",
                    p.co2, p.so2, p.no2, p.pm25);
            fprintf(f, "Estado: %s\n", p.generaAlerta ? "ALERTA PREVENTIVA" : "NORMAL");
        }

        fprintf(f, "\n");
    }

    fclose(f);
    printf("\nReporte exportado correctamente en \"%s\".\n", ARCHIVO_REPORTE);
}

/* ======================================================================
   OPCION DE EDICION SELECTIVA POR COMPONENTE (FORMATO TABLA)
   ====================================================================== */
void editarHistoricoZona(Zona *zonas, int totalZonas) {
    int idxZona, idxReg, opcionDato, i;
    printf("\n======================================================================\n");
    printf("              MENU DE EDICION SELECTIVA DE REGISTROS\n");
    printf("======================================================================\n");
    
    listarZonas(zonas, totalZonas);
    idxZona = leerEntero("\nSeleccione el ID de la zona a modificar: ", 1, totalZonas) - 1;
    Zona *z = &zonas[idxZona];

    if (z->diasRegistrados == 0) {
        printf("\n[Aviso] La zona '%s' no contiene datos registrados para editar.\n", z->nombre);
        return;
    }

    printf("\nHistorial disponible para la zona '%s':\n", z->nombre);
    for (i = 0; i < z->diasRegistrados; i++) {
        printf("  [%d] -> Datos del Dia Real: %d (Humedad: %.1f%%, CO2: %.1f)\n", 
               i + 1, z->historico[i].dia, z->historico[i].humedad, z->historico[i].co2);
    }

    idxReg = leerEntero("\nSeleccione el numero de registro que desea alterar: ", 1, z->diasRegistrados) - 1;
    RegistroDiario *r = &z->historico[idxReg];

    // Despliegue del menú de edición en un formato de tabla organizada
    printf("\n+---------------------------------------------------------------------+\n");
    printf("|     SELECCION DE VARIABLE A EDITAR (Dia Real: %2d)                  |\n", r->dia);
    printf("+------+-------------------------------------+------------------------+\n");
    printf("|  ID  | Componente / Variable               | Valor Actual           |\n");
    printf("+------+-------------------------------------+------------------------+\n");
    printf("|  [1] | Porcentaje de Humedad               | %8.1f %%             |\n", r->humedad);
    printf("|  [2] | Concentracion de CO2                | %8.1f ppm            |\n", r->co2);
    printf("|  [3] | Concentracion de SO2                | %8.1f ug/m3          |\n", r->so2);
    printf("|  [4] | Concentracion de NO2                | %8.1f ug/m3          |\n", r->no2);
    printf("|  [5] | Concentracion de PM2.5              | %8.1f ug/m3          |\n", r->pm25);
    printf("|  [6] | Temperatura Ambiente                | %8.1f C              |\n", r->temperatura);
    printf("|  [7] | Velocidad del Viento                | %8.1f km/h           |\n", r->viento);
    printf("|  [8] | Cancelar operacion y salir          | --                     |\n");
    printf("+------+-------------------------------------+------------------------+\n");
    
    opcionDato = leerEntero("Seleccione el ID del componente que desea modificar (1-8): ", 1, 8);

    printf("\n");
    switch (opcionDato) {
        case 1:
            r->humedad = leerFlotanteRango("Ingrese la nueva Humedad (%%): ", MIN_HUMEDAD, MAX_HUMEDAD);
            break;
        case 2:
            r->co2 = leerFlotante("Ingrese la nueva concentracion de CO2 (ppm): ");
            break;
        case 3:
            r->so2 = leerFlotante("Ingrese la nueva concentracion de SO2 (ug/m3): ");
            break;
        case 4:
            r->no2 = leerFlotante("Ingrese la nueva concentracion de NO2 (ug/m3): ");
            break;
        case 5:
            r->pm25 = leerFlotante("Ingrese la nueva concentracion de PM2.5 (ug/m3): ");
            break;
        case 6:
            r->temperatura = leerFlotanteRango("Ingrese la nueva Temperatura (C): ", MIN_TEMP, MAX_TEMP);
            break;
        case 7:
            r->viento = leerFlotanteRango("Ingrese la nueva velocidad del Viento (km/h): ", MIN_VIENTO, MAX_VIENTO);
            break;
        case 8:
            printf("Edicion cancelada por el usuario.\n");
            return;
    }

    printf("\n¡Dato modificado con exito en la memoria activa del sistema!\n");
}