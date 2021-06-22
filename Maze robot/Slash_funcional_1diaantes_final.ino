#include <Servo.h>                //LIBRERÍAS COMUNICACION I2C
#include "I2Cdev.h"
#include "Wire.h"
#include <MadgwickAHRS.h>
#include <IMU_MPU6050.h>
Servo Freno_I;                    //OBJETO FRENOS
Servo Freno_D;
Madgwick filter;                  //OBJETO FILTRO MADGWICK
MPU6050 imu;
int uF = 0;
int cont_sf=0;
float roll;                       //IMU
float pitch;                      //IMU
float yaw;                        //IMU
long Serialtime;                  //IMU
long last_time;                   //IMU
long microsPerReading;            //IMU

double kpf=4200;//6400;//3700
double kdf=2400;//1050;
double kif=3;   //control

int dy2,cdy2;
int cdd=0;
double ang_now, ang_referencia;   // ang_referencia: ang de comparación; ang_now: ángulo actual
double di, dd, df;                //dato de sensores de distancia
int var;                          //bandera variable para case
int cont_W=0;                     //contador para salir del while de giro
int cont;                         //contador sensor frente; saturación distancia
String proceso;

//motorderecho
int A1 = PB14;                    //PUENTE H
int A2 = PB15;                    //PUENTE H
int PWMA = PA8;                   //PUENTE H
//motorizquierdo
int B_1 = PB12;                   //PUENTE H
int B_2 = PB13;                   //PUENTE H
int PWMB = PA9;                   //PUENTE H



/*********variables de controladores********/
////frontal
int errorF = 0;
int errorF_1 = 0;
int errorF_2 = 0;
int errorF_3 = 0;


void setup()
{
  Wire.setClock(400000);        //reloj i2c a 400khz
  Wire.begin();
  Wire.beginTransmission(0x68); //Inicia la comunicación en el MPU
  Wire.write(0x6B);             //empieza a escribir en el registo PWR_MGMT_1
  Wire.write(0x00);             //Setea el registro en 0
  Wire.endTransmission();
  Serial.begin(115200);         //Comunicación serial
  filter.begin(25);             //empieza filtro
  microsPerReading = 1000 / 25; //Parámetros para el filtro
  Serialtime = 50;
  last_time = millis();
  imu.calibrateMPU6050(imu.gyroBias, imu.accelBias);
  imu.initMPU6050();

  pinMode(PA4, INPUT_ANALOG);   //SENSOR DERECHA
  pinMode(PA7, INPUT_ANALOG);   //SENSOR IZQUIERDA
  pinMode(PA6, INPUT_ANALOG);   //SENSOR FRENTE
  pinMode(PC13, OUTPUT);        //LED
  pinMode(A1, OUTPUT);          //PUENTE H
  pinMode(A2, OUTPUT);          //PUENTE H
  pinMode(B_1, OUTPUT);         //PUENTE H
  pinMode(B_2, OUTPUT);         //PUENTE H
  pinMode(PA10, OUTPUT);        //PUENTE H
  pinMode(PWMA, PWM);           //PUENTE H
  pinMode(PWMB, PWM);           //PUENTE H

  Freno_D.attach(PB9);          //FRENO IZQUIERDO
  Freno_I.attach(PB8);          //FRENO DERECHO
  Freno_I.write(55);            //ANGULO INICIAL IZQ
  Freno_D.write(130);           //ANGULO INICIAL DER
  //delay(16);
  while (dis_frente() > 10) {}
  while (dis_frente() < 10) {}
}

