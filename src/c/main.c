#include <pebble.h>

#define D_NUMBER_OF_IMAGES 29
#define SETTINGS_KEY 0
#define SETTINGS_VERSION_KEY 1
#define D_MAX_NUM_FRAMES 40

static Window *s_main_window;
static Layer *s_window_layer, *s_foreground_layer;
static char s_time_text[6] = "00:00", s_battery_text[5] = "100%", s_date_text[12], s_steps_text[13] = "-", s_temperature_text[8];
static bool is_inverted = false;
static int weather_icon = 0;
static GFont s_gb_font_large, s_gb_font_small;
static BitmapLayer *s_background_layer, *s_battery_layer, *s_weather_layer;
static GBitmap *s_background_bitmap, *s_battery_bitmap, *s_battery_charging_bitmap, *s_battery_full_bitmap, *s_weather_bitmap;
static AppTimer *s_weather_timer;

/************************************************************************* Settings */

typedef struct ClaySettings {
	bool vibrate_on_disconnect;
	bool hourly_vibration;
	bool health_enabled;
	bool weather_enabled;
	bool animate_on_shake;
	int poke_ball;
} ClaySettings;

static ClaySettings settings;

int settings_version = 1;

static void settings_init() {
	settings.vibrate_on_disconnect = true;
	settings.hourly_vibration = false;
	settings.health_enabled = true;
	settings.weather_enabled = true;
	settings.animate_on_shake = true;
	settings.poke_ball = 0;
	persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void save_settings() {
	persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
	persist_write_int(SETTINGS_VERSION_KEY, 1);
}

/************************************************************************* Time Updater */

static void foreground_update_proc(Layer *s_foreground_layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(s_window_layer);
	
	if (is_inverted) {
		graphics_context_set_text_color(ctx, GColorWhite);
	} else {
		graphics_context_set_text_color(ctx, GColorBlack);
	}
	
	#if PBL_RECT
	GSize time_text_bounds = graphics_text_layout_get_content_size(s_time_text, s_gb_font_large, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_time_text, s_gb_font_large, GRect((bounds.size.w - time_text_bounds.w) / 2, bounds.size.h / 2 + time_text_bounds.h, time_text_bounds.w, time_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	GSize date_text_bounds = graphics_text_layout_get_content_size(s_date_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_date_text, s_gb_font_small, GRect((bounds.size.w - date_text_bounds.w) / 2, bounds.size.h / 2 + 2 * time_text_bounds.h + date_text_bounds.h / 2, date_text_bounds.w, date_text_bounds.h),
										GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	GSize battery_text_bounds = graphics_text_layout_get_content_size(s_battery_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft);
	graphics_draw_text(ctx, s_battery_text, s_gb_font_small, GRect(26, 97, battery_text_bounds.w, battery_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
	GSize step_text_bounds = graphics_text_layout_get_content_size(s_steps_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_steps_text, s_gb_font_small, GRect(bounds.size.w / 2 - step_text_bounds.w / 2, bounds.size.h / 2 + 2 * time_text_bounds.h + date_text_bounds.h + battery_text_bounds.h, step_text_bounds.w, step_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); // center of screen + left half of time text - width of text - width of health icon
	GSize temperature_text_bounds = graphics_text_layout_get_content_size(s_temperature_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentRight);
	graphics_draw_text(ctx, s_temperature_text, s_gb_font_small, GRect(122 - 2 - temperature_text_bounds.w, 97, temperature_text_bounds.w, temperature_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
	#elif PBL_ROUND
	GSize time_text_bounds = graphics_text_layout_get_content_size(s_time_text, s_gb_font_large, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_time_text, s_gb_font_large, GRect((bounds.size.w - time_text_bounds.w) / 2, bounds.size.h / 2 + 1.1 * time_text_bounds.h, time_text_bounds.w, time_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	GSize date_text_bounds = graphics_text_layout_get_content_size(s_date_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_date_text, s_gb_font_small, GRect((bounds.size.w - date_text_bounds.w) / 2, bounds.size.h / 2 + 2 * 1.1 * time_text_bounds.h + date_text_bounds.h / 2, date_text_bounds.w, date_text_bounds.h),
										GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	GSize battery_text_bounds = graphics_text_layout_get_content_size(s_battery_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft);
	graphics_draw_text(ctx, s_battery_text, s_gb_font_small, GRect(41, 107, battery_text_bounds.w, battery_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
	GSize step_text_bounds = graphics_text_layout_get_content_size(s_steps_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_steps_text, s_gb_font_small, GRect(bounds.size.w / 2 - step_text_bounds.w / 2, 158, step_text_bounds.w, step_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); // center of screen + left half of time text - width of text - width of health icon
	GSize temperature_text_bounds = graphics_text_layout_get_content_size(s_temperature_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft);
	graphics_draw_text(ctx, s_temperature_text, s_gb_font_small, GRect(144 - 2 - temperature_text_bounds.w, 107, temperature_text_bounds.w, temperature_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
	#endif
}

static void update_time() {
	time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  strftime(s_time_text, sizeof(s_time_text), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
	strftime(s_date_text, sizeof(s_date_text), "%a %e %b", tick_time); // Formatted like Sun 21 May
	
	if (tick_time->tm_min == 0 && settings.hourly_vibration) {
		vibes_short_pulse();
	}
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
	layer_mark_dirty(s_foreground_layer);
}

/************************************************************************* Weather Updater */

static void request_weather() {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	dict_write_uint8(iter, 0, 0);
	
	app_message_outbox_send();
	
	s_weather_timer = app_timer_register(30 * 1000 * SECONDS_PER_MINUTE, request_weather, NULL);
}

static void draw_weather(int icon) {
	if (s_weather_bitmap) {
		gbitmap_destroy(s_weather_bitmap);
	}
	
	switch (icon) {
		case 1:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_SUN_ICON : RESOURCE_ID_WEATHER_SUN_ICON));
			break;
		case 2:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_PARTLY_ICON : RESOURCE_ID_WEATHER_PARTLY_ICON));
			break;
		case 3:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_CLOUD_ICON : RESOURCE_ID_WEATHER_CLOUD_ICON));
			break;
		case 4:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_RAIN_ICON : RESOURCE_ID_WEATHER_RAIN_ICON));
			break;
		case 5:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_THUNDER_ICON : RESOURCE_ID_WEATHER_THUNDER_ICON));
			break;
		case 6:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_SNOW_ICON : RESOURCE_ID_WEATHER_SNOW_ICON));
			break;
		case 7:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_MOON_ICON : RESOURCE_ID_WEATHER_MOON_ICON));
			break;
		case 8:
			s_weather_bitmap = gbitmap_create_with_resource((is_inverted ? RESOURCE_ID_WEATHER_INVERTED_PARTLY_NIGHT_ICON : RESOURCE_ID_WEATHER_PARTLY_NIGHT_ICON));
			break;
		default:
			break;
	}
	
	if (icon != 0) {
		bitmap_layer_set_bitmap(s_weather_layer, s_weather_bitmap);
	}
}

/************************************************************************* Bluetooth Updater */

static void bluetooth_callback(bool connected) {
	if (!connected) {
		if (settings.vibrate_on_disconnect) {
			vibes_long_pulse();
		}
	}
}

static void bluetooth_init() {
	bluetooth_callback(connection_service_peek_pebble_app_connection());
}

/************************************************************************* Battery Updater */

// Update battery icon and percentage
static void battery_handler() {
	BatteryChargeState state = battery_state_service_peek();
	if (!state.is_charging && state.is_plugged) {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_full_bitmap);
		snprintf(s_battery_text, sizeof(s_battery_text), "100");
	} else if (state.is_charging) {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_charging_bitmap);
		snprintf(s_battery_text, sizeof(s_battery_text), "CHG");
	} else {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
		int s_battery_level = state.charge_percent;
		snprintf(s_battery_text, sizeof(s_battery_text), "%d", s_battery_level);
	}
	layer_mark_dirty(s_foreground_layer);
}

/************************************************************************* Health Updater */

#if defined(PBL_HEALTH)
static void set_steps() {
	HealthValue value = health_service_sum_today(HealthMetricStepCount);
	if ((int)value >= 1000) {
		int thousands = value / 1000;
		int hundreds = value % 1000 / 100;
		snprintf(s_steps_text, sizeof(s_steps_text), "%d.%dk steps", thousands, hundreds);
	} else {
		snprintf(s_steps_text, sizeof(s_steps_text), "%d steps", (int)value);
	}
}

// Update step count
static void health_handler(HealthEventType event, void *context) {
	set_steps();
	layer_mark_dirty(s_foreground_layer);
}
#endif

/************************************************************************* Image Updater */

static void set_bitmap(int image_number) {
		switch(image_number) {
			case 0:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_POKE_BALL);
				break;
			case 1:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GREAT_BALL);
				break;
			case 2:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ULTRA_BALL);
				break;
			case 3:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MASTER_BALL);
				break;
			case 4:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SAFARI_BALL);
				break;
			case 5:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LEVEL_BALL);
				break;
			case 6:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUE_LURE_BALL);
				break;
			case 7:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GREEN_LURE_BALL);
				break;
			case 8:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MOON_BALL);
				break;
			case 9:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRIEND_BALL);
				break;
			case 10:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LOVE_BALL);
				break;
			case 11:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEAVY_BALL);
				break;
			case 12:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FAST_BALL);
				break;
			case 13:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPORT_BALL);
				break;
			case 14:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PREMIER_BALL);
				break;
			case 15:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_REPEAT_BALL);
				break;
			case 16:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TIMER_BALL);
				break;
			case 17:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NEST_BALL);
				break;
			case 18:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NET_BALL);
				break;
			case 19:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DIVE_BALL);
				break;
			case 20:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LUXURY_BALL);
				break;
			case 21:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEAL_BALL);
				break;
			case 22:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUICK_BALL);
				break;
			case 23:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DUSK_BALL);
				break;
			case 24:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHERISH_BALL);
				break;
			case 25:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PARK_BALL);
				break;
			case 26:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PARK_ALTERNATE_BALL);
				break;
			case 27:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DREAM_BALL);
				break;
			case 28:
				s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BEAST_BALL);
				break;
			default:
				break;
		}

		if (image_number == 19 || image_number == 20 || image_number == 23 || image_number == 27 || image_number == 28) {
			is_inverted = true;
			s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_INVERTED_ICON);
			s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING_INVERTED_ICON);
			s_battery_full_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL_INVERTED_ICON);
		} else {
			is_inverted = false;
			s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_ICON);
			s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING_ICON);
			s_battery_full_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL_ICON);
		}
}

