/*

	W800 by JohnEdwa
	Version 0.8

	I'm a hobby coder at best, and this is my very first Pebble Watchface,
		so most of the code I've used here is just the first thing that worked,
		maybe slightly cleaned up.

	I would highly advice not to use any of this as a base for your own project.

*/

#include <pebble.h>

#define DEBUG 0
#define COLORDEBUG 0

// Persistent storage key
#define SETTINGS_KEY 1
#define WEATHER_KEY 2

// Draws are referenced to these if the layout changes.
#if defined(PBL_RECT)
  #define SCREENTOP 29
	#define SCREENLEFT 4
#else
  #define SCREENTOP 35
	#define SCREENLEFT 22
#endif


//typedef enum {INFO_TIME, INFO_DATE, INFO_HEALTH, INFO_TOGGLES, INFO_TAP, INFO_RESET, INFO_WEATHER} infoType_e ;

// Function declarations
static void handle_battery (BatteryChargeState charge_state);
static void handle_bluetooth (bool connected);
static void handle_tap (AccelAxisType axis, int32_t direction);
static void handle_health (HealthEventType event, void *context);
static void tick_handler (struct tm *tick_time, TimeUnits units_changed);
static void update_time (struct tm *tick_time, TimeUnits units_changed, bool firstRun);
static void timeInit ();

static char *getSlotData(char *inBuf, bool tap);
static void setWeatherData(TextLayer *layer, char *inBuf, unsigned char bufSize, bool tap);

static void update_fourSlot(bool tap);
static void background_update_proc (Layer *layer, GContext *ctx);
static void toggle_update_proc (Layer *layer, GContext *ctx);
static void main_window_load (Window *window);
static void main_window_unload (Window *window);

static void save_weather();
static void load_weather();
static void clear_weather();
static void get_weather();

static void save_settings ();
static void default_settings ();
static void load_settings ();
static void inbox_received_handler (DictionaryIterator *iter, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);

static void init ();
int main (void);

static void vibes_pwm(int8_t strength, uint16_t duration, bool twice);
static char *upcase (char *str);
static void editTextLayer (TextLayer *layer, GRect location, GColor colour, GColor background, GFont font, GTextAlignment alignment);
static char *getDateString(bool four, unsigned char mode);


// -----------------
// GLOBAL VARIABLES
// -----------------

static bool s_jsReady = 0;

HealthValue bpmOld = 0;
HealthValue bpmValue = 0;
HealthValue stepsValue = 0;
char hrArrow = ' ';

static AppTimer *s_vibe_timer;
unsigned char tapDuration = 0;
bool tapTrigger = 0;
bool vibeTrigger = 0;

static const uint32_t romanVibes[13][7] = {
	{ 	0,	0,	0,	0,	0,	0,	0},
	{ 120,	0,	0,	0,	0,	0,	0}, { 120,100,120,	0,	0,	0,	0},	{ 120,100,120,100,120,	0,	0},
	{ 120,100,320,	0,	0,	0,	0},	{ 320,	0,	0,	0,	0,	0,	0},	{ 320,100,120,	0,	0,	0,	0},
	{ 320,100,120,100,120,	0,	0},	{ 320,100,120,100,120,100,120},	{ 120,100,320,120,320,	0,	0},
	{ 320,120,320,	0,	0,	0,	0},	{ 320,120,320,100,120,	0,	0},	{ 320,120,320,100,120,100,120}
};

bool bluetoothState = false;
bool bluetoothStateOld = true;
bool quietTimeState = false;

unsigned char batteryLevel = 0;
unsigned char batteryCharge = 0;

// Windows
static Window *s_main_window;

// Fonts
static GFont s_font_hour;
static GFont s_font_second;
static GFont s_font_hour_big;
static GFont s_font_4char;
static GFont s_font_infobig;
static GFont s_font_infosmall;
static GFont s_font_bat;

// Text Layers
static TextLayer *s_layer_hour;
static TextLayer *s_layer_second;
static TextLayer *s_layer_4char;
static TextLayer *s_layer_infobar_right;
static TextLayer *s_layer_infobar_left;
static TextLayer *s_layer_bottombar;

static TextLayer *s_layer_weatherTop;
static TextLayer *s_layer_weatherBottom;
static TextLayer *s_layer_branding_top;
static TextLayer *s_layer_branding_bottom;

// Canvas Bitmap Layer
static Layer *s_layer_background;
static Layer *s_layer_toggle;

// Bitmap Layers
static BitmapLayer *s_layer_pm;

// Bitmaps
static GBitmap *s_bitmap_sheet_branding;
static GBitmap *s_bitmap_sheet_toggles;

static GBitmap *s_bitmap_background;
static GBitmap *s_bitmap_brand_logo;
static GBitmap *s_bitmap_brand_top;
static GBitmap *s_bitmap_brand_bottom;
static GBitmap *s_bitmap_brand_labels;
static GBitmap *s_bitmap_brand_bg;
static GBitmap *s_bitmap_toggle_label;
static GBitmap *s_bitmap_toggle_enabled;
static GBitmap *s_bitmap_toggle_battery;
static GBitmap *s_bitmap_toggle_charge;
static GBitmap *s_bitmap_toggle_pm;

// Palettes
static GColor *cornerPalette;
static GColor *bitPalette;
static GColor *brandingPalette;
static GColor *brandingPalette2;
static GColor *togglePalette;

// Static Buffers (One larger than max size to account for end char)
static char s_bufferHour[6];
static char s_bufferSecond[3];
static char s_bufferFourData[5];
static char s_bufferInfoLeft[9];
static char s_bufferInfoRight[9];
static char s_bufferBottomLeft[9];
static char s_bufferBottomRight[9];
static char s_bufferBottom[17];
static char s_bufferWeatherTop[64];
static char s_bufferWeatherBottom[64];

// ------------------
// SETTINGS FUNCTIONS
// ------------------

// Define our settings struct
typedef struct ClaySettings {
	bool showZero;
	bool enHealth;
	bool enWeather;
	unsigned char tapDuration;

	unsigned char btDCVibe;
	unsigned char btConVibe;
	unsigned char hourVibe;
	unsigned char vibeStrength;

	unsigned char mainTimeStyle;
	unsigned char dateStyle;

	unsigned char infoLeftStyle;
	unsigned char infoRightStyle;
	unsigned char infoLeftData;
	unsigned char infoRightData;
	unsigned char infoLeftDataTap;
	unsigned char infoRightDataTap;

	unsigned char bottomStyle;
	unsigned char bottomLeftData;
	unsigned char bottomRightData;
	unsigned char bottomLeftDataTap;
	unsigned char bottomRightDataTap;

	unsigned char brandingStyle;
	unsigned char brandingLogo;
	unsigned char brandingTop;
	unsigned char brandingBottom;
	unsigned char brandingLabel;
	bool toggleBold;

	unsigned char fourData;
	unsigned char fourDataTap;
	unsigned char fourCaps;
	unsigned char toggleLabelStyle;
	unsigned char batteryStyle;

	unsigned char weatherBoxTop;
	unsigned char weatherBoxBottom;
	unsigned char weatherBoxTopTap;
	unsigned char weatherBoxBottomTap;
	unsigned char weatherProvider;
	unsigned char weatherTempUnit;
	unsigned char weatherWindUnit;
	unsigned char weatherUpdateRate;
	unsigned char weatherDayNight;
	unsigned char weatherNightMorning;

	char locationString[32];
	char wuKeyString[20];
	char owmKeyString[40];

	GColor bgColor;
	GColor bgTextColor;
	GColor displayColor;
	GColor displayTextColor;
	GColor displayBorderColor;

} ClaySettings;
static ClaySettings conf;

// Default settings function
static void default_settings() {

	conf.bgColor = GColorBlack;
	conf.bgTextColor = GColorWhite;
	conf.displayColor = GColorWhite;
	conf.displayTextColor = GColorBlack;
	conf.displayBorderColor = GColorBlack;

	conf.btDCVibe = 0;	conf.btConVibe = 0;	conf.hourVibe = 0;	conf.vibeStrength = 3;
	conf.enHealth = 1;	conf.enWeather = 1;
	conf.tapDuration = 3;
	conf.showZero = 1;	conf.mainTimeStyle = 2;	conf.dateStyle = 2;
	conf.brandingStyle = 1;	conf.brandingLogo = 1;	conf.brandingTop = 1;	conf.brandingBottom = 2;	conf.toggleBold = 1;	conf.brandingLabel = 1;
	conf.fourCaps = 1;	conf.fourData = 1;	 conf.fourDataTap = 9;
	conf.infoLeftStyle = 1;	conf.infoRightStyle = 1; conf.infoLeftData = 7; conf.infoRightData = 8; conf.infoLeftDataTap = 0;	conf.infoRightDataTap = 0;
	conf.bottomStyle = 2; conf.bottomLeftData = 4;	conf.bottomRightData = 3; conf.bottomLeftDataTap = 0;	conf.bottomRightDataTap = 0;
	conf.weatherProvider = 1;	conf.weatherTempUnit = 1;	conf.weatherWindUnit = 1; conf.weatherUpdateRate = 30;
	conf.weatherBoxTop = 0; conf.weatherBoxBottom = 0; conf.weatherBoxTopTap = 24; conf.weatherBoxBottomTap = 15;
	conf.weatherDayNight = 15;	conf.weatherNightMorning = 21;
	strcpy(conf.locationString,""); strcpy(conf.wuKeyString,""); strcpy(conf.owmKeyString,"");
	conf.batteryStyle = 0;
	
	#if defined(PBL_PLATFORM_APLITE)
	conf.enHealth = 0; conf.infoLeftData = 14; conf.infoRightData = 14;
	#endif
	
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Default settings loaded.");
}

