#include <Arduino.h>
#include <analogWrite.h>
#include <esp32-hal-ledc.h>
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>
#include <WiFi.h>

#define COUNT_LOW 	1638
#define COUNT_HIGH 	7864
#define TIMER_WIDTH	16
#define ANALOGMAX	127
#define DZ			10

#define DZ	10	//deadzone for axis


const int lContPins[4] = {
	12, 13, 16, 17
};


void initPins(){
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
	
}

void setup(){
	Serial.begin(115200);
	Serial.print("ESP Board MAC Address:  ");
  	Serial.println(WiFi.macAddress());
	PS4.begin("7C:9E:BD:FA:1B:D0");
	Serial.println("READY.");
	initPins();


}
long int getVal(long int x){
	if( x < 0 )
		return 255-0;

	return 255-x;
}

int limitVal(int val, int lim = 127){
	if( val > lim )
		return lim;

	if( val < -lim )
		return -lim;

	if( val < DZ && val > -DZ )
		val = 0;

	return val;
}
void controlRight(int val){
	//12 and 13 for right
	if (val > 0){
		analogWrite(12, val);
		analogWrite(13, 0);
	}else if(val < 0){
		analogWrite(12, 0);
		analogWrite(13, abs(val));
	}else{
		analogWrite(12, 255);
		analogWrite(13, 255);
	}
}
void controlLeft(int val){
	//16 and 17 for left
	if (val > 0){
		analogWrite(16, val);
		analogWrite(17, 0);
	}else if(val < 0){
		analogWrite(16, 0);
		analogWrite(17, abs(val));
	}else{
		analogWrite(16, 255);
		analogWrite(17, 255);
	}

}
void loop()
{
	if( PS4.isConnected()){

		int x = 0;
		int y = 0;
		/**
		Serial.print("Raw left X: ");
		Serial.print(PS4.data.analog.stick.lx);
		Serial.print(",\tRaw left y: ");
		Serial.println(PS4.data.analog.stick.ly);
		**/
		x = limitVal(PS4.data.analog.stick.lx);
		y = limitVal(PS4.data.analog.stick.ly);

		x = map(x, -127, 127, -255, 255);
		y = map(y, -127, 127, -255, 255);

		int left = 0;
		int right = 0;

		left 	= limitVal(x+y, 255);
		right 	= limitVal(x-y, 255); 
		controlRight(right);
		controlLeft(left);
		/**
		Serial.print(x);
		Serial.print("<X>,\t");
		Serial.print("left: ");
		Serial.print(left);
		Serial.print(",\tright: ");
		Serial.println(right);
		**/


		delay(25);

	}
	
}
