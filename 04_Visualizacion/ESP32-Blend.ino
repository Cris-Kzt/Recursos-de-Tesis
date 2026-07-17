#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Los pines D21 y D22 son los predeterminados para I2C en ESP32
MPU6050 mpu;

// Variables de estado del MPU
bool dmpReady = false;  
uint8_t mpuIntStatus;   
uint8_t devStatus;      
uint16_t packetSize;    
uint16_t fifoCount;     
uint8_t fifoBuffer[64]; 

// Variables para cálculos de orientación
Quaternion q;           // [w, x, y, z] contenedor de cuaterniones
VectorFloat gravity;    // [x, y, z] vector de gravedad
float ypr[3];           // [yaw, pitch, roll] contenedor para yaw/pitch/roll

// Buffer para enviar el mensaje serial
char msgBuffer[50];

void setup() {
  // Unirse al bus I2C (MPU6050 tiene dirección 0x68 por defecto)
  Wire.begin();
  
  // Iniciar la comunicación serial. IMPORTANTE: baudrate 115200 como en Blender
  Serial.begin(115200);
  while (!Serial); // Esperar a que el puerto serial se abra (para depuración)

  Serial.println(F("Iniciando MPU6050..."));
  mpu.initialize();

  // Verificar la conexión
  Serial.println(F("Verificando conexión..."));
  if (!mpu.testConnection()) {
    Serial.println(F("Conexión fallida. Revisa el cableado."));
    while(1); // Detener ejecución
  }
  Serial.println(F("Conectado con éxito."));

  // Iniciar e inicializar el DMP (Digital Motion Processor)
  Serial.println(F("Iniciando DMP..."));
  devStatus = mpu.dmpInitialize();

  // Asegúrate de usar tus propios offsets aquí para mejor precisión:
  // mpu.setXAccelOffset(-3674);
  // mpu.setYAccelOffset(-1499);
  // mpu.setZAccelOffset(1573);
  // mpu.setXGyroOffset(83);
  // mpu.setYGyroOffset(42);
  // mpu.setZGyroOffset(-35);

  // devStatus == 0 si la inicialización funcionó
  if (devStatus == 0) {
    // Activar el DMP
    Serial.println(F("Activando DMP..."));
    mpu.setDMPEnabled(true);
    dmpReady = true;

    // Obtener el tamaño del paquete esperado para FIFO
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = fallo en carga de memoria inicial, 2 = fallo en actualizaciones de configuración del DMP
    Serial.print(F("Error en DMP (código "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void loop() {
  // Si no está listo para DMP, no hacemos nada
  if (!dmpReady) return;

  // Leer el último paquete del FIFO
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { 
    // Obtener cuaterniones en 
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    // Obtener vector de gravedad en gravedad mundial
    mpu.dmpGetGravity(&gravity, &q);
    // Calcular yaw/pitch/roll en radianes
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // Convertir radianes a grados
    float yaw = ypr[0] * 180 / M_PI;
    float pitch = ypr[1] * 180 / M_PI;
    float roll = ypr[2] * 180 / M_PI;

    // --- FORMATEO CLAVE PARA BLENDER ---
    // Usamos sprintf para formatear la línea en el formato exacto:
    // R:%.2f,P:%.2f,Y:%.2f -> Roll:%, Pitch:%, Yaw:% con 2 decimales
    snprintf(msgBuffer, sizeof(msgBuffer), "R:%.2f,P:%.2f,Y:%.2f", roll, pitch, yaw);
    
    // Enviar el mensaje por el puerto serial
    Serial.println(msgBuffer);

    // Un pequeño retraso para no saturar el bus y dar tiempo al bucle modal de Blender
    delay(10); 
  }
}