// Define our Weather data struct
typedef struct WeatherData {
	unsigned char provider;
	unsigned long timeStamp;
	char tempCur[5];
	char tempMin[5];
	char tempMax[5];

	char windCur[8];
	char windMax[8];
	char humidity[5];
	char sunset[6];
	char sunrise[6];

	char location[64];
	char condMain[32];
	char condDesc[32];
	char condForecast[32];
	
	int retryTimer;
	int retryCount;
} WeatherData;
static WeatherData weather;

// Clear weather persistent weather data.
static void clear_weather() {
	weather.provider = 0;
	weather.timeStamp = 0;
	strcpy(weather.tempCur,"");
	strcpy(weather.tempMin,"");
	strcpy(weather.tempMax,"");
	strcpy(weather.windCur,"");
	strcpy(weather.windMax,"");
	strcpy(weather.humidity,"");
	strcpy(weather.sunset,"");
	strcpy(weather.sunrise,"");
	strcpy(weather.location,"Loading...");
	strcpy(weather.condMain,"");
	strcpy(weather.condDesc,"");
	strcpy(weather.condForecast,"");
	
	weather.retryTimer = 0;
	weather.retryCount = 0;

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather: Weather Cleared.");
}

// -----------------
// HANDLER FUNCTIONS
//------------------

// App timer that disables tap triggering when vibrating
static void timout_timer_handler(void *context) {vibeTrigger = false;}

// Handles the vibrations
static void vibrate(int8_t style) {
	vibeTrigger = true;
	s_vibe_timer = app_timer_register(1000, timout_timer_handler, NULL);

	quietTimeState = quiet_time_is_active();
	if (!quietTimeState) {
		switch (style) {
			case 1: vibes_pwm(conf.vibeStrength, 150, 0); break;
			case 2: vibes_pwm(conf.vibeStrength, 300, 0); break;
			case 3: vibes_pwm(conf.vibeStrength, 500, 0); break;
			case 4: vibes_pwm(conf.vibeStrength, 150, 1); break;
			case 5: vibes_pwm(conf.vibeStrength, 300, 1); break;
			case 6: vibes_pwm(conf.vibeStrength, 500, 1); break;
			case 7:
				;time_t temp = time(NULL);
				struct tm *tick_time = localtime(&temp);
				char hour[4];
				strftime(hour, sizeof(hour), "%I", tick_time);
				VibePattern hourPat = {.durations = romanVibes[atoi(hour)], .num_segments = ARRAY_LENGTH(romanVibes[atoi(hour)]),};
				vibes_enqueue_custom_pattern(hourPat); break;
			default:  break;
		}
	}
}

// Keeps track of battery state
static void handle_battery(BatteryChargeState charge_state) {	
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_battery: plugged: %d, charge: %d, state: %d", charge_state.is_plugged, charge_state.is_charging, charge_state.charge_percent);
	unsigned char oldCharge = batteryCharge;
	unsigned char oldLevel = batteryLevel;
	if (charge_state.is_charging) batteryCharge = 1;
	else if (charge_state.is_plugged) batteryCharge = 2;
	else batteryCharge = 0;
	batteryLevel = ((charge_state.charge_percent)/10);
	
	if (oldLevel != batteryLevel || oldCharge != batteryCharge) {
		layer_mark_dirty(s_layer_toggle);
	}
}

// Keeps track of BT state
static void handle_bluetooth(bool connected) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_bluetooth: %d", connected);
  bluetoothState = connected;
	if(!connected && conf.btDCVibe != 0 && bluetoothStateOld == true) { vibrate(conf.btDCVibe); }
	else if(connected && conf.btConVibe != 0 && bluetoothStateOld == false) {vibrate(conf.btConVibe);}
	if (bluetoothStateOld != bluetoothState){
		bluetoothStateOld = bluetoothState;
		layer_mark_dirty(s_layer_toggle);
	}
}

// Triggers on a tap
static void handle_tap(AccelAxisType axis, int32_t direction) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tap");

	if (!tapTrigger && !vibeTrigger && conf.tapDuration > 0) {
		tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
		tapTrigger = true;
		tapDuration = conf.tapDuration;
		//layer_mark_dirty(s_layer_background);
		update_fourSlot(true);
	}
}

// Handle Health Events
#if defined(PBL_HEALTH)
static void handle_health(HealthEventType event, void *context) {
		if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "update_health : %d", event);

		if (event == HealthEventSignificantUpdate || event == HealthEventHeartRateUpdate) {
			bpmValue = health_service_peek_current_value(HealthMetricHeartRateBPM);
			hrArrow = bpmValue > bpmOld ? ')' : (bpmValue < bpmOld ? '(' : ' ');
			if (bpmOld != bpmValue) {
				bpmOld = bpmValue;
				update_fourSlot(false);
			}
		}
		if (event == HealthEventSignificantUpdate || event == HealthEventMovementUpdate) {
			stepsValue = health_service_sum_today(HealthMetricStepCount);
			update_fourSlot(false);
		}
}
#endif

// Gets the correct buffer data for the info slots
static char *getSlotData(char *inBuf, bool tap) {
	//if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "getSlotData");
	static char outBuf[16] = {};
	strcpy(outBuf,inBuf);

	if ((tap && tapTrigger) || (!tap && !tapTrigger)) {
		
		unsigned char confValue = 255;
		unsigned char styleValue = 255;
		bool left = NULL;
		static char buf[16] = {};
		strcpy(buf,"");
		
			 	 if (inBuf == s_bufferInfoLeft)	{confValue = tap ? (conf.infoLeftDataTap > 0 ? conf.infoLeftDataTap : conf.infoLeftData) : conf.infoLeftData; styleValue = conf.infoLeftStyle; left = true;}
		else if (inBuf == s_bufferInfoRight) {confValue = tap ? (conf.infoRightDataTap > 0 ?  conf.infoRightDataTap : conf.infoRightData) : conf.infoRightData; styleValue = conf.infoRightStyle; left = false;}
		else if (inBuf == s_bufferBottomLeft) {confValue = tap ? (conf.bottomLeftDataTap > 0 ? conf.bottomLeftDataTap : conf.bottomLeftData) : conf.bottomLeftData; styleValue = conf.bottomStyle; left = true;}
		else if (inBuf == s_bufferBottomRight) {confValue = tap ? (conf.bottomRightDataTap > 0 ? conf.bottomRightDataTap : conf.bottomRightData) : conf.bottomRightData; styleValue = conf.bottomStyle; left = false;}
		else if (inBuf == s_bufferFourData) {confValue = tap ? (conf.fourDataTap > 0 ? conf.fourDataTap : conf.fourData) : conf.fourData; styleValue = 1; left = true;}

		time_t temp = time(NULL);
		struct tm *tick_time = localtime(&temp);

		switch (confValue) {
			// Disabled / Hidden
			case  0: strcpy(buf," "); break;
			// Day Word
			case  1: strftime(buf, sizeof(buf), "%a", tick_time); break;
			// Month Word
			case  2: strftime(buf, sizeof(buf), "%b", tick_time); break;
			// Date Numbers
			case  3:
				if (inBuf != s_bufferFourData) snprintf(buf, sizeof(buf), "%-5s", getDateString(false, 0));
				else snprintf(buf, sizeof(buf), "%-4s", getDateString(true, 0));
				break;
			// Year Numbers
			case  4: strftime(buf, sizeof(buf), "%Y", tick_time); break;
			// Day Number
			case 16: snprintf(buf, sizeof(buf), "%2s", getDateString(false, 2));break;
			// Week Number
			case 17: strftime(buf, sizeof(buf), "%U", tick_time); break;
			// Month Number
			case 18: snprintf(buf, sizeof(buf), "%2s", getDateString(false, 1)); break;
			// Battery Level
			case  5:
			case  6:
				if (batteryCharge == 1) snprintf(buf, sizeof(buf), "Chrg");
				else if (batteryCharge == 2) snprintf(buf, sizeof(buf), "Plug");
				 else snprintf(buf, sizeof(buf), confValue == 5 ? "%3d%%" : "%3d", (int) batteryLevel*10);
					break;
			// Steps
			case  7:
				if (inBuf == s_bufferFourData) {
					if (stepsValue < 10000) {snprintf(buf, sizeof(buf), "%4d", (int) stepsValue);}
					else {snprintf(buf, sizeof(buf), "%2d.%d", (int) (stepsValue/1000), (int) stepsValue / 100 % 10);}
				} else {
					snprintf(buf, sizeof(buf),left ? "%5d " : " %5d", (int) stepsValue);
				}			
				break;
			// HeartRate
			case  8:
				if (inBuf == s_bufferFourData) snprintf(buf, sizeof(buf), "%3d", (int) bpmValue);
				else {
					if (left)snprintf(buf, sizeof(buf), "%c$%3d ", hrArrow, (int) bpmValue);
					else snprintf(buf, sizeof(buf), " $%3d%c", (int) bpmValue,hrArrow);
				} break;
			// Temperature current
			case  9: snprintf(buf, sizeof(buf), "%3s%c", weather.tempCur, (conf.weatherTempUnit == 1 ? (inBuf == s_bufferFourData ? '#' : 'C') : (conf.weatherTempUnit == 2 ? (inBuf == s_bufferFourData ? '@' : 'F') : 'K'))); break;
			case 10:  break;
			case 11:  break;
			// Sunrise / Sunset
			case 12:
			case 13: snprintf(buf,sizeof(buf),  left ? "%5.5s " : " %5.5s", confValue == 12 ? weather.sunrise : weather.sunset); break;

			// Conditions String
			case 14:
			case 15:
				;int wLength = (confValue == 14 ? strlen(weather.condDesc) : strlen(weather.condForecast));
				if (wLength <= (styleValue == 1 ? 16 : 12)) {
					if (left) snprintf(outBuf, sizeof(buf), "%.*s", (int) (wLength/2),  (confValue == 14 ? weather.condDesc : weather.condForecast) );
					else snprintf(outBuf, sizeof(buf), "%s", (confValue == 14 ? weather.condDesc : weather.condForecast) + wLength - (wLength -(wLength/2)));
				}
				if (wLength > (styleValue == 1 ? 16 : 12)) {
					if (left) snprintf(outBuf, sizeof(buf), "%.*s", styleValue == 1 ? 8 : 6,  (confValue == 14 ? weather.condDesc : weather.condForecast));
					else snprintf(outBuf, sizeof(buf), "%.*s", styleValue == 1 ? 8 : 6,  ((confValue == 14 ? weather.condDesc : weather.condForecast) + (styleValue == 1 ? 8 : 6)));
				} break;
			default: strcpy(buf," "); break;
		}

		// Finally center the buffers properly.
		if (confValue != 14 && confValue != 15) {
			if (inBuf == s_bufferFourData){
				snprintf(outBuf, sizeof(buf), "%4s", buf);
			} else {
				if (left) snprintf(outBuf, sizeof(buf), "%*s%*s", (int) ((styleValue == 1 ? 4 : 3 )+((strlen(buf) + (2/2))/2)) , buf, (int) ((styleValue == 1 ? 4 : 3 )-((strlen(buf) + (2/2))/2)), "");
				else snprintf(outBuf, sizeof(buf), "%*s%*s", (int) ((styleValue == 1 ? 4 : 3 )+(strlen(buf)/2)), buf, (int) ((styleValue == 1 ? 4 : 3 )-(strlen(buf)/2)), "");
			}
		}
	}
	return outBuf;
}

