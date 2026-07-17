import serial
import matplotlib.pyplot as plt
import time

PUERTO = 'COM3'      # Cambia si es necesario
BAUD_RATE = 115200   
MUESTRAS = 300       

datos_ruidosos = []
datos_kalman = []

print(f"Conectando al ESP32 en el puerto {PUERTO}...")

try:
    ser = serial.Serial(PUERTO, BAUD_RATE, timeout=1)
    time.sleep(2) 
    
    print("¡Conexión exitosa! Mueve tu sensor MPU6050...")
    
    while len(datos_ruidosos) < MUESTRAS:
        linea = ser.readline().decode('utf-8').strip()
        
        if linea:
            try:
                valores = linea.split(',')
                if len(valores) == 2:
                    datos_ruidosos.append(float(valores[0]))
                    datos_kalman.append(float(valores[1]))
            except ValueError:
                pass 
                
    ser.close()
    print("Grabación terminada. Generando gráfica...")

    plt.figure(figsize=(10, 5)) 
    
    plt.plot(datos_ruidosos, label='Ángulo Bruto (Ruido)', color='lightcoral', linestyle='--', linewidth=1.5, alpha=0.8)
    plt.plot(datos_kalman, label='Ángulo Filtrado (Kalman)', color='darkred', linewidth=2.5)
    
    plt.title('Comparación: Señal Bruta vs Filtro de Kalman', fontsize=14, fontweight='bold')
    plt.xlabel('Muestras en el Tiempo', fontsize=12)
    plt.ylabel('Ángulo de Inclinación Pitch (°)', fontsize=12)
    
    plt.legend(loc='upper right', fontsize=10)
    plt.grid(True, linestyle=':', alpha=0.7)
    
    nombre_archivo = 'grafica_comparativa_kalman_tesis.png'
    plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
    
    plt.show()

except serial.SerialException:
    print(f"Error: No se pudo abrir el puerto {PUERTO}.")
except Exception as e:
    print(f"Ocurrió un error inesperado: {e}")