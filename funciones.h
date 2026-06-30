

#ifndef FUNCIONES_H
#define FUNCIONES_H

/* ------------------------- CONSTANTES GLOBALES ------------------------ */
#define MAX_ZONAS        100    /* Capacidad maxima del arreglo */
#define DIAS_HISTORICO   30     /* Dias de historico fijo por zona */
#define MAX_NOMBRE       50

#define MIN_CO2    0.0f
#define MAX_CO2    5000.0f

#define MIN_SO2    0.0f
#define MAX_SO2    500.0f

#define MIN_NO2    0.0f
#define MAX_NO2    500.0f

#define MIN_PM25   0.0f
#define MAX_PM25   500.0f

#define LIMITE_CO2   1000.0
#define LIMITE_SO2   40.0
#define LIMITE_NO2   40.0
#define LIMITE_PM25  25.0

/* --- Limites Climaticos Logicos y de Confort --- */
#define MIN_TEMP         -5.0f   
#define MAX_TEMP         35.0f

#define MIN_HUMEDAD      30.0f
#define MAX_HUMEDAD      80.0f

#define MIN_VIENTO        5.0f   
#define MAX_VIENTO       45.0f   

#define ARCHIVO_DATOS   "datos.txt"
#define ARCHIVO_REPORTE "reporte.txt"

/* ----------------------------- ESTRUCTURAS ----------------------------- */

typedef struct {
    int dia;
    float co2;
    float so2;
    float no2;
    float pm25;
    float temperatura;
    float viento;
    float humedad;
} RegistroDiario;

typedef struct {
    char nombre[MAX_NOMBRE];
    RegistroDiario historico[DIAS_HISTORICO];
    int diasRegistrados;
} Zona;

typedef struct {
    float co2;
    float so2;
    float no2;
    float pm25;
    int generaAlerta;
} Prediccion;

/* ---------------------------- PROTOTIPOS ---------------------------- */

/* --- Utilidades y entrada --- */
void  mostrarMenu(void);
void  limpiarBuffer(void);
int   leerEntero(const char *mensaje, int min, int max);
float leerFlotante(const char *mensaje);
float leerFlotanteRango(const char *mensaje, float min, float max);

/* --- Gestion de zonas --- */
void inicializarZonas(Zona *zonas);
void gestionarNombresZonas(Zona *zonas, int *totalZonas);
void editarNombreZona(Zona *zonas, int totalZonas);
void eliminarZona(Zona *zonas, int *totalZonas);
int  zonaTieneNombre(const Zona *zona);
int  todasLasZonasTienenNombre(const Zona *zonas, int totalZonas);
int  nombreYaExiste(const Zona *zonas, int totalZonas, const char *nombre, int indiceExcluir);
void registrarDatoDiario(Zona *zonas, int totalZonas);
void listarZonas(const Zona *zonas, int totalZonas);
void mostrarHistoricoZona(const Zona *zonas, int totalZonas);

/* --- Monitoreo actual --- */
void monitoreoActual(const Zona *zonas, int totalZonas);
int  superaLimites(const RegistroDiario *r);

/* --- Prediccion 24h --- */
Prediccion predecirZona(const Zona *zona);
void prediccionTodasLasZonas(const Zona *zonas, int totalZonas);
float promedioPonderado(const float valores[], int n);

/* --- Alertas y recomendaciones --- */
void emitirAlerta(const char *nombreZona, const char *contaminante, float valor, float limite);
void mostrarAlertasYRecomendaciones(const Zona *zonas, int totalZonas);

/* --- Promedios historicos 30 dias vs OMS --- */
void promedioHistorico30Dias(const Zona *zonas, int totalZonas);

/* --- Persistencia y reportes --- */
void guardarDatos(const Zona *zonas, int totalZonas);
void cargarDatos(Zona *zonas, int *totalZonas);
void exportarReporte(const Zona *zonas, int totalZonas);
void editarHistoricoZona(Zona *zonas, int totalZonas);
#endif /* FUNCIONES_H */