//// ****** THIS FILE IS AUTOGENERATED ******
////
////          >>>> PLEASE ADAPT IT TO YOUR NEEDS <<<<
////
/// 
///  Filename; D:\Insturmentacion 1C2019\Instrumentacion\Practica especial\PIDDRIVER.py
///  Source class: FLOWPIDDriver
///  Generation timestamp: 2019-06-18T18:43:32.068134
///  Class code hash: b4513fff6bb16d2ee86ec4e6995533d4e153bd09
///
/////////////////////////////////////////////////////////////

// La idea es que escriba como si no existiera python. Hay que revisar todavia lo que esta en void setup y void loop para escribirlo bien. 
// Probablemente tenga que usar las funciones que tengo definidas dentro del void loop/setup y definir aca al fondo que hace cada funcion
// OJO tambien revisar la funcion get_flow porque si bien funciona en un sketch nuevo, hay que ver con cuidado que hace

#include "inodriver_user.h"

//PID constants
double kp = 2
double ki = 5
double kd = 1

// Variable que determina si activo el loop de control o no
bool CtrlLoop = 0 

// Instancio variables que se usan durante el PID
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;


// Instancio variables necesarias para medir flujo de manera continua
volatile int flow_frequency; // Measures flow sensor pulses
float l_hour; // Calculated litres/hour
float setPoint;
float tolerance;
const unsigned char flowsensor_pin = 2; // Sensor Input
const pump_pin = 3;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

void user_setup() {

     setPoint = 0;                          //set point
     pinMode(flowsensor_pin, INPUT);
     digitalWrite(flowsensor_pin, HIGH); // Optional Internal Pull-Up
     attachInterrupt(0, flow, RISING); // Setup Interrupt
     sei(); // Enable interrupts
     currentTime = millis();
     cloopTime = currentTime;
     tolerance = 0.01
}

// Aca empieza el loop, si la variable CtrlLoop es True (1) entra en el loop de control, y si vale False(0)no hace nada
void user_loop() {

     // Esta primera parte del loop va a estar corriendo siempre
     currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter

 
      if (CtrlLoop == 1){
        input = setPoint - l_hour;                //Diferencia entre el set point y lo medido
        output = computePID(input);
        delay(100);
        analogWrite(pump_pin, output);                //control the motor based on PID value
      }

      else if (setPoint - l_hour > tolerance)
      
   }
}


// Funcion que utiliza el PID

double computePID(double inp){     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = Setpoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
 
        return out;                                        //have function return the PID output
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
  return 0.0;
};

int set_Pump_Flow(int key, float value) {
  return 0;
};


// COMMAND: Control_Loop_enabled, FEAT: control_loop_enabled
int get_Control_Loop_enabled(int value) {
  CtrlLoop = value;
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

int set_Flow(float value) {
  
  // Esta funcion no tienen sentido completarla
  
  return 0;
};

