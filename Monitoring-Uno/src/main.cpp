#include <Arduino.h>
#include <TroykaMQ.h>

// nome do pino ao qual o sensor est� conectado / pino ADC
#define PIN_MQ135       A1
#define PIN_MQ9         A0
// nome do pino ao qual o aquecedor do sensor est� conectado
#define PIN_MQ135_HEATER  7
#define PIN_MQ9_HEATER  8
//cria um objeto para trabalhar com o sensor
// e enviar-lhe o n�mero do pino do sinal de sa�da e o aquecedor
MQ135 mq135(PIN_MQ135, PIN_MQ135_HEATER);
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);

void setup()
{
  Serial.begin(9600);
  mq135.heaterPwrHigh();
  Serial.println("Heated sensor");
  mq9.cycleHeat();

}

void loop()
{
  int i = 0;
  double amostra_ratio_co2 = 0;
  double amostra_co2 = 0;
  double somappm_co2 = 0;
  double mediappm_co2 = 0;
  double amostraratioco = 0;
  double amostraco = 0;
  double somappm_co = 0;
  double mediappm_co = 0;


  if (!mq135.isCalibrated() && mq135.heatingCompleted()) {
    mq135.calibrate(395.67);  //Possivel especificar manualmente dentro do parenteses
  }
  if (!mq9.isCalibrated() && mq9.heatingCompleted()) {

    mq9.calibrate(8.62);  //Possivel especificar manualmente dentro do parenteses
    mq9.cycleHeat();
}
  if ((mq135.isCalibrated() && mq135.heatingCompleted()) && (mq9.isCalibrated() && mq9.heatingCompleted())) {
    /*&& (mq9.isCalibrated() && mq9.heatingCompleted())*/
    /*Serial.print("Ratio: ");
    Serial.print(mq135.readRatio()); // Fun��o que calcula o Ratio
    Serial.print("\tCO2: ");
    Serial.print(mq135.readCO2());   // Fun��o que relaciona o Ratio com (ppm)
    Serial.println(" ppm");
    delay(2000);
    */

    for(i=0; i<3; i++){
      mq135.readRatio();
      mq9.readRatio();
      amostra_ratio_co2 = mq135.readRatio();
      amostraratioco = mq9.readRatio();
      amostra_co2 = mq135.readCO2();
     amostraco = mq9.readCarbonMonoxide();
      somappm_co2 += amostra_co2;
     somappm_co = amostraco;
      delay(1000);
    }
    mediappm_co2 = somappm_co2 / 3;
   mediappm_co = somappm_co / 3;
    //Serial.print("MediappmCO2: ");
    Serial.print(mediappm_co2);
  //  Serial.print("MediappmCO: ");
    Serial.print(";");
   Serial.println(mediappm_co);
    //delay(2000);
  }
}
