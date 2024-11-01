#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "ch376.h"
#include "DS3231.h"
#include "imu.h"
#include "hexbits.h"
#include "TimerOne.h"

// Definition User Interface Parameters
#define FRAME_LENGHT 40
#define FRAME_BREADTH 40
#define CURSOR_Y 12
#define ICON_Y 14
#define ICON_NAME_Y 62
#define ICON_WIDTH 32
#define ICON_HIEGHT 32
#define VR_WIDTH 96
#define VR_HIEGHT 48
#define INFINITY_WIDTH 120
#define INFINITY_HIEGHT 60
#define FRUSTUM_BIT_WIDTH 47
#define FRUSTUM_BIT_HEIGHT 46
#define LEFT 0
#define SELECT 1
#define RIGHT 2
#define BACK 3
#define UP 4
#define DOWN 5
#define CURSOR_1 0
#define CURSOR_2 1
#define CURSOR_3 2
#define CURSOR_4 3
#define CURSOR_5 4
#define CURSOR_6 5
#define OPENING_SCREEN 0
#define START_SCAN_SCREEN 1
#define GEOMETRIC_SCREEN 2
#define SPARSE_SCREEN 3
#define VIEW_DATA_SCREEN 4
#define SETTINGS_SCREEN 5
#define CONE_SCREEN_1 6
#define CONE_SCREEN_2 7
#define FRUSTUM_SCREEN_1 8
#define FRUSTUM_SCREEN_2 9
#define FRUSTUM_SCREEN_3 10
#define FRUSTUM_SCREEN_4 11
#define CALIBRATION_SCREEN 12
#define SET_TIME_SCREEN 13
#define SET_DATE_SCREEN 14
#define IMU_DATA_SCREEN 15
#define MESSAGE_SCREEN_1 16
#define MESSAGE_SCREEN_2 17
#define MESSAGE_SCREEN_3 18
#define LIDAR_DATA_SCREEN 19
#define SET_HEIGHT_SCREEN 20
#define COORDINATE_SCREEN 21
#define LENGTH_SCREEN 22
#define ACQUIRE_POINTS_SCREEN_1 23
#define ACQUIRE_POINTS_SCREEN_2 24
#define NEW_VIEW_POINT_SCREEN 25
#define FREQUENCY_SET_SCREEN 26
#define COORDINATE_MODE_SCREEN 27
#define CAPTURE_BOUNDARY_SCREEN 28
#define M_FRAME_COORDINATE 2
#define M_FRAME_WIDTH 124
#define M_FRAME_LENGHT 60
#define M_ICON_X 6
#define M_ICON_WIDTH 24
#define M_ICON_LENGTH 24
#define NUMBER_OF_MONTHS 12
#define OK_FRAME_WIDTH 20
#define OK_FRAME_HIEGHT 12
#define CANCEL_FRAME_WIDTH 40
#define CANCEL_FRAME_HIEGHT 12
#define MESSAGE_FRAME_Y 46
#define MESSAGE_X_CURSOR 34
#define STR_POS_Y 56
#define POLAR_MODE 1
#define CARTESIAN_MODE 2

// X-Position of icon :- It tells what will be the atarting point of the Icon
typedef enum
{
    type_one_screen_x_1 = 8,
    type_one_screen_x_2 = 48,
    type_one_screen_x_3 = 88,
    type_two_screen_x_1 = 24,
    type_two_screen_x_2 = 72
} icon_pos_x;

// X-Position of the frame :- Tells the starting point of the Frmae
typedef enum
{
    type_one_frame_x_1 = 4,
    type_one_frame_x_2 = 44,
    type_one_frame_x_3 = 84,
    type_two_frame_x_1 = 20,
    type_two_frame_x_2 = 68,
    type_three_frame_x_1 = 22,
    type_three_frame_x_2 = 62
} frame_pos_x;

typedef enum
{
    message_y_cursor_1 = 16,
    message_y_cursor_2 = 28,
    message_y_cursor_3 = 40
} y_cur_pos;

typedef enum
{
    ok_str_pos_x = 24,
    cancel_str_pos_x = 64
} str_x_pos;

struct STRING
{
    String xAxis;
    String yAxis;
    String zAxis;
};