////////////////////////*****LOOP*****////////////////////
void loop()
{
  digitalWrite(PA10, HIGH);                        //STBY del puente H
  //imu.ay = (float)imu.accelCount[0] * imu.aRes;    //Aceleración en Y
  
  //*************lectura de sensores***************//
  di = dis_i();       // distancia izq
  df = dis_frente();  // distancia frente
  dd = dis_der();     // distancia derecha
  ang_now = YAW();    //ángulo yaw


  if (imu.ay > -0.2  && imu.ay < 0.2 ){

    dy2 = 0;
    cdy2++;
  }

  if (dd > 13)cdd++;
  
//*************************** CONDICIONES PARA EL CASE **********************************//

  if (dd > 3 && dd<12 )        //si la distancia derecha es mayor a 8 seguir hacia adelante
  {
    casos_analisis(1);
  }

  if (dd < 3  )                //si distancia derecha < 7 corregir derecha.... está muy a la derecha
  {
    casos_analisis(2);
  }

  if  (  (dd > 8  && di  > 0 && di < 10) || dd>9 )    //distancia derecha >7 y  5 < dist izq < 7 corregir izquierda... está muy a la izquierda
  {
    casos_analisis(3);
  }
    
  if(dd<4&&df>15){
    casos_analisis(13);
  }

  if  (  dd > 8  && di  > 12 && df>25)    //Hueco izauierda
  {
    casos_analisis(3);
  }
  
  if  ((dd > 13 ) || ( df < 13 && dd > 12 && di > 12 ))       // distancia derecha sin pared....... debe girar a la derecha
  {
     if(df>10 && di > 8){
      casos_analisis(4);
    }
  
    if(df < 10 && di < 8)
    {
      casos_analisis(4);
      casos_analisis(1);
      delay(15);
    }
    
      if(df<10){
      retro();
      delay(100);
      casos_analisis(4);
    }

  }

  if  (dd < 13 && df<23 && di>13)           // girar IZQUIERDA.......... pared al frente y pared a la derecha
  { 


    if(df<10){
      retro();
      delay(100);
    }

    casos_analisis(5);
  }

//    if  (df < 10 )                          //parar
//  {
//    casos_analisis(8);
//  }
//  
  if  ((df < 10  && (di < 6 || dd <6))||(df>15 && dd<5 && di>13) || (df < 10 && dd < 5 && di > 13) || (df<10 && dd < 10 && di< 10 ) )        // retro    ...(dy2 == 0 && cdy2 > 2000)
  {
    casos_analisis(6);                      //RETRO
    cdy2=0;
  }
  if  (df <24 && di < 12 && dd < 12 )       //giro U
  {
    if(df<10){
      retro();
      delay(100);
    }

    
    casos_analisis(7);
  }
}



void casos_analisis(int var){
  
  switch (var)

  {
    case 1 :
      control();
      adelante();
      control();
      proceso="adelante";
     // Serial.println(proceso);

      break;


    case 2 :

      corregir_der();
      //control();
      proceso="corregir derecha";
      //Serial.println(proceso);

      break;
    case 3 :

      corregir_iz();
      //control();
      proceso="corregir izquierda";
      //Serial.println(proceso);

      break;

    case 4 :

   
      girar_der();                // gira a la derecha
     control();
      break;
      proceso="giro derecha";
     // Serial.println(proceso);


    case 5 :

   
      girar_iz();
      control();
     proceso="giro izquierda";
      //Serial.println(proceso);

      break;
   
    case 6 :      //retro

     //  control();
      retro();
      proceso="retro";
//      Serial.println(proceso);

      break;

    case 7 :      //provisional

       if(dd<9&&di>9){
       retro(); 
       girar_iz();
       girar_iz();
       girar_iz();
         
       }
       if(di<9&&dd>9){
       retro();
       girar_der();  
       girar_der();
       girar_der();
       }
       /*if(di<=10&&dd>=10){
       retro();
       girar_der();  
       girar_der();
       girar_der();
       }*/
    break;

        
    case 8 :    //retro
  
      control();
      proceso="control";
     // Serial.println(proceso);

      break;

      case 13:
        retro_iz();
        delay(150);
      break;  
  }
  

}

double YAW(){
  updateIMU(&roll, &pitch, &yaw);
  if (millis() - last_time > Serialtime) {
    Serial.print("Orientation:      ");
    Serial.println(yaw);
    last_time = millis();
  }
  return yaw;
}


void updateIMU(float *roll, float *pitch, float *yaw) {

  if (imu.readByte(MPU6050_ADDRESS, INT_STATUS) && 0x01) {
    imu.readAccelData(imu.accelCount);
    imu.getAres();

    imu.ax = (float)imu.accelCount[0] * imu.aRes;
    imu.ay = (float)imu.accelCount[1] * imu.aRes;
    imu.az = (float)imu.accelCount[2] * imu.aRes;
    imu.readGyroData(imu.gyroCount);
    imu.getGres();

    imu.gx = (float)imu.gyroCount[0] * imu.gRes;
    imu.gy = (float)imu.gyroCount[1] * imu.gRes;
    imu.gz = (float)imu.gyroCount[2] * imu.gRes;
  }

  imu.updateTime();
  imu.delt_t = millis() - imu.count;
  if (imu.delt_t > microsPerReading) {
    filter.updateIMU(imu.gx, imu.gy, imu.gz, imu.ax, imu.ay, imu.az);
    *roll = filter.getRoll();
    *pitch = filter.getPitch();
    *yaw = filter.getYaw();
    imu.count = millis();
  }
  
}



