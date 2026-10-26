
#ifndef PIO_UNIT_TESTING

#include <Arduino.h>


extern "C" bool negnum(int x);


void setup()
{
    Serial.begin(9600);

    unsigned long start = millis();
    while (!Serial && millis() - start < 5000)
    {
    }

    Serial.println("NEGNUM TEST");

    Serial.print("negnum(-5) = ");
    Serial.println(negnum(-5));

    Serial.print("negnum(-10) = ");
    Serial.println(negnum(-10));

    Serial.print("negnum(8) = ");
    Serial.println(negnum(8));

    Serial.print("negnum(0) = ");
    Serial.println(negnum(0));
}


void loop()
{
    Serial.println("NEGNUM TEST");

    Serial.print("negnum(-5) = ");
    Serial.println(negnum(-5));

    Serial.print("negnum(-10) = ");
    Serial.println(negnum(-10));

    Serial.print("negnum(8) = ");
    Serial.println(negnum(8));

    Serial.print("negnum(0) = ");
    Serial.println(negnum(0));

    Serial.println("----------------");

    delay(2000);
}

#endif