// Sets a Weather box Data
static void setWeatherData(TextLayer *layer, char *inBuf, unsigned char bufSize, bool tap) {
	if ((tap && tapTrigger) || (!tap && !tapTrigger)) {
		unsigned char confValue = 255;
		char unit[3];
		strcpy(unit,(conf.weatherTempUnit == 1 || conf.weatherTempUnit == 2 ? "°" : "K"));
		
		if (inBuf == s_bufferWeatherTop) {confValue = tap ? conf.weatherBoxTopTap : conf.weatherBoxTop;}
		else if (inBuf == s_bufferWeatherBottom) {confValue = tap ? conf.weatherBoxBottomTap : conf.weatherBoxBottom;}
				
		/*
		if (inBuf == s_bufferWeatherTop)	{ bufSize = sizeof(s_bufferWeatherTop); confValue = tap ? (conf.infoLeftDataTap > 0 ? conf.infoLeftDataTap : conf.infoLeftData) : conf.infoLeftData;}
		else if (inBuf == s_bufferWeatherBottom) { bufSize = sizeof(s_bufferWeatherBottom); confValue = tap ? (conf.infoRightDataTap > 0 ?  conf.infoRightDataTap : conf.infoRightData) : conf.infoRightData;}
		*/

		switch (confValue) {
			case 0: strcpy(inBuf," "); break;
			case 1: snprintf(inBuf, bufSize, "%s", weather.location); break;
			case 2: snprintf(inBuf, bufSize, "Lo %s%s  [ %s%s ]   Hi %s%s", weather.tempMin, unit, weather.tempCur, unit, weather.tempMax, unit); break;
			case 3: 
			case 4: snprintf(inBuf, bufSize, "%s", confValue == 3 ? weather.condDesc : weather.condForecast); break;
			case 5: snprintf(inBuf, bufSize, "%s                %s", weather.sunrise, weather.sunset); break;
			case 21: snprintf(inBuf, bufSize, "Lo %s%s  [ %s%s ]   Hi %s%s\n%s", weather.tempMin, unit, weather.tempCur, unit, weather.tempMax, unit, weather.location); break;
			case 23:
			case 24: snprintf(inBuf, bufSize, "Lo %s%s  [ %s%s ]   Hi %s%s\n%s", weather.tempMin, unit, weather.tempCur, unit, weather.tempMax, unit, confValue == 23 ? weather.condDesc : weather.condForecast); break;
			case 25: snprintf(inBuf, bufSize, "Lo %s%s  [ %s%s ]   Hi %s%s\n%s                %s", weather.tempMin, unit, weather.tempCur, unit, weather.tempMax, unit, weather.sunrise, weather.sunset); break;
			case 12: snprintf(inBuf, bufSize, "%s\nLo %s%s  [ %s%s ]   Hi %s%s", weather.location, weather.tempMin, unit, weather.tempCur, unit, weather.tempMax, unit); break;
			case 13:
			case 14: snprintf(inBuf, bufSize, "%s\n%s", weather.location, confValue == 13 ? weather.condDesc : weather.condForecast); break;
			case 15: snprintf(inBuf, bufSize, "%s\n%s                %s",weather.location, weather.sunrise, weather.sunset); break;
			case 34: 
			case 43:
				//if (strcmp(weather.condDesc, weather.condForecast) == 0) snprintf(inBuf, bufSize, "%s", weather.condDesc);
				snprintf(inBuf, bufSize, "%s\n%s", confValue == 34 ? weather.condDesc : weather.condForecast, confValue == 34 ? weather.condForecast : weather.condDesc);
				break;
			default: strcpy(inBuf," "); break;
		}
		
		text_layer_set_text(layer, inBuf);
		
		// Vertical alignment
		GRect frame = layer_get_frame(text_layer_get_layer(layer));
		GSize content = text_layer_get_content_size(layer);
		layer_set_frame(text_layer_get_layer(layer), GRect(frame.origin.x, frame.origin.y + (frame.size.h - content.h) / 2,frame.size.w, content.h));
		//if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Size: Frame %d/%d, Layer %d/%d. GRect: %d,%d,%d,%d",frame.size.w, frame.size.h, content.w, content.h,frame.origin.x, frame.origin.y + (frame.size.h - content.h - 5) / 2,frame.size.w, content.h);
	}
}

// Keeps track of the FourCharSlot data and tap updates.
static void update_fourSlot(bool tap) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "update_fourSlot");

// We allow regular updates only when tap is not activated.
	if ((tap && tapTrigger) || (!tap && !tapTrigger)) {

		// 4char
		snprintf(s_bufferFourData, sizeof(s_bufferFourData), "%4s", conf.fourCaps ? upcase(getSlotData(s_bufferFourData, tap)) : getSlotData(s_bufferFourData, tap));
		text_layer_set_text(s_layer_4char, s_bufferFourData);

		// Info Bar
		snprintf(s_bufferInfoLeft, sizeof(s_bufferInfoLeft), conf.infoLeftStyle == 1 ? "%8s" : "%6s", upcase(getSlotData(s_bufferInfoLeft, tap)));
		text_layer_set_text(s_layer_infobar_left, s_bufferInfoLeft);

		snprintf(s_bufferInfoRight, sizeof(s_bufferInfoRight), conf.infoRightStyle == 1 ? "%-8s" : "%-6s", upcase(getSlotData(s_bufferInfoRight, tap)));
		text_layer_set_text(s_layer_infobar_right, s_bufferInfoRight);

		// Bottom bar
		snprintf(s_bufferBottomLeft, sizeof(s_bufferBottomLeft), "%s", getSlotData(s_bufferBottomLeft, tap));
		snprintf(s_bufferBottomRight, sizeof(s_bufferBottomRight), "%s", getSlotData(s_bufferBottomRight, tap));
		snprintf(s_bufferBottom, sizeof(s_bufferBottom), conf.bottomStyle == 1 ? "%8s%-8s" : "%6s%-6s", upcase(s_bufferBottomLeft), upcase(s_bufferBottomRight));
		text_layer_set_text(s_layer_bottombar,  s_bufferBottom);

		setWeatherData(s_layer_weatherTop, s_bufferWeatherTop, sizeof(s_bufferWeatherTop), tap);
		setWeatherData(s_layer_weatherBottom, s_bufferWeatherBottom, sizeof(s_bufferWeatherBottom), tap);
	}
}

