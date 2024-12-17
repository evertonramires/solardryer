#include <U8glib.h>
#include <AM2302-Sensor.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "helperFunctions.h"
#include "secretSauce.h"

AM2302::AM2302_Sensor sensor0{SENSOR0_PIN};
AM2302::AM2302_Sensor sensor1{SENSOR1_PIN};
AM2302::AM2302_Sensor sensor2{SENSOR2_PIN};
AM2302::AM2302_Sensor sensor3{SENSOR3_PIN};
AM2302::AM2302_Sensor sensor4{SENSOR4_PIN};
AM2302::AM2302_Sensor sensor5{SENSOR5_PIN};
AM2302::AM2302_Sensor sensor6{SENSOR6_PIN};

/*****************************************************
 * SETUP
 *****************************************************/
void setup()
{

  Serial.begin(115200);
  Serial.println("Starting...");

  setupTimer1(); // Setup Timer1 to trigger every second

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderButtonPin, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Initialize last encoder state
  uint8_t a = digitalRead(encoderPinA);
  uint8_t b = digitalRead(encoderPinB);
  lastEncoderBits = (a << 1) | b;

  setupTimer2ForEncoder(); // Set up a timer interrupt for stable sampling

  u8g.firstPage();
  do
  {
    u8g.setFont(font);
    u8g.setPrintPos(0, lineHeight);
    u8g.print("Starting...");
  } while (u8g.nextPage());

  Serial.println(F("### DHT22 Sensor Tester ###"));
  Serial.println(F("###   See: ideavox.org  ###"));
  Serial.println(F("###########################\n"));

  bool sensor_initialized[7] = {false, false, false, false, false, false, false};

  // for (int i = 0; i < sensorStartTry; ++i)
  // {
  //   Serial.print("Initializing sensors... attempt: ");
  //   Serial.print(i + 1);
  //   Serial.print("/");
  //   Serial.println(sensorStartTry);

  //   u8g.firstPage();
  //   do
  //   {
  //     u8g.setFont(font);
  //     u8g.setPrintPos(5, 32);
  //     u8g.print("Initializing... ");
  //     u8g.print(i + 1);
  //     u8g.print("/");
  //     u8g.print(sensorStartTry);
  //     u8g.setPrintPos(1, 62);
  //     u8g.print("ideavox.org CC-BY-SA");

  //   } while (u8g.nextPage());

  //   if (!sensor_initialized[0])
  //   {
  //     sensor_initialized[0] = sensor0.begin();
  //     if (sensor_initialized[0])
  //     {
  //       Serial.println("Sensor 0 initialized successfully.");
  //       sensorStatus[0] = true;
  //     }
  //   }

  //   if (!sensor_initialized[1])
  //   {
  //     sensor_initialized[1] = sensor1.begin();
  //     if (sensor_initialized[1])
  //     {
  //       Serial.println("Sensor 1 initialized successfully.");
  //       sensorStatus[1] = true;
  //     }
  //   }

  //   if (!sensor_initialized[2])
  //   {
  //     sensor_initialized[2] = sensor2.begin();
  //     if (sensor_initialized[2])
  //     {
  //       Serial.println("Sensor 2 initialized successfully.");
  //       sensorStatus[2] = true;
  //     }
  //   }

  //   if (!sensor_initialized[3])
  //   {
  //     sensor_initialized[3] = sensor3.begin();
  //     if (sensor_initialized[3])
  //     {
  //       Serial.println("Sensor 3 initialized successfully.");
  //       sensorStatus[3] = true;
  //     }
  //   }

  //   if (!sensor_initialized[4])
  //   {
  //     sensor_initialized[4] = sensor4.begin();
  //     if (sensor_initialized[4])
  //     {
  //       Serial.println("Sensor 4 initialized successfully.");
  //       sensorStatus[4] = true;
  //     }
  //   }

  //   if (!sensor_initialized[5])
  //   {
  //     sensor_initialized[5] = sensor5.begin();
  //     if (sensor_initialized[5])
  //     {
  //       Serial.println("Sensor 5 initialized successfully.");
  //       sensorStatus[5] = true;
  //     }
  //   }

  //   if (!sensor_initialized[6])
  //   {
  //     sensor_initialized[6] = sensor6.begin();
  //     if (sensor_initialized[6])
  //     {
  //       Serial.println("Sensor 6 initialized successfully.");
  //       sensorStatus[6] = true;
  //     }
  //   }

  //   if (sensor_initialized[0] && sensor_initialized[1] && sensor_initialized[2] && sensor_initialized[3] && sensor_initialized[4] && sensor_initialized[5] && sensor_initialized[6])
  //   {
  //     break;
  //   }

  //   delay(100);
  // }
}