class UI
{
private:
    /* data */
    int eeProm_adr;
    bool h12Flag;
    bool pmFlag;
    uint8_t position = 0;
    uint8_t last_position = 0;
    uint8_t setting_screen_number = 0;
    uint8_t view_data_screen_number = 0;
    uint8_t sparse_scan_screen_number = 0;
    uint8_t coordinate_mode = CARTESIAN_MODE;
    uint8_t current_screen = 0;
    const static uint8_t numOfInputs = 6;                             // Number of Buttons
    const int inputPins[numOfInputs] = {34, 32, 35, 33, 37, 36};      // Buttons Connected to the Arduino at Pin no. (7, 6, 5, 4)
    int inputState[numOfInputs];                                      // State in which the Buttons are LOW" or LOW".
    int lastInputState[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW}; // Keeping Last Input Sate of Button as Low
    bool inputFlags[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW};    //
    long lastDebounceTime[numOfInputs] = {0, 0, 0, 0, 0, 0};          // Consatant for Counting time after key pressed.
    unsigned long debounceDelay = 15;

    // void clearLCD();
    void icon_startScan();     // Function for Icon of Start Scan
    void icon_viewData();      // Function for Icon of View Data
    void icon_settings();      // Function for Icon of Settings
    void icon_geometricScan(); // Function for Icon of Geometric Scan
    void icon_sparseScan();    // Function for Icon of Sparse Scan
    void icon_cone();          // Function for Icon of Cone
    void icon_frustum();       // Function for Icon of Frustum
    void icon_lidar();         // Fucntion for Icon of lidar
    void icon_IMU();           // Function for Icon of MPU
    void icon_acquirePoint();  // Function for Icon of Acquire points
    void icon_newViewPoints(); // Function for Icon of New View Point
    void icon_captureBoundary();
    void icon_endScan();          // Function for Icon of End Scan
    void icon_callibration();     // Function for Icon of Calibration
    void icon_setTime();          // Function for Icon of Set time
    void icon_setDate();          // Function for Icon of Set Date
    void icon_gyroCallibration(); // Function for Icon of Gyroscope Calibration
    void icon_magCallibration();  // Function for Icon of Magnetometer Calibration
    void icon_pitchCallibration();
    void icon_coordinate();
    void icon_lenght();
    void icon_setHieght();
    void leftScroll();     // Function for Left Scrolling of the Cursor
    void selectMenu();     // Function to select the Icon on which the Cursor is
    void rightScroll();    // Function to Right scrolling of the Cusrsor
    void backtolastMenu(); // Function to go back to the previous menu
    void scrollUp();
    void scrollDown();         // Function to scroll down
    void icon_framePosition(); // Function to define the cursor position
    void iconName();           // Function to print the Icon name
    void iconPosition();       // Function
    void icon_setCoordinateMode();
    void icon_setFrequency();
    void icon_chargeIndicator();
    void embeddedIcon();
    // void display(); // Function to display hte user Interface
    void userInterfaceMessage();
    void userInput_framePosition();
    void messageBox();
    void gyroCalibration();
    void magCalibration();
    void setTime();
    void setDate();
    void timesettingDisplay();
    void setTimeCursor();
    void datesettingDisplay();
    void setDateCursor();
    void inputAction(int input);
    void driveAttached();
    float lidarDistance();
    float lidarStrength();
    void heightsettingDisplay();
    void setHeight();
    void setHeightCursor();
    void displayHeight();
    Vector coordinatesOfpoints();
    void setCoordinateCursor();
    double distanceBetweenPoints(Vector coordinate_1, Vector coordinate_2);
    void pointCapturedMessage();
    void coordinatePoint(Vector points);
    void pointPositionStringWidth();
    double volumeOfCone();
    void theLengthIs();
    void heightToEEPROM();
    float heightFromEEPROM();
    void setBiasedPitchToEEPROM();
    double getBiasedPitchFromEEPROM();
    void setFrequencytoEEPROM();
    uint8_t getFrequencyFromEEPROM();
    void heightDigit();
    void bottomEdgeLengthDisplay();
    void frustumPointStrWidth();
    void setFrustumCursor();
    void frustum_display(uint8_t const *bitmap, double length);
    void errorMessage();
    double volumeOfFrustum();
    Vector acquirePoints();
    STRING doubleToString(Vector points_to_string);
    void minuteCountFunction();
    void makeBuzzerSound();
    void displayCoordinateMode();
    void setFrequency();
    void displayFrequency();
    void setFrequencyCursor();
    void setCoordinateModeStrPosition();
    void setCoordinateModeCursor();
    void saveDataToDrive(Vector strings);
    void saveViewPoint();

public:
    void begin();
    void keysetUp();
    void logo();              // Function for Logo
    void setInputFlags();     // Debouncing of the switches
    void resolveInputFlags(); //
    void lidarsetUp();
    void lidarData_display();
    ANGLE getIMUangles();
    void imuData_display();
    void coordinate_display();
    void displayTime();
    void displayDate();
    void lengthDisplay();
    void volumeDisplay();
    void flashDataToDrive();
    void display(); // Function to display hte user Interface
    void charge_Indicator();
    void getPitchBias();
};

#endif