static void redraw_bitmap(int image_number) {
		gbitmap_destroy(s_background_bitmap);
		gbitmap_destroy(s_battery_bitmap);
		gbitmap_destroy(s_battery_full_bitmap);
		gbitmap_destroy(s_battery_charging_bitmap);
		
		set_bitmap(image_number);
		
		bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
		battery_handler();
		draw_weather(weather_icon);
}

// Change image on shake
static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	if (settings.animate_on_shake) {
		int image_number = rand() % (D_NUMBER_OF_IMAGES);
		redraw_bitmap(image_number);
	}
}

/************************************************************************* UnobstructedArea for Quick View */
static const uint8_t s_offset_percent = 40;

uint8_t relative_pixel(int16_t percent, int16_t max) {
  return (max * percent) / 100;
}

static void move_layers() {
	GRect full_bounds = layer_get_bounds(s_window_layer);
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
	if (!grect_equal(&full_bounds, &bounds)) {
		GRect frame_background = layer_get_frame(bitmap_layer_get_layer(s_background_layer));
		frame_background.origin.y = 0 - relative_pixel(s_offset_percent, bounds.size.h);
		layer_set_frame(bitmap_layer_get_layer(s_background_layer), frame_background);

		GRect frame_foreground = layer_get_frame(s_foreground_layer);
		frame_foreground.origin.y = 0 - relative_pixel(s_offset_percent, bounds.size.h);
		layer_set_frame(s_foreground_layer, frame_foreground);
	} else {
		GRect frame_background = layer_get_frame(bitmap_layer_get_layer(s_background_layer));
		frame_background.origin.y = 0;
		layer_set_frame(bitmap_layer_get_layer(s_background_layer), frame_background);

		GRect frame_foreground = layer_get_frame(s_foreground_layer);
		frame_foreground.origin.y = 0;
		layer_set_frame(s_foreground_layer, frame_foreground);
	}
}

