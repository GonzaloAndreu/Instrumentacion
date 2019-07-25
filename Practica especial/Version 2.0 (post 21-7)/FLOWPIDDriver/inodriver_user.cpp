//// ****** THIS FILE IS AUTOGENERATED ******
////
////          >>>> PLEASE ADAPT IT TO YOUR NEEDS <<<<
////
/// 
///  Filename; D:\Documentos\Licenciatura en Física\Laboratorios\Instrumentacion\Instrumentacion\Practica especial\Version 2.0 (post 21-7)\PIDDRIVER.py
///  Source class: FLOWPIDDriver
///  Generation timestamp: 2019-07-21T17:08:12.194686
///  Class code hash: b4513fff6bb16d2ee86ec4e6995533d4e153bd09
///
/////////////////////////////////////////////////////////////

// La idea es que escriba como si no existiera python. Hay que revisar todavia lo que esta en void setup y void loop para escribirlo bien. 
// Probablemente tenga que usar las funciones que tengo definidas dentro del void loop/setup y definir aca al fondo que hace cada funcion
// OJO tambien revisar la funcion get_flow porque si bien funciona en un sketch nuevo, hay que ver con cuidado que hace

#include "inodriver_user.h"
#include <AFMotor.h>
#include <Arduino.h>

//PID constants
double kp = 2;
double ki = 1;
double kd = 1;

//PWM values
float PWM_Value1;
float PWM_Value2;


//Bomba 1
int ENA = 10;
int IN1 = 9;
int IN2 = 8;
float pump_flow1=76;

//Bomba 2
int  ENB = 5;
int IN3 = 7;
int IN4 = 6;
float pump_flow2=76;

// Variable que determina si activo el loop de control o no
bool CtrlLoop = 1;

// Instancio variables que se usan durante el PID
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output;
float setPoint;
double cumError, rateError;


// Instancio variables necesarias para medir flujo de manera continua
volatile int flow_frequency; // Measures flow sensor pulses
float l_hour; // Calculated litres/hour
float tolerance;
const unsigned char flowsensor_pin = 2; // Sensor Input
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

void user_setup() {

     setPoint = 100;                          //set point
     pinMode(flowsensor_pin, INPUT);
     digitalWrite(flowsensor_pin, HIGH); // Optional Internal Pull-Up
     attachInterrupt(0, flow, RISING); // Setup Interrupt
     sei(); // Enable interrupts
     currentTime = millis();
     cloopTime = currentTime;
     tolerance = 153*(24/247);
     //Declaramos los pines de las bombas como salida
     pinMode (ENA, OUTPUT);
     pinMode (IN1, OUTPUT);
     pinMode (IN2, OUTPUT);
     pinMode (ENB, OUTPUT);
     pinMode (IN3, OUTPUT);
     pinMode (IN4, OUTPUT);
     
}


// Funcion que utiliza el PID

double computePID(double inp){     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = setPoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
 
        return out;                                        //have function return the PID output
}



// Aca empieza el loop, si la variable CtrlLoop es True (1) entra en el loop de control, y si vale False(0)no hace nada

void user_loop() {

 // Esta primera parte del loop va a estar corriendo siempre
     currentTime = millis();

 //MOVER MOTOR A
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      //Variando el próximo valor entre 0 y 255 varía la velocidad
      PWM_Value1=((112.82800-(3600*0.550/pump_flow1))/0.34092); //esto es solo para poder cambiar de flujop a unidad de la bomba
      analogWrite(ENA, PWM_Value1);


  //MOVER MOTOR B
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      //Variando el próximo valor entre 0 y 255 varía la velocidad
      PWM_Value2=((112.82800-(3600*0.550/pump_flow2))/0.34092); //esto es solo para poder cambiar de flujop a unidad de la bomba
      analogWrite(ENB, PWM_Value2);

    Serial.print(l_hour, 3);
    Serial.print(", ");
    Serial.print(flow_frequency,3);
  Serial.println("  ");
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = ((flow_frequency * 60 / (7.5))* (160/1353.8))*(153/247); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter

 
//      if (CtrlLoop == 1){
//        input = setPoint - l_hour;                //Diferencia entre el set point y lo medido
//        output = computePID(input);
//        delay(100);
//        analogWrite(ENA, output);
//        analogWrite(ENB, output); //control the motor based on PID value
//        pump_flow1= pump_flow1 + output;
//        pump_flow2= pump_flow2 + output;
//        delay(100);
//        CtrlLoop = 1000;
//      }
//      
//
//      else {}
//      
//      if ((setPoint - l_hour > tolerance)){
//        CtrlLoop=CtrlLoop-1;
//       
//      }
   }
  
}


// COMMAND: Kp, FEAT: Kp
float get_Kp() {
  return kp;
};

int set_Kp(float value) {

  kp = value;
  
  return 0;
};


// COMMAND: Ki, FEAT: Ki
float get_Ki() {
  return ki;
};

int set_Ki(float value) {
  
  ki = value;
 
  return 0;
};


// COMMAND: Kd, FEAT: Kd
float get_Kd() {
  
  return kd;
};

int set_Kd(float value) {

  kd = value;
  return 0;
};


// COMMAND: Set_Point, FEAT: set_point
float get_Set_Point() {
  return setPoint;
};

int set_Set_Point(float value) {

  setPoint = value;
  return 0;
};



// COMMAND: Pump_Flow, FEAT: pump_flow
float get_Pump_Flow(int key) {

  if(key==1){
  return pump_flow1;
  }
  if(key==2){
  return pump_flow2;
  }
};



int set_Pump_Flow(int key, float value) {

if(key == 1){
  pump_flow1 = value;
     
};
if(key == 2){
   pump_flow2 = value;
     
   
};

};


// COMMAND: Control_Loop_enabled, FEAT: control_loop_enabled
int get_Control_Loop_enabled() {
  return CtrlLoop;
};

int set_Control_Loop_enabled(int value) {

  CtrlLoop = (bool)value;
  
  return 0;
};


// COMMAND: Flow, FEAT: flow_value
float get_Flow() {
  return l_hour;
}
