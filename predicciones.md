# Informe Técnico: Sistema Integral de Gestión y Predicción de Contaminación del Aire en Zonas Urbanas

[cite_start]**Asignatura:** ISWZ1102 – PROGRAMACIÓN 1 [cite: 1]  
[cite_start]**Componente de Evaluación:** Actividad Evaluativa RC2 [cite: 5]  
[cite_start]**Elaborado por:** Carlos Andrés Guaita Ayala   
[cite_start]**Fecha de Entrega:** Octubre de 2026 [cite: 51]  

---

## [cite_start]FASE 1: Diagnóstico de la Problemática [cite: 70]

### [cite_start]1.1 Introducción [cite: 71]
[cite_start]El crecimiento urbano descontrolado y el aumento del tráfico vehicular han exacerbado la contaminación del aire en muchas ciudades, afectando gravemente la salud pública y el bienestar de la población[cite: 22]. [cite_start]Los contaminantes atmosféricos, como el $CO_2$, $SO_2$, $NO_2$ y el $PM_{2.5}$, no solo son perjudiciales para la salud, sino que también tienen un impacto negativo directo en el medio ambiente y la economía global y local[cite: 23]. 

[cite_start]Un problema clave para las autoridades municipales es la falta de un sistema eficiente de monitoreo y predicción de la contaminación, lo que impide tomar decisiones y acciones de mitigación a tiempo[cite: 24]. [cite_start]Para resolver esto, se requiere un diseño de ingeniería capaz de procesar datos actuales y tendencias históricas ambientales para salvaguardar la seguridad ciudadana[cite: 5, 25].

### [cite_start]1.2 Objetivos del Trabajo [cite: 72]
* [cite_start]**Objetivo General:** Diseñar y desarrollar una solución de software óptima en lenguaje C que permita monitorear, predecir y mitigar los niveles de contaminación en un mínimo de 5 zonas urbanas, considerando restricciones sociales, ambientales y económicas[cite: 13, 25].
* **Objetivos Específicos:**
  1. [cite_start]Formular el problema identificando detalladamente sus principales variables, restricciones normativas y limitaciones de desarrollo técnico[cite: 14].
  2. [cite_start]Evaluar al menos dos alternativas de solución algorítmica y de estructuras de datos en C para seleccionar la más eficiente en términos de tiempo de ejecución y claridad de código[cite: 42, 47].
  3. [cite_start]Implementar un prototipo modular que procese variables climáticas actuales para proyectar los niveles de polución en las próximas 24 horas y emitir alertas preventivas automáticas[cite: 26, 30].
  4. [cite_start]Proveer recomendaciones parametrizadas de mitigación frente a los límites diarios establecidos por la Organización Mundial de la Salud (OMS)[cite: 27, 32].

### [cite_start]1.3 Formulación del Problema e Identificación de Variables [cite: 73]
[cite_start]A continuación, se presenta la matriz analítica con la especificación completa de los requerimientos y dominios de datos del sistema[cite: 40]:

