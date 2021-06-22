int LED=13;
//V. medicion sensores
double ms1;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ms1=sensor1();
  if ms1>=10 {
      
  }
}

double sensor1(){
  double medida=analogRead(A2)*5./1024.;
  //Serial.println(medida);
  double dis=13.5813*pow(medida,-1.31406);
  delay(16);
  return dis;
}
