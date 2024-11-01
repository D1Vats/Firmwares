#include "UI.h"

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

USB flashDrive(Serial1, 115200);
DS3231 clock;
IMU imu;

const static char *file_name;
const static char *dir_name_set;
static int polar_mode_x;
static int cartesian_mode_x;
static int ICON_NAME_X;
static int top_pos_x;
static int bot_pos_x;
static int frustum_point_1_Pos_x;
static int frustum_point_2_Pos_x;
static int frustum_next_Pos_x;
static int sparse_scan_screen_count = 0;
static uint8_t w_1 = 0;
static uint8_t w_2 = 0;
static uint8_t w_3 = 0;
static uint8_t w_4 = 0;
static uint8_t w_5 = 0;
static uint8_t w_6 = 0;
static uint8_t w_7 = 0;
static uint8_t hour_count;
static uint8_t minute_count;
static uint8_t last_minute_count;
static uint8_t last_hour_count;
static uint8_t day_count;
static uint8_t month_count;
static uint8_t year_count;
static uint8_t height_uint_place_digit_count;
static uint8_t height_tenth_place_digit_count;
static uint8_t height_Hundred_place_digit_count;
static uint8_t data_per_second = 1;
static uint8_t change_of_view_point = 0;
static long time_period;
static long buzzer_delay = 100;
static float height;
static double distance_between_two_points;
static double volume;
static double length_of_bot_edge_1;
static double length_of_bot_edge_2;
static double length_of_top_edge_1;
static double length_of_top_edge_2;
static double biased_pitch;
static Vector point_1;
static Vector point_2;
static Vector cone_top_point;
static Vector cone_bot_point;
static Vector frustum_bot_1;
static Vector frustum_bot_2;
static Vector frustum_bot_3;
static Vector frustum_top_1;
static Vector frustum_top_2;
static Vector frustum_top_3;
static Vector acquire_points;
static uint8_t count_1, count_2; // acquire_count;
static Vector view_point;
static String month;
static String file;

// Name of the Icon that will be printed on the screen.
const static char *point_pos_str[] = {"Top", "Back", "Bottom"};
const static char *setting_str[] = {"Set Time", "Set Date", "Set Height"};
const static char *nameoficons_1st_screen[] = {"Start Scan", "View Data", "Settings"};
const static char *nameoficons_2nd_screen[] = {"Geometric Scan", "Sparse Scan"};
const static char *nameoficons_3rd_screen[] = {"Cone", "Frustum"};
const static char *nameoficons_4th_screen[] = {"Acqiure Points", "Get Boundary Point", "New View Point", "End Scan"};
const static char *nameoficons_5th_screen[] = {"Lidar Data", "IMU Data", "Coordinates", "Length"};
const static char *nameoficons_6th_screen[] = {"Set Height", "Set mode", "Set Frequency", "Set time", "Set date", "Callibration"};
const static char *nameoficons_7th_screen[] = {"Gyroscope", "Magnetometer", "Pitch Angle"};
// const static char *nameoficons_8th_screen[] = {"Capture Point", "Get Volume"};
const static char *frustum_point_str[] = {"Point1", "Point2", "Next"};
const static char *sparse_scan_str[] = {"Acquire", "Back", "Select", "Capture"};
const static char *coordinate_mode_str[] = {"Cartesian", "Polar"};
const static String colorOfPoint[] = {"25\n", "50\n", "75\n", "100\n", "125\n", "150\n", "175\n", "200\n", "225\n", "250\n"};

void UI ::begin()
{
  u8g2.begin(); // Initialiazing OLED
  imu.imuBegin();
  flashDrive.init(); // Initialiazing Ch376s
  imu.gyro_data_From_EEprom();
  imu.mag_data_From_EEprom();
  heightFromEEPROM();
  getBiasedPitchFromEEPROM();
  getFrequencyFromEEPROM();
  iconPosition();
  heightDigit();
}

// Function Definition for Logo
void UI ::logo()
{
  u8g2.firstPage();
  do
  {
    u8g2.drawXBMP(16, 8, VR_WIDTH, VR_HIEGHT, VR__bits);
  } while (u8g2.nextPage());
  delay(2000);
  // clearLCD();
  displayDate();
  delay(2000);
}

void UI ::pointCapturedMessage()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(16, 39, "Point Captured");
    u8g2.drawFrame(14, 24, 102, 18);
  } while (u8g2.nextPage());
  delay(2000);
}

// Function definition for all the Icons.
void UI ::icon_startScan()
{
  u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, start_Scan_bits);
}

void UI ::icon_viewData()
{
  u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, view_Data_bits);
}

void UI ::icon_settings()
{
  u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, settings_bits);
}

void UI ::icon_geometricScan()
{
  u8g2.drawXBMP(type_two_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Geometrica_bits);
}

void UI ::icon_sparseScan()
{
  u8g2.drawXBMP(type_two_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, sparsea_bits);
}

void UI ::icon_cone()
{
  u8g2.drawXBMP(type_two_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, cone_bits);
}

void UI ::icon_frustum()
{
  u8g2.drawXBMP(type_two_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Frustrum_1_bits);
}

void UI ::icon_acquirePoint()
{
  if ((last_position == position && sparse_scan_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Aquire_points_bits);
  }
}

void UI ::icon_newViewPoints()
{
  if ((last_position == position && sparse_scan_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && sparse_scan_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Capture_Boundary_bits);
  }
  if ((last_position == position && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && sparse_scan_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && sparse_scan_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Capture_Boundary_bits);
  }
}

void UI ::icon_captureBoundary()
{
  if ((last_position == position && sparse_scan_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && sparse_scan_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, New_view_point_bits);
  }
  if ((last_position == position && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && sparse_scan_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && sparse_scan_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, New_view_point_bits);
  }
}

void UI ::icon_endScan()
{
  if ((last_position == position && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && sparse_scan_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && sparse_scan_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && sparse_scan_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && sparse_scan_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, End_scan_bits);
  }
}

void UI ::icon_setHieght()
{
  if ((last_position == position && setting_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Hieght_set_bits);
  }
}

void UI ::icon_setCoordinateMode()
{
  if ((last_position == position && setting_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && setting_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, coordinate_mode_bits);
  }
  if ((last_position == position && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && setting_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, coordinate_mode_bits);
  }
}

void UI ::icon_setFrequency()
{
  if ((last_position == position && setting_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && setting_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Set_Frequency_1_bits);
  }
  if ((last_position == position && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && setting_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Set_Frequency_1_bits);
  }
  if ((last_position == position && setting_screen_number == 2) || (last_position == CURSOR_4 && position == CURSOR_5 && setting_screen_number == 2) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 2) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 2) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 2))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Set_Frequency_1_bits);
  }
}

void UI ::icon_setTime()
{
  if ((last_position == position && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && setting_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && setting_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, set_20time_bits);
  }
  if ((last_position == position && setting_screen_number == 2) || (last_position == CURSOR_4 && position == CURSOR_5) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 2) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 2) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 2))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, set_20time_bits);
  }
  if ((last_position == position && setting_screen_number == 3) || (last_position == CURSOR_5 && position == CURSOR_6) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 3) || (last_position == CURSOR_6 && position == CURSOR_5) || (last_position == CURSOR_4 && position == CURSOR_5 && setting_screen_number == 3))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, set_20time_bits);
  }
}

void UI ::icon_setDate()
{
  if ((last_position == position && setting_screen_number == 2) || (last_position == CURSOR_4 && position == CURSOR_5 && setting_screen_number == 2) || (last_position == CURSOR_4 && position == CURSOR_3 && setting_screen_number == 2) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 2) || (last_position == CURSOR_3 && position == CURSOR_4 && setting_screen_number == 2))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, set_20Date_bits);
  }
  if ((last_position == position && setting_screen_number == 3) || (last_position == CURSOR_5 && position == CURSOR_6) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 3) || (last_position == CURSOR_6 && position == CURSOR_5 && setting_screen_number == 3) || (last_position == CURSOR_4 && position == CURSOR_5 && setting_screen_number == 3))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, set_20Date_bits);
  }
}

void UI ::icon_callibration()
{
  if ((last_position == position && setting_screen_number == 3) || (last_position == CURSOR_5 && position == CURSOR_6) || (last_position == CURSOR_5 && position == CURSOR_4 && setting_screen_number == 3) || (last_position == CURSOR_6 && position == CURSOR_5) || (last_position == CURSOR_4 && position == CURSOR_5 && setting_screen_number == 3))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Calibrate_bits);
  }
}

void UI ::icon_gyroCallibration()
{
  u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, gyros_bits);
}

void UI ::icon_magCallibration()
{
  u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, megneto_bits);
}

void UI ::icon_pitchCallibration()
{
  u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Pitch_icon_bits);
}

void UI ::icon_lidar()
{
  if ((last_position == position && view_data_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Lidar_bits);
  }
}

void UI ::icon_IMU()
{
  if ((last_position == position && view_data_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && view_data_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, IMU_1_bits);
  }
  if ((last_position == position && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && view_data_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && view_data_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_1, ICON_Y, ICON_WIDTH, ICON_HIEGHT, IMU_1_bits);
  }
}

void UI ::icon_coordinate()
{
  if ((last_position == position && view_data_screen_number == 0) || (last_position == CURSOR_2 && position == CURSOR_1) || (last_position == CURSOR_1 && position == CURSOR_2) || (last_position == CURSOR_2 && position == CURSOR_3 && view_data_screen_number == 0) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 0))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Coordinate_point_bits);
  }
  if ((last_position == position && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && view_data_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && view_data_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_2, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Coordinate_point_bits);
  }
}