void loop()
{
  // Collect steps from the encoder
  noInterrupts();
  int diff = encoderDiff;
  encoderDiff = 0;
  interrupts();

  // Apply increments only if MOVE_DELAY_MS has passed
  if (millis() - lastMoveTime > MOVE_DELAY_MS)
  {
    bool moved = false;
    while (diff > 0)
    {
      if (currentSelection < menuLength - 1)
      {
        currentSelection++;
        moved = true;
      }
      diff -= STEPS_PER_ITEM;
    }
    while (diff < 0)
    {
      if (currentSelection > 0)
      {
        currentSelection--;
        moved = true;
      }
      diff += STEPS_PER_ITEM;
    }

    if (moved)
    {
      lastMoveTime = millis();
    }
  }

  // Handle button
  handleEncoderButton();

  // // Check if selection display time is over
  // if (showSelection && (millis() - selectionStartTime > 1000))
  // {
  //   showSelection = false;
  // }

  // // Draw the screen
  // u8g.firstPage();
  // do
  // {
  //   if (showSelection)
  //   {
  //     // drawSelectionScreen();
  //     drawMenu1();
  //   }
  //   else
  //   {
  //     drawMenu0();
  //   }
  // } while (u8g.nextPage());

  // Draw the screen
  u8g.firstPage();
  do
  {
    if (currentMenu == 0)
    {
      // drawSelectionScreen();
      drawMenu0();
    }
    else if (currentMenu == 1)
    {
      // drawSelectionScreen();
      drawMenu1();
    }
    else if (currentMenu == 2)
    {
      // drawSelectionScreen();
      drawMenu2();
    }
    else if (currentMenu == 3)
    {
      // drawSelectionScreen();
      drawMenu3();
    }

  } while (u8g.nextPage());

  delay(20);

  // ########### MEASURE ALL SENSORS ###########
  sensorStatus[0] = !sensor0.read();
  sensorStatus[1] = !sensor1.read();
  sensorStatus[2] = !sensor2.read();
  sensorStatus[3] = !sensor3.read();
  sensorStatus[4] = !sensor4.read();
  sensorStatus[5] = !sensor5.read();
  sensorStatus[6] = !sensor6.read();

  sensorTemp[0] = sensor0.get_Temperature();
  sensorTemp[1] = sensor1.get_Temperature();
  sensorTemp[2] = sensor2.get_Temperature();
  sensorTemp[3] = sensor3.get_Temperature();
  sensorTemp[4] = sensor4.get_Temperature();
  sensorTemp[5] = sensor5.get_Temperature();
  sensorTemp[6] = sensor6.get_Temperature();

  sensorHum[0] = sensor0.get_Humidity();
  sensorHum[1] = sensor1.get_Humidity();
  sensorHum[2] = sensor2.get_Humidity();
  sensorHum[3] = sensor3.get_Humidity();
  sensorHum[4] = sensor4.get_Humidity();
  sensorHum[5] = sensor5.get_Humidity();
  sensorHum[6] = sensor6.get_Humidity();

  // ########### CALL MAIN LOGIC BIG FORMULA FUNCTION ###########
  secretSauce();
  // ###########################################################

  // Map binary logic of fanSpeed to percentage to be printed
  fanSpeedPercent[0] = map(fanSpeed[0], 0, 255, 0, 100);
  fanSpeedPercent[1] = map(fanSpeed[1], 0, 255, 0, 100);
  fanSpeedPercent[2] = map(fanSpeed[2], 0, 255, 0, 100);
  fanSpeedPercent[3] = map(fanSpeed[3], 0, 255, 0, 100);
  fanSpeedPercent[4] = map(fanSpeed[4], 0, 255, 0, 100);
  fanSpeedPercent[5] = map(fanSpeed[5], 0, 255, 0, 100);
  fanSpeedPercent[6] = map(fanSpeed[6], 0, 255, 0, 100);

  peltiersPowerPercent = map(peltiersPower, 0, 255, 0, 100);

  Serial.println("\n#####################################################################\n");

  Serial.println("Sensors Status: ");
  Serial.print("0 | 1 | 2 | 3 | 4 | 5 | 6 \n");
  Serial.println("---------------------------");

  Serial.print(sensorStatus[0]);
  Serial.print(" | ");
  Serial.print(sensorStatus[1]);
  Serial.print(" | ");
  Serial.print(sensorStatus[2]);
  Serial.print(" | ");
  Serial.print(sensorStatus[3]);
  Serial.print(" | ");
  Serial.print(sensorStatus[4]);
  Serial.print(" | ");
  Serial.print(sensorStatus[5]);
  Serial.print(" | ");
  Serial.print(sensorStatus[6]);
  Serial.println();
  Serial.println();

  for (int i = 0; i < 7; ++i)
  {
    if (sensorStatus[i])
    {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(" (Before Peltiers): ");
      Serial.print("Temperature: ");
      Serial.print(i == 0 ? sensor0.get_Temperature() : i == 1 ? sensor1.get_Temperature()
                                                    : i == 2   ? sensor2.get_Temperature()
                                                    : i == 3   ? sensor3.get_Temperature()
                                                    : i == 4   ? sensor4.get_Temperature()
                                                    : i == 5   ? sensor5.get_Temperature()
                                                               : sensor6.get_Temperature());
      Serial.print("ºC");
      Serial.print(" | Humidity: ");
      Serial.print(i == 0 ? sensor0.get_Humidity() : i == 1 ? sensor1.get_Humidity()
                                                 : i == 2   ? sensor2.get_Humidity()
                                                 : i == 3   ? sensor3.get_Humidity()
                                                 : i == 4   ? sensor4.get_Humidity()
                                                 : i == 5   ? sensor5.get_Humidity()
                                                            : sensor6.get_Humidity());
      Serial.print("%");
      Serial.println();
    }
  }
  // delay(3000);
}