// Builds the background layer
static void background_update_proc(Layer *layer, GContext *ctx) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "background_update_proc");
	
	// Draw the BG bitmap
	graphics_context_set_compositing_mode(ctx, GCompOpSet);	
	
	//graphics_context_set_fill_color(ctx, conf.bgColor);
	//graphics_fill_rect(ctx, GRect(0, 0, 180, 180), 0, 0);
	
	// Draw the display box
		graphics_context_set_fill_color(ctx, conf.displayColor);		
		graphics_fill_rect(ctx, GRect(SCREENLEFT-3, SCREENTOP-3, 142, 116), 0, 0);	
		graphics_context_set_fill_color(ctx, conf.displayBorderColor);
		graphics_fill_rect(ctx, GRect(SCREENLEFT-2, SCREENTOP-2, 140, 114), 0, 0);
		graphics_context_set_fill_color(ctx, conf.displayColor);
		graphics_fill_rect(ctx, GRect(SCREENLEFT, SCREENTOP, 136, 110), 0, 0);
	
	// Draw the corners
		graphics_context_set_fill_color(ctx, conf.bgColor);
		#if defined(PBL_RECT)
			gbitmap_set_bounds(s_bitmap_background, GRect(10, 10, 10,10));
			graphics_draw_bitmap_in_rect(ctx, s_bitmap_background, GRect(SCREENLEFT-4, SCREENTOP-4, 10,10));	
			gbitmap_set_bounds(s_bitmap_background, GRect(0, 10, 10,10));
			graphics_draw_bitmap_in_rect(ctx, s_bitmap_background, GRect(SCREENLEFT+130, SCREENTOP-4, 10,10));
			gbitmap_set_bounds(s_bitmap_background, GRect(10, 0, 10,10));
			graphics_draw_bitmap_in_rect(ctx, s_bitmap_background, GRect(SCREENLEFT-4, SCREENTOP+104, 10,10));
			gbitmap_set_bounds(s_bitmap_background, GRect(0, 0, 10,10));
			graphics_draw_bitmap_in_rect(ctx, s_bitmap_background, GRect(SCREENLEFT+130, SCREENTOP+104, 10,10));
		#else
			//gbitmap_set_bounds(s_bitmap_background, GRect(0, 0, 128,23));
			graphics_draw_rotated_bitmap(ctx, s_bitmap_background, GPoint(0,0), DEG_TO_TRIGANGLE(90), GPoint(SCREENLEFT+154, SCREENTOP-9));
			//gbitmap_set_bounds(s_bitmap_background, GRect(0, 23, 128,23));
			graphics_draw_rotated_bitmap(ctx, s_bitmap_background, GPoint(128,23), DEG_TO_TRIGANGLE(270), GPoint(SCREENLEFT+4, SCREENTOP-10));
		#endif
	
	// Show the W800 border...
	if (conf.brandingStyle == 1) {
		graphics_context_set_fill_color(ctx, conf.displayBorderColor);
		graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_bg, GRect(SCREENLEFT+3, SCREENTOP+3, gbitmap_get_bounds(s_bitmap_brand_bg).size.w, gbitmap_get_bounds(s_bitmap_brand_bg).size.h));
	}
	else if (conf.brandingStyle == 2) {
		// ..or draw the UI Lines for W86
		graphics_context_set_fill_color(ctx, conf.displayBorderColor);
		graphics_fill_rect(ctx, GRect(SCREENLEFT, SCREENTOP+25, 75, 2), 0, 0);
		graphics_fill_rect(ctx, GRect(SCREENLEFT+75, SCREENTOP+25, 1, 1), 0, 0);
		graphics_fill_rect(ctx, GRect(SCREENLEFT+76, SCREENTOP, 1, 25), 0, 0);
	}
	if (conf.brandingStyle != 0 && conf.brandingStyle != 3) {
	// Draw Toggle line
		graphics_context_set_fill_color(ctx, conf.displayBorderColor);
		graphics_fill_rect(ctx, GRect(SCREENLEFT+79, SCREENTOP+26, PBL_IF_RECT_ELSE(57,68), 1), 0, 0);
		graphics_fill_rect(ctx, GRect(SCREENLEFT+80, SCREENTOP+25, PBL_IF_RECT_ELSE(56,68), 1), 0, 0);
	}
	if (conf.brandingStyle != 0 && conf.brandingStyle != 2 && conf.brandingStyle != 4) {
		// Draw Bottom Line
		graphics_context_set_fill_color(ctx, conf.displayBorderColor);
		graphics_fill_rect(ctx, GRect(PBL_IF_RECT_ELSE(SCREENLEFT,SCREENLEFT-8),SCREENTOP+90, PBL_IF_RECT_ELSE(144-SCREENLEFT*2, 180-(SCREENLEFT-9)*2),2), 0,0);
	}
		
	// If weather box is empty, draw the branding.
	if ((!tapTrigger && conf.weatherBoxTop == 0) || (tapTrigger && conf.weatherBoxTopTap == 0)) {
		if (conf.brandingLogo != 0) graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_logo, GRect(SCREENLEFT+7, SCREENTOP-23, gbitmap_get_bounds(s_bitmap_brand_logo).size.w, gbitmap_get_bounds(s_bitmap_brand_logo).size.h));
		if (conf.brandingTop != 0) graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_top, GRect(SCREENLEFT+66, SCREENTOP-19, gbitmap_get_bounds(s_bitmap_brand_top).size.w, gbitmap_get_bounds(s_bitmap_brand_top).size.h));
	}
	
	// Bottom Branding
	if ((!tapTrigger && conf.weatherBoxBottom == 0) || (tapTrigger && conf.weatherBoxBottomTap == 0)) {
		if (conf.brandingBottom != 0) graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_bottom, GRect(SCREENLEFT+4, SCREENTOP+121, gbitmap_get_bounds(s_bitmap_brand_bottom).size.w, gbitmap_get_bounds(s_bitmap_brand_bottom).size.h));
	}
	
	// NExt/Prev/Light labels
	if (conf.brandingLabel != 0) {
		graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_labels, GRect(SCREENLEFT-9, SCREENTOP-50, gbitmap_get_bounds(s_bitmap_brand_labels).size.w, gbitmap_get_bounds(s_bitmap_brand_labels).size.h));
		graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_labels, GRect(SCREENLEFT+135, SCREENTOP-49, gbitmap_get_bounds(s_bitmap_brand_labels).size.w, gbitmap_get_bounds(s_bitmap_brand_labels).size.h));
		graphics_draw_bitmap_in_rect(ctx, s_bitmap_brand_labels, GRect(SCREENLEFT+135, SCREENTOP+117, gbitmap_get_bounds(s_bitmap_brand_labels).size.w, gbitmap_get_bounds(s_bitmap_brand_labels).size.h));
	}
}

