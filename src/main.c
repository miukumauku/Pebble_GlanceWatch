#include <pebble.h>

Window *s_window;
TextLayer *s_hour_text_layer;
TextLayer *s_min_text_layer;
TextLayer *s_bt_text_layer;

static void update_bt(BatteryChargeState charge_state) {
  static char s_battery_buffer[4];
  char charging_c = charge_state.is_charging ? '.' : '\0';
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%c", charge_state.charge_percent, charging_c);
  text_layer_set_text(s_bt_text_layer, s_battery_buffer);
}

static void battery_handler(BatteryChargeState charge_state) {
  update_bt(charge_state);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_hour_str[3];
  static char s_min_str[3];
  strftime(s_hour_str, sizeof(s_hour_str), "%H", tick_time);
  strftime(s_min_str, sizeof(s_min_str), "%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_hour_text_layer, s_hour_str);
  text_layer_set_text(s_min_text_layer, s_min_str);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void handle_init(void) {
  s_window = window_create();

  s_bt_text_layer = text_layer_create(GRect(90, 10, 50, 40));
  text_layer_set_text_color(s_bt_text_layer, GColorBlack);
  text_layer_set_text_alignment(s_bt_text_layer, GTextAlignmentRight);
  text_layer_set_font(s_bt_text_layer, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
  text_layer_set_text(s_bt_text_layer, "---");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_bt_text_layer));
  
  s_hour_text_layer = text_layer_create(GRect(95, 50, 50, 40));
  text_layer_set_text_color(s_hour_text_layer, GColorBlack);
  text_layer_set_text_alignment(s_hour_text_layer, GTextAlignmentRight);
  text_layer_set_font(s_hour_text_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text(s_hour_text_layer, "12");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_hour_text_layer));
  
  s_min_text_layer = text_layer_create(GRect(95, 85, 50, 40));
  text_layer_set_text_alignment(s_min_text_layer, GTextAlignmentRight);
  text_layer_set_text_color(s_min_text_layer, GColorBlack);
  text_layer_set_font(s_min_text_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text(s_min_text_layer, "34");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_min_text_layer));
  window_stack_push(s_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register with BatteryStateService
  battery_state_service_subscribe(battery_handler);
  update_time();
  
  BatteryChargeState charge_state = battery_state_service_peek();
  update_bt(charge_state);
}

static void handle_deinit(void) {
  text_layer_destroy(s_bt_text_layer);
  text_layer_destroy(s_hour_text_layer);
  text_layer_destroy(s_min_text_layer);
  window_destroy(s_window);
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