//SENSOR DERECHO S1
double dis_der() {
  int med = 0;
  double medida;
  for (int i = 0; i < 50; i++) {
    med = med + analogRead(PA4);
  }
  medida = med / 50;
  double dis = 16.743 * exp(-0.0009 * medida);
  if (dis < 2){dis=2;}
  return dis;
}


//SENSOR DELANTERO S2

double dis_frente() {

  int med_f = 0;
  double medida_f;
  for (int e = 0; e < 50; e++) {
    med_f = med_f + analogRead(PA6);
  }
  medida_f = med_f / 50;
  double dis_f= pow(3027.4 / (medida_f/4), 1.2134)*2;; // conversión a centímetros
  if (dis_f > 70)dis_f = 70;
  if (dis_f < 9) cont=1;
  if (cont==1){
    dis_f=8;
    cont_sf>10;
    cont_sf++;
  }
  if (cont_sf>10)cont=0;
  dis_f=dis_f-2;
  return dis_f;  
  
}

//SENSOR IZQUIERDO S3
double dis_i() {
  int med_i = 0;
  double medida_i;
  for (int i = 0; i < 50; i++) {
    med_i = med_i + analogRead(PA7);
  }
  medida_i = med_i / 50;
  double dis_iz = 16.743 * exp(-0.0009 * medida_i);
  if (dis_iz < 2){dis_iz=2;}
  return dis_iz;
}
/**************Sensores de distancia************/




  
/*****************Motores********************/
//DERECHO
void motord_on(double pwm){
    pwm=controlm(pwm);
   
    if (pwm>20000){
       pwmWrite(PWMA,abs(pwm));
      digitalWrite(A1,LOW);
      digitalWrite(A2,HIGH);
      Freno_D.write(130);
    }
    else if (pwm<- 6000){
       pwmWrite(PWMA,abs(pwm));
      digitalWrite(A1,HIGH);
      digitalWrite(A2,HIGH);
      Freno_D.write(165);
    }
    else{
       pwmWrite(PWMA,55000);
      Freno_D.write(130);
      digitalWrite(A1,HIGH);
      digitalWrite(A2,LOW);
      pwmWrite(PWMB,65000);
      //delay(44);
    }
}
//IZQUIERDO
void motori_on(double pwm){
    pwm=controlm(pwm);
    pwmWrite(PWMB,abs(pwm));
    if (pwm>20000){
       pwmWrite(PWMA,abs(pwm));
      digitalWrite(B_1,HIGH);
      digitalWrite(B_2,LOW);
      Freno_I.write(55);
    }
    else if (pwm< -6000){
       pwmWrite(PWMA,abs(pwm));
      digitalWrite(B_1,HIGH);
      digitalWrite(B_2,HIGH);
      Freno_I.write(20);
    }
    else{
       pwmWrite(PWMA,55000);
      Freno_I.write(55);
      digitalWrite(B_1,LOW);
      digitalWrite(B_2,HIGH);
      pwmWrite(PWMB,65000);
      //delay(44);
    }
}


//corrección pwm
int controlm(int pwm){
  if (pwm>45535){
    pwm=35000;
  }
  else if (pwm<-45535){
    pwm=-35000;
  }
  return pwm;
}

//************************** CASE *******************//


//*******IR AL FRENTE************//


void adelante() {
  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 35000);
  pwmWrite(PWMA, 35000);      //Correción  derecha
  
}

void corregir_der() {

  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 37000);
  pwmWrite(PWMA, 35000);      //Correción  derecha


}
void corregir_iz() {

  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 37000);
  pwmWrite(PWMA, 35000);      //Correción izquierda


}
void control() {

  int setF = 10;
  digitalWrite(B_1, HIGH);
  digitalWrite(B_2, HIGH);
  Freno_D.write(130);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  Freno_I.write(55);


 // **********************PID__FRENTE**********//
    errorF=df-setF;
    errorF_2=errorF-errorF_1;
    errorF_1=errorF;
    errorF_3=errorF+errorF_3;
    if (errorF_3>1000){
      errorF_3=1000;
    }
    if (errorF_3<-1000){
      errorF_3=-1000;
    }
    uF=errorF*kpf+kif*errorF_3+errorF_2*kdf;


//  ****************motores***************************//
  motord_on(uF);
  motori_on(uF);
  
}

