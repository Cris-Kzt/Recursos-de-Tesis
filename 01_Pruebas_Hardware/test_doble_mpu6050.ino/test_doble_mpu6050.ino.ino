#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Crear los objetos para cada sensor
Adafruit_MPU6050 mpu1;
Adafruit_MPU6050 mpu2;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  Serial.println("Iniciando prueba de doble MPU6050 en ESP32...");

  // Inicializar I2C en el ESP32 (SDA = 21, SCL = 22)
  Wire.begin(21, 22);

  // Iniciar el primer sensor (Dirección por defecto 0x68 - Pin AD0 a GND)
  if (!mpu1.begin(0x68, &Wire)) {
    Serial.println("¡Error! No se encuentra el MPU6050 #1 (0x68). Revisa los cables.");
  } else {
    Serial.println("MPU6050 #1 (0x68) detectado correctamente.");
  }

  // Iniciar el segundo sensor (Dirección 0x69 - Pin AD0 a 3.3V)
  if (!mpu2.begin(0x69, &Wire)) {
    Serial.println("¡Error! No se encuentra el MPU6050 #2 (0x69). Revisa el pin AD0 y los cables.");
  } else {
    Serial.println("MPU6050 #2 (0x69) detectado correctamente.");
  }

  Serial.println("");
  delay(1000);
}

void loop() {
  // Variables para guardar las lecturas
  sensors_event_t a1, g1, temp1;
  sensors_event_t a2, g2, temp2;

  // Leer los eventos de ambos sensores
  mpu1.getEvent(&a1, &g1, &temp1);
  mpu2.getEvent(&a2, &g2, &temp2);

  // Imprimir una lectura básica para comprobar que mandan señal
  // (Mostramos la aceleración en el eje X de ambos)
  Serial.print("Sensor 1 (X): ");
  Serial.print(a1.acceleration.x);
  Serial.print(" m/s^2  \t|\t ");
  
  Serial.print("Sensor 2 (X): ");
  Serial.print(a2.acceleration.x);
  Serial.println(" m/s^2");

  // Pequeña pausa para no saturar el monitor serie
  delay(300);
}
