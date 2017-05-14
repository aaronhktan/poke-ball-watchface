#include <pebble.h>

static Window *s_main_window; // Main window
static Layer *s_window_layer, *s_foreground_layer;
static char s_time_text[6] = "00:00", s_battery_text[5] = "100%", s_date_text[12], s_steps_text[6];
static GFont s_gb_font_large, s_gb_font_small;
static BitmapLayer *s_background_layer, *s_battery_layer, *s_health_layer;
static GBitmap *s_background_bitmap, *s_battery_bitmap, *s_battery_charging_bitmap, *s_battery_full_bitmap, *s_health_bitmap;

// Update procedure for foreground layer
static void foreground_update_proc(Layer *s_foreground_layer, GContext *ctx) {
	// Set bounds of window
	GRect bounds = layer_get_bounds(s_window_layer);
	
	// Set colour to black
	graphics_context_set_text_color(ctx, GColorBlack);
	
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
	if (state.is_charging && state.charge_percent == 100) {
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
	
	layer_destroy(s_foreground_layer);
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