// Builds the toggle layer
static void toggle_update_proc(Layer *layer, GContext *ctx) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "toggle_update_proc");

	// Draw the labels
	graphics_draw_bitmap_in_rect(ctx, s_bitmap_toggle_label, GRect(0, 0, gbitmap_get_bounds(s_bitmap_toggle_label).size.w, gbitmap_get_bounds(s_bitmap_toggle_label).size.h));

	// Battery
	if (conf.batteryStyle == 1) {
		// Full bar
		if (batteryLevel >= 1 || batteryCharge == 1) {
			graphics_context_set_fill_color(ctx, conf.displayTextColor);
			graphics_context_set_stroke_color(ctx, conf.displayTextColor);
			graphics_draw_bitmap_in_rect(ctx, s_bitmap_toggle_charge, GRect(28, 0, gbitmap_get_bounds(s_bitmap_toggle_charge).size.w, gbitmap_get_bounds(s_bitmap_toggle_charge).size.h));
			graphics_fill_rect(ctx, GRect(30,1,(batteryLevel == 10 ? 13 : batteryLevel+2),2), 0,0);
			if (batteryCharge != 1) {
				graphics_fill_rect(ctx, GRect(31,0,(batteryLevel == 10 ? 13 : batteryLevel+2),1), 0,0);
				graphics_fill_rect(ctx, GRect(29,3,(batteryLevel == 10 ? 13 : batteryLevel+2),1), 0,0);
			}
			
		}
	}
	else if (conf.batteryStyle == 2) {
		// Text
		char batBuf[8];
		if (batteryLevel >= 1  || batteryCharge == 1) { snprintf(batBuf, sizeof(batBuf), batteryCharge == 1 ? "+%d" : "%d", batteryLevel*10); }
		else { snprintf(batBuf, sizeof(batBuf), "!!!"); }
		graphics_context_set_text_color(ctx, conf.displayTextColor);		
		graphics_draw_text(ctx, batBuf, s_font_bat, GRect(26,-11,16,16), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
	}
	else {
		if (batteryLevel >= 1 || batteryCharge == 1) {
			// Toggle button
			graphics_context_set_fill_color(ctx, conf.displayTextColor);
			graphics_context_set_stroke_color(ctx, conf.displayTextColor);
			graphics_draw_bitmap_in_rect(ctx, (batteryCharge == 1 ? s_bitmap_toggle_charge : s_bitmap_toggle_battery), GRect(28, 0, gbitmap_get_bounds(s_bitmap_toggle_charge).size.w, gbitmap_get_bounds(s_bitmap_toggle_charge).size.h));
			graphics_fill_rect(ctx, GRect(30,1,(batteryLevel == 10 ? 13 : batteryLevel+2),1), 0,0);
			graphics_fill_rect(ctx, GRect(29,2,(batteryLevel == 10 ? 13 : batteryLevel+2),1), 0,0);
		}
	}

	// BT
	if (bluetoothState) graphics_draw_bitmap_in_rect(ctx, s_bitmap_toggle_enabled, GRect(28, 7, gbitmap_get_bounds(s_bitmap_toggle_enabled).size.w, gbitmap_get_bounds(s_bitmap_toggle_enabled).size.h));
 
	// QT
	if (quietTimeState) graphics_draw_bitmap_in_rect(ctx, s_bitmap_toggle_enabled, GRect(28, 14, gbitmap_get_bounds(s_bitmap_toggle_enabled).size.w, gbitmap_get_bounds(s_bitmap_toggle_enabled).size.h));
}


// Time update function
static void update_time(struct tm *tick_time, TimeUnits units_changed, bool firstRun) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Time Tick - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());

	// Hourly Vibration
	if (units_changed & HOUR_UNIT && !quietTimeState && conf.hourVibe) {
		 vibrate(conf.hourVibe);
	}

	// Handle the tap countdown
	if (tapTrigger && units_changed & SECOND_UNIT) {
		if (tapDuration > 0) {
			tapDuration = tapDuration - 1;
		}
		if (tapDuration <= 0) {
			// Unsubscribe back to the minute tick service if seconds aren't needed anymore
			if (conf.mainTimeStyle != 3) {
				tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
			}
			tapDuration = 0;
			tapTrigger = false;
			//layer_mark_dirty(s_layer_background);
			update_fourSlot(false);
		}
	}

	// Time, Seconds
	if (conf.mainTimeStyle == 3 && (units_changed & SECOND_UNIT || firstRun)) {
		strftime(s_bufferSecond, sizeof(s_bufferSecond), "%S", tick_time);
		text_layer_set_text(s_layer_second, s_bufferSecond);
	}

	// Time, Minutes/Hours (Also weather update)
	if (units_changed & MINUTE_UNIT || firstRun) {
			// Poll for Quiet Time
			quietTimeState = quiet_time_is_active();
		
			// Check for weather update, assuming we need to and haven't retried in a while 		
			if (weather.retryTimer > 0) weather.retryTimer--;		
			if ((time(NULL) - weather.timeStamp) > (conf.weatherUpdateRate < 15 ? conf.weatherUpdateRate*3600 : conf.weatherUpdateRate*60)) {
				if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather Weather Update:  %lu > %d, try %d, timer %d", (time(NULL) - weather.timeStamp), (conf.weatherUpdateRate < 15 ? conf.weatherUpdateRate*3600 : conf.weatherUpdateRate*60), weather.retryCount, weather.retryTimer);
				if (weather.retryTimer == 0) get_weather();
			}

			strftime(s_bufferHour, sizeof(s_bufferHour), clock_is_24h_style() ? (conf.showZero ? "%H:%M" : "%k:%M" ) : (conf.showZero ? "%I:%M" : "%l:%M"), tick_time);
			layer_set_hidden(bitmap_layer_get_layer(s_layer_pm), (clock_is_24h_style() ? true : (tick_time->tm_hour >= 12 ? false : true)));
			text_layer_set_text(s_layer_hour, s_bufferHour);
			update_fourSlot(false);
	}
}

// Tick handles, just calls update_time.
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {update_time(tick_time, units_changed, false);}

// Mostly so we can call this after receiving configs
static void timeInit() {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "timeInit");

	// Register with TickTimerService
	if (conf.mainTimeStyle == 3) tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	else tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	update_time(tick_time, SECOND_UNIT, true);
	update_fourSlot(false);
}

