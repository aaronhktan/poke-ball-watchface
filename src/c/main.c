#include <pebble.h>

#define D_NUMBER_OF_IMAGES 29

static Window *s_main_window; // Main window
static Layer *s_window_layer, *s_foreground_layer;
static char s_time_text[6] = "00:00", s_battery_text[5] = "100%", s_date_text[12], s_steps_text[6];
static bool is_inverted = false;
static GFont s_gb_font_large, s_gb_font_small;
static BitmapLayer *s_background_layer, *s_battery_layer, *s_health_layer;
static GBitmap *s_background_bitmap, *s_battery_bitmap, *s_battery_charging_bitmap, *s_battery_full_bitmap, *s_health_bitmap;

// Update procedure for foreground layer
static void foreground_update_proc(Layer *s_foreground_layer, GContext *ctx) {
	// Set bounds of window
	GRect bounds = layer_get_bounds(s_window_layer);
	
	// Set colour to legible
	if (is_inverted) {
		graphics_context_set_text_color(ctx, GColorWhite);
	} else {
		graphics_context_set_text_color(ctx, GColorBlack);
	}
	
	// Draw time text
	GSize time_text_bounds = graphics_text_layout_get_content_size("24:00", s_gb_font_large, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_time_text, s_gb_font_large, GRect((bounds.size.w - time_text_bounds.w) / 2, bounds.size.h / 2 + time_text_bounds.h, time_text_bounds.w, time_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	// Draw date text
	GSize date_text_bounds = graphics_text_layout_get_content_size("Fri 29 Jan", s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_date_text, s_gb_font_small, GRect((bounds.size.w - date_text_bounds.w) / 2, bounds.size.h / 2 + 2 * time_text_bounds.h + date_text_bounds.h / 2, date_text_bounds.w, date_text_bounds.h),
										GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	// Draw battery text
	GSize battery_text_bounds = graphics_text_layout_get_content_size("100%", s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft);
	graphics_draw_text(ctx, s_battery_text, s_gb_font_small, GRect((bounds.size.w - time_text_bounds.w) / 2 + 13, bounds.size.h / 2 + 2 * time_text_bounds.h + date_text_bounds.h + battery_text_bounds.h, battery_text_bounds.w, battery_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
	
	// Draw step count
	GSize step_text_bounds = graphics_text_layout_get_content_size(s_steps_text, s_gb_font_small, GRect(0, 0, bounds.size.w, bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft);
	graphics_draw_text(ctx, s_steps_text, s_gb_font_small, GRect(bounds.size.w / 2 + time_text_bounds.w / 2 - step_text_bounds.w - 13, bounds.size.h / 2 + 2 * time_text_bounds.h + date_text_bounds.h + battery_text_bounds.h, step_text_bounds.w, step_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
}

static void update_ui() {
	// Get current time and put into string
	time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  strftime(s_time_text, sizeof(s_time_text), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
	
	// Get current date and put into string
	strftime(s_date_text, sizeof(s_date_text), "%a %e %b", tick_time);
	
	// Redraw the screen
	layer_mark_dirty(s_foreground_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_ui();
}

// Update battery icon and percentage
static void battery_handler() {
	BatteryChargeState state = battery_state_service_peek();
	if (!state.is_charging && state.is_plugged) {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_full_bitmap);
		snprintf(s_battery_text, sizeof(s_battery_text), "-");
	} else if (state.is_charging) {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_charging_bitmap);
		snprintf(s_battery_text, sizeof(s_battery_text), "-");
	} else {
		bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
		int s_battery_level = state.charge_percent;
		snprintf(s_battery_text, sizeof(s_battery_text), "%d", s_battery_level);
	}
	layer_mark_dirty(s_foreground_layer);
}

// Update step count
static void health_handler(HealthEventType event, void *context) {
	HealthValue value = health_service_sum_today(HealthMetricStepCount);
	if ((int)value >= 1000) {
		int thousands = value / 1000;
		int hundreds = value % 1000 / 100;
		snprintf(s_steps_text, sizeof(s_steps_text), "%d.%dk", thousands, hundreds);
	} else {
		snprintf(s_steps_text, sizeof(s_steps_text), "%d", (int)value);
	}
}

// Change image on shake
static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
	int image_number = rand() % (D_NUMBER_OF_IMAGES);
	
	gbitmap_destroy(s_background_bitmap);
	
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
			s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUICK_BALL);
			break;
		case 22:
			s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEAL_BALL);
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
	
	gbitmap_destroy(s_battery_bitmap);
	gbitmap_destroy(s_battery_full_bitmap);
	gbitmap_destroy(s_battery_charging_bitmap);
	gbitmap_destroy(s_health_bitmap);
	
	if (image_number == 19 || image_number == 20 || image_number == 23 || image_number == 27 || image_number == 28) {
		is_inverted = true;
		s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_INVERTED_ICON);
		s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING_INVERTED_ICON);
		s_battery_full_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL_INVERTED_ICON);
		s_health_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_INVERTED_ICON);
	} else {
		is_inverted = false;
		s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_ICON);
		s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING_ICON);
		s_battery_full_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL_ICON);
		s_health_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_ICON);
	}
	
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	battery_handler();
	bitmap_layer_set_bitmap(s_health_layer, s_health_bitmap);
}