void UI ::icon_lenght()
{
  if ((last_position == position && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_4 && view_data_screen_number == 1) || (last_position == CURSOR_4 && position == CURSOR_3 && view_data_screen_number == 1) || (last_position == CURSOR_3 && position == CURSOR_2 && view_data_screen_number == 1) || (last_position == CURSOR_2 && position == CURSOR_3 && view_data_screen_number == 1))
  {
    u8g2.drawXBMP(type_one_screen_x_3, ICON_Y, ICON_WIDTH, ICON_HIEGHT, Lenght_line_bits);
  }
}

void UI ::icon_chargeIndicator()
{
  u8g2.drawXBMP(28, 14, 72, 36, Battery_charging_bits);
}

void UI ::keysetUp()
{
  for (int i = 0; i < 6; i++)
  {
    pinMode(inputPins[i], INPUT);     // Setting up Pin Mode to take input from Keys
    digitalWrite(inputPins[i], HIGH); // pull-up 20k
  }
  pinMode(4, OUTPUT);
  pinMode(A1, INPUT);
}

void UI ::setInputFlags()
{
  for (int i = 0; i < 6; i++)
  {
    int reading = digitalRead(inputPins[i]); // Reading data from keys through pins{7, 6, 53, 9, 8}
    if (reading != lastInputState[i])        // Checking if the reading is not equal to Last input state if its not
    {
      lastDebounceTime[i] = millis(); // timer will start which counts in millisecond
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) // Checking if debounce Delay time is less than difference between millis() and lastdebounce Time if yes
    {

      if (reading != inputState[i]) // check if reading is not equal to inputState[i] if not
      {
        inputState[i] = reading;   // then make it equal
        if (inputState[i] == HIGH) // check if inputstate is HIGH if yes
        {
          inputFlags[i] = HIGH; // make input flag HIGH
        }
      }
    }
    lastInputState[i] = reading; // making last Input State equal to reading
  }
}

void UI ::resolveInputFlags()
{
  for (int i = 0; i < 6; i++)
  {
    if (inputFlags[i] == HIGH)
    {
      inputAction(i); // Function To dertermine which action to take when given key is pressed.
      iconPosition();
      pointPositionStringWidth();
      setCoordinateModeStrPosition();
      frustumPointStrWidth();
      userInterfaceMessage();
      timesettingDisplay();
      datesettingDisplay();
      heightsettingDisplay();
      bottomEdgeLengthDisplay();
      minuteCountFunction();
      displayFrequency();
      displayCoordinateMode();
      inputFlags[i] = LOW; // Setting Input Flag to Zero.
    }
  }
}

void UI ::inputAction(int input)
{
  switch (input)
  {
  case LEFT:
    leftScroll();
    last_position = position + 1;
    break;
  case SELECT:
    selectMenu();
    last_position = position;
    break;
  case RIGHT:
    rightScroll();
    last_position = position - 1;
    break;
  case BACK:
    backtolastMenu();
    last_position = position;
    break;
  case UP:
    scrollUp();
    break;
  case DOWN:
    scrollDown();
  default:
    break;
  }
}

void UI ::leftScroll()
{
  makeBuzzerSound();
  if (current_screen == OPENING_SCREEN || current_screen == SETTINGS_SCREEN || current_screen == SPARSE_SCREEN || current_screen == SET_HEIGHT_SCREEN || current_screen == LENGTH_SCREEN || current_screen == FRUSTUM_SCREEN_4 || current_screen == FREQUENCY_SET_SCREEN || current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    if (position != CURSOR_1)
    {
      position--;
    }
    if (position == CURSOR_1 && current_screen == SETTINGS_SCREEN)
    {
      setting_screen_number = 0;
    }
    if ((position == CURSOR_2 && last_position == CURSOR_4 && setting_screen_number == 2 && current_screen == SETTINGS_SCREEN))
    {
      setting_screen_number = 1;
    }
    if ((position == CURSOR_3 && last_position == CURSOR_5 && setting_screen_number == 3 && current_screen == SETTINGS_SCREEN))
    {
      setting_screen_number = 2;
    }
    if (position == CURSOR_1 && current_screen == SPARSE_SCREEN)
    {
      sparse_scan_screen_number = 0;
    }
  }
  if (current_screen == START_SCAN_SCREEN || current_screen == GEOMETRIC_SCREEN || current_screen == MESSAGE_SCREEN_1 || current_screen == MESSAGE_SCREEN_2 || current_screen == FRUSTUM_SCREEN_2 || current_screen == FRUSTUM_SCREEN_3 || current_screen == NEW_VIEW_POINT_SCREEN)
  {
    if (position != CURSOR_1)
    {
      position--;
    }
  }
  if (current_screen == SET_TIME_SCREEN || current_screen == SET_DATE_SCREEN || current_screen == VIEW_DATA_SCREEN || current_screen == CALIBRATION_SCREEN || current_screen == FRUSTUM_SCREEN_1 || current_screen == CONE_SCREEN_1 || current_screen == CONE_SCREEN_2 || current_screen == MESSAGE_SCREEN_3)
  {
    if (position != CURSOR_1)
    {
      position--;
    }
    if (position == CURSOR_1 && current_screen == VIEW_DATA_SCREEN)
    {
      view_data_screen_number = 0;
    }
  }
}

