module.exports = [
{
	"type": "heading",
	"defaultValue": "W800 / W96H Watchface Configuration"
},
{
	"type": "text",
	"defaultValue": "Made by <a href='http://johnedwa.dy.fi'>Jonne 'JohnEdwa' Kuusela</a> - /u/JohnEdwa"
},
	{
	"type": "section",
	"items": [
	{
		"type": "toggle",
		"id":"changelog",
		"label": "Version 0.6 - Tap for older changelog",
		"defaultValue": false
	},
	{
		"type": "text",
		"id" : "change4",
		"defaultValue": "Version 0.6:  2017-06-17<br> * Added customizable colours.<br> * Added a second Weather Box, Tap support and automatic branding blanking.<br> * Added a lot of new Weather Box data combinations<br> * Made PMkey import the API keys instead of polling every time, eventually reaching a daily limit.<br> * Added 'CASIO' Logo and new top slogans.<br> * Optimized and cleaned the code.<br> * <a href='https://github.com/JohnEdwa/W800'>Github Release.</a><br>"
	},
	{
		"type": "text",
		"id" : "change3",
		"defaultValue": "Version 0.5:  2017-06-15<br> * Added Pebble Master Key (pmkey.xyz) support.<br> * Added Week, Day and Month numbers.<br> * Added Debug/Error display to Weather Box.<br> * Fiddled with the config page order and added some descriptions. <br> * Fixed weather autolocation.<br> * Font Fixes.<br> * Temp fix for sunset/sunrise times (Forces 24 hour mode until I figure it out properly)"
	},
	{
		"type": "text",
		"id" : "change2",
		"defaultValue": "Version 0.4: 2017-06-13<br> * Fixed weather refresh after settings changes."
	},
	{
		"type": "text",
		"id" : "change1",
		"defaultValue": "Version 0.1: 2017-06-13<br> * Initial release for Basalt and Diorite."
	},
	{
		"type": "text",
		"id" : "change0",
		"defaultValue": "Known Issues: <br> * Roman Numeral Vibes are always full strength."
	},		
	]
},
{
	"type": "section",
	"items": [
		{
			"type": "heading",
			"defaultValue": "General Settings:"
		},
		{
			"type": "toggle",
			"messageKey": "enWeather",
			"label": "Enable Weather.",
			"defaultValue": true
		},
		{
			"type": "toggle",
			"messageKey": "enHealth",
			"label": "Enable Health",
			"defaultValue": true
		},
		{
			"type": "slider",
			"messageKey": "TapDuration",
			"defaultValue": 3,
			"label": "Tap/Shake display duration.",
			"min": 0,
			"max": 15,
			"step": 1
		},
		{
			"type": "text",
			"defaultValue": "Vibration Settings:"
		},
		{
			"type": "select",
			"messageKey": "VibeConf[2]",
			"defaultValue": "0",
			"label": "Hourly Vibration",
			"options": [
			{ 
				"label": "none", 
				"value": "0"
			},
			{ 
				"label": "Short",
				"value": "1"
			},
			{ 
				"label": "Medium",
				"value": "2"
			},
			{ 
				"label": "Long", 
				"value": "3"
			},
			{ 
				"label": "Double Short",
				"value": "4"
			},
				{ 
				"label": "Double Medium",
				"value": "5"
			},
			{ 
				"label": "Double Long",
				"value": "6"
			},
			{ 
				"label": "Roman Numeral Vibrations",
				"value": "7"
			},
			]
		},
		{
			"type": "select",
			"messageKey": "VibeConf[0]",
			"defaultValue": "0",
			"label": "Bluetooth Disconnect Vibration",
			"options": [
			{ 
				"label": "none", 
				"value": "0"
			},
			{ 
				"label": "Short",
				"value": "1"
			},
			{ 
				"label": "Medium",
				"value": "2"
			},
			{ 
				"label": "Long", 
				"value": "3"
			},
			{ 
				"label": "Double Short",
				"value": "4"
			},
				{ 
				"label": "Double Medium",
				"value": "5"
			},
			{ 
				"label": "Double Long",
				"value": "6"
			}
			]
		},
		{
			"type": "select",
			"messageKey": "VibeConf[1]",
			"defaultValue": "0",
			"label": "Bluetooth Reconnect Vibration",
			"options": [
			{ 
				"label": "none", 
				"value": "0"
			},
			{ 
				"label": "Short",
				"value": "1"
			},
			{ 
				"label": "Medium",
				"value": "2"
			},
			{ 
				"label": "Long", 
				"value": "3"
			},
			{ 
				"label": "Double Short",
				"value": "4"
			},
				{ 
				"label": "Double Medium",
				"value": "5"
			},
			{ 
				"label": "Double Long",
				"value": "6"
			}
			]
		},
		{
			"type": "slider",
			"messageKey": "VibeConf[3]",
			"defaultValue": 4,
			"label": "Vibe Strength.",
			"min": 1,
			"max": 7,
			"step": 1
		}
	]
},
{
"type": "section",
"items": [
  {
	"type": "heading",
			"defaultValue": "Time and Date:"
  },			
		{
			"type": "toggle",
			"messageKey": "showZero",
			"label": "Show leading zero on time.",
			"defaultValue": true
		},
		{
			"type": "select",
			"messageKey": "mainTimeStyle",
			"defaultValue": "2",
			"label": "Main Time Style",
			"options": [
				{ 
					"label": "Larger",
					"value": "2"
				},
				{ 
					"label": "Smaller", 
					"value": "1"
				},
				{ 
					"label": "Smaller with Seconds",
					"value": "3"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "dateStyle",
			"defaultValue": "2",
			"label": "Date Style",
			"options": [
				{
					"label": "Example: January 6th",
					"value": [
					{ 
						"label": "( m- d)  1- 6", 
						"value": "1"
					},
					{ 
						"label": "( m-dd)  1-06", 
						"value": "2"
					},
					{ 
						"label": "(mm- d) 01- 6", 
						"value": "3"
					},
					{ 
						"label": "(mm-dd) 01-06", 
						"value": "4"
					},
					{ 
						"label": "( d- m)  6- 1", 
						"value": "5"
					},
					{ 
						"label": "( d-mm)  6-01", 
						"value": "6"
					},
					{ 
						"label": "(dd- m) 06- 1", 
						"value": "7"
					},
					{ 
						"label": "(dd-mm) 06-01", 
						"value": "8"
					}
				]
				}
			]
		}
	]
},
// Branding
{
"type": "section",
"items": [
  {
	"type": "heading",
			"defaultValue": "Branding and Visuals:",
  },
		{
			"type": "color",
			"messageKey": "Color[0]",
			"defaultValue": "000000",
			"label": "Background Color",
			"sunlight": false,
			"allowGray": true
		},
		{
			"type": "color",
			"messageKey": "Color[1]",
			"defaultValue": "ffffff",
			"label": "Background Text Color",
			"sunlight": false,
			"allowGray": false
		},
		{
			"type": "color",
			"messageKey": "Color[2]",
			"defaultValue": "ffffff",
			"label": "Display Color",
			"sunlight": false,
			"allowGray": true
		},
		{
			"type": "color",
			"messageKey": "Color[4]",
			"defaultValue": "000000",
			"label": "Display Border Color",
			"sunlight": false,
			"allowGray": true
		},
		{
			"type": "color",
			"messageKey": "Color[3]",
			"defaultValue": "000000",
			"label": "Display Text Color",
			"sunlight": false,
			"allowGray": false
		},
		{
			"type": "select",
			"messageKey": "bConf[5]",
			"defaultValue": "1",
			"label": "Background style",
			"options": [
				{ 
					"label": "W800",
					"value": "1"
				},
				{ 
					"label": "W96H",
					"value": "2"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "bConf[0]",
			"defaultValue": "1",
			"label": "Top Logo",
			"description" : "Overwritten by Top Weather Box",
			"options": [
				{ 
					"label": "Nothing", 
					"value": "0"
				},
				{ 
					"label": "Pebble, Slim",
					"value": "1"
				},
				{ 
					"label": "Pebble, Bold",
					"value": "2"
				},
				{ 
					"label": "Casio",
					"value": "3"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "bConf[1]",
			"defaultValue": "1",
			"label": "Top Slogan",
			"description" : "Overwritten by Top Weather Box",
			"options": [
				{ 
					"label": "Nothing", 
					"value": "0"
				},
				{ 
					"label": "'7 Day Battery'",
					"value": "1"
				},
				{ 
					"label": "'Smartwatch'",
					"value": "2"
				},
				{ 
					"label": "'ALARM CHRONO'",
					"value": "3"
				},
				{ 
					"label": "'Not an iWatch'",
					"value": "4"
				},
				{ 
					"label": "'WATER RESIST 3ATM'",
					"value": "5"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "bConf[2]",
			"defaultValue": "2",
			"label": "Bottom Slogan",
			"description" : "Overwritten by Bottom Weather Box",
			"options": [
				{ 
					"label": "Nothing", 
					"value": "0"
				},
				{ 
					"label": "'Water 30M Resist' #1",
					"value": "1"
				},
				{ 
					"label": "'Water 30M Resist' #2",
					"value": "2"
				},
				{ 
					"label": "'7 DAY BATTERY'",
					"value": "3"
				}
			]
		},
		{
			"type": "toggle",
			"messageKey": "bConf[3]",
			"label": "Bold Bat/Ble/Qtm labels.",
			"defaultValue": true
		},
		{
			"type": "toggle",
			"messageKey": "bConf[4]",
			"label": "Show Light/Next/Prev labels.",
			"defaultValue": true
		}
	]
},

// Word Slot
{
"type": "section",
"items": [
  {
	"type": "heading",
			"defaultValue": "Word slot:"
  },
		{
			"type": "text",
			"defaultValue": "Fits four characters max, and has a wide segment font.",
		},
		{
			"type": "toggle",
			"messageKey": "FourConf[0]",
			"label": "Word Slot All Caps",
			"defaultValue": true
		},
		{
			"type": "select",
			"messageKey": "FourConf[1]",
			"defaultValue": "1",
			"label": "Word Slot Data",
			"options": [
				{ 
					"label": "Empty", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "FourConf[2]",
			"defaultValue": "9",
			"label": "Word Slot Tap Data",
			"options": [
				{ 
					"label": "Disable Tap", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						}
					]
				}
			]
		},
	]
},
// Info Bar
{
"type": "section",
"items": [
  {
	"type": "heading",
			"defaultValue": "Top Data Slots:"
  },
		{
			"type": "text",
			"defaultValue": "Smaller fits 8 characters per slot, larger fits 6.",
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[0]",
			"defaultValue": "1",
			"label": "Left Slot Style",
			"options": [
				{ 
					"label": "Hidden", 
					"value": "0"
				},
				{ 
					"label": "Smaller",
					"value": "1"
				},
				{ 
					"label": "Larger",
					"value": "2"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[3]",
			"defaultValue": "1",
			"label": "Right Slot Style",
			"options": [
				{ 
					"label": "Hidden", 
					"value": "0"
				},
				{ 
					"label": "Smaller",
					"value": "1"
				},
				{ 
					"label": "Larger",
					"value": "2"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[1]",
			"defaultValue": "7",
			"label": "Left Data",
			"options": [
				{ 
					"label": "Empty", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[4]",
			"defaultValue": "8",
			"label": "Right Data",
			"options": [
				{ 
					"label": "Empty", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[2]",
			"defaultValue": "0",
			"label": "Left Tap Data",
			"options": [
				{ 
					"label": "Disable Tap", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "InfoBarConf[5]",
			"defaultValue": "0",
			"label": "Right Tap Data",
			"options": [
				{ 
					"label": "Disable Tap", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
	]
},
// Bottom Bar
{
"type": "section",
"items": [
  {
	"type": "heading",
	"defaultValue": "Bottom Data Slots"
  },
		{
			"type": "text",
			"defaultValue": "Smaller fits 16 characters, larger fits 12.",
		},
		{
			"type": "select",
			"messageKey": "BottomBarConf[0]",
			"defaultValue": "2",
			"label": "Bottom Bar Style",
			"options": [
				{ 
					"label": "Hidden", 
					"value": "0"
				},
				{ 
					"label": "Smaller",
					"value": "1"
				},
				{ 
					"label": "Larger",
					"value": "2"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "BottomBarConf[1]",
			"defaultValue": "4",
			"label": "Left Data",
			"options": [
				{ 
					"label": "Empty", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "BottomBarConf[3]",
			"defaultValue": "3",
			"label": "Right Data",
			"options": [
				{ 
					"label": "Empty", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "BottomBarConf[2]",
			"defaultValue": "0",
			"label": "Left Tap Data",
			"options": [
				{ 
					"label": "Disable Tap", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
		{
			"type": "select",
			"messageKey": "BottomBarConf[4]",
			"defaultValue": "0",
			"label": "Right Tap Data",
			"options": [
				{ 
					"label": "Disable Tap", 
					"value": "0"
				},
				{ 
					"label": "Day Word",
					"value": "1"
				},
				{ 
					"label": "Month Word",
					"value": "2"
				},
				{ 
					"label": "Day Number",
					"value": "16"
				},
				{ 
					"label": "Week Number",
					"value": "17"
				},
				{ 
					"label": "Month Number",
					"value": "18"
				},
				{ 
					"label": "Date Numbers",
					"value": "3"
				},
				{ 
					"label": "Year Number",
					"value": "4"
				},
				{ 
					"label": "Battery w/ %",
					"value": "5"
				},
				{ 
					"label": "Battery w/o %",
					"value": "6"
				},
				{
				"label": "Health",
					"value": [
						{ 
							"label": "Health - Steps",
							"capabilities": ["HEALTH"],
							"value": "7"
						},
						{ 
							"label": "Health - Heartrate",
							"capabilities": ["HEALTH"],
							"value": "8"
						}
					]
				},
				{
				"label": "Weather",
					"value": [
						{ 
						"label": "Temperature, current",
						"value": "9"
						},
						{ 
							"label": "Sunrise Time",
							"value": "12"
						},
						{ 
							"label": "Sunset Time",
							"value": "13"
						},
						{ 
							"label": "Current Condition (set to both)",
							"value": "14"
						},
						{ 
							"label": "Forecast Condition (set to both)",
							"value": "15"
						}
					]
				}
			]
		},
	]
},
// Weather Box
{
	"type": "section",
	"items": [
		{
			"type": "heading",
			"defaultValue": "Weather Box",
		},
		{
			"type": "text",
			"defaultValue": "Replace top or bottom branding with weather information. 'Location' also returns error messages, use them to make sure PMkey, WU & OWM API keys are correct.<br>Note: OWM will fail 'silently' on an invalid location, returning the closest location it geomaps your connection to.",
		},
		{
			"type": "select",
			"messageKey": "wConf[4]",
			"defaultValue": "0",
			"label": "Top data",
			"options": [
				{ 
					"label": "Empty",
					"value": "0"
				},
				{ 
					"label": "Location / Error / Debug",
					"value": "1"
				},
				{ 
					"label": "Temperatures (Low/Current/Max)",
					"value": "2"
				},
				{ 
					"label": "Current Weather",
					"value": "3"
				},
				{ 
					"label": "Forecast Weather",
					"value": "4"
				},
				{ 
					"label": "Sunrise + Sunset Times",
					"value": "5"
				},
				{ 
					"label": "Temps + Location",
					"value": "21"
				},
				{ 
					"label": "Temps + Current Weather",
					"value": "23"
				},
				{ 
					"label": "Temps + Forecast Weather",
					"value": "24"
				},
				{ 
					"label": "Temps + Sunrise + Sunset",
					"value": "25"
				},	
				{ 
					"label": "Location + Temps",
					"value": "12"
				},
				{ 
					"label": "Location + Current Weather",
					"value": "13"
				},
				{ 
					"label": "Location + Forecast Weather",
					"value": "14"
				},
				{ 
					"label": "Location + Sunrise + Sunset",
					"value": "15"
				},
				{ 
					"label": "Current Weather + Forecast Weather",
					"value": "34"
				},
				{ 
					"label": "Forecast Weather + Current Weather",
					"value": "43"
				},
			]
		},
		{
			"type": "select",
			"messageKey": "wConf[5]",
			"defaultValue": "0",
			"label": "Bottom data",
			"options": [
				{ 
					"label": "Empty",
					"value": "0"
				},
				{ 
					"label": "Location / Error / Debug",
					"value": "1"
				},
				{ 
					"label": "Temperatures (Low/Current/Max)",
					"value": "2"
				},
				{ 
					"label": "Current Weather",
					"value": "3"
				},
				{ 
					"label": "Forecast Weather",
					"value": "4"
				},
				{ 
					"label": "Sunrise + Sunset Times",
					"value": "5"
				},
				{ 
					"label": "Temps + Location",
					"value": "21"
				},
				{ 
					"label": "Temps + Current Weather",
					"value": "23"
				},
				{ 
					"label": "Temps + Forecast Weather",
					"value": "24"
				},
				{ 
					"label": "Temps + Sunrise + Sunset",
					"value": "25"
				},	
				{ 
					"label": "Location + Temps",
					"value": "12"
				},
				{ 
					"label": "Location + Current Weather",
					"value": "13"
				},
				{ 
					"label": "Location + Forecast Weather",
					"value": "14"
				},
				{ 
					"label": "Location + Sunrise + Sunset",
					"value": "15"
				},
				{ 
					"label": "Current Weather + Forecast Weather",
					"value": "34"
				},
				{ 
					"label": "Forecast Weather + Current Weather",
					"value": "43"
				},
			]
		},
		{
			"type": "select",
			"messageKey": "wConf[8]",
			"defaultValue": "24",
			"label": "Top Tap data ",
			"options": [
				{ 
					"label": "Disable Tap",
					"value": "0"
				},
				{ 
					"label": "Location / Error / Debug",
					"value": "1"
				},
				{ 
					"label": "Temperatures (Low/Current/Max)",
					"value": "2"
				},
				{ 
					"label": "Current Weather",
					"value": "3"
				},
				{ 
					"label": "Forecast Weather",
					"value": "4"
				},
				{ 
					"label": "Sunrise + Sunset Times",
					"value": "5"
				},
				{ 
					"label": "Temps + Location",
					"value": "21"
				},
				{ 
					"label": "Temps + Current Weather",
					"value": "23"
				},
				{ 
					"label": "Temps + Forecast Weather",
					"value": "24"
				},
				{ 
					"label": "Temps + Sunrise + Sunset",
					"value": "25"
				},	
				{ 
					"label": "Location + Temps",
					"value": "12"
				},
				{ 
					"label": "Location + Current Weather",
					"value": "13"
				},
				{ 
					"label": "Location + Forecast Weather",
					"value": "14"
				},
				{ 
					"label": "Location + Sunrise + Sunset",
					"value": "15"
				},
				{ 
					"label": "Current Weather + Forecast Weather",
					"value": "34"
				},
				{ 
					"label": "Forecast Weather + Current Weather",
					"value": "43"
				},
			]
		},
		{
			"type": "select",
			"messageKey": "wConf[9]",
			"defaultValue": "15",
			"label": "Bottom Tap data",
			"options": [
				{ 
					"label": "Disable Tap",
					"value": "0"
				},
				{ 
					"label": "Location / Error / Debug",
					"value": "1"
				},
				{ 
					"label": "Temperatures (Low/Current/Max)",
					"value": "2"
				},
				{ 
					"label": "Current Weather",
					"value": "3"
				},
				{ 
					"label": "Forecast Weather",
					"value": "4"
				},
				{ 
					"label": "Sunrise + Sunset Times",
					"value": "5"
				},
				{ 
					"label": "Temps + Location",
					"value": "21"
				},
				{ 
					"label": "Temps + Current Weather",
					"value": "23"
				},
				{ 
					"label": "Temps + Forecast Weather",
					"value": "24"
				},
				{ 
					"label": "Temps + Sunrise + Sunset",
					"value": "25"
				},	
				{ 
					"label": "Location + Temps",
					"value": "12"
				},
				{ 
					"label": "Location + Current Weather",
					"value": "13"
				},
				{ 
					"label": "Location + Forecast Weather",
					"value": "14"
				},
				{ 
					"label": "Location + Sunrise + Sunset",
					"value": "15"
				},
				{ 
					"label": "Current Weather + Forecast Weather",
					"value": "34"
				},
				{ 
					"label": "Forecast Weather + Current Weather",
					"value": "43"
				},
			]
		},
]
},	
// Weather Config
{
"type": "section",
"items": [
  {
	"type": "heading",
	"defaultValue": "Weather configuration."
		},
		{
			"type": "select",
			"messageKey": "wConf[0]",
			"defaultValue": "1",
			"label": " Weather info provider",
			"description": "WeatherUnderground requires your own API key.",
			"options": [
				{ 
					"label": "OpenWeatherMap", 
					"value": "1"
				},
				{ 
					"label": "Weather Underground",
					"value": "2"
				}
			]
		},
		{
		"type": "input",
		"messageKey": "wLoc",
		"defaultValue": "",
		"label": "Custom Location String",
		"description" : "Leave blank for Autolocation. This gets passed straight to OWM/WU, so you need to know what you enter here.",
		"attributes": {
			"placeholder": "AutoLocation On",
			"limit": 32,
			}
		},
		{
			"type": "select",
			"messageKey": "wConf[1]",
			"defaultValue": "1",
			"label": "Temperature unit",
			"options": [
				{ 
					"label": "Celcius", 
					"value": "1"
				},
				{ 
					"label": "Fahrenheit",
					"value": "2"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "wConf[2]",
			"defaultValue": "1",
			"label": "Wind unit",
			"options": [
				{ 
					"label": "m/s", 
					"value": "1"
				},
				{ 
					"label": "km/h",
					"value": "2"
				},
				{ 
					"label": "mph",
					"value": "3"
				}
			]
		},
		{
			"type": "select",
			"messageKey": "wConf[3]",
			"defaultValue": "30",
			"label": "Weather update period.",
			"options": [
				{ 
					"label": "15min",
					"value": "15"
				},
				{ 
					"label": "30min",
					"value": "30"
				},
				{ 
					"label": "1 hour",
					"value": "1"
				},
				{ 
					"label": "2 hours",
					"value": "2"
				}
			]
		},
		{
		"type": "text",
		"defaultValue": "Forecast weather data (min/max temp & forecast description) is based on these trigger hours. <br>Note: OWM only has a single daily forecast - tonight is equal to today.",
		},
		{
	"type": "slider",
	"messageKey": "wConf[6]",
	"label": "Today -> Tonight switch hour",
			"defaultValue" : 16,
			"min" : 0,
			"max" : 24				
		},
		{
	"type": "slider",
	"messageKey": "wConf[7]",
	"label": "Tonight -> Tomorrow switch hour",
			"defaultValue" : 22,
			"min" : 0,
			"max" : 24
		},
		{
			"type": "text",
			"id" : "sliderText",
			"defaultValue": "Current Settings : ",
		},
	]
	},
//API keys
{
"type": "section",
"items": [
		{ 
			"type": "heading", 
			"defaultValue": "API Keys." 
		}, 
		{ 
			"type": "text",
			"defaultValue": "You can import missing API keys through Pebble Master Key (pmkey.xyz), or manually write them below.",
		},
		{
		"type": "input",
		"messageKey": "pmkEmail",
		"defaultValue": "",
		"label": "PMKey Email",
			"attributes": {
			"placeholder": "Pebble Master Key Email Address",
			"limit": 16,
			"type": "email",
			}
		},
		{
		"type": "input",
		"messageKey": "pmkPin",
		"defaultValue": "",
		"label": "PMKey PIN",
		"attributes": {
			"placeholder": "Pebble Master Key 5-digit PIN",
			"limit": 5,
			"type": "number",
			}
		},
			{ 
			"type": "text",
			"id" : "pmkeyText",
			"defaultValue": "",
		},
		{
			"type": "button",
			"primary": false,
			"id" : "import",
			"defaultValue": "Import from PMKey",
		},
		{
		"type": "input",
		"messageKey": "keyWU",
		"defaultValue": "",
		"label": "Weather Undeground API Key",
		//"description": "Get yours free from <a href='https://www.wunderground.com/weather/api/d/pricing.html'>Wunderground.com.</a>",
		"attributes": {
			"placeholder": "API key is REQUIRED for WU.",
			"limit": 19,
			}
		},
		{
		"type": "input",
		"messageKey": "keyOWM",
		"defaultValue": "",
		"label": "OpenWeatherMap API Key",
		//"description": "Optional, default key is unlimited.",
		"attributes": {
			"placeholder": "Using default OWM Key.",
			"limit": 39,
			}
		},
		]
	},

	{
	"type": "section",
"items": [
		{
			"type": "submit",
			"defaultValue": "Save Settings"
		},
		{
		"type": "text",
			"defaultValue": "<a rel='license' href='http://creativecommons.org/licenses/by-sa/4.0/'><img alt='Creative Commons License' style='border-width:0' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAAAPCAMAAABEF7i9AAAAllBMVEUAAAD///+rsapERER3d3eIiIjMzMzu7u4iIiKUmZO6v7rKzsoODg4RERFVVVUNDQ0NDg0PEA8zMzNLTEtbXltmZmZydnF9gn2AgICPkI+ZmZmqqqq7u7vFxsXIzMgNDQwZGRkgICAhISEkJSMnKCcuMC4xMzE5Ozk7PTtBQkFCQkJDQ0Nna2eGhoaHh4ezuLLGysbd3d1wVGpAAAAA0klEQVR42q2T1xbCIAyGk0CXdlmto3XvPd7/5QRL1432WHI4XED4+PMHALQHag0JJBlXMNQScx2ibu9PdTnQcY3iErYl6oxbAo/rUrUJA6J5vx0wNNC0gVkMbBPZXtQ8Uf6qpNJwOf0EjmCKto+ce1aSIg9FzTO1/xlYyVeL34FDMC3ZFT+SNXOX6PEsBKm0rIOYPYuGClnsWTxBHtQV1gRhI4UUGJh6EAsPoxdedjUPK+dzT38DDyvNXXYW9wJ43mh4h6ItN8U7Ldv+FN1/WXO8Aed5CQIgC4KiAAAAAElFTkSuQmCC' /></a>&nbsp; &nbsp; &nbsp;<a href='https://github.com/JohnEdwa/W800'><img src='data:image/png;base64,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'></a>&nbsp; &nbsp; &nbsp;<a  href='https://paypal.me/johnedwa'><img alt='Donate with PayPal.me' style='border-width:0' src='data:image/png;base64,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' /></a>"
		},
		{
			"type": "text",
			"defaultValue": "Made by <a href='http://johnedwa.dy.fi'>Jonne 'JohnEdwa' Kuusela</a> - /u/JohnEdwa"
		},
		{
			"type": "text",
			//"defaultValue": "<a rel='license' href='http://creativecommons.org/licenses/by-sa/4.0/'><img alt='Creative Commons License' style='border-width:0' src='https://i.creativecommons.org/l/by-sa/4.0/80x15.png' /></a><br>Thanks to orvivan for the <a href='https://apps.getpebble.com/en_US/application/52b231c2b70e1c159500009b'>91 DUB V4.0</a>, and Pedro for the <a href'https://apps.getpebble.com/en_US/application/540888f892ecf49ebe000106>DB 31</a> watchfaces which were my main inspiration for this project.<br><br><a href='https://www.wunderground.com'><br><img style='width:50%;'src='https://icons.wxug.com/logos/PNG/wundergroundLogo_4c_rev_horz.png'><br>“Powered by Weather Underground”"
			"defaultValue": "Thanks to orviwan for the <a href='https://apps.getpebble.com/en_US/application/52b231c2b70e1c159500009b'>91 DUB V4.0</a>, and Pedro for the <a href='https://apps.getpebble.com/en_US/application/540888f892ecf49ebe000106'>DB 31</a> watchfaces which were my main inspiration for this project, and also the source for a few of the resources used."
		},
		{
			"type": "text",
		"defaultValue": "Weather provided by: <a href='https://openweathermap.org/'><img src='http://openweathermap.org/themes/openweathermap/assets/vendor/owm/img/logo_OpenWeatherMap_orange.svg' style='width:55%'></a><a href='https://www.wunderground.com/'><img style='width:80%' src='https://icons.wxug.com/logos/PNG/wundergroundLogo_4c_rev_horz.png'></a>"
		},
	]
},
];