| Dimensión Analítica | Descripción e Identificación de Variables |
| :--- | :--- |
| **Nombre del sistema:** | [cite_start]Sistema Integral de Gestión y Predicción de Contaminación del Aire Urbano (SIGPCA)[cite: 41]. |
| **Usuarios:** | [cite_start]Autoridades ambientales, analistas de salud pública y tomadores de decisiones municipales[cite: 41]. |
| **Objetivo del sistema:** | [cite_start]Monitorear la calidad del aire, predecir el comportamiento a 24 horas basándose en el clima, emitir alertas y generar planes de mitigación urbana[cite: 41]. |
| **Variables de entrada:** | [cite_start]• **Identificadores:** Nombre de las zonas (mínimo 5) [cite: 41][cite_start].<br>• **Contaminantes:** Niveles actuales e históricos (30 días) de $PM_{2.5}$ ($\mu\text{g/m}^3$), $NO_2$ ($\mu\text{g/m}^3$), $SO_2$ ($\mu\text{g/m}^3$) y $CO_2$ ($\text{ppm}$) [cite: 41][cite_start].<br>• **Clima actual:** Temperatura ($^\circ\text{C}$), velocidad del viento ($\text{km/h}$) y porcentaje de humedad ($\%$)[cite: 41]. |
| **Variables de salida:** | [cite_start]• Diagnóstico de contaminación actual por zona vs. límites aceptables [cite: 41][cite_start].<br>• Predicción de niveles de contaminantes para las próximas 24 horas [cite: 41][cite_start].<br>• Alertas preventivas activadas (Normal, Amarilla, Roja) [cite: 41][cite_start].<br>• Listado de recomendaciones de mitigación socioambiental [cite: 41][cite_start].<br>• Reporte ejecutivo físico exportado (`reporte_contaminacion_urbana.md`)[cite: 41]. |
| **Procesos asociados:** | 1. [cite_start]**Lectura e Inicialización:** Carga de datos actuales y de matrices históricas de 30 días[cite: 41].<br>2. [cite_start]**Análisis Crónico:** Cálculo de promedios aritméticos de los últimos 30 días por contaminante[cite: 41].<br>3. [cite_start]**Modelado Predictivo:** Cálculo de tendencias futuras ponderadas integrando factores climáticos[cite: 41].<br>4. [cite_start]**Evaluación de Reglas:** Disparo de alertas preventivas si la predicción supera el umbral de la OMS[cite: 41].<br>5. [cite_start]**Persistencia:** Apertura, escritura y cierre de archivos físicos de reporte[cite: 41]. |
| **Restricciones:** | [cite_start]• **Normativas:** Uso estricto de los límites diarios de la OMS para evaluar excedencias [cite: 41][cite_start].<br>• **Sociales/Económicas:** Las medidas sugeridas deben modularse para evitar paros económicos totales innecesarios en zonas residenciales sin picos críticos[cite: 41]. |
| **Limitaciones:** | [cite_start]• **Lenguaje:** Programación exclusiva en lenguaje ANSI C estándar [cite: 41][cite_start].<br>• **Estructuras:** Uso obligatorio de arreglos, estructuras, punteros, funciones y archivos [cite: 36, 41][cite_start].<br>• **Dependencias:** Prohibición estricta del uso de librerías externas o de terceros para el manejo de archivos o cálculos avanzados[cite: 39, 41]. |

---

## [cite_start]FASE 2: Planteamiento de Alternativas de Solución [cite: 74]

[cite_start]Para resolver la problemática planteada, se estructuran dos alternativas de arquitectura de software bajo el estándar C[cite: 42]:

### [cite_start]2.1 Alternativa A: Programación Estructurada Plana con Arreglos Paralelos [cite: 75]
* [cite_start]**Descripción:** Los datos se almacenan en múltiples arreglos independientes de tipo primitivo vinculados exclusivamente de forma implícita por su índice común (por ejemplo: `float pm25[5]`, `float no2[5]`, `char nombresZonas[5][50]`)[cite: 42].
* [cite_start]**Lógica Predictiva:** Implementa un promedio lineal simple sobre los 30 días históricos, aplicando condicionales planos secuenciales para modificar el resultado final según las variables climáticas actuales[cite: 37, 42].
* [cite_start]**Control de Persistencia:** Manejo directo de archivos de texto plano dentro del flujo de la función principal `main()`, abriendo y cerrando el flujo en cada interacción[cite: 38].

### [cite_start]2.2 Alternativa B: Programación Modular con Estructuras Anidadas y Punteros [cite: 75]
* [cite_start]**Descripción:** Se define un tipo de dato compuesto (`struct`) para los contaminantes, otro para el clima y se anidan dentro de una estructura unificada llamada `ZonaUrbana`[cite: 36, 42]. [cite_start]Las zonas se manejan en un único arreglo estructurado y se transfieren a funciones especializadas mediante el uso de **punteros** (paso por referencia) para optimizar la memoria[cite: 36, 42].
* [cite_start]**Lógica Predictiva:** Implementación de un algoritmo de **promedio ponderado móvil**, otorgando una ponderación del 60% a los datos de los últimos 5 días (tendencia reciente) y 40% al resto del mes[cite: 37]. [cite_start]El resultado se multiplica por un coeficiente climático dinámico calculado a partir de la interacción termodinámica del viento, la humedad y la temperatura[cite: 30].
* [cite_start]**Control de Persistencia:** Módulo aislado de exportación de reportes que utiliza punteros de tipo `FILE *` para la generación estructurada del archivo físico Markdown[cite: 34, 38].

