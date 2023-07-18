#include <U8glib.h>
#include <AccelStepper.h>

// Define the pin connections
#define LCD_PIN_RS 16
#define LCD_PIN_EN 17
#define LCD_PIN_D4 23
#define LCD_PIN_D5 25
#define LCD_PIN_D6 27
#define LCD_PIN_D7 29
#define BUZZER_PIN 7
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 4
#define BUTTON_PIN 5
#define MOTOR1_STEP_PIN 54
#define MOTOR1_DIR_PIN 55
#define MOTOR2_STEP_PIN 60
#define MOTOR2_DIR_PIN 61

// Define the maximum speed and acceleration for the stepper motors
#define MAX_SPEED 1000
#define ACCELERATION 500

U8GLIB_ST7920_128X64_1X u8g(LCD_PIN_EN, LCD_PIN_RS, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);
AccelStepper stepper1(AccelStepper::DRIVER, MOTOR1_STEP_PIN, MOTOR1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);

int encoderValue = 0;
int lastEncoderValue = 0;

void setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    u8g.setRot180();             // Set the display rotation if needed
    u8g.setFont(u8g_font_8x13B); // Set the font for text

    // Initialize the LCD
    u8g.begin();

    // Set the maximum speed and acceleration for the stepper motors
    stepper1.setMaxSpeed(MAX_SPEED);
    stepper1.setAcceleration(ACCELERATION);
    stepper2.setMaxSpeed(MAX_SPEED);
    stepper2.setAcceleration(ACCELERATION);
}

void loop()
{
    u8g.firstPage();
    do
    {
        // Display "Hello, World!" on the screen
        u8g.drawStr(10, 30, "Hello, World!");

        // Check if the button is pressed
        if (digitalRead(BUTTON_PIN) == LOW)
        {
            // Perform some action when the button is pressed
            digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
            delay(1000);                    // Delay for 1 second
            digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
        }

        // Read the encoder rotation
        int encoderPinA = digitalRead(ENCODER_PIN_A);
        if (encoderPinA != lastEncoderValue)
        {
            if (digitalRead(ENCODER_PIN_B) != encoderPinA)
            {
                encoderValue++;
            }
            else
            {
                encoderValue--;
            }
        }
        lastEncoderValue = encoderPinA;

        // Display the encoder value on the screen
        u8g.setPrintPos(10, 50);
        u8g.print("Encoder: ");
        u8g.print(encoderValue);

        // Adjust the speed of stepper1 based on the encoder value
        int mappedSpeed1 = map(encoderValue, 0, 1023, 0, MAX_SPEED);
        stepper1.setSpeed(mappedSpeed1);

        // Start the steppers
        stepper1.runSpeed();
        stepper2.runSpeed();

    } while (u8g.nextPage());

    // Add delay or other code here if needed
}
