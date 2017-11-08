var DEBUG = 0;

var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customClay = require('./custom-clay');
var clay = new Clay(clayConfig,customClay);


var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

var keys = require('message_keys');

var settings = {};
var err = {};

var owmDefaultKey = '47890866bbb2ccff2ce7017025bd0ebb';

var provider = 1;
var owmAPIkey = null;
var wuAPIkey = null;
var locationString = null;
var tempUnit = 1;
var forecastTime = 19;


var url = null;	
var tempCurrent = null;
var tempMin = null;
var tempMax = null;
var sunrise = null;
var sunset = null;	
var condMain = null;
var condDesc = null;
var condForecast = null;	
var location = null;	
var forecastNumber = 0;
var time = null;

function round(acc, vars) { return (Math.round( (vars)*10 ) / 10).toFixed(acc);}

function tempTo(temp) {
		if (tempUnit == 1) return (Math.round( (temp - 273.15)*10 ) / 10).toFixed(0);
		if (tempUnit == 2) return (Math.round( ( (temp * 9/5) - 459.67)*10 ) / 10).toFixed(0);
		//if (tempUnit == 3) return (Math.round(temp*10) / 10).toFixed(0);
		else return temp;
}

function timeToUnix(hour, minute) {
	var time = new Date();
	time.setHours(hour);
	time.setMinutes(minute);
	return time;
}

function getSunTime(timeIn) {
	//console.log('Weather - getSunTime in [' + timeIn + ']');
	var time = new Date(timeIn);
	var hours = time.getHours();
	var minutes = time.getMinutes();
	
	var date = new Date(Date.UTC(2012, 11, 12, 3, 0, 0));
	var dateString = date.toLocaleTimeString();
	if (dateString.match(/am|pm/i) || date.toString().match(/am|pm/i) )
	{
		var ampm = hours >= 12 ? 'P' : 'A';
  	hours = hours % 12;
  	hours = hours ? hours : 12; // the hour '0' should be '12'
  	minutes = minutes < 10 ? '0'+minutes : minutes;
		return hours <= 9 ? (hours + ':' + minutes + '' + ampm) : (hours + ':' + minutes);
	}
	else {
		hours = hours < 10 ? '0'+hours : hours;
		minutes = minutes < 10 ? '0'+minutes : minutes;
		return hours + ':' + minutes;
	}	
}


function sendMessage(d) {
	if (DEBUG) console.log('Weather - Sending Message!');
	// Send to Pebble
	Pebble.sendAppMessage
	(d,
		function(e) {if (DEBUG) console.log('Weather - Message sent to Pebble successfully!');},
		function(e) {console.error('Weather - Error sending message to Pebble!');}
	);
}

function buildMessage(provider) {
	var d = {};
	if (provider != 0) {
		try {
			d[keys.jsWeatherData + 1] = tempCurrent +'';
			d[keys.jsWeatherData + 2] = tempMin +'';
			d[keys.jsWeatherData + 3] = tempMax + '';
			d[keys.jsWeatherData + 7] = sunrise + '';
			d[keys.jsWeatherData + 8] = sunset + '';
			d[keys.jsWeatherData + 12] = location + '';
			d[keys.jsWeatherData + 13] = provider + '';
			d[keys.jsWeatherData + 14] = condForecast + '';
			d[keys.jsWeatherData + 15] = condMain + '';
			d[keys.jsWeatherData + 16] = condDesc + '';

			sendMessage(d);
		} catch (e) {console.error('buildMessage: Failed!');}
	}
	else {
		if (DEBUG) console.log('Weather - Building Error Message');
		d[keys.jsWeatherData + 12] = location + '';
		d[keys.jsWeatherData + 13] = 0 + '';
		sendMessage(d);
	}
}
	