void UI ::selectMenu()
{
  makeBuzzerSound();
  if (current_screen == OPENING_SCREEN && position == CURSOR_1)
  {
    current_screen = START_SCAN_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == START_SCAN_SCREEN && position == CURSOR_1)
  {
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == GEOMETRIC_SCREEN && position == CURSOR_1)
  {
    current_screen = CONE_SCREEN_1;
    position = CURSOR_1;
  }
  else if (current_screen == CONE_SCREEN_1 && position == CURSOR_1)
  {
    cone_top_point = coordinatesOfpoints();
    pointCapturedMessage();
    current_screen = CONE_SCREEN_2;
    position = CURSOR_1;
  }
  else if (current_screen == CONE_SCREEN_2 && position == CURSOR_1)
  {
    cone_bot_point = coordinatesOfpoints();
    pointCapturedMessage();
    volumeDisplay();
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == CONE_SCREEN_1 && position == CURSOR_2)
  {
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == CONE_SCREEN_2 && position == CURSOR_2)
  {
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == GEOMETRIC_SCREEN && position == CURSOR_2)
  {
    current_screen = FRUSTUM_SCREEN_1;
    position = CURSOR_1;
  }
  else if (current_screen == FRUSTUM_SCREEN_1 && position == CURSOR_1)
  {
    frustum_bot_1 = coordinatesOfpoints();
    pointCapturedMessage();
    count_1++;
    position = CURSOR_2;
  }
  else if (current_screen == FRUSTUM_SCREEN_1 && position == CURSOR_2)
  {
    frustum_bot_2 = coordinatesOfpoints();
    pointCapturedMessage();
    length_of_bot_edge_1 = distanceBetweenPoints(frustum_bot_1, frustum_bot_2);
    count_2++;
    position = CURSOR_3;
  }
  else if (current_screen == FRUSTUM_SCREEN_1 && position == CURSOR_3)
  {
    if ((count_1 == 0 || count_2 == 0) && (length_of_bot_edge_1 == 0))
    {
      errorMessage();
      delay(1000);
      current_screen = FRUSTUM_SCREEN_1;
      position = CURSOR_1;
    }
    else
    {
      current_screen = FRUSTUM_SCREEN_2;
      position = CURSOR_1;
      count_1 = 0;
      count_2 = 0;
    }
  }
  else if (current_screen == FRUSTUM_SCREEN_3 && position == CURSOR_1)
  {
    frustum_bot_2 = coordinatesOfpoints();
    pointCapturedMessage();
    count_1++;
    position = CURSOR_2;
  }
  else if (current_screen == FRUSTUM_SCREEN_3 && position == CURSOR_2)
  {
    frustum_bot_3 = coordinatesOfpoints();
    pointCapturedMessage();
    length_of_bot_edge_2 = distanceBetweenPoints(frustum_bot_2, frustum_bot_3);
    count_2++;
    position = CURSOR_3;
  }
  else if (current_screen == FRUSTUM_SCREEN_3 && position == CURSOR_3)
  {
    if ((count_1 == 0 || count_2 == 0) && (length_of_bot_edge_2 == 0))
    {
      errorMessage();
      delay(1000);
      current_screen = FRUSTUM_SCREEN_3;
      position = CURSOR_1;
    }
    else
    {
      current_screen = FRUSTUM_SCREEN_4;
      position = CURSOR_1;
      count_1 = 0;
      count_2 = 0;
    }
  }
  else if (current_screen == FRUSTUM_SCREEN_2 && position == CURSOR_1)
  {
    frustum_top_1 = coordinatesOfpoints();
    pointCapturedMessage();
    count_1++;
    position = CURSOR_2;
  }
  else if (current_screen == FRUSTUM_SCREEN_2 && position == CURSOR_2)
  {
    frustum_top_2 = coordinatesOfpoints();
    pointCapturedMessage();
    length_of_top_edge_1 = distanceBetweenPoints(frustum_top_1, frustum_top_2);
    count_2++;
    position = CURSOR_3;
  }
  else if (current_screen == FRUSTUM_SCREEN_2 && position == CURSOR_3)
  {
    if ((count_1 == 0 || count_2 == 0) && (length_of_top_edge_1 == 0))
    {
      errorMessage();
      delay(1000);
      current_screen = FRUSTUM_SCREEN_2;
      position = CURSOR_1;
    }
    else
    {
      current_screen = FRUSTUM_SCREEN_3;
      position = CURSOR_1;
      count_1 = 0;
      count_2 = 0;
    }
  }
  else if (current_screen == FRUSTUM_SCREEN_4 && position == CURSOR_1)
  {
    frustum_top_2 = coordinatesOfpoints();
    pointCapturedMessage();
    count_1++;
    position = CURSOR_2;
  }
  else if (current_screen == FRUSTUM_SCREEN_4 && position == CURSOR_2)
  {
    frustum_top_3 = coordinatesOfpoints();
    pointCapturedMessage();
    length_of_top_edge_2 = distanceBetweenPoints(frustum_top_2, frustum_top_3);
    count_2++;
    position = CURSOR_3;
  }
  else if (current_screen == FRUSTUM_SCREEN_4 && position == CURSOR_3)
  {
    if ((count_1 == 0 || count_2 == 0) && (length_of_top_edge_2 == 0))
    {
      errorMessage();
      delay(1000);
      current_screen = FRUSTUM_SCREEN_4;
      position = CURSOR_1;
    }
    else
    {
      volumeDisplay();
      count_1 = 0;
      count_2 = 0;
      current_screen = GEOMETRIC_SCREEN;
      position = CURSOR_2;
    }
  }
  else if (current_screen == OPENING_SCREEN && position == CURSOR_2)
  {
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == OPENING_SCREEN && position == CURSOR_3)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_1)
  {
    current_screen = SET_HEIGHT_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_6)
  {
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == CALIBRATION_SCREEN && position == CURSOR_2)
  {
    current_screen = MESSAGE_SCREEN_2;
    position = CURSOR_1;
  }
  else if (current_screen == CALIBRATION_SCREEN && position == CURSOR_3)
  {
    current_screen = MESSAGE_SCREEN_3;
    position = CURSOR_1;
  }
  else if (current_screen == MESSAGE_SCREEN_1 && position == CURSOR_1)
  {
    gyroCalibration();
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == MESSAGE_SCREEN_2 && position == CURSOR_1)
  {
    magCalibration();
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == MESSAGE_SCREEN_3 && position == CURSOR_1)
  {
    getPitchBias();
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == MESSAGE_SCREEN_1 && position == CURSOR_2)
  {
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == MESSAGE_SCREEN_2 && position == CURSOR_2)
  {
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == MESSAGE_SCREEN_3 && position == CURSOR_2)
  {
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_2)
  {
    current_screen = COORDINATE_MODE_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_3)
  {
    current_screen = FREQUENCY_SET_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_4)
  {
    current_screen = SET_TIME_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SETTINGS_SCREEN && position == CURSOR_5)
  {
    current_screen = SET_DATE_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == COORDINATE_MODE_SCREEN && position == CURSOR_1)
  {
    coordinate_mode = CARTESIAN_MODE;
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == COORDINATE_MODE_SCREEN && position == CURSOR_2)
  {
    coordinate_mode = POLAR_MODE;
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == SET_TIME_SCREEN && (position == CURSOR_3 || position == CURSOR_2 || position == CURSOR_1))
  {
    setTime();
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_4;
  }
  else if (current_screen == SET_TIME_SCREEN && position == CURSOR_4)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_4;
  }
  else if (current_screen == SET_DATE_SCREEN && (position == CURSOR_4 || position == CURSOR_3 || position == CURSOR_2 || position == CURSOR_1))
  {
    setDate();
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_5;
  }
  else if (current_screen == SET_DATE_SCREEN && position == CURSOR_5)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_5;
  }
  else if (current_screen == FREQUENCY_SET_SCREEN && (position == CURSOR_2 || position == CURSOR_1))
  {
    setFrequencytoEEPROM();
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == FREQUENCY_SET_SCREEN && position == CURSOR_3)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == START_SCAN_SCREEN && position == CURSOR_2)
  {
    setFrequency();
    sparse_scan_screen_count = 0;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SPARSE_SCREEN && position == CURSOR_1)
  {
    if (flashDrive.driveReady())
    {
      current_screen = ACQUIRE_POINTS_SCREEN_1;
      position = CURSOR_1;
    }
    else
    {
      u8g2.firstPage();
      do
      {
        u8g2.setFont(u8g2_font_nokiafc22_tf);
        u8g2.drawFrame(16, 24, 96, 24);
        u8g2.drawStr(22, 40, "USB Not Attached!");
      } while (u8g2.nextPage());
      delay(2000);
    }
  }
  else if (current_screen == ACQUIRE_POINTS_SCREEN_1 && position == CURSOR_1)
  {
    saveViewPoint();
    current_screen = ACQUIRE_POINTS_SCREEN_2;
    position = CURSOR_1;
    // acquire_count++;
  }
  else if (current_screen == ACQUIRE_POINTS_SCREEN_2 && position == CURSOR_1)
  {
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SPARSE_SCREEN && position == CURSOR_3)
  {
    current_screen = NEW_VIEW_POINT_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == NEW_VIEW_POINT_SCREEN && position == CURSOR_1)
  {
    Vector new_view_point = coordinatesOfpoints();
    view_point.xAxis = view_point.xAxis + new_view_point.xAxis;
    view_point.yAxis = view_point.yAxis + new_view_point.yAxis;
    view_point.zAxis = view_point.zAxis + new_view_point.zAxis;
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_3;
    change_of_view_point++;
  }
  else if (current_screen == NEW_VIEW_POINT_SCREEN && position == CURSOR_2)
  {
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == SPARSE_SCREEN && position == CURSOR_2)
  {
    current_screen = CAPTURE_BOUNDARY_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == CAPTURE_BOUNDARY_SCREEN && position == CURSOR_1)
  {
    saveViewPoint();
  }
  else if (current_screen == CAPTURE_BOUNDARY_SCREEN && position == CURSOR_2)
  {
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == SPARSE_SCREEN && position == CURSOR_4)
  {
    sparse_scan_screen_count = 0;
    sparse_scan_screen_number = 0;
    view_point.xAxis = 0.00;
    view_point.yAxis = 0.00;
    view_point.zAxis = 0.00;
    current_screen = START_SCAN_SCREEN;
    position = CURSOR_2;
    change_of_view_point = 0;
  }
  else if (current_screen == VIEW_DATA_SCREEN && position == CURSOR_2)
  {
    current_screen = IMU_DATA_SCREEN;
  }
  else if (current_screen == VIEW_DATA_SCREEN && position == CURSOR_1)
  {
    current_screen = LIDAR_DATA_SCREEN;
  }
  else if (current_screen == SET_HEIGHT_SCREEN && (position == CURSOR_4 || position == CURSOR_3 || position == CURSOR_2 || position == CURSOR_1))
  {
    setHeight();
    heightToEEPROM();
    displayHeight();
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == SET_HEIGHT_SCREEN && position == CURSOR_5)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == VIEW_DATA_SCREEN && position == CURSOR_3)
  {
    current_screen = COORDINATE_SCREEN;
  }
  else if (current_screen == LENGTH_SCREEN && position == CURSOR_1)
  {
    point_1 = coordinatesOfpoints();
    pointCapturedMessage();
    position = CURSOR_2;
  }
  else if (current_screen == LENGTH_SCREEN && position == CURSOR_2)
  {
    point_2 = coordinatesOfpoints();
    pointCapturedMessage();
    theLengthIs();
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_4;
  }
  else if (current_screen == VIEW_DATA_SCREEN && position == CURSOR_4)
  {
    current_screen = LENGTH_SCREEN;
    position = CURSOR_1;
  }
  else
  {
    current_screen = current_screen;
    position = position;
  }
}

void UI ::rightScroll()
{
  makeBuzzerSound();
  if (current_screen == OPENING_SCREEN || current_screen == FRUSTUM_SCREEN_1 || current_screen == FRUSTUM_SCREEN_2 || current_screen == FRUSTUM_SCREEN_3 || current_screen == FRUSTUM_SCREEN_4 || current_screen == FREQUENCY_SET_SCREEN || current_screen == CALIBRATION_SCREEN)
  {
    if (position != CURSOR_3)
    {
      position++;
    }
  }
  if (current_screen == START_SCAN_SCREEN || current_screen == GEOMETRIC_SCREEN || current_screen == LENGTH_SCREEN || current_screen == CONE_SCREEN_1 || current_screen == CONE_SCREEN_2 || current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    if (position != CURSOR_2)
    {
      position++;
    }
  }
  if (current_screen == MESSAGE_SCREEN_1 || current_screen == MESSAGE_SCREEN_2 || current_screen == MESSAGE_SCREEN_3)
  {
    if (position != CURSOR_2)
    {
      position++;
    }
  }
  if (current_screen == SET_TIME_SCREEN || current_screen == VIEW_DATA_SCREEN || current_screen == SPARSE_SCREEN)
  {
    if (position != CURSOR_4)
    {
      position++;
    }
    if (position == CURSOR_4 && current_screen == SPARSE_SCREEN)
    {
      sparse_scan_screen_number = 1;
    }
    if (position == CURSOR_4 && current_screen == VIEW_DATA_SCREEN)
    {
      view_data_screen_number = 1;
    }
  }
  if (current_screen == SET_DATE_SCREEN || current_screen == SET_HEIGHT_SCREEN)
  {
    if (position != CURSOR_5)
    {
      position++;
    }
  }
  if (current_screen == SETTINGS_SCREEN)
  {
    if (position != CURSOR_6)
    {
      position++;
    }
    if (position == CURSOR_4 && (last_position == CURSOR_2 || last_position == CURSOR_3) && setting_screen_number != 2)
    {
      setting_screen_number = 1;
    }
    if (position == CURSOR_5 && (last_position == CURSOR_3 || last_position == CURSOR_4) && setting_screen_number != 3)
    {
      setting_screen_number = 2;
    }
    if (position == CURSOR_6)
    {
      setting_screen_number = 3;
    }
  }
}

