#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>
#include <Stepper.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// Motor paso a paso
int pines_motor[] = {1, 2, 3, 4};
int steps_per_revolution = 0;
int speed = 0;
int position = 0;
Stepper motor(steps_per_revolution, pines_motor[0], pines_motor[1], pines_motor[2], pines_motor[3]);

int pasos_maximos = 0;

int pin_limit_switch = 0;

int pin_button_start = 0;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
    std::string value = pChar->getValue();

    Serial.print("Comando recibido: ");
    Serial.println(value.c_str());

    if (value == "ERASER") {
      Serial.println("Limpiando...");

      // Mover el borrador
      moverMotor(pasos_maximos);
      int limit_switch = digitalRead(pin_limit_switch);
      if (limit_switch == LOW)
      {
        // Regresar el borrador
        moverMotor(-pasos_maximos);
        pChar->setValue("Limpiado");
        Serial.println("Limpiado");

      } else {
        pChar->setValue("Error al limpiar");
        Serial.println("Error al limpiar");
      }

    } else {
      pChar->setValue("Comando desconocido");
      Serial.println("Comando desconocido");
    }
  }
};

void moverMotor(int steps) {
  position += steps;
  motor.step(steps);
}

void setup() {
  Serial.begin(115200);

  // Setup stepper
  motor.setSpeed(speed);

  pinMode(pin_limit_switch, INPUT);
  pinMode(pin_button_start, INPUT);

  BLEDevice::init("EraserBot");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Esperando comando");

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("Esperando conexión BLE...");
}

void loop() {

  int button_start = digitalRead(pin_button_start);
  
  if (button_start == HIGH) {
  {
    // Mover el borrador
    moverMotor(pasos_maximos);
    int limit_switch = digitalRead(pin_limit_switch);
    if (limit_switch == LOW)
    {
      // Regresar el borrador
      moverMotor(-pasos_maximos);
      Serial.println("Limpiado");

    } else {
      Serial.println("Error al limpiar");
    }

  }
  
}