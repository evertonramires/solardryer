#define DEBUG_TARGET_TEMP 50
#define DEBUG_TARGET_HUM 10
#define DEBUG_TARGET_OFFSET 1
#define REGISTER_INTERVAL_MINUTES 1 // Interval in minutes to register data

float dewPoint = 0;

constexpr unsigned int SENSOR0_PIN{11}; // Pin for the first sensor
constexpr unsigned int SENSOR1_PIN{4};  // Pin for the second sensor
constexpr unsigned int SENSOR2_PIN{5};  // Pin for the third sensor
constexpr unsigned int SENSOR3_PIN{6};  // Pin for the fourth sensor
constexpr unsigned int SENSOR4_PIN{24}; // Pin for the fifth sensor
constexpr unsigned int SENSOR5_PIN{25}; // Pin for the sixth sensor
constexpr unsigned int SENSOR6_PIN{26}; // Pin for the seventh sensor

constexpr unsigned int FAN0_PIN{2};  // Pin for the first fan
constexpr unsigned int FAN1_PIN{3};  // Pin for the second fan
constexpr unsigned int FAN2_PIN{7};  // Pin for the third fan
constexpr unsigned int FAN3_PIN{8};  // Pin for the fourth fan
constexpr unsigned int FAN4_PIN{9};  // Pin for the fifth fan
constexpr unsigned int FAN5_PIN{10}; // Pin for the sixth fan
constexpr unsigned int FAN6_PIN{12}; // Pin for the seventh fan

constexpr unsigned int VALVE0_PIN{13}; // Pin for the first valve
constexpr unsigned int VALVE1_PIN{14}; // Pin for the second valve
constexpr unsigned int VALVE2_PIN{15}; // Pin for the third valve
constexpr unsigned int VALVE3_PIN{16}; // Pin for the fourth valve
constexpr unsigned int VALVE4_PIN{17}; // Pin for the fifth valve
constexpr unsigned int VALVE5_PIN{18}; // Pin for the sixth valve
constexpr unsigned int VALVE6_PIN{19}; // Pin for the seventh valve

constexpr unsigned int PELTIERS_PIN{40};

// Encoder and button pins from the full graphic controller
const int encoderPinA = 31;
const int encoderPinB = 33;
const int encoderButtonPin = 35;

// Buzzer pin (commonly 37 on RAMPS with full graphic controller)
const int buzzerPin = 37;

// Pin configuration
const int SD_CS_PIN = 53; // Change to 10 for Arduino UNO or 53 for MEGA
File logFile;             // File object for SD card

bool sensorStatus[7] = {0, 0, 0, 0, 0, 0, 0};
int sensorTemp[7] = {0, 0, 0, 0, 0, 0, 0};
int sensorHum[7] = {0, 0, 0, 0, 0, 0, 0};

bool sdPresent = false;
int globalFanSpeed = 0;
unsigned int fanSpeed[7] = {0, 0, 0, 0, 0, 0, 0};
unsigned int fanSpeedPercent[7] = {0, 0, 0, 0, 0, 0, 0};

bool valveStatus[7] = {0, 0, 0, 0, 0, 0, 0};

volatile unsigned long timeSeconds = 0;
volatile unsigned long timeMinutes = 0;
volatile unsigned long lastTimeRegistered = -1;

unsigned int peltiersPower = 0;
unsigned int peltiersPowerPercent = 0;

constexpr unsigned int sensorStartTry = 3;

// Number of encoder increments needed to move one menu item
const int STEPS_PER_ITEM = 3;
// Invert direction if needed
const bool INVERT_DIRECTION = false;
// Delay after moving the cursor to prevent too rapid moves (in ms)
const unsigned long MOVE_DELAY_MS = 250;

// Display (RepRap Discount Full Graphic Smart Controller)
U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);

// Menu items: now we have 8 options total
const char *menuItems[] = {
    "0:------",
    "1:------",
    "2:------",
    "3:------",
    "4:------",
    "5:------",
    "6:------",
    "About s2"};
const int menuLength = 7; // Now we have 8 items total

// Current selection in the menu
int currentSelection = 0;

unsigned short currentMenu = 3; // Start with menu 3, the summarized one

// Font and layout
const u8g_fntpgm_uint8_t *font = u8g_font_6x10;
int lineHeight = 12; // vertical spacing per line

// For two columns: each column will have 4 items
// We'll define column coordinates
int columnX[2] = {0, 64}; // left x position for column 1 and column 2
int textIndent = 10;      // text indent from marker

// Encoder state variables
volatile int8_t encoderDiff = 0;    // Accumulated increments from the encoder
static uint8_t lastEncoderBits = 0; // Last 2-bit state of encoder signals

// Button state tracking
uint8_t lastButtonState = HIGH;
unsigned long lastButtonTime = 0;
const unsigned long BUTTON_DEBOUNCE_MS = 50;

// Movement delay tracking
unsigned long lastMoveTime = 0; // When did we last move the cursor?

// Encoder state table from a Marlin-like approach
static const int8_t encoder_states[16] PROGMEM = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0};

// After selecting an option, show it on screen for 1 second
bool showSelection = false;
unsigned long selectionStartTime = 0;
char selectedItem[20];

// Timer interrupt setup
void setupTimer2ForEncoder();