void UI ::backtolastMenu()
{
  makeBuzzerSound();
  if (current_screen == OPENING_SCREEN)
  {
    current_screen = OPENING_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == START_SCAN_SCREEN)
  {
    current_screen = OPENING_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == GEOMETRIC_SCREEN)
  {
    current_screen = START_SCAN_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == CONE_SCREEN_1)
  {
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == FRUSTUM_SCREEN_1 || current_screen == FRUSTUM_SCREEN_2 || current_screen == FRUSTUM_SCREEN_3 || current_screen == FRUSTUM_SCREEN_4)
  {
    current_screen = GEOMETRIC_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == VIEW_DATA_SCREEN)
  {
    view_data_screen_number = 0;
    current_screen = OPENING_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == SETTINGS_SCREEN)
  {
    current_screen = OPENING_SCREEN;
    position = CURSOR_3;
    setting_screen_number = 0;
  }
  else if (current_screen == NEW_VIEW_POINT_SCREEN)
  {
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    sparse_scan_screen_count++;
    current_screen = SPARSE_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == IMU_DATA_SCREEN)
  {
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == LIDAR_DATA_SCREEN)
  {
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == COORDINATE_SCREEN)
  {
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == LENGTH_SCREEN)
  {
    current_screen = VIEW_DATA_SCREEN;
    position = CURSOR_4;
  }
  else if (current_screen == SET_TIME_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_4;
  }
  else if (current_screen == SET_DATE_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_5;
  }
  else if (current_screen == CALIBRATION_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_6;
  }
  else if (current_screen == SET_HEIGHT_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_1;
  }
  else if (current_screen == COORDINATE_MODE_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_2;
  }
  else if (current_screen == FREQUENCY_SET_SCREEN)
  {
    current_screen = SETTINGS_SCREEN;
    position = CURSOR_3;
  }
  else if (current_screen == MESSAGE_SCREEN_3)
  {
    current_screen = CALIBRATION_SCREEN;
    position = CURSOR_3;
  }
}

void UI ::scrollUp()
{
  makeBuzzerSound();
  if (current_screen == SET_TIME_SCREEN && position == CURSOR_1)
  {
    hour_count++;
    if (hour_count > 23)
    {
      hour_count = 0;
    }
  }
  else if (current_screen == SET_TIME_SCREEN && position == CURSOR_2)
  {
    minute_count++;
    if (minute_count > 59)
    {
      minute_count = 0;
    }
  }
  if (current_screen == SET_DATE_SCREEN && position == CURSOR_1)
  {
    day_count++;
    if (day_count > 31)
    {
      day_count = 1;
    }
  }
  else if (current_screen == SET_DATE_SCREEN && position == CURSOR_2)
  {
    month_count++;
    if (month_count > 12)
    {
      month_count = 1;
    }
  }
  else if (current_screen == SET_DATE_SCREEN && position == CURSOR_3)
  {
    year_count++;
    if (year_count > 99)
    {
      year_count = 0;
    }
  }
  if (current_screen == FREQUENCY_SET_SCREEN)
  {
    data_per_second++;
    if (data_per_second > 10)
    {
      data_per_second = 1;
    }
  }
  if (current_screen == SET_HEIGHT_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      height_Hundred_place_digit_count++;
      if (height_Hundred_place_digit_count > 9)
      {
        height_Hundred_place_digit_count = 0;
      }
      break;
    case CURSOR_2:
      height_tenth_place_digit_count++;
      if (height_tenth_place_digit_count > 9)
      {
        height_tenth_place_digit_count = 0;
      }
      break;
    case CURSOR_3:
      height_uint_place_digit_count++;
      if (height_uint_place_digit_count > 9)
      {
        height_uint_place_digit_count = 0;
      }
      break;
    }
  }
  if (current_screen == COORDINATE_MODE_SCREEN)
  {
    if (position != CURSOR_1)
    {
      position--;
    }
  }
}

void UI ::scrollDown()
{
  if (current_screen == COORDINATE_MODE_SCREEN)
  {
    if (position != CURSOR_2)
    {
      position++;
    }
  }
  makeBuzzerSound();
  if (current_screen == SET_TIME_SCREEN)
  {
    if (position == CURSOR_1)
    {
      hour_count--;
      if (hour_count == 255)
      {
        hour_count = 23;
      }
    }
    else if (position == CURSOR_2)
    {
      minute_count--;
      if (minute_count == 255)
      {
        minute_count = 59;
      }
    }
  }
  else if (current_screen == SET_DATE_SCREEN)
  {
    if (position == CURSOR_1)
    {
      day_count--;
      if (day_count < 1)
      {
        day_count = 31;
      }
    }
    else if (position == CURSOR_2)
    {
      month_count--;
      if (month_count < 1)
      {
        month_count = 12;
      }
    }
    else if (position == CURSOR_3)
    {
      year_count--;
      if (year_count == 255)
      {
        year_count = 99;
      }
    }
  }
  if (current_screen == FREQUENCY_SET_SCREEN)
  {
    data_per_second--;
    if (data_per_second == 0)
    {
      data_per_second = 10;
    }
  }
  if (current_screen == SET_HEIGHT_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      height_Hundred_place_digit_count--;
      if (height_Hundred_place_digit_count == 255)
      {
        height_Hundred_place_digit_count = 9;
      }
      break;
    case CURSOR_2:
      height_tenth_place_digit_count--;
      if (height_tenth_place_digit_count == 255)
      {
        height_tenth_place_digit_count = 9;
      }
      break;
    case CURSOR_3:
      height_uint_place_digit_count--;
      if (height_uint_place_digit_count == 255)
      {
        height_uint_place_digit_count = 9;
      }
      break;
    }
  }
}

void UI ::icon_framePosition()
{
  if (current_screen == OPENING_SCREEN || current_screen == CALIBRATION_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_3:
      u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    default:
      break;
    }
  }
  if (current_screen == START_SCAN_SCREEN || current_screen == GEOMETRIC_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_two_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(type_two_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    default:
      break;
    }
  }
  if (current_screen == VIEW_DATA_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_2:
      switch (view_data_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_3:
      switch (view_data_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_4:
      u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    default:
      break;
    }
  }
  if (current_screen == SPARSE_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_2:
      switch (sparse_scan_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_3:
      switch (sparse_scan_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_4:
      u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    default:
      break;
    }
  }
  if (current_screen == SETTINGS_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    case CURSOR_2:
      switch (setting_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_3:
      switch (setting_screen_number)
      {
      case 0:
        u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 1:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 2:
        u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_4:
      switch (setting_screen_number)
      {
      case 1:
        u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 2:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 3:
        u8g2.drawFrame(type_one_frame_x_1, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_5:
      switch (setting_screen_number)
      {
      case 2:
        u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      case 3:
        u8g2.drawFrame(type_one_frame_x_2, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
        break;
      }
      break;
    case CURSOR_6:
      u8g2.drawFrame(type_one_frame_x_3, CURSOR_Y, FRAME_BREADTH, FRAME_LENGHT);
      break;
    default:
      break;
    }
  }
}

void UI ::userInput_framePosition()
{
  if (current_screen == MESSAGE_SCREEN_1 || current_screen == MESSAGE_SCREEN_2 || current_screen == MESSAGE_SCREEN_3)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_three_frame_x_1, MESSAGE_FRAME_Y, OK_FRAME_WIDTH, OK_FRAME_HIEGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(type_three_frame_x_2, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
      break;
    default:
      break;
    }
  }
}

void UI ::iconName()
{
  switch (current_screen)
  {
  case OPENING_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_1st_screen[position]);
    break;
  case START_SCAN_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_2nd_screen[position]);
    break;
  case GEOMETRIC_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_3rd_screen[position]);
    break;
  case SPARSE_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_4th_screen[position]);
    break;
  case VIEW_DATA_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_5th_screen[position]);
    break;
  case SETTINGS_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_6th_screen[position]);
    break;
  case CALIBRATION_SCREEN:
    u8g2.drawStr(ICON_NAME_X, ICON_NAME_Y, nameoficons_7th_screen[position]);
    break;
  default:
    break;
  }
}

void UI ::iconPosition()
{
  uint8_t w = 0;
  u8g2.setFont(u8g2_font_6x10_tr);
  switch (current_screen)
  {
  case OPENING_SCREEN:
    w = u8g2.getStrWidth(nameoficons_1st_screen[position]);
    break;
  case START_SCAN_SCREEN:
    w = u8g2.getStrWidth(nameoficons_2nd_screen[position]);
    break;
  case GEOMETRIC_SCREEN:
    w = u8g2.getStrWidth(nameoficons_3rd_screen[position]);
    break;
  case SPARSE_SCREEN:
    w = u8g2.getStrWidth(nameoficons_4th_screen[position]);
    break;
  case VIEW_DATA_SCREEN:
    w = u8g2.getStrWidth(nameoficons_5th_screen[position]);
    break;
  case SETTINGS_SCREEN:
    w = u8g2.getStrWidth(nameoficons_6th_screen[position]);
    break;
  case CALIBRATION_SCREEN:
    w = u8g2.getStrWidth(nameoficons_7th_screen[position]);
    break;
  case SET_TIME_SCREEN:
    u8g2.setFont(u8g2_font_lubB12_tr);
    w = u8g2.getStrWidth(setting_str[0]);
    break;
  case SET_DATE_SCREEN:
    u8g2.setFont(u8g2_font_lubB12_tr);
    w = u8g2.getStrWidth(setting_str[1]);
    break;
  case SET_HEIGHT_SCREEN:
    u8g2.setFont(u8g2_font_lubB12_tr);
    w = u8g2.getStrWidth(setting_str[2]);
    break;
  default:
    break;
  }
  ICON_NAME_X = (128 - w);
  ICON_NAME_X = ICON_NAME_X / 2;
}

void UI ::embeddedIcon()
{
  float strength = lidarStrength();
  u8g2.setFont(u8g2_font_open_iconic_embedded_1x_t);
  u8g2.drawGlyph(118, 8, 73);
  if (strength <= 100 && strength >= 66)
  {
    u8g2.drawXBMP(108, 0, 8, 8, signal_1_bits);
  }
  else if (strength < 66 && strength >= 33)
  {
    u8g2.drawXBMP(108, 0, 8, 8, signal_2_bits);
  }
  else if (strength < 33)
  {
    u8g2.drawXBMP(108, 0, 8, 8, signal_3_bits);
  }
}