// Loads the main window, bitmaps and layers
static void main_window_load(Window *window) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Main Window Load - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());

		// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create GFonts
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Create Fonts - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	s_font_hour = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HOUR_35));
	s_font_hour_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HOUR_BIG_42));
	s_font_second = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SECOND_25));
	s_font_4char = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FOURCHAR_16));
	s_font_infobig = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INFO_BIG_16));
	s_font_infosmall = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INFO_SMALL_16));
	s_font_bat = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BAT_16));

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Load Bitmap Sheets - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	// Load Bitmaps
	s_bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_TINY);
	s_bitmap_brand_bg = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_BIT);
	s_bitmap_sheet_branding = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SHEET_BRANDING);
	s_bitmap_sheet_toggles = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SHEET_TOGGLES);
	s_bitmap_brand_labels = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BUTTON_LABELS);

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Creating bitmaps - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	s_bitmap_brand_logo = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_branding, GRect(0,(conf.brandingLogo > 0 ? (conf.brandingLogo-1) * 16 : 0) , 52, 16));
	s_bitmap_brand_top = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_branding, GRect(64,(conf.brandingTop > 0 ? (conf.brandingTop-1) * 10 : 0) , 70, 9));
	s_bitmap_brand_bottom = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_branding, GRect(0,(conf.brandingBottom > 0 ? ((conf.brandingBottom-1) * 12) + 48 : 0), 128, 12));
	s_bitmap_toggle_enabled = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_toggles, GRect(46,0,17,4));
	s_bitmap_toggle_battery = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_toggles, GRect(46,4,17,4));
	s_bitmap_toggle_charge = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_toggles, GRect(46,8,17,4));
	s_bitmap_toggle_label = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_toggles, GRect((conf.toggleBold == true ? 23 : 0),0,23,20));
	s_bitmap_toggle_pm = gbitmap_create_as_sub_bitmap(s_bitmap_sheet_toggles, GRect(46,12,6,7));
	
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting Bitmap Palettes - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	cornerPalette = gbitmap_get_palette(s_bitmap_background);
	cornerPalette[0] = conf.displayBorderColor;
	cornerPalette[1] = conf.bgColor;
	cornerPalette[3] = conf.displayColor;
	gbitmap_set_palette(s_bitmap_background, cornerPalette, false);
	
	bitPalette = gbitmap_get_palette(s_bitmap_brand_bg);
	bitPalette[0] = conf.displayBorderColor;
	bitPalette[1] = conf.displayColor;
	gbitmap_set_palette(s_bitmap_brand_bg, bitPalette, false);
	
	brandingPalette = gbitmap_get_palette(s_bitmap_sheet_branding);
	brandingPalette[0] = conf.bgTextColor;
	brandingPalette[1] = conf.bgColor;
	gbitmap_set_palette(s_bitmap_sheet_branding, brandingPalette, false);
	
	brandingPalette2 = gbitmap_get_palette(s_bitmap_brand_labels);
	brandingPalette2[0] = conf.bgTextColor;
	brandingPalette2[1] = conf.bgColor;
	gbitmap_set_palette(s_bitmap_brand_labels, brandingPalette2, false);
	
	togglePalette = gbitmap_get_palette(s_bitmap_sheet_toggles);
	togglePalette[0] = conf.displayTextColor;
	togglePalette[1] = conf.displayColor;
	gbitmap_set_palette(s_bitmap_sheet_toggles, togglePalette, false);
	
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Creating layers - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	// Create background Layer
	s_layer_background = layer_create(bounds);
	layer_set_update_proc(s_layer_background, background_update_proc);
	layer_add_child(window_get_root_layer(window), s_layer_background);
	
	// Create Toggles Layer
	s_layer_toggle = layer_create(GRect(SCREENLEFT+85,SCREENTOP+3,55,22));
	layer_set_update_proc(s_layer_toggle, toggle_update_proc);
	layer_add_child(window_get_root_layer(window), s_layer_toggle);

	// Create AM/PM toggle Layers
	s_layer_pm = bitmap_layer_create(GRect(SCREENLEFT+13, SCREENTOP+55, 10,11));
	bitmap_layer_set_compositing_mode(s_layer_pm, GCompOpSet);
	bitmap_layer_set_bitmap(s_layer_pm, s_bitmap_toggle_pm);
	layer_set_hidden(bitmap_layer_get_layer(s_layer_pm), true);

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Creating Text Layers - heap used %d", (int) heap_bytes_used());
	// Text Layers
	s_layer_hour = text_layer_create(GRect(0,0,0,0));
	s_layer_second = text_layer_create(GRect(0,0,0,0));
	s_layer_4char = text_layer_create(GRect(0,0,0,0));
	s_layer_infobar_right = text_layer_create(GRect(0,0,0,0));
	s_layer_infobar_left = text_layer_create(GRect(0,0,0,0));
	s_layer_bottombar = text_layer_create(GRect(0,0,0,0));
	s_layer_branding_top = text_layer_create(GRect(0,0,0,0));
	s_layer_branding_bottom = text_layer_create(GRect(0,0,0,0));
	s_layer_weatherTop = text_layer_create(GRect(0,0,0,0));
	s_layer_weatherBottom = text_layer_create(GRect(0,0,0,0));

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Configuring Layers - heap used %d", (int) heap_bytes_used());

	// Configure Main Time Layers
	if (conf.mainTimeStyle == 1) {
		editTextLayer(s_layer_hour, GRect(SCREENLEFT+19, SCREENTOP+48, 100, 70), conf.displayTextColor, GColorClear, s_font_hour, GTextAlignmentLeft);
		layer_set_frame(bitmap_layer_get_layer(s_layer_pm), GRect(SCREENLEFT+25, SCREENTOP+55, 10, 11));
	}
	else if (conf.mainTimeStyle == 2) {
		editTextLayer(s_layer_hour, GRect(SCREENLEFT+4, SCREENTOP+45, 144, 100), conf.displayTextColor, GColorClear, s_font_hour_big, GTextAlignmentLeft);
		layer_set_frame(bitmap_layer_get_layer(s_layer_pm), GRect(SCREENLEFT+12, SCREENTOP+52, 10, 11));
	}
	else if (conf.mainTimeStyle == 3) {
		editTextLayer(s_layer_hour,GRect(SCREENLEFT+4, SCREENTOP+48, 100, 70), conf.displayTextColor, GColorClear, s_font_hour, GTextAlignmentLeft);
		editTextLayer(s_layer_second,GRect(SCREENLEFT+104, SCREENTOP+58, 40, 40), conf.displayTextColor, GColorClear, s_font_second, GTextAlignmentLeft);
		layer_set_frame(bitmap_layer_get_layer(s_layer_pm), GRect(SCREENLEFT+10, SCREENTOP+55, 10, 11));
	}

	// Four Char Layer
	if (conf.fourData != 0 || conf.fourDataTap != 0) editTextLayer(s_layer_4char, GRect(SCREENLEFT+9, SCREENTOP+4, 64, 20), conf.displayTextColor, GColorClear, s_font_4char, GTextAlignmentRight);

	// Top Infobar
	if (conf.infoLeftStyle == 1) editTextLayer(s_layer_infobar_left, GRect(SCREENLEFT-4, SCREENTOP+26, 72, 20), conf.displayTextColor, GColorClear, s_font_infosmall, GTextAlignmentRight);
	else if (conf.infoLeftStyle == 2) editTextLayer(s_layer_infobar_left, GRect(SCREENLEFT-4, SCREENTOP+25, 72, 20), conf.displayTextColor, GColorClear, s_font_infobig, GTextAlignmentRight);
	if (conf.infoRightStyle == 1)	editTextLayer(s_layer_infobar_right, GRect(SCREENLEFT+69, SCREENTOP+26, 72, 20), conf.displayTextColor, GColorClear, s_font_infosmall, GTextAlignmentLeft);
	else if (conf.infoRightStyle == 2) editTextLayer(s_layer_infobar_right, GRect(SCREENLEFT+69, SCREENTOP+25, 72, 20), conf.displayTextColor, GColorClear, s_font_infobig, GTextAlignmentLeft);

	// Bottom Infobar
	if(conf.bottomStyle == 1) editTextLayer(s_layer_bottombar, GRect(SCREENLEFT-4, SCREENTOP+91, 144, 20), conf.displayTextColor, GColorClear, s_font_infosmall, GTextAlignmentCenter);
	else if (conf.bottomStyle == 2) editTextLayer(s_layer_bottombar, GRect(SCREENLEFT-4, SCREENTOP+90, 144, 20), conf.displayTextColor, GColorClear, s_font_infobig, GTextAlignmentCenter);

	// Weather Box layer
	editTextLayer(s_layer_weatherTop, GRect(SCREENLEFT-4, SCREENTOP-29-4, 144, 28), conf.bgTextColor, GColorClear, fonts_get_system_font(FONT_KEY_GOTHIC_14), GTextAlignmentCenter);
	editTextLayer(s_layer_weatherBottom, GRect(SCREENLEFT-4, SCREENTOP-29+139, 144, 28), conf.bgTextColor, GColorClear, fonts_get_system_font(FONT_KEY_GOTHIC_14), GTextAlignmentCenter);	
	text_layer_set_overflow_mode(s_layer_weatherTop, GTextOverflowModeWordWrap);
	text_layer_set_overflow_mode(s_layer_weatherBottom, GTextOverflowModeWordWrap);
		

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Hiding Layers - heap used %d", (int) heap_bytes_used());

	// Sets the layer hidden or visible based on config
	layer_set_hidden(text_layer_get_layer(s_layer_hour), conf.mainTimeStyle == 0 ? true : false);
	layer_set_hidden(text_layer_get_layer(s_layer_second), conf.mainTimeStyle != 3 ? true : false);
	layer_set_hidden(text_layer_get_layer(s_layer_4char), (conf.fourData == 0 && conf.fourDataTap == 0)? true : false);
	layer_set_hidden(text_layer_get_layer(s_layer_infobar_left), conf.infoLeftStyle == 0 ? true : false);
	layer_set_hidden(text_layer_get_layer(s_layer_infobar_right), conf.infoRightStyle == 0 ? true : false);
	layer_set_hidden(text_layer_get_layer(s_layer_bottombar), conf.bottomStyle == 0 ? true : false);

	// Marks bg and toggle layer dirty to force a redraw
	layer_mark_dirty(s_layer_background);
	layer_mark_dirty(s_layer_toggle);

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Adding layers to window - heap used %d", (int) heap_bytes_used());

	// Add text layers
	layer_add_child(window_layer, text_layer_get_layer(s_layer_hour));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_second));
	layer_add_child(window_layer, bitmap_layer_get_layer(s_layer_pm));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_4char));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_infobar_left));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_infobar_right));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_bottombar));

	layer_add_child(window_layer, text_layer_get_layer(s_layer_branding_top));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_branding_bottom));

	layer_add_child(window_layer, text_layer_get_layer(s_layer_weatherTop));
	layer_add_child(window_layer, text_layer_get_layer(s_layer_weatherBottom));

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Handlers - heap used %d", (int) heap_bytes_used());

	// Bluetooth
	connection_service_subscribe((ConnectionHandlers) {.pebble_app_connection_handler = handle_bluetooth});
	handle_bluetooth(bluetooth_connection_service_peek());

	// Battery
	battery_state_service_subscribe(handle_battery);
	handle_battery(battery_state_service_peek());

	#if defined(PBL_HEALTH)
	// Attempt to subscribe
	if (conf.enHealth) {
		if(!health_service_events_subscribe(handle_health, NULL)) APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
		else health_service_set_heart_rate_sample_period(0);
	}
	#endif

	// TAp handler
	accel_tap_service_subscribe(handle_tap);

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Main Window Loaded - heap used %d", (int) heap_bytes_used());
	timeInit();
}

// Ask JS for a weather update
static void get_weather() {
	if (s_jsReady) {
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		dict_write_uint8(iter, 0, 0);
		app_message_outbox_send();
	}
	else APP_LOG(APP_LOG_LEVEL_ERROR, "GetWeather: js connection not ready.");
}

// Save the weather to persistent storage
static void save_weather() {
	int ret = persist_write_data(WEATHER_KEY, &weather, sizeof(weather));
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Config: Persistent Weather Saved: (%d)", ret);
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather: Saved info with timestamp [%lu]", weather.timeStamp);
}

// Load weather from persistent storage and check it's not too old.
static void load_weather() {
	int ret = persist_read_data(WEATHER_KEY, &weather, sizeof(weather));
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Config: Persistent Weather Loaded: (%d)", ret);
	APP_LOG(APP_LOG_LEVEL_INFO, "Weather: loaded (%d), ts: [%lu], timeNow: [%lu], age: [%lu] seconds",ret, weather.timeStamp, time(NULL), time(NULL)-weather.timeStamp);

	if ( ((time(NULL)-weather.timeStamp) > (conf.weatherUpdateRate < 30 ? (conf.weatherUpdateRate*3600)*4 : (conf.weatherUpdateRate*60)*4) ) || weather.provider != conf.weatherProvider) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Weather: data too old, clearing. ");
		//APP_LOG(APP_LOG_LEVEL_INFO, "Weather: (wp: [%d], cwp: [%d], ts: [%lu], timeNow: [%lu], age: [%lu] seconds)", weather.provider, conf.weatherProvider, weather.timeStamp, time(NULL), time(NULL)-weather.timeStamp);
		clear_weather();
	}
}

