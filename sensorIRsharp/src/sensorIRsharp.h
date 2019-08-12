/**
 *
 * @author Heverton Costa
 *
 * @since 12/08/2019
 */

#ifndef sharp_h
#define sharp_h

#include <Arduino.h>

class sensorIR {
	
	public:
		#define GP2Y0A41SK0F 0
		#define GP2Y0A21YK0F 1

		sensorIR(int sensorPin, double deadZone = 0.0, byte model = 0);
		
		double getValue();
		void loop();
	
	private:
		int sensorPin;
		double deadZone;
		double distance;
		
		byte model;

		double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
};
#endif