void UI ::displayTime()
{
  embeddedIcon();
  uint8_t hour;
  uint8_t minute;
  hour = clock.getHour(h12Flag, pmFlag);
  minute = clock.getMinute();
  if (pmFlag && hour != 12)
  {
    hour_count = hour + 12;
  }
  else
  {
    hour_count = hour;
  }
  minute_count = minute;
  u8g2.setFont(u8g2_font_6x10_tr);
  if (hour < 10)
  {
    u8g2.setCursor(1, 7);
    u8g2.print("0");
    u8g2.setCursor(7, 7);
    u8g2.print(hour);
  }
  else
  {
    u8g2.setCursor(1, 7);
    u8g2.print(hour);
  }
  u8g2.setCursor(13, 6);
  u8g2.print(":");
  if (minute < 10)
  {
    u8g2.setCursor(20, 7);
    u8g2.print("0");
    u8g2.setCursor(26, 7);
    u8g2.print(minute);
  }
  else
  {
    u8g2.setCursor(20, 7);
    u8g2.print(minute);
  }
}

void UI ::display()
{
  switch (current_screen)
  {
  case OPENING_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_startScan();
      icon_viewData();
      icon_settings();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case START_SCAN_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_geometricScan();
      icon_sparseScan();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case GEOMETRIC_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_cone();
      icon_frustum();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case SPARSE_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_acquirePoint();
      icon_newViewPoints();
      icon_captureBoundary();
      icon_endScan();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case VIEW_DATA_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_lidar();
      icon_IMU();
      icon_coordinate();
      icon_lenght();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case SETTINGS_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_callibration();
      icon_setTime();
      icon_setDate();
      icon_setHieght();
      icon_setCoordinateMode();
      icon_setFrequency();
      icon_framePosition();
    } while (u8g2.nextPage());
    // displayTime();
    break;
  case CALIBRATION_SCREEN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      iconName();
      driveAttached();
      displayTime();
      icon_gyroCallibration();
      icon_magCallibration();
      icon_pitchCallibration();
      icon_framePosition();
    } while (u8g2.nextPage());
    break;
  default:
    break;
  }
}

void UI ::charge_Indicator()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_6x10_tr);
    icon_chargeIndicator();
  } while (u8g2.nextPage());
}

void UI ::userInterfaceMessage()
{
  if (current_screen == MESSAGE_SCREEN_1 || current_screen == MESSAGE_SCREEN_2 || current_screen == MESSAGE_SCREEN_3)
  {
    u8g2.firstPage();
    do
    {
      messageBox();
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(ok_str_pos_x, STR_POS_Y, "OK");
      u8g2.drawStr(cancel_str_pos_x, STR_POS_Y, "CANCEL");
      userInput_framePosition();
      u8g2.drawFrame(M_FRAME_COORDINATE, M_FRAME_COORDINATE, M_FRAME_WIDTH, M_FRAME_LENGHT);
    } while (u8g2.nextPage());
  }
}

void UI ::messageBox()
{
  switch (current_screen)
  {
  case MESSAGE_SCREEN_1:
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(13, message_y_cursor_1, "To Calibrate Gyro");
    u8g2.drawStr(19, message_y_cursor_2, "Keep The Device");
    u8g2.drawStr(4, message_y_cursor_3, "In Rest and Press OK");
    break;
  case MESSAGE_SCREEN_2:
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(7, message_y_cursor_1, "Press OK and Move The");
    u8g2.drawStr(7, message_y_cursor_2, "Device In Figure 8 in");
    u8g2.drawStr(34, message_y_cursor_3, "All Three Dimensions.");
    break;
  case MESSAGE_SCREEN_3:
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(19, message_y_cursor_1, "Point at Ground");
    u8g2.drawStr(29, message_y_cursor_2, "Press Ok and");
    u8g2.drawStr(9, message_y_cursor_3, "Move it To and Fro");
    break;
  }
}

void UI ::bottomEdgeLengthDisplay()
{
  if (current_screen == FRUSTUM_SCREEN_1)
  {
    frustum_display(Frustum_bit_bits, length_of_bot_edge_1);
  }
  if (current_screen == FRUSTUM_SCREEN_3)
  {
    frustum_display(frustum_bit_2_bits, length_of_bot_edge_2);
  }
  if (current_screen == FRUSTUM_SCREEN_2)
  {
    frustum_display(frustum_bit_4_bits, length_of_top_edge_1);
  }
  if (current_screen == FRUSTUM_SCREEN_4)
  {
    frustum_display(frustum_bit_3_bits, length_of_top_edge_2);
  }
}

void UI ::frustum_display(uint8_t const *bitmap, double length)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_nokiafc22_tf);
    u8g2.drawXBMP(2, 2, FRUSTUM_BIT_WIDTH, FRUSTUM_BIT_HEIGHT, bitmap);
    setFrustumCursor();
    u8g2.setCursor(80, 16);
    u8g2.print("Length:");
    u8g2.setCursor(88, 32);
    u8g2.print(length);
    u8g2.drawStr(frustum_point_1_Pos_x, STR_POS_Y + 2, frustum_point_str[0]);
    u8g2.drawStr(frustum_point_2_Pos_x, STR_POS_Y + 2, frustum_point_str[1]);
    u8g2.drawStr(frustum_next_Pos_x, STR_POS_Y + 2, frustum_point_str[2]);
  } while (u8g2.nextPage());
}

void UI ::frustumPointStrWidth()
{
  u8g2.setFont(u8g2_font_nokiafc22_tf);
  w_3 = u8g2.getStrWidth(frustum_point_str[0]);
  w_4 = u8g2.getStrWidth(frustum_point_str[1]);
  w_5 = u8g2.getStrWidth(frustum_point_str[2]);

  frustum_point_1_Pos_x = (43 - w_3) / 2;
  frustum_point_2_Pos_x = 43 + (43 - w_4) / 2;
  frustum_next_Pos_x = 86 + (42 - w_5) / 2;
}

void UI ::setFrustumCursor()
{
  switch (position)
  {
  case CURSOR_1:
    u8g2.drawFrame(frustum_point_1_Pos_x - 2, MESSAGE_FRAME_Y + 4, w_3 + 4, CANCEL_FRAME_HIEGHT);
    break;
  case CURSOR_2:
    u8g2.drawFrame(frustum_point_2_Pos_x - 2, MESSAGE_FRAME_Y + 4, w_4 + 4, CANCEL_FRAME_HIEGHT);
    break;
  case CURSOR_3:
    u8g2.drawFrame(frustum_next_Pos_x - 2, MESSAGE_FRAME_Y + 4, w_5 + 4, CANCEL_FRAME_HIEGHT);
    break;
  }
}

void UI ::errorMessage()
{
  if (current_screen == FRUSTUM_SCREEN_1 || current_screen == FRUSTUM_SCREEN_2 || current_screen == FRUSTUM_SCREEN_3 || current_screen == FRUSTUM_SCREEN_4)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(45, 24, "ERROR");
      u8g2.drawStr(8, 48, "Points Not Taken");
      u8g2.drawFrame(2, 8, 124, 44);
    } while (u8g2.nextPage());
  }
}

void UI ::gyroCalibration()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(14, 17, "Saving Data to");
    u8g2.drawStr(42, 36, "EEPROM");
  } while (u8g2.nextPage());
  delay(1000);
  imu.gyro_data_To_EEprom();
}

void UI ::magCalibration()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawXBMP(2, 2, INFINITY_WIDTH, INFINITY_HIEGHT, Infinty_bits);
  } while (u8g2.nextPage());
  delay(1000);
  imu.mag_data_To_EEprom();
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(14, 17, "Saving Data to");
    u8g2.drawStr(42, 36, "EEPROM");
  } while (u8g2.nextPage());
  delay(1000);
}

void UI ::setTime()
{
  clock.setHour(hour_count);
  clock.setMinute(minute_count);
}

void UI ::timesettingDisplay()
{
  if (current_screen == SET_TIME_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_lubB12_tr);
      u8g2.drawStr(ICON_NAME_X, 14, setting_str[0]);
      u8g2.setFont(u8g2_font_10x20_tn);
      if (hour_count < 10)
      {
        u8g2.setCursor(36, 34);
        u8g2.print("0");
        u8g2.setCursor(45, 34);
        u8g2.print(hour_count);
      }
      else
      {
        u8g2.setCursor(36, 34);
        u8g2.print(hour_count);
      }
      setTimeCursor();
      u8g2.setCursor(58, 33);
      u8g2.print(":");
      if (minute_count < 10)
      {
        u8g2.setCursor(72, 34);
        u8g2.print("0");
        u8g2.setCursor(81, 34);
        u8g2.print(minute_count);
      }
      else
      {
        u8g2.setCursor(72, 34);
        u8g2.print(minute_count);
      }
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(ok_str_pos_x + 2, STR_POS_Y, "OK");
      u8g2.drawStr(cancel_str_pos_x + 2, STR_POS_Y, "CANCEL");
    } while (u8g2.nextPage());
  }
}

