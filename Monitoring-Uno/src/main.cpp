#include <Arduino.h>
// библиотека для работы с датчиками MQ (Troyka-модуль)
#include <TroykaMQ.h>

// nome do pino ao qual o sensor está conectado / pino ADC
#define PIN_MQ9         A0
#define PIN_MQ135       A1
// nome do pino ao qual o aquecedor do sensor está conectado
#define PIN_MQ9_HEATER  8
#define PIN_MQ135_HEATER  7

// cria um objeto para trabalhar com o sensor
// e enviar-lhe o número do pino do sinal de saída e o aquecedor
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);
MQ135 mq135(PIN_MQ135, PIN_MQ135_HEATER);

void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  // запускаем термоцикл
  // в течении 60 секунд на нагревательный элемент подаётся 5 вольт
  // в течении 90 секунд — 1,5 вольта
  mq9.cycleHeat();
  mq135.heaterPwrHigh();
  Serial.println("Heated sensor");
}

void loop()
{
  // если прошёл интервал нагрева датчика
  // и калибровка не была совершена
  if (!mq9.isCalibrated() && mq9.atHeatCycleEnd()) {
    // выполняем калибровку датчика на чистом воздухе
    mq9.calibrate();
    // при знании сопративления датчика на чистом воздухе
    // можно его указать вручную, допустим 7.2
    // mq9.calibrate(7.2);
    // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
    Serial.print("Ro Mq9 = ");
    Serial.println(mq9.getRo());
    // запускаем термоцикл
    mq9.cycleHeat();
  }
  if (!mq135.isCalibrated() && mq135.heatingCompleted()) {
    // выполняем калибровку датчика на чистом воздухе
    mq135.calibrate();
    // при знании сопративления датчика на чистом воздухе
    // можно его указать вручную, допустим 160
    // mq135.calibrate(160);
    // выводим сопротивление датчика в чистом воздухе (Ro) в serial-порт
    Serial.print("Ro Mq135 = ");
    Serial.println(mq135.getRo());
  }
  // если прошёл интевал нагрева датчика
  // и калибровка была совершена
  if (mq9.isCalibrated() && mq9.atHeatCycleEnd()) {
    // выводим отношения текущего сопротивление датчика
    // к сопротивлению датчика в чистом воздухе (Rs/Ro)
    Serial.print("Ratio: ");
    Serial.print(mq9.readRatio());
    // выводим значения газов в ppm
    Serial.print(" LPG: ");
    Serial.print(mq9.readLPG());
    Serial.print(" ppm ");
    Serial.print(" Methane: ");
    Serial.print(mq9.readMethane());
    Serial.print(" ppm ");
    Serial.print(" CarbonMonoxide: ");
    Serial.print(mq9.readCarbonMonoxide());
    Serial.println(" ppm ");
    delay(200);
    // запускаем термоцикл
    mq9.cycleHeat();
  }
  if (mq135.isCalibrated() && mq135.heatingCompleted()) {
    // выводим отношения текущего сопротивление датчика
    // к сопротивлению датчика в чистом воздухе (Rs/Ro)
    Serial.print("Ratio: ");
    Serial.print(mq135.readRatio());
    // выводим значения газов в ppm
    Serial.print("\tCO2: ");
    Serial.print(mq135.readCO2());
    Serial.println(" ppm");
    delay(2000);
  }
}