function getWeather(locationString, autoLocation) {
	console.log("Weather - Sending weather requests.");
	
	// Get weather from WU		
	if (settings['wConf[0]'] == 2) {
		try {
			wuAPIkey = settings.keyWU;
			if (DEBUG) console.log('Weather - Using WU with API key [' + wuAPIkey + ']');
			url = 'https://api.wunderground.com/api/' + wuAPIkey + '/forecast/conditions/astronomy/q/'+ locationString + '.json';
			
			xhrRequest
					 (url, 'GET', 
						function(responseText) {
							if (DEBUG)  console.log("Weather - Got response, parsing weather.");	

							var json = JSON.parse(responseText);
							if (!('error' in json.response)) {

								time = new Date();
								if (time.getHours() < forecastTime) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
								else {forecastNumber = 1; console.log("Weather - Forecast : Tomorrow.");}
								/*
								if (time.getHours() <= settings['wConf[6]']) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
								else if ( (time.getHours() >= settings['wConf[7]']) && (settings['wConf[7]'] >= settings['wConf[6]']) ) {forecastNumber = 2; console.log("Weather - Forecast : Tomorrow.");}
								else {forecastNumber = 1; console.log("Weather - Forecast : Tonight.");}
								*/

								if (tempUnit == 1) {
									tempCurrent = round(0,json.current_observation.temp_c);
									tempMin = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].low.celsius);
									tempMax = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].high.celsius);
								}
								else if (tempUnit == 2) {
									tempCurrent = round(0,json.current_observation.temp_f);
									tempMin = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].low.fahrenheit);
									tempMax = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].high.fahrenheit);
								}

								condMain = json.current_observation.weather;						
								condDesc = json.current_observation.weather;
								condForecast = json.forecast.simpleforecast.forecastday[forecastNumber].conditions;
								location = json.current_observation.display_location.city;
								sunrise = getSunTime(timeToUnix(json.sun_phase.sunrise.hour, json.sun_phase.sunrise.minute));
								sunset =  getSunTime(timeToUnix(json.sun_phase.sunset.hour, json.sun_phase.sunset.minute));

								buildMessage(2);
							}
							else {
								console.error("Weather - Current Weather Failure.");
								location = 'WU: ' + json.response.error.description + '';
								buildMessage(0);
							}
						}
					 );
			}
			catch(e) {				
				if (wuAPIkey == null) location = ('WU API key error');
				else location = 'WU error';
				console.error("Weather - WU Weather Failure: " + location);
				buildMessage(0); 
				}
			}
	// Get weather from OWM
		else {
			if (owmAPIkey === null || owmAPIkey === undefined || owmAPIkey === "") owmAPIkey = owmDefaultKey;
			if (DEBUG) console.log('Weather - Using OWM with API key [' + owmAPIkey + ']');
			//url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + owmAPIkey;
			var urlForecast;
			var urlCurrent;
			if (autoLocation) {
				urlForecast = 'https://api.openweathermap.org/data/2.5/forecast/daily?' + locationString + '&appid=' + owmAPIkey + '&cnt=2';
				urlCurrent = 'https://api.openweathermap.org/data/2.5/weather?' + locationString + '&appid=' + owmAPIkey;
			} else {
				urlForecast = 'https://api.openweathermap.org/data/2.5/forecast/daily?q=' + locationString + '&appid=' + owmAPIkey + '&cnt=2';
				urlCurrent = 'https://api.openweathermap.org/data/2.5/weather?q=' + locationString + '&appid=' + owmAPIkey;
			}

			if (DEBUG) console.log(urlForecast); console.log(urlCurrent);

			xhrRequest
			(urlForecast, 'GET', 
			 function(responseText) {
				 if (DEBUG) console.log("Weather - Got response, pParsing forecast");
				 var json = JSON.parse(responseText);
				 if (json.cod == "200") {
					 
					 time = new Date();
					 if (time.getHours() < forecastTime) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
						else {forecastNumber = 1; console.log("Weather - Forecast : Tomorrow.");}
					 /*
					 if ( (time.getHours() >= settings['wConf[7]']) && (settings['wConf[7]'] >= settings['wConf[6]']) ) {forecastNumber = 1; console.log("Weather - Forecast : Tomorrow.");}
					 else {forecastNumber = 0; console.log("Weather - Forecast : Today");}
					 */
					 
					 //tempCurrent = tempTo(json.list[0].temp.day);
					 tempMin = tempTo(json.list[forecastNumber].temp.min);
					 tempMax = tempTo(json.list[forecastNumber].temp.max);				
					 condForecast = json.list[forecastNumber].weather[0].description;

					 xhrRequest
					 (urlCurrent, 'GET', 
						function(responseText) {
							if (DEBUG) console.log("Weather - Got responce, parsing current");	

							var json = JSON.parse(responseText);
							if (json.cod == "200") {

								tempCurrent = tempTo(json.main.temp,1);
								condMain = json.weather[0].main;
								condDesc = json.weather[0].description;
								sunrise = getSunTime(json.sys.sunrise*1000);
								sunset = getSunTime(json.sys.sunset*1000);
								location = json.name;

								buildMessage(1);
								//sendMessage(d);
							} else {
								console.error("Weather - Current Weather Failure.");
								location = 'OWM: ' + json.message + '';
								buildMessage(0);
							}
						}
					 );
				 } else {
					 	console.error("Weather - Forecast Weather Failure.");
					 	location = 'OWM: ' + json.message + '';
						buildMessage(0);
					}
			 }
			);
		}
}

