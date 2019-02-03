#define ADC_data  3
#define ADC_sck   2

int media = 0;
int valores[30];
int j = 0;
int k = 0;
int flag = 0;

int maximoValor = 0; //trocar por função módulo ou multiplica por -1 if(outputValue < 0) outputvalue = outputValue * (-1)
int minimoValor = 0;

int condut = 0;

void setup() {
  Serial.begin(250000);
  Serial.println("Starting...");

  pinMode(ADC_data,INPUT_PULLUP);
  pinMode(ADC_sck,OUTPUT);
//  pinMode(13,OUTPUT);
  //digitalWrite(ADC_sck,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  long Count=ReadCount();
  delay(500);

  float sensorValue = 0;     
  int outputValue = 0;   
  float ReadValue = 0;
  int ReadTimes = 0;
  for (int i =0; i < 50; i++)
    {
    ReadValue = Count;
//    Serial.println(ReadValue);
    if (ReadValue > 0)
      {
      sensorValue = sensorValue + ReadValue;
      ReadTimes++;         
      delay(2);
      }
    }
  outputValue = sensorValue / ReadTimes;

  outputValue = outputValue * (-1);
  
  Serial.print("Media: ");
  Serial.println(outputValue);

  if(maximoValor == 0)
    maximoValor = outputValue;

  if(minimoValor == 0)
    minimoValor = outputValue;

  if(maximoValor - outputValue > 500)
    maximoValor = outputValue;

  if(minimoValor - outputValue > 500)
    minimoValor = outputValue;
  
  if(outputValue > maximoValor)
  {
    Serial.println("entrei no maximo");
    maximoValor = outputValue;
  } 

  Serial.print("Máximo: "); 
  Serial.println(maximoValor);
  
  if(outputValue < minimoValor)
  {
     Serial.println("entrei no minimo");
     minimoValor = outputValue;
  }
  
  Serial.print("Minimo: "); 
  Serial.println(minimoValor);

  Serial.print("Condutivimetro: ");

  condut = analogRead(A1);
  Serial.println(condut);
}

unsigned long ReadCount(void){
   
   unsigned long Count=0;
   int i;
  
   digitalWrite(ADC_data,HIGH);
   digitalWrite(ADC_sck,LOW);
   
   Count=0;
   
   while(digitalRead(ADC_data));
   
      for (i=0;i<24;i++){
        digitalWrite(ADC_sck,HIGH);
        Count=Count<<1;
        digitalWrite(ADC_sck,LOW);
        
        if(digitalRead(ADC_data)==1){
          Count++;
        }
  
      }
    
    digitalWrite(ADC_sck,HIGH);
    Count=Count^0x800000;
    //delayMicroseconds(1);
    digitalWrite(ADC_sck,LOW);
    
    Serial.print(Count);
    Serial.print("\t");
    Serial.println(Count,2);
    return(Count);
    delay(50);
}