static void prv_unobstructed_change(AnimationProgress progress, void *context) {
	move_layers();
}

/************************************************************************* Setup and Teardown */

static void initialize_ui() {
	GRect bounds = layer_get_bounds(s_window_layer);
	
	s_background_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_background_color(s_background_layer, GColorWhite);
	set_bitmap(settings.poke_ball);
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_background_layer));
	
	#if PBL_RECT
	s_battery_layer = bitmap_layer_create(GRect(13, 99, 11, 6));
	#elif PBL_ROUND
	s_battery_layer = bitmap_layer_create(GRect(28, 109, 11, 6));
	#endif
	bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
	bitmap_layer_set_compositing_mode(s_battery_layer, GCompOpSet);
	layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_battery_layer));
	
	s_foreground_layer = layer_create(bounds);
	layer_set_update_proc(s_foreground_layer, foreground_update_proc);
	layer_add_child(window_get_root_layer(s_main_window), s_foreground_layer);
	
	#if PBL_RECT
	s_weather_layer = bitmap_layer_create(GRect(122, 98, 8, 7));
	#else
	s_weather_layer = bitmap_layer_create(GRect(144, 108, 7, 7));
	#endif
	bitmap_layer_set_compositing_mode(s_weather_layer, GCompOpSet);
	layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_weather_layer));
	
	if (settings.weather_enabled) {
		layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_weather_layer));
		s_weather_timer = app_timer_register(30 * 100 * SECONDS_PER_MINUTE, request_weather, NULL);
	}
	
	move_layers();
}

