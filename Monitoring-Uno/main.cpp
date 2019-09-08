#include <Arduino.h>
#include <TroykaMQ.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Ticker.h>
#include <DHT.h>
#include <SPI.h>          // Biblioteca de comunicação SPI Nativa
#include <SD.h>           // Biblioteca de comunicação com cartão SD Nativa

const int chipSelect = 4;

// nome do pino ao qual o sensor está conectado / pino ADC
#define PIN_MQ9         A0
#define PIN_MQ135       A1
// nome do pino ao qual o aquecedor do sensor está conectado
#define PIN_MQ9_HEATER  8
#define PIN_MQ135_HEATER  7

static const int RXPin = 6, TXPin = 7; // 6 7
static const uint32_t GPSBaud = 9600;

// cria um objeto para trabalhar com o sensor
// e enviar-lhe o número do pino do sinal de saída e o aquecedor
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);
MQ135 mq135(PIN_MQ135, PIN_MQ135_HEATER);
DHT dht;
File dataFile;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
void displayInfo();

long latitude = 0;
long longitude = 0;
int dia, mes, ano = 0;
int hora, minutos, segundos, centisecond = 0;

int CO, CO2 = 0;
float temperatura, umidade = 0;
double MQ135R0, MQ9R0 = 0;
double RatioMQ9, RatioMQ135 = 0;

bool cartaoOk = true;
String leitura, localizacao, data, sensores = "";

void setup()
{

  dht.setup(2); // data pin 2
  Serial.begin(115200);

  ss.begin(GPSBaud);
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());

  mq9.cycleHeat();
  mq135.heaterPwrHigh();
  Serial.println("Heated sensor");

  if (!SD.begin(chipSelect)) {
    Serial.println("Erro na leitura do arquivo não existe um cartão SD ou o módulo está conectado corretamente ?");
    cartaoOk = false;
    return;
  }
}

void loop()
{

  if (cartaoOk){
      dataFile = SD.open("datalog.csv", FILE_WRITE);
      Serial.println("Cartão SD Inicializado para escrita :D ");
    }
 displayInfo();
}
void displayInfo()
{
  if (gps.location.isValid())
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
  else
  {
    Serial.print(F("INVALID;"));
  }

//  Serial.print(F(";Date/Time: "));
  if (gps.date.isValid())
  {
    dia = gps.date.day();
    mes = gps.date.month();
    ano = gps.date.year();
  }
  else
  {
    Serial.print(F("INVALID;"));
  }

  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    hora = gps.time.hour();

    if (gps.time.minute() < 10) Serial.print(F("0"));
    minutos = gps.time.minute();

    if (gps.time.second() < 10) Serial.print(F("0"));
    segundos = gps.time.second();

    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    centisecond = gps.time.centisecond();

  }
  else
  {
    Serial.print(F("INVALID;"));
  }

  if (!mq9.isCalibrated() && mq9.atHeatCycleEnd()) {
    //MQ9R0 = mq9.getRo();
    //Serial.print("Ro MQ9: ");
    //Serial.println(MQ9R0);
    mq9.calibrate(8.39);
    mq9.cycleHeat();
  }
  if (!mq135.isCalibrated() && mq135.heatingCompleted()) {

    mq135.calibrate(621.06);

    //MQ135R0 = mq135.getRo();
    //Serial.print("Ro MQ135: ");
    //Serial.println(MQ135R0);

  }
  if (mq9.isCalibrated() && mq9.atHeatCycleEnd()) {

    RatioMQ9 = mq9.readRatio();
    CO = mq9.readCarbonMonoxide();

    delay(2000);
    mq9.cycleHeat();
  }
  if (mq135.isCalibrated() && mq135.heatingCompleted()) {

    RatioMQ135 =  mq135.readRatio();
    CO2 = mq135.readCO2();


    delay(2000);
  }

  temperatura = dht.getTemperature(); // Função que realiza a leitura da temperatura do sensor dht11
  umidade = dht.getHumidity();        // Função que realiza a leitura da umidade do sensor dht11

  if (isnan(temperatura) || isnan(umidade)){
    Serial.println ("Falha na leitura do sensor");  // Informar ao usuário que houve uma falha na leitura do sensor
    delay(1500);    // Aguardamos 1,5 segundos
    return ;        // Retornamos a função para o seu ponto inicial
  }

  localizacao = String(String(latitude) + ";" + String(longitude));
  data = String(String(dia) + "/" + String(mes) + "/" + String(ano) + ";" + String(hora) + ":" + String(minutos) + ":" + String(segundos));
  sensores = String(String(CO) + ";" + String(CO2) + ";" + String(temperatura) + ";" + String(umidade));
  leitura = String(String(localizacao) + ";" + String(data) + ";" + String(sensores));
  Serial.println(sensores);
  Serial.println(leitura);
  if (dataFile) {
    Serial.println(leitura);    // Mostramos no monitor a linha que será escrita
    dataFile.println(leitura);  // Escrevemos no arquivos e pulamos uma linha
    dataFile.close();           // Fechamos o arquivo
  }
}
