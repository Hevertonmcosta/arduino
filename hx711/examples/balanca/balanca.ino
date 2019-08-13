/**
 *
 * @author Heverton Costa
 *
 * @since 13/08/2019
 */
 
#include <hx711.h>
hx711 balanca(A4, A5, 0, g, 10.0);

void setup()  {
	Serial.begin(9600);
	balanca.iniciarBalanca(20);
}

void loop() {
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == 'C') {
		  Serial.println("START");
		  balanca.calibrarBalanca(20);
		  Serial.println("END");
    } else if (inChar == 'R') {
		  balanca.ativarHX711();
		  Serial.print(balanca.consultarPesoBalanca());
		  Serial.print(" ");
		  Serial.println(balanca.getEstavelBalanca());
		  balanca.desativarHX711();
    }
  }
}