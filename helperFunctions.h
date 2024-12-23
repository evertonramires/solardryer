/*****************************************************
 * HELPER FUNCTIONS
 *****************************************************/
void beep()
{
  digitalWrite(buzzerPin, HIGH);
  delay(100); // Beep duration in milliseconds
  digitalWrite(buzzerPin, LOW);
}

/*****************************************************
 * ENCODER & BUTTON HANDLING
 *****************************************************/
void selectCurrentOption()
{
  Serial.print("Selected: ");
  Serial.println(menuItems[currentSelection]);
  strncpy(selectedItem, menuItems[currentSelection], sizeof(selectedItem) - 1);
  selectedItem[sizeof(selectedItem) - 1] = '\0';

  showSelection = true;
  selectionStartTime = millis();
}

void handleEncoderButton()
{
  uint8_t reading = digitalRead(encoderButtonPin);
  // Button is active LOW
  if (reading != lastButtonState && (millis() - lastButtonTime) > BUTTON_DEBOUNCE_MS)
  {
    lastButtonTime = millis();
    lastButtonState = reading;
    if (reading == LOW)
    {
      // Button pressed - select current option and beep
      if (currentMenu == 0)
      {
        currentMenu = 1;
      }
      else if (currentMenu == 1)
      {
        currentMenu = 2;
      }
      else if (currentMenu == 2)
      {
        currentMenu = 3;
      }
      else if (currentMenu == 3)
      {
        currentMenu = 0;
      }

      selectCurrentOption();
      beep();
    }
  }
}

/*****************************************************
 * DISPLAY FUNCTIONS
 *****************************************************/