### [cite_start]2.3 Análisis Comparativo de las Alternativas [cite: 76]
[cite_start]A continuación, se evalúan las ventajas y desventajas de ambas propuestas de diseño, considerando las restricciones y limitaciones del proyecto[cite: 43]:

| Criterio Técnico | Alternativa A (Arreglos Paralelos) | Alternativa B (Estructuras y Punteros) |
| :--- | :--- | :--- |
| **Mantenibilidad del Código** | **Desventaja:** Compleja. Modificar o agregar un nuevo contaminante obliga a redeclarar arreglos paralelos en múltiples firmas de funciones, aumentando el riesgo de desfasamiento de índices. | **Ventaja:** Alta. Encapsular los atributos dentro de un `struct` permite expandir el sistema modificando únicamente la definición de la estructura. |
| **Eficiencia de Memoria (RAM)** | **Desventaja:** Ineficiente. Al no usar punteros, las llamadas a funciones duplican grandes volúmenes de datos en la pila de ejecución (*Stack*). | **Ventaja:** Óptima. Pasar estructuras mediante punteros transmite únicamente una dirección de memoria (4 u 8 bytes), agilizando el rendimiento. |
| **Precisión Matemática** | **Desventaja:** Baja. Un promedio simple oculta los picos de contaminación severos acumulados en los días laborables más recientes. | **Ventaja:** Alta. El promedio ponderado captura la inercia industrial reciente combinada con los coeficientes físicos del clima. |
| **Modularidad y Escalar** | **Desventaja:** Deficiente. Toda la lógica tiende a acumularse en bloques monolíticos difíciles de auditar. | **Ventaja:** Excelente. Separa los requerimientos en funciones independientes y limpias fáciles de depurar. |

---

## [cite_start]FASE 3: Selección de la Solución [cite: 77]

### [cite_start]3.1 Respuestas a Preguntas de Criterio Técnico [cite: 44]

* [cite_start]**¿Cuál de estas alternativas sería la más fácil de implementar y por qué?** [cite: 45]  
  La **Alternativa A** es más sencilla de codificar a corto plazo debido a que utiliza sintaxis básica y directa de arreglos lineales primitivos, evitando la abstracción y el uso explícito de aritmética de punteros o de estructuras anidadas. Sin embargo, se vuelve caótica al escalar la persistencia de datos históricos de 30 días para múltiples contaminantes en paralelo.
* [cite_start]**¿Qué aspectos son importantes al elegir una solución eficiente?** [cite: 46]  
  [cite_start]Es fundamental considerar: la **complejidad espacial** (consumo de memoria RAM al transferir datos), la **complejidad temporal** (velocidad de cómputo en el procesamiento de alertas en tiempo real), la mantenibilidad del código ante auditorías ambientales y la correcta representación lógica de las restricciones reales del entorno sin requerir librerías externas.

### [cite_start]3.2 Selección de la Mejor Alternativa y Fundamentación Técnica [cite: 57, 78]
[cite_start]Se selecciona la **Alternativa B (Programación Modular con Estructuras y Punteros)**.

**Justificación Técnica:** Esta alternativa garantiza un diseño de ingeniería robusto y óptimo. El uso de abstracciones mediante estructuras encapsula el dominio del problema de forma limpia. El uso de punteros permite que el paso de parámetros a través de los requerimientos de cálculo histórico y predicción se realice en **tiempo constante**, evitando copias innecesarias de matrices en la memoria RAM del sistema. [cite_start]Adicionalmente, el modelo predictivo ponderado es el único que satisface la precisión requerida para emitir alertas sanitarias preventivas confiables para la población[cite: 26, 37].

* [cite_start]**¿Cómo garantizaría que la solución seleccionada es la más eficiente en términos de tiempo de ejecución y claridad en el código?**   
  Se garantiza mediante el uso sistemático de la palabra clave `const` en los punteros de funciones de solo lectura (como `monitorearContaminacionActual`), lo que permite al compilador de C aplicar optimizaciones de bajo nivel en el uso de registros del procesador. La claridad queda asegurada al aislar cada requerimiento funcional en un bloque funcional único, con responsabilidades acotadas y libre de efectos secundarios en variables globales.

---