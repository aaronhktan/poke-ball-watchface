module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
	},
	{
		"type": "text",
		"defaultValue": "This is the settings page for the Poké Ball watchface. You are running version 1.2.",
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "General Settings"
			},
			{
				"type": "toggle",
				"messageKey": "disconnectEnabled",
				"defaultValue": true,
				"label": "Vibrate on Bluetooth disconnect",
			},
			{
				"type": "toggle",
				"messageKey": "hourlyVibrationEnabled",
				"defaultValue": true,
				"label": "Hourly Vibration",
			},
			{
				"type": "toggle",
				"messageKey": "animateOnShake",
				"defaultValue": true,
				"label": "Change Poké Balls on shake",
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE"],
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Health"
			},
			{
				"type": "toggle",
				"messageKey": "healthEnabled",
				"description": "If disabled, step count will not be shown on the watch face.",
				"defaultValue": true,
				"label": "Enable Health",
			},
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Weather"
			},
			{
				"type": "toggle",
				"messageKey": "weatherEnabled",
				"defaultValue": true,
				"label": "Enable Weather",
			},
			{
				"type": "select",
				"messageKey": "weatherProvider",
				"defaultValue": "0",
				"description": "Weather Underground and Dark Sky/Forecast.io require your own API key.",
				"label": "Weather Source",
				"options": [
					{
						"label": "OpenWeatherMap",
						"value": "0"
					},
					{
						"label": "Weather Underground",
						"value": "1"
					},
					{
						"label": "Dark Sky/Forecast.io",
						"value": "2"
					}
				]
			},
			{
				"type": "select",
				"messageKey": "temperatureUnit",
				"defaultValue": "0",
				"label": "Temperature Unit",
				"options": [
					{
						"label": "Celsius",
						"value": "0"
					},
					{
						"label": "Fahrenheit",
						"value": "1"
					}
				]
			},
			{
				"type": "toggle",
				"messageKey": "defaultEnabled",
				"defaultValue": false,
				"description": "If you don't have location services enabled, you can set a default address for which weather should be fetched.",
				"label": "Use set location for weather",
			},
			{
				"type": "input",
				"messageKey": "defaultLocation",
				"label": "Default address",
				"attributes": {
					"placeholder": "e.g. Ottawa, Canada",
				},
				"enable": false,
			},
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "API Keys"
			},
			{
				"type": "input",
				"messageKey": "owmKey",
				"label": "OpenWeatherMap Key",
				"attributes": {
					"placeholder": "Default key is being used"
				}
			},
			{
				"type": "input",
				"messageKey": "wuKey",
				"label": "Weather Underground Key",
				"attributes": {
					"placeholder": "Key required for use"
				}
			},
			{
				"type": "input",
				"messageKey": "dsKey",
				"label": "Dark Sky/Forecast.io Key",
				"attributes": {
					"placeholder": "Key required for use"
				}
			},
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Pebble Master Key"
			},
			{
				"type": "text",
				"defaultValue": "Import your API keys for OpenWeatherMap, Weather Underground, and Dark Sky/Forecast.io from <a href=pmkey.xyz>pmkey.xyz</a> here.",
			},
			{
				"type": "input",
				"messageKey": "pmEmail",
				"label": "Email Address",
				"attributes": {
					"placeholder": "e.g. name@url.com",
					"type": "email"
				},
			},
			{
				"type": "input",
				"messageKey": "pmPIN",
				"label": "PIN",
				"attributes": {
					"placeholder": "e.g. 91365",
					"type": "tel",
					"limit": 5
				},
			},
			{
				"type": "button",
				"primary": false,
				"defaultValue": "Import Keys",
				"id": "importButton"
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Poké Ball Selection"
			},
			{
				"type": "radiogroup",
				"capabilities": ["PLATFORM_BASALT"],
				"messageKey": "pokeBall",
				"options": [
					{ 
						"label": "<p align='center'>Poké Ball</p><img alt='Poké Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/pokeball%7Ebasalt.png'/>", 
						"value": 0
					},
					{ 
						"label": "<p align='center'>Great Ball</p><img alt='Great Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/greatball%7Ebasalt.png'/>", 
						"value": 1 
					},
					{ 
						"label": "<p align='center'>Ultra Ball</p><img alt='Ultra Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/ultraball%7Ebasalt.png'/>", 
						"value": 2 
					},
					{ 
						"label": "<p align='center'>Master Ball</p><img alt='Master Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/masterball%7Ebasalt.png'/>", 
						"value": 3 
					},
					{ 
						"label": "<p align='center'>Safari Ball</p><img alt='Safari Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/safariball%7Ebasalt.png'/>", 
						"value": 4
					},
					{ 
						"label": "<p align='center'>Level Ball</p><img alt='Level Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/levelball%7Ebasalt.png'/>", 
						"value": 5
					},
					{ 
						"label": "<p align='center'>Lure Ball (Blue)</p><img alt='Lure Ball (Blue)' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/bluelureball%7Ebasalt.png'/>", 
						"value": 6 
					},
					{ 
						"label": "<p align='center'>Lure Ball (Green)</p><img alt='Lure Ball (Green)' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/greenlureball%7Ebasalt.png'/>", 
						"value": 7 
					},
					{ 
						"label": "<p align='center'>Moon Ball</p><img alt='Moon Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/moonball%7Ebasalt.png'/>", 
						"value": 8 
					},
					{ 
						"label": "<p align='center'>Friend Ball</p><img alt='Friend Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/friendball%7Ebasalt.png'/>", 
						"value": 9
					},
					{ 
						"label": "<p align='center'>Love Ball</p><img alt='Love Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://github.com/cheeseisdisgusting/poke-ball-watchface/blob/master/resources/images/loveball%7Ebasalt.png'/>", 
						"value": 10 
					},
					{ 
						"label": "<p align='center'>Heavy Ball</p><img alt='Heavy Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://github.com/cheeseisdisgusting/poke-ball-watchface/blob/master/resources/images/heavyball%7Ebasalt.png'/>", 
						"value": 11 
					},
					{ 
						"label": "<p align='center'>Fast Ball</p><img alt='Fast Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/fastball%7Ebasalt.png'/>", 
						"value": 12 
					},
					{ 
						"label": "<p align='center'>Sport Ball</p><img alt='Sport Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/sportball%7Ebasalt.png'/>", 
						"value": 13 
					},
					{ 
						"label": "<p align='center'>Premier Ball</p><img alt='Premier Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/premiereball%7Ebasalt.png'/>", 
						"value": 14 
					},
					{ 
						"label": "<p align='center'>Repeat Ball</p><img alt='Repeat Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/repeatball%7Ebasalt.png'/>", 
						"value": 15 
					},
					{ 
						"label": "<p align='center'>Timer Ball</p><img alt='Timer Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/timerball%7Ebasalt.png'/>", 
						"value": 16
					},
					{ 
						"label": "<p align='center'>Nest Ball</p><img alt='Nest Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/nestball%7Ebasalt.png'/>", 
						"value": 17
					},
					{ 
						"label": "<p align='center'>Net Ball</p><img alt='Net Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/netball%7Ebasalt.png'/>", 
						"value": 18 
					},
					{ 
						"label": "<p align='center'>Dive Ball</p><img alt='Dive Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/diveball%7Ebasalt.png'/>", 
						"value": 19 
					},
					{ 
						"label": "<p align='center'>Luxury Ball</p><img alt='Luxury Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/luxuryball%7Ebasalt.png'/>", 
						"value": 20 
					},
					{ 
						"label": "<p align='center'>Heal Ball</p><img alt='Heal Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/healball%7Ebasalt.png'/>", 
						"value": 21 
					},
					{ 
						"label": "<p align='center'>Quick Ball</p><img alt='Quick Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/quickball%7Ebasalt.png'/>", 
						"value": 22 
					},
					{ 
						"label": "<p align='center'>Dusk Ball</p><img alt='Dusk Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/duskball%7Ebasalt.png'/>", 
						"value": 23 
					},
					{ 
						"label": "<p align='center'>Cherish Ball</p><img alt='Cherish Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/cherishball%7Ebasalt.png'/>", 
						"value": 24 
					},
					{ 
						"label": "<p align='center'>Park Ball</p><img alt='Park Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/parkball%7Ebasalt.png'/>", 
						"value": 25 
					},
					{ 
						"label": "<p align='center'>Park Ball (Alternate)</p><img alt='Park Ball (Alternate)' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/parkball(alternate)%7Ebasalt.png'/>", 
						"value": 26 
					},
					{ 
						"label": "<p align='center'>Dream Ball</p><img alt='Dream Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/dreamball%7Ebasalt.png'/>", 
						"value": 27 
					},
					{ 
						"label": "<p align='center'>Beast Ball</p><img alt='Beast Ball' style='border-radius:5px;margin:1rem' height='84' width='72' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/beastball%7Ebasalt.png'/>", 
						"value": 28 
					},
				]
			},
			{
				"type": "radiogroup",
				"capabilities": ["PLATFORM_CHALK"],
				"messageKey": "pokeBall",
				"options": [
					{ 
						"label": "<p align='center'>Poké Ball</p><img alt='Poké Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/pokeball.png'/>", 
						"value": 0
					},
					{ 
						"label": "<p align='center'>Great Ball</p><img alt='Great Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/greatball%7Echalk.png'/>", 
						"value": 1 
					},
					{ 
						"label": "<p align='center'>Ultra Ball</p><img alt='Ultra Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/ultraball%7Echalk.png'/>", 
						"value": 2 
					},
					{ 
						"label": "<p align='center'>Master Ball</p><img alt='Master Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/masterball%7Echalk.png'/>", 
						"value": 3 
					},
					{ 
						"label": "<p align='center'>Safari Ball</p><img alt='Safari Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/safariball%7Echalk.png'/>", 
						"value": 4
					},
					{ 
						"label": "<p align='center'>Level Ball</p><img alt='Level Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/levelball%7Echalk.png'/>", 
						"value": 5
					},
					{ 
						"label": "<p align='center'>Lure Ball (Blue)</p><img alt='Lure Ball (Blue)' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/bluelureball%7Echalk.png'/>", 
						"value": 6 
					},
					{ 
						"label": "<p align='center'>Lure Ball (Green)</p><img alt='Lure Ball (Green)' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/greenlureball%7Echalk.png'/>", 
						"value": 7 
					},
					{ 
						"label": "<p align='center'>Moon Ball</p><img alt='Moon Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/moonball%7Echalk.png'/>", 
						"value": 8 
					},
					{ 
						"label": "<p align='center'>Friend Ball</p><img alt='Friend Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/friendball%7Echalk.png'/>", 
						"value": 9
					},
					{ 
						"label": "<p align='center'>Love Ball</p><img alt='Love Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/loveball%7Echalk.png'/>", 
						"value": 10 
					},
					{ 
						"label": "<p align='center'>Heavy Ball</p><img alt='Heavy Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/heavyball%7Echalk.png'/>", 
						"value": 11 
					},
					{ 
						"label": "<p align='center'>Fast Ball</p><img alt='Fast Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/fastball%7Echalk.png'/>", 
						"value": 12 
					},
					{ 
						"label": "<p align='center'>Sport Ball</p><img alt='Sport Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/sportball%7Echalk.png'/>", 
						"value": 13 
					},
					{ 
						"label": "<p align='center'>Premier Ball</p><img alt='Premier Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/premiereball%7Echalk.png'/>", 
						"value": 14 
					},
					{ 
						"label": "<p align='center'>Repeat Ball</p><img alt='Repeat Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/repeatball%7Echalk.png'/>", 
						"value": 15 
					},
					{ 
						"label": "<p align='center'>Timer Ball</p><img alt='Timer Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/timerball%7Echalk.png'/>", 
						"value": 16
					},
					{ 
						"label": "<p align='center'>Nest Ball</p><img alt='Nest Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/nestball%7Echalk.png'/>", 
						"value": 17
					},
					{ 
						"label": "<p align='center'>Net Ball</p><img alt='Net Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/netball%7Echalk.png'/>", 
						"value": 18 
					},
					{ 
						"label": "<p align='center'>Dive Ball</p><img alt='Dive Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/diveball%7Echalk.png'/>", 
						"value": 19 
					},
					{ 
						"label": "<p align='center'>Luxury Ball</p><img alt='Luxury Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/luxuryball%7Echalk.png'/>", 
						"value": 20 
					},
					{ 
						"label": "<p align='center'>Heal Ball</p><img alt='Heal Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/healball%7Echalk.png'/>", 
						"value": 21 
					},
					{ 
						"label": "<p align='center'>Quick Ball</p><img alt='Quick Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://github.com/cheeseisdisgusting/poke-ball-watchface/blob/master/resources/images/quickball%7Echalk.png'/>", 
						"value": 22 
					},
					{ 
						"label": "<p align='center'>Dusk Ball</p><img alt='Dusk Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/duskball%7Echalk.png'/>", 
						"value": 23 
					},
					{ 
						"label": "<p align='center'>Cherish Ball</p><img alt='Cherish Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/cherishball%7Echalk.png'/>", 
						"value": 24 
					},
					{ 
						"label": "<p align='center'>Park Ball</p><img alt='Park Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/parkball%7Echalk.png'/>", 
						"value": 25 
					},
					{ 
						"label": "<p align='center'>Park Ball (Alternate)</p><img alt='Park Ball (Alternate)' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/parkball(alternate)%7Echalk.png'/>", 
						"value": 26 
					},
					{ 
						"label": "<p align='center'>Dream Ball</p><img alt='Dream Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/dreamball%7Echalk.png'/>", 
						"value": 27 
					},
					{ 
						"label": "<p align='center'>Beast Ball</p><img alt='Beast Ball' style='border-radius:45px;margin:1rem' height='90 width='90' align='center' src='https://raw.githubusercontent.com/cheeseisdisgusting/poke-ball-watchface/master/resources/images/beastball%7Echalk.png'/>", 
						"value": 28 
					},
				]
			}
		]
	},
	{
		"type": "submit",
		"defaultValue": "Save Settings"
	}
];