void girar_der() {
  
  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 37000);
  pwmWrite(PWMA, 37000);    
  delay(30);
 
  double resta=0;
  ang_referencia = YAW();
  cont_W=0;
  do
  {
    digitalWrite(A1, HIGH);      //Motor y freno derecho habilitados
    digitalWrite(A2, LOW);
    Freno_I.write(55);
    digitalWrite(B_1, HIGH);     //Motor y freno izquierdo habilitados
    digitalWrite(B_2, LOW);
    Freno_D.write(130);
    pwmWrite(PWMB, 40000);
    pwmWrite(PWMA, 35000);
    yaw = YAW();
    resta=ang_referencia-7;
    if (resta<0){
        resta=resta+360;
    }
    cont_W++;
  }while ((yaw >= resta)||(cont_W>50000));
  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 37000);
  pwmWrite(PWMA, 37000);    
  delay(30);
}

void girar_iz() {

   
  double suma=0;
  ang_referencia = YAW();
  cont_W=0;
  do
  {
    digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
    digitalWrite(A2, HIGH);
    Freno_I.write(55);
    digitalWrite(B_1, LOW);     //Motor y freno izquierdo habilitados
    digitalWrite(B_2, HIGH);
    Freno_D.write(130);
    pwmWrite(PWMB, 40000);
    pwmWrite(PWMA, 38000);
    yaw = YAW();
    suma=ang_referencia+7;
    if (suma>360){
        suma=suma-360;
    }
    cont_W++;
  }while ((yaw <= suma)||(cont_W>88000));
  digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
  digitalWrite(A2, HIGH);
  Freno_I.write(55);
  digitalWrite(B_1, HIGH);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, LOW);
  Freno_D.write(130);
  pwmWrite(PWMB, 37000);
  pwmWrite(PWMA, 37000);    
  delay(15);
}


void girar_u_d() {
  double resta=0;
  ang_referencia = YAW();
  cont_W=0;
  do
  {
    digitalWrite(A1, HIGH);      //Motor y freno derecho habilitados
    digitalWrite(A2, LOW);
    Freno_I.write(55);
    digitalWrite(B_1, HIGH);     //Motor y freno izquierdo habilitados
    digitalWrite(B_2, LOW);
    Freno_D.write(130);
    pwmWrite(PWMB, 50000);
    pwmWrite(PWMA, 50000);
    yaw = YAW();
    resta=ang_referencia-10;
    if (resta<0){
        resta=resta+360;
    }
    cont_W=cont_W++;
  }while ((yaw >= resta)||(cont_W>1000));
}

void girar_u_i() {
  double resta=0;
  ang_referencia = YAW();
  cont_W=0;
  do
  {
    digitalWrite(A1, LOW);      //Motor y freno derecho habilitados
    digitalWrite(A2, HIGH);
    Freno_I.write(55);
    digitalWrite(B_1, LOW);     //Motor y freno izquierdo habilitados
    digitalWrite(B_2, HIGH);
    Freno_D.write(130);
    pwmWrite(PWMB, 50000);
    pwmWrite(PWMA, 50000);
    yaw = YAW();
    resta=ang_referencia-10;
    if (resta<0){
        resta=resta+360;
    }
    cont_W=cont_W++;
  }while ((yaw >= resta)||(cont_W>1000));
}

void retro_iz(){
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  Freno_I.write(50);
  digitalWrite(B_1, LOW);
  digitalWrite(B_2, HIGH);
  Freno_D.write(132);
  pwmWrite(PWMB, 55000);
  pwmWrite(PWMA, 40000);  
}
void retro() {

  digitalWrite(A1, HIGH);      //Motor y freno derecho habilitados
  digitalWrite(A2, LOW);
  Freno_I.write(55);
  digitalWrite(B_1, LOW);    //Motor y freno izquierdo habilitados
  digitalWrite(B_2, HIGH);
  Freno_D.write(130);
  pwmWrite(PWMB, 55000);
  pwmWrite(PWMA, 55000);      //Correción izquierda 

   delay(10);
}



