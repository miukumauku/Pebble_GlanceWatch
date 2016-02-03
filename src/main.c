#include <pebble.h>

#define RIGHT_SIDE_KEY 1000

Window *s_window;
TextLayer *s_hour_text_layer;
TextLayer *s_min_text_layer;
TextLayer *s_bt_text_layer;
bool s_draw_right_side;

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

static void draw_time_batteryLevel(void) {
  
  if (s_draw_right_side) {
    s_bt_text_layer = text_layer_create(GRect(90, 10, 50, 40));
    text_layer_set_text_alignment(s_bt_text_layer, GTextAlignmentRight);
  } else {
    s_bt_text_layer = text_layer_create(GRect(5, 10, 50, 40));
    text_layer_set_text_alignment(s_bt_text_layer, GTextAlignmentLeft);
  }
  text_layer_set_text_color(s_bt_text_layer, GColorBlack);
  text_layer_set_font(s_bt_text_layer, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
  text_layer_set_text(s_bt_text_layer, "---");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_bt_text_layer));
  
  if (s_draw_right_side) {
    s_hour_text_layer = text_layer_create(GRect(95, 50, 50, 40));
    text_layer_set_text_alignment(s_hour_text_layer, GTextAlignmentRight);
  } else {
    s_hour_text_layer = text_layer_create(GRect(0, 50, 50, 40));
    text_layer_set_text_alignment(s_hour_text_layer, GTextAlignmentLeft);
  }
  text_layer_set_text_color(s_hour_text_layer, GColorBlack);
  text_layer_set_font(s_hour_text_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text(s_hour_text_layer, "12");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_hour_text_layer));
  
  if (s_draw_right_side) {
    s_min_text_layer = text_layer_create(GRect(95, 85, 50, 40));
    text_layer_set_text_alignment(s_min_text_layer, GTextAlignmentRight);
  } else {
    s_min_text_layer = text_layer_create(GRect(0, 85, 50, 40));
    text_layer_set_text_alignment(s_min_text_layer, GTextAlignmentLeft);
  }
  text_layer_set_text_color(s_min_text_layer, GColorBlack);
  text_layer_set_font(s_min_text_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text(s_min_text_layer, "34");
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_min_text_layer));
}

static void change_side_time_batteryLevel(void) {
  if (s_draw_right_side) {
    layer_set_frame(text_layer_get_layer(s_bt_text_layer), GRect(90, 10, 50, 40));
    text_layer_set_text_alignment(s_bt_text_layer, GTextAlignmentRight);
    layer_set_frame(text_layer_get_layer(s_hour_text_layer), GRect(95, 50, 50, 40));
    text_layer_set_text_alignment(s_hour_text_layer, GTextAlignmentRight);
    layer_set_frame(text_layer_get_layer(s_min_text_layer), GRect(95, 85, 50, 40));
    text_layer_set_text_alignment(s_min_text_layer, GTextAlignmentRight);
  } else {
    layer_set_frame(text_layer_get_layer(s_bt_text_layer), GRect(5, 10, 50, 40));
    text_layer_set_text_alignment(s_bt_text_layer, GTextAlignmentLeft);
    layer_set_frame(text_layer_get_layer(s_hour_text_layer), GRect(0, 50, 50, 40));
    text_layer_set_text_alignment(s_hour_text_layer, GTextAlignmentLeft);
    layer_set_frame(text_layer_get_layer(s_min_text_layer), GRect(0, 85, 50, 40));
    text_layer_set_text_alignment(s_min_text_layer, GTextAlignmentLeft);
  }
  //layer_mark_dirty(&timeLayer.layer);
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_text_layer, "Select");
  s_draw_right_side = !s_draw_right_side;
  change_side_time_batteryLevel();
}

#if 0
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_text_layer, "Up");
  //layer_set_frame(text_layer_get_layer(s_text_layer), (GRect){.origin = {0, 10}, .size = {144, 120}});
  //text_layer_set_text(s_text_layer, "    0123456789ABCDEF\n -------------------\n 3x:\u2d30\u2d31\u2d32\u2d33\u2d34\u2d35\u2d36\u2d37\u2d38\u2d39\u2d3a\u2d3b\u2d3c\u2d3d\u2d3e\u2d3f\n 4x:\u2d40\u2d41\u2d42\u2d43\u2d44\u2d45\u2d46\u2d47\u2d48\u2d49\u2d4a\u2d4b\u2d4c\u2d4d\u2d4e\u2d4f\n 5x:\u2d50\u2d51\u2d52\u2d53\u2d54\u2d55\u2d56\u2d57\u2d58\u2d59\u2d5a\u2d5b\u2d5c\u2d5d\u2d5e\u2d5f\n 6x:\u2d60\u2d61\u2d62\u2d63\u2d64\u2d65\u2d66\u2d67\u2d68\u2d69\u2d6a\u2d6b\u2d6c\u2d6d\u2d6e\u2d6f\n\n\u2d4f\u2d7f\u2d4f");
  text_layer_set_text(s_text_layer, "\u2d30\u2d31\u2d32\u2d33\u2d34\u2d35\u2d36\u2d37\n\u2d38\u2d39\u2d3a\u2d3b\u2d3c\u2d3d\u2d3e\u2d3f\n\u2d40\u2d41\u2d42\u2d43\u2d44\u2d45\u2d46\u2d47\n\u2d48\u2d49\u2d4a\u2d4b\u2d4c\u2d4d\u2d4e\u2d4f\n\u2d50\u2d51\u2d52\u2d53\u2d54\u2d55\u2d56\u2d57\n\u2d58\u2d59\u2d5a\u2d5b\u2d5c\u2d5d\u2d5e\u2d5f\n\u2d60\u2d61\u2d62\u2d63\u2d64\u2d65\u2d66\u2d67\n\u2d68\u2d69\u2d6a\u2d6b\u2d6c\u2d6d\u2d6e\u2d6f\n\n\u2d4f\u2d7f\u2d4f");
  text_layer_set_font(s_text_layer, s_custom_font2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_text_layer, "Down");
  //layer_set_frame(text_layer_get_layer(s_text_layer), (GRect){.origin = {0, 30}, .size = {144, 120}});
  window_stack_pop_all(true);
}
#endif

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
#if 0
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
#endif
}

static void handle_init(void) {
  if (persist_exists(RIGHT_SIDE_KEY)) {
    s_draw_right_side = persist_read_bool(RIGHT_SIDE_KEY);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "read: %d", s_draw_right_side);
  } else {
    s_draw_right_side = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "not read: %d", s_draw_right_side);
  }
  
  s_window = window_create();

  window_set_click_config_provider(s_window, click_config_provider);
  
  draw_time_batteryLevel();
  
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
  persist_write_bool(RIGHT_SIDE_KEY, s_draw_right_side);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "write: %d", s_draw_right_side);
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