// Save config settings and issue reloads to displays
static void save_settings() {
	int ret = persist_write_data(SETTINGS_KEY, &conf, sizeof(conf));
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Config: Persistent Settings Saved: (%d)", ret);

	clear_weather();
	get_weather();
	window_stack_pop_all(true);
	window_stack_push(s_main_window, true);
	
}

// Load Config settings
static void load_settings() {
	// Load the default settings
	default_settings();
	// Read settings from persistent storage, if they exist
	int ret = persist_read_data(SETTINGS_KEY, &conf, sizeof(conf));
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Config: Persistent Settings Loaded (%d)", ret);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Colours: %d / %d / %d / %d", conf.displayTextColor, conf.displayColor, conf.displayBorderColor, conf.bgTextColor);
}

// Receive JS Messages from the phone
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Received js Data, Reading");
	
	// Check if it was the jsReady confirmation
	Tuple *t_jsReady = dict_find(iter, MESSAGE_KEY_jsReady);
	if (t_jsReady && !s_jsReady) {
		APP_LOG(APP_LOG_LEVEL_INFO, "jsReady received.");
		s_jsReady = t_jsReady->value->int32 == 1;
	}
	else {	
		// Check for weather data tuples
		
		Tuple *t_weather[17];
		for (int i = 0; i <= 16; i++) {
			t_weather[i] = dict_find(iter, MESSAGE_KEY_jsWeatherData + i);
		}
		
		// weather data is available, save it
		if (t_weather[13] != 0 && t_weather[12])
		{
			APP_LOG(APP_LOG_LEVEL_INFO, "Received Weather data.");

			// Reset the retry
			weather.retryCount = 0;
			weather.retryTimer = 0;
			
			weather.provider = atoi(t_weather[13]->value->cstring);
			snprintf(weather.tempCur, sizeof(weather.tempCur), "%s", t_weather[1]->value->cstring);
			snprintf(weather.tempMin, sizeof(weather.tempMin), "%s", t_weather[2]->value->cstring);
			snprintf(weather.tempMax, sizeof(weather.tempMax), "%s", t_weather[3]->value->cstring);
			//snprintf(weather.windCur, sizeof(weather.windCur), "%s", t_weather[]->value->cstring);
			//snprintf(weather.windMax, sizeof(weather.windMax), "%s", t_weather[]->value->cstring);
			//snprintf(weather.humidity, sizeof(weather.humidity), "%s", t_weather[]->value->cstring);
			snprintf(weather.sunrise, sizeof(weather.sunrise), "%s", t_weather[7]->value->cstring);
			snprintf(weather.sunset, sizeof(weather.sunset), "%s", t_weather[8]->value->cstring);
			snprintf(weather.condMain, sizeof(weather.condMain), "%s", t_weather[15]->value->cstring);
			snprintf(weather.condDesc, sizeof(weather.condDesc), "%s", t_weather[16]->value->cstring);
			snprintf(weather.condForecast, sizeof(weather.condForecast), "%s", t_weather[14]->value->cstring);
			snprintf(weather.location, sizeof(weather.location), "%s", t_weather[12]->value->cstring);			
			weather.timeStamp = time(NULL);
			APP_LOG(APP_LOG_LEVEL_INFO, "Location: %s, Forecast %s, Provider: %d", weather.location, weather.condForecast, weather.provider);
			save_weather();
			update_fourSlot(false);
		}
		else if (t_weather[13] == 0 && t_weather[12]) {
			// Error happened.
			snprintf(weather.location, sizeof(weather.location), "%s", t_weather[13]->value->cstring);
			
			weather.retryTimer = weather.retryTimer + weather.retryCount;
			weather.retryCount++;
			APP_LOG(APP_LOG_LEVEL_ERROR, "Weather Error: %s, Count: %d, Timer: %d",weather.location, weather.retryCount, weather.retryTimer);
			update_fourSlot(false);
		}
		else {
			APP_LOG(APP_LOG_LEVEL_INFO, "Received Config data.");
		 	// Must have been config settings then :)

			// Colour config
			Tuple *t_cConfig[5];
			for (int i = 0; i <= 5; i++) {
				t_cConfig[i] = dict_find(iter, MESSAGE_KEY_Color + i);
			}
			if (t_cConfig[0]) conf.bgColor = GColorFromHEX(t_cConfig[0]->value->int32);
			if (t_cConfig[1]) conf.bgTextColor = GColorFromHEX(t_cConfig[1]->value->int32);
			if (t_cConfig[2]) conf.displayColor = GColorFromHEX(t_cConfig[2]->value->int32);
			if (t_cConfig[3]) conf.displayTextColor = GColorFromHEX(t_cConfig[3]->value->int32);
			if (t_cConfig[4]) conf.displayBorderColor = GColorFromHEX(t_cConfig[4]->value->int32);

			// Weather Config t_wConfig[]
			Tuple *t_wConfig[10];
			for (int i = 0; i <= 10; i++) {
				t_wConfig[i] = dict_find(iter, MESSAGE_KEY_wConf + i);
			}
			if (t_wConfig[0]) conf.weatherProvider = atoi(t_wConfig[0]->value->cstring);
			if (t_wConfig[1]) conf.weatherTempUnit = atoi(t_wConfig[1]->value->cstring);
			if (t_wConfig[2]) conf.weatherWindUnit = atoi(t_wConfig[2]->value->cstring);
			if (t_wConfig[3]) conf.weatherUpdateRate = atoi(t_wConfig[3]->value->cstring);
			if (t_wConfig[4]) conf.weatherDayNight = atoi(t_wConfig[4]->value->cstring);
			if (t_wConfig[5]) conf.weatherNightMorning = atoi(t_wConfig[5]->value->cstring);
			
			if (t_wConfig[6]) conf.weatherBoxTop = atoi(t_wConfig[6]->value->cstring);
			if (t_wConfig[7]) conf.weatherBoxBottom = atoi(t_wConfig[7]->value->cstring);
			if (t_wConfig[8]) conf.weatherBoxTopTap = atoi(t_wConfig[8]->value->cstring);
			if (t_wConfig[9]) conf.weatherBoxBottomTap = atoi(t_wConfig[9]->value->cstring);
			
			// Configuration
			
			// Vibration Settings
			Tuple *t_vConfig[5];
			for (int i = 0; i <= 4; i++) {
				t_vConfig[i] = dict_find(iter, MESSAGE_KEY_VibeConf + i);
			}
			if (t_vConfig[0]) conf.btDCVibe = atoi(t_vConfig[0]->value->cstring);
			if (t_vConfig[1]) conf.btConVibe = atoi(t_vConfig[1]->value->cstring);
			if (t_vConfig[2]) conf.hourVibe = atoi(t_vConfig[2]->value->cstring);
			if (t_vConfig[3]) conf.vibeStrength = t_vConfig[3]->value->int32;
			
			// Other Misc settings
			Tuple *t_miscConfig[10];
			t_miscConfig[0] = dict_find(iter, MESSAGE_KEY_enHealth);			if (t_miscConfig[0]) conf.enHealth = t_miscConfig[0]->value->int32 == 1;
			t_miscConfig[1] = dict_find(iter, MESSAGE_KEY_enWeather);			if (t_miscConfig[1]) conf.enWeather = t_miscConfig[1]->value->int32 == 1;
			t_miscConfig[2] = dict_find(iter, MESSAGE_KEY_TapDuration);		if (t_miscConfig[2]) conf.tapDuration = t_miscConfig[2]->value->int32;

			t_miscConfig[3] = dict_find(iter, MESSAGE_KEY_showZero);			if (t_miscConfig[3]) conf.showZero = t_miscConfig[3]->value->int32 == 1;
			t_miscConfig[4] = dict_find(iter, MESSAGE_KEY_mainTimeStyle);	if (t_miscConfig[4]) conf.mainTimeStyle = atoi(t_miscConfig[4]->value->cstring);
			t_miscConfig[5] = dict_find(iter, MESSAGE_KEY_dateStyle);			if (t_miscConfig[5]) conf.dateStyle = atoi(t_miscConfig[5]->value->cstring);

			t_miscConfig[6] = dict_find(iter, MESSAGE_KEY_FourConf+1);		if (t_miscConfig[6]) conf.fourData = atoi(t_miscConfig[6]->value->cstring);
			t_miscConfig[7] = dict_find(iter, MESSAGE_KEY_FourConf+2);		if (t_miscConfig[7]) conf.fourDataTap = atoi(t_miscConfig[7]->value->cstring);
			t_miscConfig[8] = dict_find(iter, MESSAGE_KEY_FourConf);			if (t_miscConfig[8]) conf.fourCaps = t_miscConfig[8]->value->int32 == 1;

			// InfoBar Settings
			Tuple *t_ibConfig[6];
			for (int i = 0; i <= 5; i++) {
				t_ibConfig[i] = dict_find(iter, MESSAGE_KEY_InfoBarConf + i);
			}
			if (t_ibConfig[0]) conf.infoLeftStyle = atoi(t_ibConfig[0]->value->cstring);
			if (t_ibConfig[1]) conf.infoLeftData = atoi(t_ibConfig[1]->value->cstring);
			if (t_ibConfig[2]) conf.infoLeftDataTap = atoi(t_ibConfig[2]->value->cstring);
			if (t_ibConfig[3]) conf.infoRightStyle = atoi(t_ibConfig[3]->value->cstring);
			if (t_ibConfig[4]) conf.infoRightData = atoi(t_ibConfig[4]->value->cstring);
			if (t_ibConfig[5]) conf.infoRightDataTap = atoi(t_ibConfig[5]->value->cstring);

			// Bottom Bar settings
			Tuple *t_bbConfig[5];
			for (int i = 0; i <= 4; i++) {
				t_bbConfig[i] = dict_find(iter, MESSAGE_KEY_BottomBarConf + i);
			}

			if (t_bbConfig[0]) conf.bottomStyle = atoi(t_bbConfig[0]->value->cstring);
			if (t_bbConfig[1]) conf.bottomLeftData = atoi(t_bbConfig[1]->value->cstring);
			if (t_bbConfig[2]) conf.bottomLeftDataTap = atoi(t_bbConfig[2]->value->cstring);
			if (t_bbConfig[3]) conf.bottomRightData = atoi(t_bbConfig[3]->value->cstring);
			if (t_bbConfig[4]) conf.bottomRightDataTap = atoi(t_bbConfig[4]->value->cstring);

			// Branding settings
			Tuple *t_bConfig[7];
			for (int i = 0; i <= 6; i++) {
				t_bConfig[i] = dict_find(iter, MESSAGE_KEY_bConf + i);
			}
			if (t_bConfig[0]) conf.brandingLogo = atoi(t_bConfig[0]->value->cstring);
			if (t_bConfig[1]) conf.brandingTop = atoi(t_bConfig[1]->value->cstring);
			if (t_bConfig[2]) conf.brandingBottom = atoi(t_bConfig[2]->value->cstring);
			if (t_bConfig[3]) conf.toggleBold = t_bConfig[3]->value->int32 == 1;
			if (t_bConfig[4]) conf.brandingLabel = t_bConfig[4]->value->int32 == 1;
			if (t_bConfig[5]) conf.brandingStyle = atoi(t_bConfig[5]->value->cstring);
			if (t_bConfig[6]) conf.batteryStyle = atoi(t_bConfig[6]->value->cstring);
			
			save_settings();
		}
	}
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! [%d]", reason);}
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! [%d]", reason);}
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");}


