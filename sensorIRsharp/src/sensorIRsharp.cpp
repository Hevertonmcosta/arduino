#include <MATH.h>
#include <Arduino.h>
#include "sensorIRsharp.h"  
  
sensorIR::sensorIR(int sensorPin, double deadZone, byte model = 0)	{
	
	analogReference(EXTERNAL);
	this->sensorPin = sensorPin;
	this->deadZone = deadZone;
	this->model = model;
	pinMode(this->sensorPin, INPUT);
	
	distance = 0.0;
	
}

double sensorIR::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void sensorIR::loop()	{	

	double leitura = mapDouble(analogRead(this->sensorPin),0.0,1024.0,0.0,5.0);
	double distancia = 0.0;
	long distanciaMM = 0;
	
	switch (this->model)	{
		case GP2Y0A41SK0F:
			distancia = 51.627-(96.135*leitura)+(91.732*pow(leitura,2))-(46.731*pow(leitura,3))+(11.97*pow(leitura,4))-(1.2083*pow(leitura,5));
			break;
		case GP2Y0A21YK0F:
			distancia = 228.02-(685.72*leitura)+(990.09*pow(leitura,2))-(772.88*pow(leitura,3))+(332.75*pow(leitura,4))-(74.169*pow(leitura,5))+(6.6716*pow(leitura,6));
			break;
	}
	
	distanciaMM = distancia*10.0;
	distance = (double)distanciaMM;
}

double sensorIR::getValue()	{	
	return (distance - this->deadZone);
}