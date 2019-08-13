/**
 *
 * @author Heverton Costa
 *
 * @since 13/08/2019
 */

#include <Arduino.h>
#include "hx711.h"  
  
hx711::hx711(int ADDO, int ADSK, int Address, int model, double defaultWeight)	{
	this->ADDO = ADDO;
	this->ADSK = ADSK;
	this->Address = Address;
	this->model = model;
	this->defaultWeight = defaultWeight;

	pinMode(this->ADDO, INPUT_PULLUP);   //entrada para receber os dados
	pinMode(this->ADSK, OUTPUT);         //saída para SCK
	
	valueCalibrated = 0.0;
	weightlessValue = 0.0;
	pesoAnterior = 0.0;
	desvioPadraoCal = 1.6;
	
	estavel = false;
}

void hx711::ativarHX711()  {
  digitalWrite(this->ADSK, LOW);
}

void hx711::desativarHX711() {
  digitalWrite(this->ADSK, LOW);
  digitalWrite(this->ADSK, HIGH);
}

unsigned long hx711::readHX711()  {
  unsigned long Count = 0;
  unsigned char i;
  
  delay(10);
  
  digitalWrite(this->ADSK, LOW);
  
  while (digitalRead(this->ADDO));

  for (i = 0; i < 24; i++)  {
    digitalWrite(this->ADSK, HIGH);
    Count = Count << 1;
    digitalWrite(this->ADSK, LOW);
    if (digitalRead(this->ADDO))
      Count++;
  }
  
  digitalWrite(this->ADSK, HIGH);
  
  Count = Count ^ 0x800000;
  
  digitalWrite(this->ADSK, LOW);
  
  return (Count);
}

void hx711::calibrarBalanca(unsigned long n)  {
  ativarHX711();
  
  double value = 0.0;

  value = (double)readHX711();
  
  for (long i = 0; i < n; i++)  {
    value =  (value + (double)readHX711()) / 2.0;
  }
  valueCalibrated = value;
  
  write_double_eeprom(this->Address, value);

  desativarHX711();
}

double hx711::mapLong(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long hx711::consultarPesoBalanca()	{
	double amostrasPesos[2];
	double mediaPeso = 0.0;
	double variancia = 0.0;
	double mediaAritmetica = 0.0;
	double desvioPadrao = 0.0;
	double convert = readHX711();

	switch (this->model)	{
		case g: //weightless value
			amostrasPesos[0] = mapLong(convert, weightlessValue, valueCalibrated, 0.0, this->defaultWeight) * 1000.0;
			mediaPeso += amostrasPesos[0];
			convert = readHX711();
			amostrasPesos[1] = mapLong(convert, weightlessValue, valueCalibrated, 0.0, this->defaultWeight) * 1000.0;
			mediaPeso += amostrasPesos[1];
			break;
		case kg:
			amostrasPesos[0] = mapLong(convert, weightlessValue, valueCalibrated, 0.0, this->defaultWeight);
			mediaPeso += amostrasPesos[0];
			convert = readHX711();
			amostrasPesos[1] = mapLong(convert, weightlessValue, valueCalibrated, 0.0, this->defaultWeight);
			mediaPeso += amostrasPesos[1];
			break;
	}
	
	mediaAritmetica = mediaPeso / 2.0;

	amostrasPesos[0] = mediaAritmetica;
	amostrasPesos[1] = pesoAnterior;

	for (byte i = 0; i < 2; i++) {
		amostrasPesos[i] = pow((amostrasPesos[i] - mediaAritmetica), 2);
		variancia += amostrasPesos[i];
	}

	variancia /= 2.0;

	desvioPadrao = sqrt(variancia);

	if (desvioPadrao < desvioPadraoCal)  {
		estavel = true;
	} else {
		estavel = false;
	}

	pesoAnterior = mediaAritmetica;

	return mediaAritmetica;
}

void hx711::iniciarBalanca(byte n)  {
	ativarHX711();
  
	weightlessValue = 0;
	weightlessValue = readHX711();
  
	for (long i = 0; i < n; i++)  {
		weightlessValue = (weightlessValue + readHX711()) / 2.0;
	}
  
	desativarHX711();
}

void hx711::eeprom_erase(int8_t address) {
  EEPROM.write(address, 0);
}

double hx711::read_double_eeprom(int8_t address) {
  int8_t i;
  double data;

  for (i = 0; i < 4; ++i) {
    *((int8_t *)(&data) + i) = EEPROM.read(address + i);
  }

  return data;
}

void hx711::write_double_eeprom(int8_t address, double data) {
  int8_t i;

  for (i = 0; i < 4; ++i) {
    EEPROM.write(address + i, *((int8_t *)(&data) + i));
  }
}

boolean hx711::getEstavelBalanca()  {
  return estavel;
}