# Poké Ball Watchface
A Poké Ball watchface for Pebble, written in C.

Gotta catch 'em all! You'll always have the right Poké Ball on hand so that you can be the Pokémon Master.

- - - - -

#### Important instructions for cloning!

The default OpenWeatherMap key is not kept in this repository. To use your own, create a "keys.js" file in /src/pkjs if local, or add a new JavaScript file called "keys.js" by clicking the "Add new" button next to the App Source in CloudPebble. This file should contain:
```javascript
module.exports = {
	owmKey: "YOUR_KEY_HERE",
};
```
You can get your free API key at [openweathermap.org]().