void UI ::setTimeCursor()
{
  switch (position)
  {
  case CURSOR_1:
    u8g2.drawHLine(34, 36, 24);
    break;
  case CURSOR_2:
    u8g2.drawHLine(70, 36, 24);
    break;
  case CURSOR_3:
    u8g2.drawFrame(type_three_frame_x_1 + 2, MESSAGE_FRAME_Y, OK_FRAME_WIDTH, OK_FRAME_HIEGHT);
    break;
  case CURSOR_4:
    u8g2.drawFrame(type_three_frame_x_2 + 2, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
    break;
  }
}

void UI ::setDate()
{
  clock.setDate(day_count);
  clock.setMonth(month_count);
  clock.setYear(year_count);
}

void UI ::datesettingDisplay()
{
  if (current_screen == SET_DATE_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_lubB12_tr);
      u8g2.drawStr(ICON_NAME_X, 14, setting_str[1]);
      u8g2.setFont(u8g2_font_10x20_tn);
      if (day_count < 10)
      {
        u8g2.setCursor(18, 34);
        u8g2.print("0");
        u8g2.setCursor(27, 34);
        u8g2.print(day_count);
      }
      else
      {
        u8g2.setCursor(18, 34);
        u8g2.print(day_count);
      }
      setDateCursor();
      u8g2.setCursor(41, 33);
      u8g2.print("/");
      if (month_count < 10)
      {
        u8g2.setCursor(55, 34);
        u8g2.print("0");
        u8g2.setCursor(64, 34);
        u8g2.print(month_count);
      }
      else
      {
        u8g2.setCursor(55, 34);
        u8g2.print(month_count);
      }
      u8g2.setCursor(76, 33);
      u8g2.print("/");
      if (year_count < 10)
      {
        u8g2.setCursor(90, 34);
        u8g2.print("0");
        u8g2.setCursor(99, 34);
        u8g2.print(year_count);
      }
      else
      {
        u8g2.setCursor(90, 34);
        u8g2.print(year_count);
      }
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(ok_str_pos_x + 2, STR_POS_Y, "OK");
      u8g2.drawStr(cancel_str_pos_x + 2, STR_POS_Y, "CANCEL");
    } while (u8g2.nextPage());
  }
}

void UI ::setDateCursor()
{
  switch (position)
  {
  case CURSOR_1:
    u8g2.drawHLine(16, 36, 24);
    break;
  case CURSOR_2:
    u8g2.drawHLine(53, 36, 24);
    break;
  case CURSOR_3:
    u8g2.drawHLine(88, 36, 24);
    break;
  case CURSOR_4:
    u8g2.drawFrame(type_three_frame_x_1 + 2, MESSAGE_FRAME_Y, OK_FRAME_WIDTH, OK_FRAME_HIEGHT);
    break;
  case CURSOR_5:
    u8g2.drawFrame(type_three_frame_x_2 + 2, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
    break;
  }
}

void UI ::displayDate()
{
  uint8_t day;
  uint8_t month;
  uint16_t year;
  bool century = false;
  day = clock.getDate();
  month = clock.getMonth(century);
  year = clock.getYear();
  day_count = day;
  month_count = month;
  year_count = year;
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_10x20_tn);
    if (day < 10)
    {
      u8g2.setCursor(18, 34);
      u8g2.print("0");
      u8g2.setCursor(27, 34);
      u8g2.print(day);
    }
    else
    {
      u8g2.setCursor(18, 34);
      u8g2.print(day);
    }
    u8g2.setCursor(41, 33);
    u8g2.print("/");
    if (month < 10)
    {
      u8g2.setCursor(55, 34);
      u8g2.print("0");
      u8g2.setCursor(64, 34);
      u8g2.print(month);
    }
    else
    {
      u8g2.setCursor(55, 34);
      u8g2.print(month);
    }
    u8g2.setCursor(76, 33);
    u8g2.print("/");
    if (year < 10)
    {
      u8g2.setCursor(90, 34);
      u8g2.print("0");
      u8g2.setCursor(99, 34);
      u8g2.print(year);
    }
    else
    {
      u8g2.setCursor(90, 34);
      u8g2.print(year);
    }
  } while (u8g2.nextPage());
}

void UI ::setHeight()
{
  height = (100 * height_Hundred_place_digit_count);
  height += (10 * height_tenth_place_digit_count);
  height += height_uint_place_digit_count;
  height /= 100;
}

void UI ::heightToEEPROM()
{
  eeProm_adr = 24;
  EEPROM.put(eeProm_adr, height);
  eeProm_adr += sizeof(height);
}

void UI ::heightDigit()
{
  int H = height * 100;
  height_Hundred_place_digit_count = (H / 100);
  height_tenth_place_digit_count = ((H % 100) / 10);
  height_uint_place_digit_count = ((H % 100) % 10);
}

float UI ::heightFromEEPROM()
{
  eeProm_adr = 24;
  EEPROM.get(eeProm_adr, height);
  eeProm_adr += sizeof(height);
  return height;
}

void UI ::heightsettingDisplay()
{
  if (current_screen == SET_HEIGHT_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_lubB12_tr);
      u8g2.drawStr(ICON_NAME_X, 14, setting_str[2]);
      u8g2.setFont(u8g2_font_10x20_tn);
      u8g2.setCursor(40, 34);
      u8g2.print(height_Hundred_place_digit_count);
      u8g2.setCursor(54, 34);
      u8g2.print(height_tenth_place_digit_count);
      u8g2.setCursor(68, 34);
      u8g2.print(height_uint_place_digit_count);
      setHeightCursor();
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.setCursor(82, 34);
      u8g2.print("cm");
      u8g2.drawStr(ok_str_pos_x + 2, STR_POS_Y, "OK");
      u8g2.drawStr(cancel_str_pos_x + 2, STR_POS_Y, "CANCEL");
    } while (u8g2.nextPage());
  }
}

void UI ::setHeightCursor()
{
  switch (position)
  {
  case CURSOR_1:
    u8g2.drawHLine(39, 36, 12);
    break;
  case CURSOR_2:
    u8g2.drawHLine(53, 36, 12);
    break;
  case CURSOR_3:
    u8g2.drawHLine(67, 36, 12);
    break;
  case CURSOR_4:
    u8g2.drawFrame(type_three_frame_x_1 + 2, MESSAGE_FRAME_Y, OK_FRAME_WIDTH, OK_FRAME_HIEGHT);
    break;
  case CURSOR_5:
    u8g2.drawFrame(type_three_frame_x_2 + 2, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
    break;
  }
}

void UI ::displayHeight()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_nokiafc22_tf);
    u8g2.drawFrame(16, 8, 96, 48);
    u8g2.drawStr(22, 20, "The Height is Set");
    u8g2.drawStr(56, 32, "To");
    u8g2.setFont(u8g2_font_10x20_tn);
    u8g2.setCursor(44, 48);
    u8g2.print(height);
  } while (u8g2.nextPage());
  delay(2000);
}

float UI ::lidarDistance()
{
  float distance;
  byte a, b;
  Wire.beginTransmission(0x66);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x66, 2);
  while (Wire.available())
  {
    a = Wire.read();
    b = Wire.read();
  }
  distance = (float)(a * 256 + b) / 100;
  return distance;
}

float UI ::lidarStrength()
{
  float strength;
  byte a, b;
  Wire.beginTransmission(0x66);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x66, 2);
  while (Wire.available())
  {
    a = Wire.read();
    b = Wire.read();
  }
  strength = float(a * 256 + b) / 100;
  return strength;
}

void UI ::lidarData_display()
{
  float distance = lidarDistance();
  float strength = lidarStrength();
  if (current_screen == LIDAR_DATA_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(0, 16, "Distance:");
      u8g2.setCursor(74, 16);
      u8g2.print(distance, 2);
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(0, 32, "Strength:");
      u8g2.setCursor(74, 32);
      u8g2.print(strength, 2);
    } while (u8g2.nextPage());
  }
}

Vector UI ::coordinatesOfpoints()
{
  ANGLE angle = imu.calAttitude();
  float distance = lidarDistance();
  angle.theta = angle.theta + (biased_pitch * (PI / 180));
  Vector coordinate;
  coordinate.xAxis = distance * cos(angle.theta) * sin(angle.psi);
  coordinate.yAxis = distance * cos(angle.theta) * cos(angle.psi);
  coordinate.zAxis = height + distance * sin(angle.theta);
  return coordinate;
}

void UI ::coordinatePoint(Vector points)
{
  if (current_screen != COORDINATE_SCREEN)
  {
    u8g2.setFont(u8g2_font_nokiafc22_tf);
    u8g2.drawStr(32, 12, "X:");
    u8g2.setCursor(80, 12);
    u8g2.print(points.xAxis);
    u8g2.drawStr(32, 24, "Y:");
    u8g2.setCursor(80, 24);
    u8g2.print(points.yAxis);
    u8g2.drawStr(32, 36, "Z:");
    u8g2.setCursor(80, 36);
    u8g2.print(points.zAxis);
  }
  if (current_screen == COORDINATE_SCREEN)
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(4, 16, "X:");
    u8g2.setCursor(32, 16);
    u8g2.print(points.xAxis);
    u8g2.drawStr(4, 32, "Y:");
    u8g2.setCursor(32, 32);
    u8g2.print(points.yAxis);
    u8g2.drawStr(4, 48, "Z:");
    u8g2.setCursor(32, 48);
    u8g2.print(points.zAxis);
  }
}