static void initialize_ui() {
	GRect bounds = layer_get_bounds(s_window_layer);
	
	// Create background bitmap layer
	s_background_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_background_color(s_background_layer, GColorWhite);
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_POKE_BALL);
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_background_layer));
	
	// Create battery bitmaps
	s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_ICON);
	s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_CHARGING_ICON);
	s_battery_full_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL_ICON);
	
	s_battery_layer = bitmap_layer_create(GRect(13, 150, 11, 6));
	bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
	bitmap_layer_set_compositing_mode(s_battery_layer, GCompOpSet);
	layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_battery_layer));
	
	// Create health bitmap
	s_health_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_ICON);
	
	s_health_layer = bitmap_layer_create(GRect(122, 149, 7, 7));
	bitmap_layer_set_bitmap(s_health_layer, s_health_bitmap);
	bitmap_layer_set_compositing_mode(s_health_layer, GCompOpSet);
	layer_add_child(bitmap_layer_get_layer(s_background_layer), bitmap_layer_get_layer(s_health_layer));
	
	// Create foreground layer, set update procedures, and add to window.
	s_foreground_layer = layer_create(bounds);
	layer_set_update_proc(s_foreground_layer, foreground_update_proc);
	layer_add_child(window_get_root_layer(s_main_window), s_foreground_layer);
}

static void main_window_load(Window *window) {
	s_window_layer = window_get_root_layer(window);
	initialize_ui();
	update_ui();
	battery_handler();
}

// Destroy layers upon unloading
static void main_window_unload(Window *window) {
	gbitmap_destroy(s_background_bitmap);
	bitmap_layer_destroy(s_background_layer);
	
	gbitmap_destroy(s_battery_bitmap);
	gbitmap_destroy(s_battery_full_bitmap);
	gbitmap_destroy(s_battery_charging_bitmap);
	bitmap_layer_destroy(s_battery_layer);
	
	gbitmap_destroy(s_health_bitmap);
	bitmap_layer_destroy(s_health_layer);
	
	layer_destroy(s_foreground_layer);
	
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	health_service_events_unsubscribe();
	accel_tap_service_unsubscribe();
}

static void init() {
	// Create window and set mmethods
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	// Load custom fonts
	s_gb_font_large = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GB_24));
	s_gb_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GB_8));
	
	window_stack_push(s_main_window, true);
	
	// Subscribe to services
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe(battery_handler);
	health_service_events_subscribe(health_handler, NULL);
	accel_tap_service_subscribe(accel_tap_handler);
}

// Destroy main window upon leaving
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