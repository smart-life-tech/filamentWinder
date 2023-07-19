#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define outputA 2 // CLK pin
#define outputB 3 // DT pin
#define rstbtn 4  // reset button pin
// Button pins
const int UP_BUTTON_PIN = 6;
const int DOWN_BUTTON_PIN = 7;
const int OK_BUTTON_PIN = 8;
// EEPROM address to store the value
const int EEPROM_ADDRESS = 0;
int buzzerPin = 9;
int value = 0;       // The value to be set
int storedValue = 0; // The value stored in EEPROM

int counter = 0;
const float pi = 3.14; // Pi value

const int R = 7;  // Radius of the wheel from center to edge
const int N = 40; // no of steps for one rotation
float distance = 0;

void setup()
{
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Measuring Wheel");
    delay(2000);
    lcd.clear();

    lcd.print("Value: ");
    lcd.setCursor(7, 0);
    lcd.print(value);

    // Read the stored value from EEPROM
    storedValue = EEPROM.read(EEPROM_ADDRESS);
    lcd.setCursor(7, 1);
    lcd.print("Stored: ");
    lcd.print(storedValue);
    delay(2000);
    lcd.clear();
    pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(OK_BUTTON_PIN, INPUT_PULLUP);

    // Initialize encoder pins
    pinMode(outputA, INPUT);
    pinMode(outputB, INPUT);
    pinMode(rstbtn, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(outputA), readEncoder, FALLING);
}

void readEncoder()
{
    int bValue = digitalRead(outputB);
    if (bValue == HIGH)
    {
        counter++; // Clockwise
    }
    if (bValue == LOW)
    {
        counter--; // Counterclockwise
    }
}

// Get the counter value, disabling interrupts.
// This make sure readEncoder() doesn't change the value
// while we're reading it.
int getCounter()
{
    int result;
    noInterrupts();
    result = counter;
    interrupts();
    return result;
}

void resetCounter()
{
    noInterrupts();
    counter = 0;
    interrupts();
}

void loop()
{
    distance = ((2 * pi * R) / N) * getCounter();
    lcd.setCursor(0, 0);
    lcd.print("Distance: / cms");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print("        ");
    while (distance >= value)
    {
        tone(buzzerPin, 1000, 2000);
        noTone();
        tone(buzzerPin, 1000, 2000);
        noTone();
        if (digitalRead(OK_BUTTON_PIN) == LOW)
        {
            resetCounter();
            distance = 0;
            break;
        }
    }
    if (digitalRead(OK_BUTTON_PIN) == LOW)
    {
        resetCounter();
        delay(1000);
        lcd.clear();
        do
        {
            setDistance();
            if (!digitalRead(OK_BUTTON_PIN))
            {
                delay(1000);
                break;
            }
            /* code */
        } while (1);
    }
}

void setDistance()
{
    int upButtonState = digitalRead(UP_BUTTON_PIN);
    int downButtonState = digitalRead(DOWN_BUTTON_PIN);
    int okButtonState = digitalRead(OK_BUTTON_PIN);

    if (upButtonState == LOW)
    {
        value++;
        lcd.setCursor(7, 0);
        lcd.print("   "); // Clear previous value
        lcd.setCursor(7, 0);
        lcd.print(value);
        delay(200); // Debounce delay
    }

    if (downButtonState == LOW)
    {
        value--;
        lcd.setCursor(7, 0);
        lcd.print("   "); // Clear previous value
        lcd.setCursor(7, 0);
        lcd.print(value);
        delay(200); // Debounce delay
    }

    if (okButtonState == LOW)
    {
        lcd.setCursor(7, 1);
        lcd.print("   "); // Clear previous stored value
        lcd.setCursor(7, 1);
        lcd.print("Saving...");
        EEPROM.write(EEPROM_ADDRESS, value);
        delay(1000); // Display "Saving..." for 1 second
        lcd.setCursor(7, 1);
        lcd.print("       "); // Clear "Saving..."
        lcd.setCursor(7, 1);
        lcd.print("Stored: ");
        lcd.print(value);
        delay(200); // Debounce delay
    }
}