void drawMenu0() // SENSORS
{
  u8g.setFont(font);

  // We have 8 items, split into 2 columns, 4 items per column.
  // item i:
  // row = i % 4
  // col = i / 4  (0 or 1)
  for (int i = 0; i < menuLength; i++)
  {
    int col = i / 4; // 0 for items 0-3, 1 for items 4-7
    int row = i % 4;
    int y = (row + 1) * lineHeight; // line position
    int x = columnX[col];           // column start

    if (i == currentSelection)
    {
      // Draw ">" marker
      u8g.setPrintPos(x, y);
      u8g.print("> ");
      u8g.setPrintPos(x + textIndent, y);
    }
    else
    {
      u8g.setPrintPos(x + textIndent, y);
    }

    // If i == 0, print currentTemp instead of the static string
    if (i == 0)
    {
      // Show temperature and humidity for each sensor
      char tempStr[16];
      if (i == 0)
      {
        snprintf(tempStr, sizeof(tempStr), "0:%dC|%d%%", sensorTemp[0], sensorHum[0]);
      }
      else if (i == 1)
      {
        snprintf(tempStr, sizeof(tempStr), "1:%dC|%d%%", sensorTemp[1], sensorHum[1]);
      }
      else if (i == 2)
      {
        snprintf(tempStr, sizeof(tempStr), "2:%dC|%d%%", sensorTemp[2], sensorHum[2]);
      }
      else if (i == 3)
      {
        snprintf(tempStr, sizeof(tempStr), "3:%dC|%d%%", sensorTemp[3], sensorHum[3]);
      }
      else if (i == 4)
      {
        snprintf(tempStr, sizeof(tempStr), "4:%dC|%d%%", sensorTemp[4], sensorHum[4]);
      }
      else if (i == 5)
      {
        snprintf(tempStr, sizeof(tempStr), "5:%dC|%d%%", sensorTemp[5], sensorHum[5]);
      }
      else if (i == 6)
      {
        snprintf(tempStr, sizeof(tempStr), "6:%dC|%d%%", sensorTemp[6], sensorHum[6]);
      }
      u8g.print(tempStr);
    }
    else
    {
      u8g.print(menuItems[i]);
    }
  }
}
void drawMenu1() // FANS
{
  u8g.setFont(font);

  // We have 8 items, split into 2 columns, 4 items per column.
  // item i:
  // row = i % 4
  // col = i / 4  (0 or 1)
  for (int i = 0; i < menuLength + 1; i++)
  {
    int col = i / 4; // 0 for items 0-3, 1 for items 4-7
    int row = i % 4;
    int y = (row + 1) * lineHeight; // line position
    int x = columnX[col];           // column start

    if (i == currentSelection)
    {
      // Draw ">" marker
      u8g.setPrintPos(x, y);
      u8g.print("> ");
      u8g.setPrintPos(x + textIndent, y);
    }
    else
    {
      u8g.setPrintPos(x + textIndent, y);
    }
    // Show speed for each fan
    char tempStr[16];
    if (i == 0)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan0:%d%%", fanSpeedPercent[0]);
    }
    else if (i == 1)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan1:%d%%", fanSpeedPercent[1]);
    }
    else if (i == 2)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan2:%d%%", fanSpeedPercent[2]);
    }
    else if (i == 3)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan3:%d%%", fanSpeedPercent[3]);
    }
    else if (i == 4)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan4:%d%%", fanSpeedPercent[4]);
    }
    else if (i == 5)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan5:%d%%", fanSpeedPercent[5]);
    }
    else if (i == 6)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan6:%d%%", fanSpeedPercent[6]);
    }
    else if (i == 7)
    {
      snprintf(tempStr, sizeof(tempStr), "PELT:%d%%", peltiersPowerPercent);
    }
    u8g.print(tempStr);
  }
}
void drawMenu2() // VALVES
{
  u8g.setFont(font);

  // We have 8 items, split into 2 columns, 4 items per column.
  // item i:
  // row = i % 4
  // col = i / 4  (0 or 1)
  for (int i = 0; i < menuLength; i++)
  {
    int col = i / 4; // 0 for items 0-3, 1 for items 4-7
    int row = i % 4;
    int y = (row + 1) * lineHeight; // line position
    int x = columnX[col];           // column start

    if (i == currentSelection)
    {
      // Draw ">" marker
      u8g.setPrintPos(x, y);
      u8g.print("> ");
      u8g.setPrintPos(x + textIndent, y);
    }
    else
    {
      u8g.setPrintPos(x + textIndent, y);
    }
    // Show speed for each fan
    char tempStr[16];
    if (i == 0)
    {
      snprintf(tempStr, sizeof(tempStr), "Val0:%s", valveStatus[0] == 0 ? "OFF" : "ON");
    }
    else if (i == 1)
    {
      snprintf(tempStr, sizeof(tempStr), "Val1:%s", valveStatus[1] == 0 ? "OFF" : "ON");
    }
    else if (i == 2)
    {
      snprintf(tempStr, sizeof(tempStr), "Val2:%s", valveStatus[2] == 0 ? "OFF" : "ON");
    }
    else if (i == 3)
    {
      snprintf(tempStr, sizeof(tempStr), "Val3:%s", valveStatus[3] == 0 ? "OFF" : "ON");
    }
    else if (i == 4)
    {
      snprintf(tempStr, sizeof(tempStr), "Val4:%s", valveStatus[4] == 0 ? "OFF" : "ON");
    }
    else if (i == 5)
    {
      snprintf(tempStr, sizeof(tempStr), "Val5:%s", valveStatus[5] == 0 ? "OFF" : "ON");
    }
    else if (i == 6)
    {
      snprintf(tempStr, sizeof(tempStr), "Val6:%s", valveStatus[6] == 0 ? "OFF" : "ON");
    }
    u8g.print(tempStr);
  }
}
void drawMenu3() // SUMMARY
{
  u8g.setFont(font);

  // We have 8 items, split into 2 columns, 4 items per column.
  // item i:
  // row = i % 4
  // col = i / 4  (0 or 1)
  for (int i = 0; i < menuLength + 1; i++)
  {
    int col = i / 4; // 0 for items 0-3, 1 for items 4-7
    int row = i % 4;
    int y = (row + 1) * lineHeight; // line position
    int x = columnX[col];           // column start

    if (i == currentSelection)
    {
      // Draw ">" marker
      u8g.setPrintPos(x, y);
      u8g.print("> ");
      u8g.setPrintPos(x + textIndent, y);
    }
    else
    {
      u8g.setPrintPos(x + textIndent, y);
    }
    // Show speed for each fan
    char tempStr[16];
    if (i == 0)
    {
      snprintf(tempStr, sizeof(tempStr), "0:%dC|%d%%", sensorTemp[0], sensorHum[0]);
    }
    else if (i == 1)
    {
      snprintf(tempStr, sizeof(tempStr), "1:%dC|%d%%", sensorTemp[1], sensorHum[1]);
    }
    else if (i == 2)
    {
      snprintf(tempStr, sizeof(tempStr), "2:%dC|%d%%", sensorTemp[2], sensorHum[2]);
    }
    else if (i == 3)
    {
      snprintf(tempStr, sizeof(tempStr), "PELT:%d%%", peltiersPowerPercent);
    }
    else if (i == 4)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan0:%d%%", fanSpeedPercent[0]);
    }
    else if (i == 5)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan1:%d%%", fanSpeedPercent[1]);
    }
    else if (i == 6)
    {
      snprintf(tempStr, sizeof(tempStr), "Fan2:%d%%", fanSpeedPercent[2]);
    }
    else if (i == 7)
    {
      snprintf(tempStr, sizeof(tempStr), "%d min", timeMinutes);
    }

    u8g.print(tempStr);
    u8g.setPrintPos(0, 64);
    if (sdPresent == true)
    {
      u8g.print(".");
    }
    else
    {
      u8g.print("SD NOT PRESENT");
    }
  }
}