void UI ::coordinate_display()
{
  Vector coordinate = coordinatesOfpoints();
  if (current_screen == COORDINATE_SCREEN && coordinate_mode == CARTESIAN_MODE)
  {
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
    } while (u8g2.nextPage());
  }
  if (current_screen == COORDINATE_SCREEN && coordinate_mode == POLAR_MODE)
  {
    ANGLE angle = imu.calAttitude();
    float distance = lidarDistance();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(0, 16, "Distance:");
      u8g2.setCursor(74, 16);
      u8g2.print(distance, 2);
      u8g2.drawStr(0, 32, "Pitch:");
      u8g2.setCursor(64, 32);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 48, "Yaw:");
      u8g2.setCursor(64, 48);
      u8g2.print(angle.psi * (180 / PI), 2);
    } while (u8g2.nextPage());
  }
  if (current_screen == CONE_SCREEN_1)
  {
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, point_pos_str[0]);
      u8g2.drawStr(bot_pos_x, STR_POS_Y, point_pos_str[1]);
    } while (u8g2.nextPage());
  }
  if (current_screen == CONE_SCREEN_2)
  {
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, point_pos_str[2]);
      u8g2.drawStr(bot_pos_x, STR_POS_Y, point_pos_str[1]);
    } while (u8g2.nextPage());
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_1 && coordinate_mode == CARTESIAN_MODE)
  {
    coordinate = acquirePoints();
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[0]);
    } while (u8g2.nextPage());
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_2 && coordinate_mode == CARTESIAN_MODE)
  {
    coordinate = acquirePoints();
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(bot_pos_x, STR_POS_Y, sparse_scan_str[1]);
    } while (u8g2.nextPage());
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_1 && coordinate_mode == POLAR_MODE)
  {
    ANGLE angle = imu.calAttitude();
    float distance = lidarDistance();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(0, 12, "Distance:");
      u8g2.setCursor(74, 12);
      u8g2.print(distance, 2);
      u8g2.drawStr(0, 24, "Pitch:");
      u8g2.setCursor(64, 24);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 36, "Yaw:");
      u8g2.setCursor(64, 36);
      u8g2.print(angle.psi * (180 / PI), 2);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[0]);
    } while (u8g2.nextPage());
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_2 && coordinate_mode == POLAR_MODE)
  {
    ANGLE angle = imu.calAttitude();
    float distance = lidarDistance();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(0, 12, "Distance:");
      u8g2.setCursor(74, 12);
      u8g2.print(distance, 2);
      u8g2.drawStr(0, 24, "Pitch:");
      u8g2.setCursor(64, 24);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 36, "Yaw:");
      u8g2.setCursor(64, 36);
      u8g2.print(angle.psi * (180 / PI), 2);
      setCoordinateCursor();
      u8g2.drawStr(bot_pos_x, STR_POS_Y, sparse_scan_str[1]);
    } while (u8g2.nextPage());
  }
  if (current_screen == NEW_VIEW_POINT_SCREEN && coordinate_mode == CARTESIAN_MODE)
  {
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[2]);
    } while (u8g2.nextPage());
  }
  if (current_screen == NEW_VIEW_POINT_SCREEN && coordinate_mode == POLAR_MODE)
  {
    ANGLE angle = imu.calAttitude();
    float distance = lidarDistance();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(0, 12, "Distance:");
      u8g2.setCursor(74, 12);
      u8g2.print(distance, 2);
      u8g2.drawStr(0, 24, "Pitch:");
      u8g2.setCursor(64, 24);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 36, "Yaw:");
      u8g2.setCursor(64, 36);
      u8g2.print(angle.psi * (180 / PI), 2);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[2]);
    } while (u8g2.nextPage());
  }
  if (current_screen == CAPTURE_BOUNDARY_SCREEN && coordinate_mode == CARTESIAN_MODE)
  {
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[3]);
      u8g2.drawStr(bot_pos_x, STR_POS_Y, sparse_scan_str[1]);
    } while (u8g2.nextPage());
  }
  if (current_screen == CAPTURE_BOUNDARY_SCREEN && coordinate_mode == POLAR_MODE)
  {
    ANGLE angle = imu.calAttitude();
    float distance = lidarDistance();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.drawStr(0, 12, "Distance:");
      u8g2.setCursor(74, 12);
      u8g2.print(distance, 2);
      u8g2.drawStr(0, 24, "Pitch:");
      u8g2.setCursor(64, 24);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 36, "Yaw:");
      u8g2.setCursor(64, 36);
      u8g2.print(angle.psi * (180 / PI), 2);
      setCoordinateCursor();
      u8g2.drawStr(top_pos_x, STR_POS_Y, sparse_scan_str[3]);
      u8g2.drawStr(bot_pos_x, STR_POS_Y, sparse_scan_str[1]);
    } while (u8g2.nextPage());
  }
}

void UI ::pointPositionStringWidth()
{
  u8g2.setFont(u8g2_font_nokiafc22_tf);
  switch (current_screen)
  {
  case CONE_SCREEN_1:
    w_1 = u8g2.getStrWidth(point_pos_str[0]);
    w_2 = u8g2.getStrWidth(point_pos_str[1]);
    top_pos_x = (64 - w_1) / 2;
    bot_pos_x = 64 + (64 - w_2) / 2;
    break;
  case CONE_SCREEN_2:
    w_1 = u8g2.getStrWidth(point_pos_str[2]);
    w_2 = u8g2.getStrWidth(point_pos_str[1]);
    top_pos_x = (64 - w_1) / 2;
    bot_pos_x = 64 + (64 - w_2) / 2;
    break;
  case ACQUIRE_POINTS_SCREEN_1:
    w_1 = u8g2.getStrWidth(sparse_scan_str[0]);
    top_pos_x = (128 - w_1) / 2;
    break;
  case ACQUIRE_POINTS_SCREEN_2:
    w_2 = u8g2.getStrWidth(sparse_scan_str[1]);
    bot_pos_x = (128 - w_2) / 2;
    break;
  case NEW_VIEW_POINT_SCREEN:
    w_1 = u8g2.getStrWidth(sparse_scan_str[2]);
    top_pos_x = (128 - w_1) / 2;
    break;
  case CAPTURE_BOUNDARY_SCREEN:
    w_1 = u8g2.getStrWidth(sparse_scan_str[3]);
    w_2 = u8g2.getStrWidth(sparse_scan_str[1]);
    top_pos_x = (64 - w_1) / 2;
    bot_pos_x = 64 + (64 - w_2) / 2;
    break;
  }
}

