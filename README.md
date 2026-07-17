# Sistema de Teleoperación Inercial Basado en ESP32 y Filtro de Kalman

![Estado](https://img.shields.io/badge/Estado-Completado-success)
![Hardware](https://img.shields.io/badge/Hardware-ESP32%20%7C%20MPU6050-blue)
![Licencia](https://img.shields.io/badge/Licencia-MIT-green)

Este repositorio contiene el código fuente, modelos 3D y documentación de un sistema de teleoperación de bajo costo desarrollado como proyecto de tesis. El sistema utiliza una red inercial distribuida (dos sensores MPU-6050) para capturar los movimientos del brazo humano y transmitirlos a un entorno digital o brazo robótico esclavo.

<p align="center">
  <!-- Reemplaza el enlace de abajo por la ruta de tu foto real del prototipo -->
  <img src="docs/Prototipo.jpg" alt="Foto del Prototipo" width="600"/>
</p>

## 🚀 Características Principales

*   **Fusión Sensorial:** Implementación desde cero de un **Filtro de Kalman 1D** en C++ para mitigar la deriva del giroscopio y el ruido inercial del acelerómetro.
*   **Estimación Articular:** Uso de orientación relativa entre dos IMUs (brazo y antebrazo) para aislar la cinemática del codo.
*   **Gemelo Digital:** Visualización fluida y sin *gimbal lock* mediante el uso de cuaterniones extraídos a través del DMP interno del sensor.
*   **Pipeline de Baja Latencia:** Transmisión de datos seriales optimizada a 115200 baudios para garantizar la "transparencia" del sistema y evitar retrasos (time delay).
*   **Hardware Accesible:** Diseño modular imprimible en 3D y basado en microcontroladores de bajo costo (ESP32).

## 📁 Contenido del Repositorio

La organización de los archivos refleja las diferentes etapas del proyecto:

*   `/01_Pruebas_Hardware`: Códigos de diagnóstico para validar la conexión concurrente de los sensores.
*   `/02_Impresion_3D`: Archivos en formato `.STL` listos para laminado e impresión 3D de los brazaletes ajustables.
*   `/03_Filtros_y_Control`: Implementación algorítmica principal, incluyendo el Filtro de Kalman en C++ y scripts de Python para análisis de señales.
*   `/04_Visualizacion`: Código dedicado a la extracción de cuaterniones y empaquetado de datos para renderizado en tiempo real (Blender).
*   `/Software_teleoperacion`: Entorno de desarrollo para iteraciones del control y scripts complementarios.

## 🛠️ Requisitos de Hardware

Para replicar este proyecto necesitarás:

*   1x Microcontrolador ESP32 (o equivalente).
*   2x Módulos IMU MPU-6050.
*   Cables jumper y protoboard (o placa PCB personalizada).
*   Cinta velcro (para ajustar los brazaletes impresos en 3D).

## ⚙️ Instalación y Uso

1.  **Impresión 3D:** Imprime los archivos ubicados en la carpeta `/02_Impresion_3D`. Recomendado usar PLA o PETG con un 20% de relleno.
2.  **Conexión Electrónica:**
    *   Conecta ambos MPU-6050 al bus I2C del ESP32 (Pines SDA=21 y SCL=22).
    *   *Nota importante:* Para usar dos sensores en el mismo bus, debes cambiar la dirección I2C de uno de ellos conectando su pin `AD0` a 3.3V (cambiando su dirección de `0x68` a `0x69`).
3.  **Software:**
    *   Abre los archivos `.ino` (por ejemplo, en `03_Filtros_y_Control`) en Arduino IDE.
    *   Asegúrate de tener instaladas las librerías `Wire.h`, `Adafruit_MPU6050` y `Adafruit_Sensor`.
    *   Sube el código a la placa estableciendo la velocidad a 115200 baudios.
4.  **Prueba:** Abre el Monitor Serie a 115200 baudios o ejecuta los scripts de Python adjuntos para observar el flujo de datos filtrados en tiempo real.

## 📊 Resultados del Filtrado

A continuación se muestra el impacto de la implementación matemática del Filtro de Kalman sobre la señal inercial bruta del acelerómetro frente a movimientos oscilatorios:

<p align="center">
  <!-- Reemplaza el enlace por la gráfica de Kalman -->
  <img src="docs/grafica_kalman_resultado.png" alt="Comparativa Filtro de Kalman" width="600"/>
</p>

## 📝 Referencias Principales

Este proyecto se fundamenta teóricamente en los siguientes trabajos académicos:

*   Anderson, R. J., & Spong, M. W. (1989). *Bilateral control of teleoperators with time delay*.
*   Kalman, R. E. (1960). *A new approach to linear filtering and prediction problems*.
*   Madgwick, S. O. H. (2010). *An efficient orientation filter for inertial and inertial/magnetic sensor arrays*.

---
*Desarrollado como parte de un proyecto de investigación para la obtención de grado.*
