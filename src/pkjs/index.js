var DEBUG = 1;

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

var defaultOWMkey = '47890866bbb2ccff2ce7017025bd0ebb';
//var defaultWUkey = '2a463e04116b6a6c'

var owmAPIkey = '';
var wuAPIkey = '';

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
		if (settings['wConf[1]'] == 1) return (Math.round( (temp - 273.15)*10 ) / 10).toFixed(0);
		if (settings['wConf[1]'] == 2) return (Math.round( ( (temp * 9/5) - 459.67)*10 ) / 10).toFixed(0);
		if (settings['wConf[1]'] == 3) return (Math.round(temp*10) / 10).toFixed(0);
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
	// Send to Pebble
	Pebble.sendAppMessage
	(d,
		function(e) {if (DEBUG) console.log('Info sent to Pebble successfully!');},
		function(e) {console.error('Error sending info to Pebble!');}
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
		d[keys.jsWeatherData + 12] = location + '';
		d[keys.jsWeatherData + 13] = 0;
		sendMessage(d);
	}
}
	
function getWeather(locationString, autoLocation) {
	console.log("Weather - Sending Requests");
	// Get weather from WU
		if (settings['wConf[0]'] == 2) {
			if (DEBUG) console.log('Weather - Using WU with API key [' + wuAPIkey + ']');
			url = 'https://api.wunderground.com/api/' + wuAPIkey + '/forecast/conditions/astronomy/q/'+ locationString + '.json';
			
			xhrRequest
					 (url, 'GET', 
						function(responseText) {
							console.log("Weather - Got Request, Parsing Current");	

							var json = JSON.parse(responseText);
							if (!('error' in json.response)) {

								time = new Date();
								if (time.getHours() < settings['wConf[6]']) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
								else {forecastNumber = 1; console.log("Weather - Forecast : Tomorrow.");}
								/*
								if (time.getHours() <= settings['wConf[6]']) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
								else if ( (time.getHours() >= settings['wConf[7]']) && (settings['wConf[7]'] >= settings['wConf[6]']) ) {forecastNumber = 2; console.log("Weather - Forecast : Tomorrow.");}
								else {forecastNumber = 1; console.log("Weather - Forecast : Tonight.");}
								*/

								if (settings['wConf[1]'] == 1) {
									tempCurrent = round(0,json.current_observation.temp_c);
									tempMin = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].low.celsius);
									tempMax = round(0,json.forecast.simpleforecast.forecastday[forecastNumber].high.celsius);
								}
								else if (settings['wConf[1]'] == 2) {
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
								/*
								console.log(JSON.stringify(json, null, 4));
								err[keys.jsWeatherData + 12] = 'WU: ' + json.response.error.description + '';
								err[keys.jsWeatherData + 13] = '0';
					 			sendMessage(err);
								*/
							}
						}
					 );
		}
	// Get weather from OWM
		else {
			if (owmAPIkey === '') owmAPIkey = defaultOWMkey;
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
				 if (DEBUG) console.log("Weather - Got Request, Parsing Forecast");
				 var json = JSON.parse(responseText);
				 if (json.cod == "200") {
					 
					 time = new Date();
					 if (time.getHours() < settings['wConf[6]']) {forecastNumber = 0; console.log("Weather - Forecast : Today.");}
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
							if (DEBUG) console.log("Weather - Got Request, Parsing Current");	

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
								/*
								console.log(JSON.stringify(json, null, 4));
								err[keys.jsWeatherData + 12] = 'OWM: ' + json.message + '';
								err[keys.jsWeatherData + 13] = '0';
					 			sendMessage(err);
								*/
							}
						}
					 );
				 } else {
					 	console.error("Weather - Forecast Weather Failure.");
					 	location = 'OWM: ' + json.message + '';
						buildMessage(0);
					 /*
						console.log(JSON.stringify(json, null, 4));
						err[keys.jsWeatherData + 12] = 'OWM: ' + json.message + '';
					  err[keys.jsWeatherData + 13] = '0';
					 	sendMessage(err);
						*/
					}
			 }
			);
		}
}

function locationSuccess(pos) {
  // Construct URL
	var location = null;
	if (settings['wConf[0]'] == 2) { location = pos.coords.latitude + ',' + pos.coords.longitude;	}
	else { location = 'lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude; }
	getWeather(location, 1);
}

function locationError(error) {
	if(error.code == error.PERMISSION_DENIED) {
    console.error('Weather: Location access was denied by the user.');
		err[keys.jsWeatherData + 12] = 'Location Permission Denied.';
  } else {
		console.error('Weather: : (' + error.code + '): ' + error.message);
		err[keys.jsWeatherData + 12] = 'Err: (' + error.code + '): ' + error.message;
  }
	sendMessage(err);
}

function getWeatherSetup() {
	owmAPIkey = settings.keyOWM;
	wuAPIkey = settings.keyWU;
	if (settings.wLoc === null || settings.wLoc === undefined || settings.wLoc === "") {
		navigator.geolocation.getCurrentPosition(locationSuccess,locationError, {timeout: 15000, maximumAge: 60000, enableHighAccuracy: false});
	}	else { getWeather(settings.wLoc, 0); }
	//watchId = navigator.geolocation.watchPosition(locationSuccess, locationError, {timeout: 5000, maximumAge: 0}); // Register to location updates
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
		try {settings = JSON.parse(localStorage.getItem('clay-settings')) || {};
		} catch (ee) { console.error("Could not load clay settings!");}		
    if (DEBUG) console.log('PebbleKit JS ready!');
		
		Pebble.sendAppMessage
		({'jsReady': 1},
			function(e) {if (DEBUG) console.log('jsReady sent to Pebble successfully!');},
			function(e) {console.error('Error sending info to Pebble!');}
		);
		
    // Get the initial weather
		//if (settings.enWeather === true) { getWeatherSetup(); }
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
		try { settings = JSON.parse(localStorage.getItem('clay-settings')) || {};
		} catch (ee) { console.error("Could not load clay settings!");}
		
		console.log('Weather - Update Request received.');
    if (settings.enWeather === true) { getWeatherSetup(); }
  }                     
);