function locationSuccess(pos) {
  // Construct URL
	var location = null;
	if (provider == 2) { location = pos.coords.latitude + ',' + pos.coords.longitude;	}
	else { location = 'lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude; }
	getWeather(location, 1);
}

function locationError(error) {
	if(error.code == error.PERMISSION_DENIED) {
    console.error('Weather: Location access was denied by the user.');
		location = 'Location Permission Denied.';
  } else {
		console.error('Weather: : (' + error.code + '): ' + error.message);
		location = 'Err: ' + error.message;
  }
	buildMessage(0);
}

function getWeatherSetup() {
	console.log('Weather - Determening location...');
	if (locationString === null || locationString === undefined || locationString === "") {
		navigator.geolocation.getCurrentPosition(locationSuccess,locationError, {timeout: 45000, maximumAge: 120000, enableHighAccuracy: false});
	}	else { getWeather(locationString, 0); }
	//watchId = navigator.geolocation.watchPosition(locationSuccess, locationError, {timeout: 5000, maximumAge: 0}); // Register to location updates
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if (DEBUG) console.log('PebbleKit JS ready!');	
		
		Pebble.sendAppMessage
		({'jsReady': 1},
			function(e) {if (DEBUG) console.log('jsReady sent to Pebble successfully!');},
			function(e) {console.error('Error sending jsReady to Pebble!');}
		);
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
		if (JSON.parse(localStorage.getItem('clay-settings')) != null){
			try {
				settings = JSON.parse(localStorage.getItem('clay-settings'));
				try { provider = settings['wConf[0]']; } catch (ee) {console.error('Weather - Clay provider missing');}			
				try { owmAPIkey = settings.keyOWM; } catch (ee) {console.error('Weather - Clay OWMkey missing');}		
				try { wuAPIkey = settings.keyWU; } catch (ee) {console.error('Weather - Clay WUKey missing');}			
				try { tempUnit = settings['wConf[1]']; } catch (ee) {console.error('Weather - Clay TempUnit missing');}			
				try { forecastTime = settings['wConf[4]']; } catch (ee) {console.error('Weather - Clay forecastTime missing');}
				try { locationString = settings.wLoc; } catch (ee) {console.error('Weather - Clay location missing');}
				console.log('Weather - Clay settings loaded.');
			} catch (ee) {}			
		}
		
		console.log('Weather - Update Request received.');
    getWeatherSetup();
  }                     
);