void drawSelectionScreen()
{
  u8g.setFont(font);
  u8g.setPrintPos(0, lineHeight);
  u8g.print("Selected:");
  u8g.setPrintPos(0, lineHeight * 2);
  u8g.print(selectedItem);
}

/*****************************************************
 * TIMER INTERRUPT LOGIC
 *****************************************************/
void setupTimer2ForEncoder()
{
  noInterrupts();
  // Timer2 setup for ~1ms interrupt:
  TCCR2A = (1 << WGM21);  // CTC mode
  OCR2A = 249;            // 1ms interval at prescale=64 (16MHz CPU)
  TCCR2B = (1 << CS22);   // prescale=64
  TIMSK2 = (1 << OCIE2A); // enable compare interrupt
  interrupts();
}

// Timer2 compare match interrupt service routine (~1ms)
ISR(TIMER2_COMPA_vect)
{
  uint8_t a = digitalRead(encoderPinA);
  uint8_t b = digitalRead(encoderPinB);
  uint8_t current = (a << 1) | b;

  uint8_t index = (lastEncoderBits << 2) | current;
  int8_t delta = pgm_read_byte(&encoder_states[index]);

  if (INVERT_DIRECTION)
    delta = -delta;

  if (delta)
    encoderDiff += delta;
  lastEncoderBits = current;
}

void setupTimer1()
{
  cli();      // Disable global interrupts
  TCCR1A = 0; // Reset Timer1 control register A
  TCCR1B = 0; // Reset Timer1 control register B

  TCCR1B |= (1 << WGM12);              // Configure Timer1 in CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024

  OCR1A = 15624; // Compare match value for 1 second
                 // Formula: (16MHz / (prescaler * frequency)) - 1

  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  sei();                   // Enable global interrupts
}

ISR(TIMER1_COMPA_vect)
{
  timeSeconds++; // Increment the second counter
  if (timeSeconds >= 60)
  {                  // If 60 seconds have passed
    timeMinutes++;   // Increment the minute variable
    timeSeconds = 0; // Reset the second counter
  }
}

void sanitizeSensors()
{
  const int minValue = -200;
  // const int maxValue = 256;

  for (int i = 0; i < 7; ++i)
  {
    if (sensorTemp[i] < minValue)
    {
      sensorTemp[i] = 0;
    }
    // else if (sensorTemp[i] > maxValue)
    // {
    //   sensorTemp[i] = maxValue;
    // }
    if (sensorHum[i] < minValue)
    {
      sensorHum[i] = 0;
    }
    // else if (sensorHum[i] > maxValue)
    // {
    //   sensorHum[i] = maxValue;
    // }
  }
}
