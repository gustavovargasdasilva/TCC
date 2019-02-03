//#include <SoftwareSerial.h>

//SoftwareSerial myserial(8, 9);//rx8 tx9

char c = '1';
char d = '2';
unsigned long intervalo = 0;

char pacote[11] = {'0', '0', '0', '0', '0','0', '0', '0', '0', '0', '0'};
//char pacote[5];

void setup() {
  Serial.begin(9600);
  //myserial.begin(9600);
}

void loop() {

  while(Serial.available() > 0)
  {
    delay(100);
    
    Serial.print("Lendo Pacote: ");

    
      for (int i = 0; i<11; i++)
      {
        if(pacote[0] = 'B')
        {
         pacote[i] = Serial.read();
        }
      }
    Serial.println(pacote);
    //fazer verificacao se o pacote ta certo
    //nao printar numeros que nao estao na escala certa do sensor
    //atribuir um numero ou concatenar dois pacote[2] = tibia; pacote[3] = tibia2
    }
  delay(700);
}
