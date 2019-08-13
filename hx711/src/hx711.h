/**
 *
 * @author Heverton Costa
 *
 * @since 13/08/2019
 */

#ifndef hx711_h
#define hx711_h

#include <Arduino.h>
#include <EEPROM.h>

class hx711 {
	
	public:
		
		#define g	0
		#define kg	1
	
		hx711(int ADDO, int ADSK, int Address, int model, double defaultWeight);
		void ativarHX711();
		void desativarHX711();
		long consultarPesoBalanca();
		void iniciarBalanca(byte n);
		void calibrarBalanca(unsigned long n = 100);
		boolean getEstavelBalanca();
	
	
	private:
		int ADDO; 
		int ADSK;
		int Address;
		int model;
		
		double valueCalibrated;
		double defaultWeight;
		double weightlessValue;
		double pesoAnterior;
		double desvioPadraoCal;
		
		boolean estavel;
	
		unsigned long readHX711();
		double mapLong(double x, double in_min, double in_max, double out_min, double out_max);
		void eeprom_erase(int8_t address);
		double read_double_eeprom(int8_t address);
		void write_double_eeprom(int8_t address, double data);

};
#endif