static void init() {
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Init - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	
	load_settings();
	load_weather();

	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Opening App Message - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
	// Open AppMessage connection
	app_message_register_inbox_received(inbox_received_handler);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(768, 64);

	// Create main Window element
	if (DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "Main Window Creation - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());

	s_main_window = window_create();
	window_set_background_color(s_main_window, conf.bgColor);
	//window_set_background_color(s_main_window, GColorBlack);
	window_set_window_handlers(s_main_window, (WindowHandlers) { .load = main_window_load, .unload = main_window_unload });
	window_stack_push(s_main_window, true);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Init Done - heap used %d, heap free %d", (int) heap_bytes_used(), (int) heap_bytes_free());
}



// Unload resources. Most likely I have forgotten half of them, oops.
static void main_window_unload(Window *window) {
	// Service unload
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	connection_service_unsubscribe();
	health_service_events_unsubscribe();
	accel_tap_service_unsubscribe();

	// Destroy TextLayer
	text_layer_destroy(s_layer_hour);
	text_layer_destroy(s_layer_second);
	text_layer_destroy(s_layer_4char);
	text_layer_destroy(s_layer_infobar_right);
	text_layer_destroy(s_layer_infobar_left);
	text_layer_destroy(s_layer_bottombar);
	text_layer_destroy(s_layer_branding_top);
	text_layer_destroy(s_layer_branding_bottom);
	text_layer_destroy(s_layer_weatherTop);
	text_layer_destroy(s_layer_weatherBottom);

	// Destroy canvas BG layer
	layer_destroy(s_layer_background);
	layer_destroy(s_layer_toggle);

	// Destroy GBitmap
	gbitmap_destroy(s_bitmap_background);
	gbitmap_destroy(s_bitmap_brand_logo);
	gbitmap_destroy(s_bitmap_brand_top);
	gbitmap_destroy(s_bitmap_brand_bottom);
	gbitmap_destroy(s_bitmap_toggle_label);
	gbitmap_destroy(s_bitmap_toggle_battery);
	gbitmap_destroy(s_bitmap_toggle_enabled);
	gbitmap_destroy(s_bitmap_brand_labels);
	gbitmap_destroy(s_bitmap_brand_bg);
	gbitmap_destroy(s_bitmap_toggle_charge);
	gbitmap_destroy(s_bitmap_toggle_pm);

	gbitmap_destroy(s_bitmap_sheet_branding);
	gbitmap_destroy(s_bitmap_sheet_toggles);
	
	// Destroy BitmapLayer
	bitmap_layer_destroy(s_layer_pm);

	// Unload GFont
	fonts_unload_custom_font(s_font_hour);
	fonts_unload_custom_font(s_font_hour_big);
	fonts_unload_custom_font(s_font_second);
	fonts_unload_custom_font(s_font_4char);
	fonts_unload_custom_font(s_font_infobig);
	fonts_unload_custom_font(s_font_infosmall);
	fonts_unload_custom_font(s_font_bat);
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

// Creates a custom Vibe pattern that emulates a strength setting
void vibes_pwm(int8_t strength, uint16_t duration, bool twice) {
	uint32_t pwm_segments[100];
	strength = strength + 3;
	uint8_t totalSegments = 0;
	if (strength >= 10) {
		if (twice) {
			pwm_segments[0] = duration/2;
			pwm_segments[1] = duration/3;
			pwm_segments[2] = duration/2;
			totalSegments = 3;
		}
		else {
			pwm_segments[0] = duration;
			totalSegments = 1;
		}
	}
	else {
		totalSegments = (int) duration/5;
		uint8_t currentSegment = 0;
		for(int i = 0; i < (int) duration/10; i++) {
			pwm_segments[currentSegment] = strength;
			if ((twice == 1) && (i == totalSegments/4)) pwm_segments[currentSegment+1] = (int) duration/3;
			else pwm_segments[currentSegment+1] = 10 - strength;
			currentSegment = currentSegment + 2;
		}
	}
	vibes_cancel();
	VibePattern pwm_pat = {.durations = pwm_segments, .num_segments = totalSegments};
	vibes_enqueue_custom_pattern(pwm_pat);
}

// Helper that makes a string ALL CAPS
static char *upcase(char *str){for (int i = 0; str[i] != 0; i++) {if (str[i] >= 'a' && str[i] <= 'z') {str[i] -= 0x20;}}return str;}

// Helper function to edit text layers
static void editTextLayer(TextLayer *layer, GRect location, GColor colour, GColor background, GFont font, GTextAlignment alignment) {
	layer_set_frame(text_layer_get_layer(layer), location);
	text_layer_set_text_color(layer, colour);
	text_layer_set_background_color(layer, background);
	text_layer_set_font(layer, font);
	text_layer_set_text_alignment(layer, alignment);
	//return layer;
}

// Helper function to return a properly formatted date string.
static char *getDateString(bool four, unsigned char mode) {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char dateStringBuffer[8] = {};

	const char *datestr[] = {	"%2d%2d",		"%2d%02d",		"%02d%2d",		"%02d%02d",	"%2d-%2d",		"%2d-%02d",		"%02d-%2d",		"%02d-%02d",	};
	
	if (mode == 0) {
		if (conf.dateStyle < 5) snprintf(dateStringBuffer, sizeof(dateStringBuffer),four ? datestr[conf.dateStyle-1] : datestr[conf.dateStyle+3], tick_time->tm_mon+1, tick_time->tm_mday);
		else snprintf(dateStringBuffer, sizeof(dateStringBuffer),four ? datestr[conf.dateStyle-5] : datestr[conf.dateStyle-1], tick_time->tm_mday, tick_time->tm_mon+1);
	}
		else if (mode == 1) {snprintf(dateStringBuffer, sizeof(dateStringBuffer), (conf.dateStyle == 1 || conf.dateStyle == 2 || conf.dateStyle == 5 || conf.dateStyle == 7) ? "%2d":"%02d" ,tick_time->tm_mon+1);}
		else if (mode == 2) {snprintf(dateStringBuffer, sizeof(dateStringBuffer), (conf.dateStyle == 1 || conf.dateStyle == 3 || conf.dateStyle == 5 || conf.dateStyle == 6) ? "%2d":"%02d" ,tick_time->tm_mday);}
	return dateStringBuffer;
}