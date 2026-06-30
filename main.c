

#include <stdio.h>
#include "funciones.h"

int main(void) {
    Zona zonas[MAX_ZONAS];
    int opcion;
    int continuar = 1;
    int totalZonas = 0; 

    inicializarZonas(zonas);
    cargarDatos(zonas, &totalZonas);

    printf("\nBienvenido al Sistema Integral de Gestion y Prediccion\n");
    printf("de Contaminacion del Aire en Zonas Urbanas.\n");

    while (continuar) {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion (1-9): ", 1, 9);

        switch (opcion) {
            case 1: {
                int subopcion;
                int enSubmenu = 1;

                while (enSubmenu) {
                    printf("\n--- GESTION DE ZONAS URBANAS ---\n");
                    printf("1. Registrar / Agregar nueva zona\n");
                    printf("2. Editar nombre de una zona existente\n");
                    printf("3. Eliminar una zona\n");
                    printf("4. Listar zonas existentes\n");
                    printf("5. Ver historico de una zona\n");
                    printf("6. Volver al menu principal\n");
                    subopcion = leerEntero("Seleccione una opcion: ", 1, 6);

                    switch (subopcion) {
                        case 1:
                            gestionarNombresZonas(zonas, &totalZonas);
                            break;
                        case 2:
                            if (totalZonas == 0) printf("\nNo hay zonas registradas para editar.\n");
                            else editarNombreZona(zonas, totalZonas);
                            break;
                        case 3:
                            eliminarZona(zonas, &totalZonas);
                            break;
                        case 4:
                            listarZonas(zonas, totalZonas);
                            break;
                        case 5:
                            if (totalZonas == 0) printf("\nNo hay zonas registradas para ver el historico.\n");
                            else mostrarHistoricoZona(zonas, totalZonas);
                            break;
                        case 6:
                            enSubmenu = 0;
                            break;
                    }
                }
                break;
            }

            case 2:
                if (totalZonas == 0) {
                    printf("\nNo hay zonas registradas. Registre una zona primero.\n");
                } else {
                    registrarDatoDiario(zonas, totalZonas);
                }
                break;

            case 3:
                if (totalZonas == 0) printf("\nNo hay zonas registradas.\n");
                else monitoreoActual(zonas, totalZonas);
                break;

            case 4:
                if (totalZonas == 0) printf("\nNo hay zonas registradas.\n");
                else prediccionTodasLasZonas(zonas, totalZonas);
                break;

            case 5:
                if (totalZonas == 0) printf("\nNo hay zonas registradas.\n");
                else promedioHistorico30Dias(zonas, totalZonas);
                break;

            case 6:
                if (totalZonas == 0) printf("\nNo hay zonas registradas.\n");
                else mostrarAlertasYRecomendaciones(zonas, totalZonas);
                break;

            case 7:
                if (totalZonas == 0) printf("\nNo hay zonas registradas para exportar.\n");
                else exportarReporte(zonas, totalZonas);
                break;

            case 8:
                if (totalZonas == 0) {
                    printf("\nNo hay zonas registradas para editar.\n");
                } else {
                    editarHistoricoZona(zonas, totalZonas);
                }
                break;

            case 9: // Ahora la salida es el caso 9
                guardarDatos(zonas, totalZonas);
                printf("\nGuardando datos y saliendo del sistema...\n");
                printf("Gracias por usar el sistema. Hasta pronto.\n");
                continuar = 0;
                break;

            default:
                printf("\nOpcion invalida.\n");
                break;
        }
    }

    return 0;
}