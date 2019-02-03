//#include <SoftwareSerial.h>
//SoftwareSerial myserial(8, 9);//rx8 tx9

#include <Printers.h>
#include <XBee.h>
#include <OneWire.h>

OneWire  ds(2);
void sensorTemperatura();

int cebola = 44; //variavel teste sensor
int i = 0; //for
int sensor1 = '8'; //variavel teste sensor1
int sensor2 = '8'; //variavel teste sensor2

int leitura = 0; //leitura variavel sensor de temperatura
int leitura2 = 0; //leitura variavel sensor de temperatura
char sensorTemp[4];
unsigned long intervalo = 0; //intervalo millis
char pacote[11] = {'B', '0' , '0' , '0', '0', '0', '0', '0', '0', 'C', '0'};

//ph
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
unsigned long int avgValue;    //Store the average value of the sensor feedback
char sensorph[3];

void setup()
{
  // myserial.begin(9600);

  Serial.begin(9600);
  intervalo = millis();
}

void loop()
{
  if (i == 0)
  {
    Serial.println("A"); //iniciando conexao xbee
    i++;
  }

  sensorTemperatura();

  pacote[1] = (cebola / 10) + '0';
  pacote[2] = (cebola % 10) + '0';

  pacote[3] = sensorTemp[0];
  pacote[4] = sensorTemp[1];
  pacote[5] = sensorTemp[2];
  pacote[6] = sensorTemp[3];

  Serial.print(pacote);

  //medindo ph
  int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue+Offset;                      //convert the millivolt into pH value

  phValue = phValue * 100;
  int leituraph = (int)phValue;
//  int leituraph = (int)phValue + 100;
  sprintf(sensorph, "%d", leituraph);
  
  Serial.print("pH:");  
  Serial.println(phValue,2);

  Serial.print("pH char:");  
  Serial.println(leituraph);
  
  pacote[7] = sensorph[0];
  pacote[8] = sensorph[1];
  pacote[9] = sensorph[2];

  while (Serial.available() > 0)
  {
    if (millis() - intervalo > 3000)
    {
      intervalo = millis();
      Serial.write("");

      Serial.write(pacote);
    }
  }
}

void sensorTemperatura()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;

  celsius = celsius * 100;
  leitura = (int)celsius;
  sprintf(sensorTemp, "%d", leitura);

  //  Serial.print("Celsius: ");
  //  Serial.println(sensorTemp);


  //  leitura = leitura * 100;
  //  leitura2 = (int)leitura; //convertendo float pra int
  //  sprintf(sensorTemp, "%d", leitura2); //convertendo int(leitura2)pra array(char)
}