void UI ::setCoordinateCursor()
{
  if (current_screen == LENGTH_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(type_three_frame_x_1 - 12, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(type_three_frame_x_2 + 18, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == CONE_SCREEN_1)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(top_pos_x - 2, MESSAGE_FRAME_Y, w_1 + 4, CANCEL_FRAME_HIEGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(bot_pos_x - 2, MESSAGE_FRAME_Y, w_2 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == CONE_SCREEN_2)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(top_pos_x - 2, MESSAGE_FRAME_Y, w_1 + 4, CANCEL_FRAME_HIEGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(bot_pos_x - 2, MESSAGE_FRAME_Y, w_2 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_1)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(top_pos_x - 2, MESSAGE_FRAME_Y, w_1 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_2)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(bot_pos_x - 2, MESSAGE_FRAME_Y, w_2 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == NEW_VIEW_POINT_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(top_pos_x - 2, MESSAGE_FRAME_Y, w_1 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
  if (current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    switch (position)
    {
    case CURSOR_1:
      u8g2.drawFrame(top_pos_x - 2, MESSAGE_FRAME_Y, w_1 + 4, CANCEL_FRAME_HIEGHT);
      break;
    case CURSOR_2:
      u8g2.drawFrame(bot_pos_x - 2, MESSAGE_FRAME_Y, w_2 + 4, CANCEL_FRAME_HIEGHT);
      break;
    }
  }
}

double UI ::volumeOfCone()
{
  double height_of_cone = (cone_top_point.zAxis - cone_bot_point.zAxis);
  double radius_of_cone = sqrt(pow((cone_top_point.xAxis - cone_bot_point.xAxis), 2) + pow((cone_top_point.yAxis - cone_bot_point.yAxis), 2));
  double volume_of_cone = (1.0 / 3.0) * PI * pow(radius_of_cone, 2) * height_of_cone;
  return volume_of_cone;
}

void UI ::volumeDisplay()
{
  if (current_screen == CONE_SCREEN_2)
  {
    volume = volumeOfCone();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(8, 16, "Volume Of Conical");
      u8g2.drawStr(24, 32, "Structure is");
      u8g2.setCursor(48, 48);
      u8g2.print(volume, 2);
    } while (u8g2.nextPage());
    delay(5000);
  }
  if (current_screen == FRUSTUM_SCREEN_4)
  {
    volume = volumeOfFrustum();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(8, 16, "Volume Of The");
      u8g2.drawStr(24, 32, "Structure is");
      u8g2.setCursor(48, 48);
      u8g2.print(volume, 2);
    } while (u8g2.nextPage());
    delay(5000);
  }
}

double UI ::volumeOfFrustum()
{
  double h = (frustum_top_1.zAxis - frustum_bot_1.zAxis);
  double H = h * (length_of_bot_edge_1 / length_of_top_edge_1);
  double area_b = length_of_bot_edge_1 * length_of_bot_edge_2;
  double area_t = length_of_top_edge_1 * length_of_top_edge_2;
  double volume_of_Frustum = ((1.0 / 3.0) * area_b * H) - ((1.0 / 3.0) * area_t * h);
  return volume_of_Frustum;
}

double UI ::distanceBetweenPoints(Vector coordinate_1, Vector coordinate_2)
{
  double difference_in_x = (coordinate_2.xAxis - coordinate_1.xAxis);
  double difference_in_y = (coordinate_2.yAxis - coordinate_1.yAxis);
  double difference_in_z = (coordinate_2.zAxis - coordinate_1.zAxis);
  double length = sqrt((pow(difference_in_x, 2.0) + pow(difference_in_y, 2.0) + pow(difference_in_z, 2.0)));
  return length;
}

void UI ::lengthDisplay()
{
  if (current_screen == LENGTH_SCREEN)
  {
    Vector coordinate = coordinatesOfpoints();
    u8g2.firstPage();
    do
    {
      coordinatePoint(coordinate);
      setCoordinateCursor();
      u8g2.drawStr(ok_str_pos_x - 12, STR_POS_Y, "POINT1");
      u8g2.drawStr(cancel_str_pos_x + 18, STR_POS_Y, "POINT2");
    } while (u8g2.nextPage());
  }
}

void UI ::theLengthIs()
{
  distance_between_two_points = distanceBetweenPoints(point_1, point_2);
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(8, 16, "Distance Between");
    u8g2.drawStr(32, 32, "Two Points");
    u8g2.setCursor(48, 48);
    u8g2.print(distance_between_two_points, 2);
  } while (u8g2.nextPage());
  delay(5000);
}

void UI ::imuData_display()
{
  if (current_screen == IMU_DATA_SCREEN)
  {
    ANGLE angle = imu.calAttitude();
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14B_tr);
      u8g2.drawStr(0, 16, "Roll:");
      u8g2.setCursor(64, 16);
      u8g2.print(angle.phi * (180 / PI), 2);
      u8g2.drawStr(0, 32, "Pitch:");
      u8g2.setCursor(64, 32);
      u8g2.print(angle.theta * (180 / PI) + biased_pitch, 2);
      u8g2.drawStr(0, 48, "Yaw:");
      u8g2.setCursor(64, 48);
      u8g2.print(angle.psi * (180 / PI), 2);
    } while (u8g2.nextPage());
  }
}

Vector UI ::acquirePoints()
{
  Vector acquire_points;
  Vector points = coordinatesOfpoints();
  acquire_points.xAxis = view_point.xAxis + points.xAxis;
  acquire_points.yAxis = view_point.yAxis + points.yAxis;
  acquire_points.zAxis = view_point.zAxis + points.zAxis;
  return acquire_points;
}

void UI ::driveAttached()
{
  if (flashDrive.driveReady())
  {
    u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
    u8g2.drawGlyph(36, 8, 172);
  }
  else
  {
    ;
  }
}

STRING UI ::doubleToString(Vector points_to_string)
{
  STRING points;
  points.xAxis = String(points_to_string.xAxis, 2);
  points.yAxis = String(points_to_string.yAxis, 2);
  points.zAxis = String(points_to_string.zAxis, 2);

  points.xAxis = points.xAxis + ",";
  points.yAxis = points.yAxis + ",";
  points.zAxis = points.zAxis + ",";

  return points;
}

void UI ::saveDataToDrive(Vector strings)
{
  STRING points = doubleToString(strings);
  String coordinate = points.xAxis + points.yAxis;
  coordinate = coordinate + points.zAxis;
  coordinate = coordinate + colorOfPoint[change_of_view_point];
  int str_len = coordinate.length() + 1;
  char data[str_len];
  coordinate.toCharArray(data, str_len);
  String day = String(day_count);
  String hour = String((hour_count));
  String minute = String(minute_count);
  switch (month_count)
  {
  case 1:
    month = "JAN";
    break;
  case 2:
    month = "FEB";
    break;
  case 3:
    month = "MAR";
    break;
  case 4:
    month = "APR";
    break;
  case 5:
    month = "MAY";
    break;
  case 6:
    month = "JUN";
    break;
  case 7:
    month = "JUL";
    break;
  case 8:
    month = "AUG";
    break;
  case 9:
    month = "SEP";
    break;
  case 10:
    month = "OCT";
    break;
  case 11:
    month = "NOV";
    break;
  case 12:
    month = "DEC";
    break;
  default:
    break;
  }
  String slash = "/";
  String dash = "_";
  String txt = ".CSV";
  if (current_screen == ACQUIRE_POINTS_SCREEN_1 || current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    file = "V@";
  }
  if (current_screen == ACQUIRE_POINTS_SCREEN_2)
  {
    file = "F@";
  }
  String dir_name = slash + month + day;
  int str_len2 = dir_name.length() + 1;
  char dirName[str_len2];
  dir_name.toCharArray(dirName, str_len2);
  dir_name_set = dirName;
  String name = file + hour + dash + minute + txt;
  int str_len1 = name.length() + 1;
  char fileName[str_len1];
  name.toCharArray(fileName, str_len1);
  file_name = fileName;
  switch (flashDrive.cd(dir_name_set, 1))
  {
  case ANSW_USB_INT_SUCCESS:
    flashDrive.setFileName(file_name);
    if (flashDrive.openFile() == ANSW_USB_INT_SUCCESS)
    {
      flashDrive.moveCursor(flashDrive.getFileSize());
    }
    flashDrive.writeFile(data, strlen(data));
    flashDrive.closeFile();
    break;
  case ANSW_ERR_OPEN_DIR:
    flashDrive.setFileName(file_name);
    if (flashDrive.openFile() == ANSW_USB_INT_SUCCESS)
    {
      flashDrive.moveCursor(flashDrive.getFileSize());
    }
    flashDrive.writeFile(data, strlen(data));
    flashDrive.closeFile();
    break;
  default:
    break;
  }
}

void UI ::flashDataToDrive()
{
  if (current_screen == ACQUIRE_POINTS_SCREEN_2)
  {
    acquire_points = acquirePoints();
    saveDataToDrive(acquire_points);
    makeBuzzerSound();
    delay(time_period);
  }
}

void UI ::minuteCountFunction()
{
  if (SPARSE_SCREEN)
  {
    if (sparse_scan_screen_count == 0)
    {
      last_minute_count = minute_count;
      last_hour_count = hour_count;
    }
    else
    {
      minute_count = minute_count - (minute_count - last_minute_count);
      hour_count = hour_count - (hour_count - last_hour_count);
    }
  }
}

void UI ::makeBuzzerSound()
{
  TCCR0B = (TCCR0B & B11111000) | B00000010;
  analogWrite(4, 150);
  delay(buzzer_delay);
  TCCR0B = (TCCR0B & B11111000) | B00000011;
  analogWrite(4, 0);
}

void UI ::setFrequency()
{
  time_period = ((1000 / data_per_second) - buzzer_delay);
}

void UI ::displayFrequency()
{
  if (current_screen == FREQUENCY_SET_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_lubB12_tr);
      u8g2.drawStr(10, 14, "Capture Rate");
      u8g2.setFont(u8g2_font_10x20_tn);
      u8g2.setCursor(59, 34);
      u8g2.print(data_per_second);
      setFrequencyCursor();
      u8g2.setFont(u8g2_font_nokiafc22_tf);
      u8g2.setCursor(82, 34);
      u8g2.print("dps");
      u8g2.drawStr(ok_str_pos_x + 2, STR_POS_Y, "OK");
      u8g2.drawStr(cancel_str_pos_x + 2, STR_POS_Y, "CANCEL");
    } while (u8g2.nextPage());
  }
}

void UI ::setFrequencyCursor()
{
  switch (position)
  {
  case CURSOR_1:
    if (data_per_second != 10)
    {
      u8g2.drawHLine(58, 36, 12);
    }
    else
    {
      u8g2.drawHLine(58, 36, 24);
    }
    break;
  case CURSOR_2:
    u8g2.drawFrame(type_three_frame_x_1 + 2, MESSAGE_FRAME_Y, OK_FRAME_WIDTH, OK_FRAME_HIEGHT);
    break;
  case CURSOR_3:
    u8g2.drawFrame(type_three_frame_x_2 + 2, MESSAGE_FRAME_Y, CANCEL_FRAME_WIDTH, CANCEL_FRAME_HIEGHT);
    break;
  }
}

void UI ::displayCoordinateMode()
{
  if (current_screen == COORDINATE_MODE_SCREEN)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_lubB12_tr);
      u8g2.drawStr(cartesian_mode_x, 24, coordinate_mode_str[0]);
      u8g2.drawStr(polar_mode_x, 48, coordinate_mode_str[1]);
      setCoordinateModeCursor();
    } while (u8g2.nextPage());
  }
}

void UI ::setCoordinateModeStrPosition()
{
  u8g2.setFont(u8g2_font_lubB12_tr);
  w_6 = u8g2.getStrWidth(coordinate_mode_str[0]);
  w_7 = u8g2.getStrWidth(coordinate_mode_str[1]);
  cartesian_mode_x = (128 - w_6) / 2;
  polar_mode_x = (128 - w_7) / 2;
}

void UI ::setCoordinateModeCursor()
{
  switch (position)
  {
  case CURSOR_1:
    u8g2.drawFrame(cartesian_mode_x - 2, 9, w_6 + 4, 18);
    break;
  case CURSOR_2:
    u8g2.drawFrame(polar_mode_x - 2, 33, w_7 + 4, 18);
    break;
  }
}

void UI ::setBiasedPitchToEEPROM()
{
  eeProm_adr = 24 + sizeof(height);
  EEPROM.put(eeProm_adr, biased_pitch);
}

double UI ::getBiasedPitchFromEEPROM()
{
  EEPROM.get(eeProm_adr, biased_pitch);
  eeProm_adr += sizeof(biased_pitch);
  return biased_pitch;
}

void UI ::setFrequencytoEEPROM()
{
  eeProm_adr = 24 + sizeof(height) + sizeof(biased_pitch);
  EEPROM.put(eeProm_adr, data_per_second);
}

uint8_t UI ::getFrequencyFromEEPROM()
{
  EEPROM.get(eeProm_adr, data_per_second);
  return data_per_second;
}

void UI ::getPitchBias()
{
  biased_pitch = 0;
  u8g2.firstPage();
  do
  {
    u8g2.drawXBMP(2, 2, INFINITY_WIDTH, INFINITY_HIEGHT, Pitch_biased_bits);
  } while (u8g2.nextPage());
  delay(1000);
  for (int i = 1; i <= 2000; i++)
  {
    ANGLE angle = imu.calAttitude();
    float d = lidarDistance();
    float h = height;
    double Theta_estimated = -asin(h / d);
    biased_pitch = biased_pitch + ((Theta_estimated * (180 / PI)) - (angle.theta * (180 / PI)));
  }
  biased_pitch = biased_pitch / 2000;
  setBiasedPitchToEEPROM();
  delay(100);
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(0, 16, "Pitch Bias:");
    u8g2.setCursor(84, 16);
    u8g2.print(biased_pitch, 2);
  } while (u8g2.nextPage());
  delay(1000);
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(14, 17, "Saving Data to");
    u8g2.drawStr(42, 36, "EEPROM");
  } while (u8g2.nextPage());
  delay(1000);
}

void UI ::saveViewPoint()
{
  if (current_screen == CAPTURE_BOUNDARY_SCREEN)
  {
    acquire_points = acquirePoints();
    saveDataToDrive(acquire_points);
  }
  else
  {
    saveDataToDrive(view_point);
  }
}