/************************************************************************* Communication */

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_temperature);
	Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_conditions);
	Tuple *disconnect_enabled_tuple = dict_find(iterator, MESSAGE_KEY_disconnectEnabled);
	
	if (temp_tuple && conditions_tuple) {
		snprintf(s_temperature_text, sizeof(s_temperature_text), "%s", temp_tuple->value->cstring);
		layer_mark_dirty(s_foreground_layer);
		weather_icon = conditions_tuple->value->int8;
		draw_weather(weather_icon);
	} else if (disconnect_enabled_tuple) {
		Tuple *hourly_vibration_tuple = dict_find(iterator, MESSAGE_KEY_hourlyVibrationEnabled);
		Tuple *health_enabled_tuple = dict_find(iterator, MESSAGE_KEY_healthEnabled);
		Tuple *weather_enabled_tuple = dict_find(iterator, MESSAGE_KEY_weatherEnabled);
		Tuple *animate_on_shake_tuple = dict_find(iterator, MESSAGE_KEY_animateOnShake);
		Tuple *poke_ball_tuple = dict_find(iterator, MESSAGE_KEY_pokeBall);
		
		settings.vibrate_on_disconnect = disconnect_enabled_tuple->value->int32 == 1;
		settings.hourly_vibration = hourly_vibration_tuple->value->int32 == 1;
		settings.health_enabled = health_enabled_tuple->value->int32 == 1;
		settings.weather_enabled = weather_enabled_tuple->value->int32 == 1;
		settings.animate_on_shake = animate_on_shake_tuple->value->int32 == 1;
		settings.poke_ball = atoi(poke_ball_tuple->value->cstring);
		
		save_settings();
		
		if (settings.weather_enabled) {
			request_weather();
			layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_weather_layer));
		} else {
			snprintf(s_temperature_text, sizeof(s_temperature_text), " ");
			layer_remove_from_parent(bitmap_layer_get_layer(s_weather_layer));
		}
		
		if (settings.animate_on_shake) {
			accel_tap_service_subscribe(accel_tap_handler);
		} else {
			accel_tap_service_unsubscribe();
		}
		
		#if defined(PBL_HEALTH)
		if (settings.health_enabled) {
			health_service_events_subscribe(health_handler, NULL);
		} else {
			health_service_events_unsubscribe();
			snprintf(s_steps_text, sizeof(s_steps_text), "-");
		}
		#endif
		
		redraw_bitmap(settings.poke_ball);
		
		layer_mark_dirty(s_foreground_layer);
	}
}

/************************************************************************* Main app Logic */

static void main_window_load(Window *window) {
	s_window_layer = window_get_root_layer(window);
	initialize_ui();
	battery_handler();
	bluetooth_init();
	if (settings.health_enabled) {
		set_steps();
	}
	update_time();
	layer_mark_dirty(s_foreground_layer);
}

// Destroy layers upon unloading
static void main_window_unload(Window *window) {
	if (s_weather_bitmap) {
		gbitmap_destroy(s_weather_bitmap);
	}
	
	if (s_weather_layer) {
		bitmap_layer_destroy(s_weather_layer);
	}
	
	gbitmap_destroy(s_background_bitmap);
	bitmap_layer_destroy(s_background_layer);
	
	gbitmap_destroy(s_battery_bitmap);
	gbitmap_destroy(s_battery_full_bitmap);
	gbitmap_destroy(s_battery_charging_bitmap);
	bitmap_layer_destroy(s_battery_layer);
	
	layer_destroy(s_foreground_layer);
	
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	#if defined(PBL_HEALTH)
		health_service_events_unsubscribe();
	#endif
	accel_tap_service_unsubscribe();
	
	app_timer_cancel(s_weather_timer);
}

static void init() {
	app_message_register_inbox_received(inbox_received_callback);
	app_message_open(256, 128);
	
	settings_init();
	
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	s_gb_font_large = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GB_24));
	s_gb_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GB_8));
	
	window_stack_push(s_main_window, true);
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe(battery_handler);
	if (settings.animate_on_shake) {
		accel_tap_service_subscribe(accel_tap_handler);
	}
	connection_service_subscribe((ConnectionHandlers) {
		.pebble_app_connection_handler = bluetooth_callback
	});
	#if defined(PBL_HEALTH)
	if (settings.health_enabled) {
		health_service_events_subscribe(health_handler, NULL);
	}
	#endif
	
	UnobstructedAreaHandlers handlers = {
		.change = prv_unobstructed_change,
	};
	unobstructed_area_service_subscribe(handlers, NULL);
}

static void deinit() {
	fonts_unload_custom_font(s_gb_font_large);
	fonts_unload_custom_font(s_gb_font_small);
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}