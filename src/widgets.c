// Copyright 2016 Stefan Heule
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <pebble.h>
#include "graphite.h"


////////////////////////////////////////////
//// Complication array
////////////////////////////////////////////

// -- jsalternative
// -- var widgets = [
widget_render_t widgets[] = {
// -- end jsalternative

// -- autogen
// -- ## for key in widgets_idsorted
// --     {{ key["key"] | lower }}, // id {{ key["id"] }}
// -- ## endfor
    widget_empty, // id 0
    widget_weather_cur_temp_icon, // id 1
    widget_weather_cur_temp, // id 2
    widget_weather_cur_icon, // id 3
    widget_weather_low_temp, // id 4
    widget_weather_high_temp, // id 5
    widget_bluetooth_disconly, // id 6
    widget_bluetooth_disconly_alt, // id 7
    widget_bluetooth_yesno, // id 8
    widget_battery_icon, // id 9
    widget_quiet_offonly, // id 10
    widget_quiet, // id 11
    widget_steps_icon, // id 12
    widget_steps, // id 13
    widget_steps_short_icon, // id 14
    widget_steps_short, // id 15
    widget_calories_resting_icon, // id 16
    widget_calories_resting, // id 17
    widget_calories_active_icon, // id 18
    widget_calories_active, // id 19
    widget_calories_all_icon, // id 20
    widget_calories_all, // id 21
    widget_calories_resting_short_icon, // id 22
    widget_calories_resting_short, // id 23
    widget_calories_active_short_icon, // id 24
    widget_calories_active_short, // id 25
    widget_calories_all_short_icon, // id 26
    widget_calories_all_short, // id 27
    widget_ampm, // id 28
    widget_ampm_lower, // id 29
    widget_seconds, // id 30
    widget_day_of_week, // id 31
    widget_battery_text, // id 32
    widget_battery_text2, // id 33
    widget_tz_0, // id 34
    widget_tz_1, // id 35
    widget_tz_2, // id 36
    widget_weather_sunrise_icon0, // id 37
    widget_weather_sunrise_icon1, // id 38
    widget_weather_sunrise_icon2, // id 39
    widget_weather_sunset_icon0, // id 40
    widget_weather_sunset_icon1, // id 41
    widget_weather_sunset_icon2, // id 42
    widget_phone_battery_icon, // id 43
    widget_phone_battery_text, // id 44
    widget_phone_battery_text2, // id 45
    widget_both_battery_icon, // id 46
    widget_both_battery_flipped_icon, // id 47
    widget_both_battery_text, // id 48
    widget_both_battery_flipped_text, // id 49
    widget_both_battery_text2, // id 50
    widget_both_battery_flipped_text2, // id 51
// -- end autogen

// -- jsalternative
// -- ];
};
// -- end jsalternative


////////////////////////////////////////////
//// Complication render implementations
////////////////////////////////////////////

fixed_t widget_tz(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color, uint8_t tz_id, const char* format) {
// -- jsalternative
// --     var dat = moment(new Date()).tz(eval("config_tz_" + tz_id + "_local")).format('YYYY-MM-DD HH:mm');
// --     buffer_1 = strftime(format, new Date(dat));
// --     buffer_1 =
    int8_t dataidx = get_current_tz_idx(&tzinfo.data[tz_id]);
    if (dataidx == -1) return 0;
    time_t adjusted = time(NULL) - tzinfo.data[tz_id].offsets[dataidx] * 60;
    struct tm* t = gmtime(&adjusted);
    strftime(buffer_1, sizeof(buffer_1), format, t);
// -- end jsalternative
    remove_leading_zero(buffer_1, sizeof(buffer_1));
    if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
    return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

// -- autogen
// -- ## for i in range(num_tzs)
// -- fixed_t widget_tz_{{ i }}(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
// --     return widget_tz(fctx, draw, position, align, foreground_color, background_color, {{ i }}, config_tz_{{ i }}_format);
// -- }
// -- ## endfor
fixed_t widget_tz_0(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_tz(fctx, draw, position, align, foreground_color, background_color, 0, config_tz_0_format);
}
fixed_t widget_tz_1(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_tz(fctx, draw, position, align, foreground_color, background_color, 1, config_tz_1_format);
}
fixed_t widget_tz_2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_tz(fctx, draw, position, align, foreground_color, background_color, 2, config_tz_2_format);
}
// -- end autogen

typedef char* (*num_formater_t)(int num, void* data);

fixed_t draw_icon_number_widget(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color, const char* icon, const char* text, bool show_icon, bool flip_order) {
  fixed_t fontsize_icon = (fixed_t)(fontsize_widgets * 31 / 50); // 0.62
  fixed_t w1 = !show_icon ? 0 : string_width(fctx, icon, font_icon, fontsize_icon);
  fixed_t w2 = string_width(fctx, text, font_main, fontsize_widgets);
  fixed_t sep = REM(2);
  fixed_t w = w1 + w2 + sep;
  GTextAlignment a = GTextAlignmentLeft;
  uint8_t color = foreground_color;

  if (draw) {
      fixed_t icon_y = position.y + fontsize_icon*2/5; // 0.4
// -- jsalternative
// -- icon_y += REM(7);
// -- end jsalternative
      fixed_t offset2 = flip_order ? 0 : w1 + sep;
      fixed_t offset1 = flip_order ? w2 + sep : 0;
      if (align == GTextAlignmentCenter) {
          if (w1) {
              FPoint p1 = FPoint(position.x - w / 2 + offset1, icon_y);
              draw_string(fctx, icon, p1, font_icon, color, fontsize_icon, a);
          }
          FPoint p2 = FPoint(position.x - w / 2 + offset2, position.y);
          draw_string(fctx, text, p2, font_main, color, fontsize_widgets, a);
      } else if (align == GTextAlignmentLeft) {
          if (w1) {
              FPoint p1 = FPoint(position.x + offset1, icon_y);
              draw_string(fctx, icon, p1, font_icon, color, fontsize_icon, a);
          }
          FPoint p2 = FPoint(position.x + offset2, position.y);
          draw_string(fctx, text, p2, font_main, color, fontsize_widgets, a);
      } else {
          if (w1) {
              FPoint p1 = FPoint(position.x - w + offset1, icon_y);
              draw_string(fctx, icon, p1, font_icon, color, fontsize_icon, a);
          }
          FPoint p2 = FPoint(position.x - w + offset2, position.y);
          draw_string(fctx, text, p2, font_main, color, fontsize_widgets, a);
      }
  }

  return w;
}

char* format_unitless(int num) {
  snprintf(buffer_1, 10, "%d", num);
  return buffer_1;
}

char* format_thousands(int num) {
  if (num < 1000) {
    return format_unitless(num);
  }
  if (num < 10000) {
    int thousands = num/1000;
    int rest = (num % 1000) / 100;
    if (rest == 0) {
      snprintf(buffer_1, 10, "%dk", thousands);
    } else {
      snprintf(buffer_1, 10, "%d.%dk", thousands, rest);
    }
  } else {
    snprintf(buffer_1, 10, "%dk", num/1000);
  }
  return buffer_1;
}

fixed_t widget_empty(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return 0;
}

fixed_t drawBatText(FContext *fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, bool perc, uint8_t level) {
    snprintf(buffer_1, sizeof(buffer_1), perc ? "%d%%" : "%d", level);
    if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
    return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t drawBatText2(FContext *fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, bool perc, uint8_t level, uint8_t level2) {
    snprintf(buffer_1, sizeof(buffer_1), perc ? "%d%% %d%%" : "%d %d", level, level2);
    if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
    return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t drawBat(FContext *fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color, uint8_t level) {
    // battery logo (not scaled, to allow pixel-aligned rects)
    fixed_t bat_thickness = PIX(1);
    fixed_t bat_gap_thickness = PIX(1);
    fixed_t bat_height = PIX(15);
    fixed_t bat_width = PIX(9);
    fixed_t bat_top = PIX(2);
    fixed_t bat_inner_height = bat_height - 2 * bat_thickness - 2 * bat_gap_thickness;
    fixed_t bat_inner_width = bat_width - 2 * bat_thickness - 2 * bat_gap_thickness;

    if (!draw) return bat_width;

    fixed_t offset = 0;
    if (align == GTextAlignmentCenter) offset = bat_width / 2;
    if (align == GTextAlignmentRight) offset = bat_width;
    FPoint bat_origin = FPoint(FIXED_ROUND(position.x - offset), FIXED_ROUND(
            position.y + (REM(21) - bat_height) / 2));
    // outer rect
    draw_rect(fctx, FRect(bat_origin, FSize(bat_width, bat_height)), foreground_color);
    // inner background rect
    draw_rect(fctx, FRect(FPoint(bat_origin.x + bat_thickness, bat_origin.y + bat_thickness), FSize(bat_width - 2*bat_thickness, bat_height - 2*bat_thickness)), background_color);
    // inner charge rect
    draw_rect(fctx, FRect(FPoint(bat_origin.x + bat_thickness + bat_gap_thickness, bat_origin.y + bat_thickness + bat_gap_thickness + (100 - level) * bat_inner_height / 100), FSize(
            bat_inner_width, level * bat_inner_height / 100)), foreground_color);
    // top of battery
    draw_rect(fctx, FRect(FPoint(bat_origin.x + bat_thickness + bat_gap_thickness, bat_origin.y - bat_top), FSize(bat_inner_width, bat_top)), foreground_color);
    return bat_width;
}

fixed_t widget_battery_text(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return drawBatText(fctx, draw, position, align, foreground_color, true, battery_state_service_peek().charge_percent);
}

fixed_t widget_battery_text2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return drawBatText(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent);
}

fixed_t widget_battery_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return drawBat(fctx, draw, position, align, foreground_color, background_color, battery_state_service_peek().charge_percent);
}

bool showPhoneBattery() {
    bool battery_is_outdated = (time(NULL) - phonebat.timestamp) > (config_phone_battery_expiration * 60);
    bool invalid_bat_level = phonebat.level > 100;
    return !battery_is_outdated && !invalid_bat_level;
}

fixed_t widget_phone_battery_text(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return 0;
    return drawBatText(fctx, draw, position, align, foreground_color, true, phonebat.level);
}

fixed_t widget_phone_battery_text2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return 0;
    return drawBatText(fctx, draw, position, align, foreground_color, false, phonebat.level);
}

fixed_t widget_phone_battery_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return 0;
    return drawBat(fctx, draw, position, align, foreground_color, background_color, phonebat.level);
}

fixed_t widget_both_battery_text(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBatText(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent);
    return drawBatText2(fctx, draw, position, align, foreground_color, true, battery_state_service_peek().charge_percent, phonebat.level);
}

fixed_t widget_both_battery_text2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBatText(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent);
    return drawBatText2(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent, phonebat.level);
}

fixed_t widget_both_battery_flipped_text(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBatText(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent);
    return drawBatText2(fctx, draw, position, align, foreground_color, true, phonebat.level, battery_state_service_peek().charge_percent);
}

fixed_t widget_both_battery_flipped_text2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBatText(fctx, draw, position, align, foreground_color, false, battery_state_service_peek().charge_percent);
    return drawBatText2(fctx, draw, position, align, foreground_color, false, phonebat.level, battery_state_service_peek().charge_percent);
}

fixed_t widget_both_battery_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBat(fctx, draw, position, align, foreground_color, background_color, battery_state_service_peek().charge_percent);

    fixed_t bat_icon_sep = PIX(2);
    int w = PIX(9) * 2 + bat_icon_sep;
    if (!draw) return w;
    if (align == GTextAlignmentCenter) position.x -= w/2;
    if (align == GTextAlignmentRight) position.x -= w;
    drawBat(fctx, draw, position, GTextAlignmentLeft, foreground_color, background_color, battery_state_service_peek().charge_percent);
    position.x += w/2;
    drawBat(fctx, draw, position, GTextAlignmentLeft, foreground_color, background_color, phonebat.level);
    return w;
}

fixed_t widget_both_battery_flipped_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    if (!showPhoneBattery()) return drawBat(fctx, draw, position, align, foreground_color, background_color, battery_state_service_peek().charge_percent);

    fixed_t bat_icon_sep = PIX(2);
    int w = PIX(9) * 2 + bat_icon_sep;
    if (!draw) return w;
    if (align == GTextAlignmentCenter) position.x -= w/2;
    if (align == GTextAlignmentRight) position.x -= w;
    drawBat(fctx, draw, position, GTextAlignmentLeft, foreground_color, background_color, phonebat.level);
    position.x += w/2;
    drawBat(fctx, draw, position, GTextAlignmentLeft, foreground_color, background_color, battery_state_service_peek().charge_percent);
    return w;
}

fixed_t widget_bluetooth_disconly(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  if (!bluetooth_connection_service_peek()) {
    fixed_t fontsize_bt_icon = REM(25);
    if (draw) draw_string(fctx, "H", FPoint(position.x, position.y + REM(11)), font_icon, foreground_color, fontsize_bt_icon, align);
    return string_width(fctx, "H", font_icon, fontsize_widgets);
  }
  return 0;
}

fixed_t widget_bluetooth_disconly_alt(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  if (!bluetooth_connection_service_peek()) {
    fixed_t fontsize_bt_icon = REM(25);
    if (draw) draw_string(fctx, "I", FPoint(position.x, position.y + REM(11)), font_icon, foreground_color, fontsize_bt_icon, align);
    return string_width(fctx, "I", font_icon, fontsize_widgets);
  }
  return 0;
}

fixed_t widget_bluetooth_yesno(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  fixed_t fontsize_bt_icon = REM(25);
  char* icon = "DH";
  if (!bluetooth_connection_service_peek()) {
    icon = "BH";
  }
  if (draw) draw_string(fctx, icon, FPoint(position.x, position.y + REM(11)), font_icon, foreground_color, fontsize_bt_icon, align);
  return string_width(fctx, icon, font_icon, fontsize_bt_icon);
}

fixed_t widget_quiet_offonly(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  if (quiet_time_is_active()) {
    fixed_t fontsize_bt_icon = REM(25);
    if (draw) draw_string(fctx, "F", FPoint(position.x, position.y + REM(11)), font_icon, foreground_color, fontsize_bt_icon, align);
    return string_width(fctx, "F", font_icon, fontsize_bt_icon);
  }
  return 0;
}

fixed_t widget_quiet(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  fixed_t fontsize_bt_icon = REM(25);
  char* icon = "G";
  if (quiet_time_is_active()) {
    icon = "F";
  }
  if (draw) draw_string(fctx, icon, FPoint(position.x, position.y + REM(11)), font_icon, foreground_color, fontsize_bt_icon, align);
  return string_width(fctx, icon, font_icon, fontsize_bt_icon);
}

fixed_t widget_ampm(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  time_t now = time(NULL);
    struct tm *t = localtime(&now);
  strftime(buffer_1, sizeof(buffer_1), "%p", t);
  if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
  return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t widget_ampm_lower(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  time_t now = time(NULL);
    struct tm *t = localtime(&now);
  strftime(buffer_1, sizeof(buffer_1), "%P", t);
  if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
  return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t widget_seconds(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  time_t now = time(NULL);
    struct tm *t = localtime(&now);
  strftime(buffer_1, sizeof(buffer_1), "%S", t);
// -- jsalternative
// --   buffer_1 = 
// -- end jsalternative
  remove_leading_zero(buffer_1, sizeof(buffer_1));
  if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
  return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t widget_day_of_week(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer_1, sizeof(buffer_1), "%a", t);
  if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
  return string_width(fctx, buffer_1, font_main, fontsize_widgets);
}

fixed_t widget_weather_temp(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, int16_t temp) {
  if (show_weather()) {
    if (temp == GRAPHITE_UNKNOWN_WEATHER) return 0;
    if (weather.failed) {
        snprintf(buffer_1, 10, "%d", temp);
    } else {
        snprintf(buffer_1, 10, "%d°", temp);
    }
    if (draw) draw_string(fctx, buffer_1, position, font_main, foreground_color, fontsize_widgets, align);
    return string_width(fctx, buffer_1, font_main, fontsize_widgets);
  }
  return 0;
}

fixed_t widget_weather_cur_temp(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return widget_weather_temp(fctx, draw, position, align, foreground_color, weather.temp_cur);
}

fixed_t widget_weather_cur_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  if (show_weather()) {
    if (weather.temp_cur == GRAPHITE_UNKNOWN_WEATHER) return 0;
    snprintf(buffer_1, 10, "%c", weather.icon);
// -- jsalternative
// -- buffer_2 = "";
      buffer_2[0] = 0;
// -- end jsalternative
    return draw_weather(fctx, draw, buffer_1, buffer_2, position, foreground_color, fontsize_widgets, align, false);
  }
  return 0;
}

fixed_t widget_weather_cur_temp_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  if (show_weather()) {
    if (weather.temp_cur == GRAPHITE_UNKNOWN_WEATHER) return 0;
    snprintf(buffer_1, 10, "%c", weather.icon);
    if (weather.failed) {
        snprintf(buffer_2, 10, "%d", weather.temp_cur);
    } else {
        snprintf(buffer_2, 10, "%d°", weather.temp_cur);
    }
    return draw_weather(fctx, draw, buffer_1, buffer_2, position, foreground_color, fontsize_widgets, align, false);
  }
  return 0;
}

fixed_t widget_weather_low_temp(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return widget_weather_temp(fctx, draw, position, align, foreground_color, weather.temp_low);
}

fixed_t widget_weather_high_temp(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return widget_weather_temp(fctx, draw, position, align, foreground_color, weather.temp_high);
}



fixed_t widget_weather_sunrise_sunset(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, const char* icon, bool flip, time_t time) {
    if (!show_weather_impl(config_weather_sunrise_expiration * 60)) return 0;
    if (weather.sunrise == 0) return 0;

    struct tm *t = localtime(&time);
    strftime(buffer_1, sizeof(buffer_1), config_sunrise_format, t);
// -- jsalternative
// --   buffer_1 =
// -- end jsalternative
    remove_leading_zero(buffer_1, sizeof(buffer_1));
    return draw_weather(fctx, draw, icon, buffer_1, position, foreground_color, fontsize_widgets, align, flip);
}
fixed_t widget_weather_sunrise_icon0(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "", false, weather.sunrise);
}
fixed_t widget_weather_sunrise_icon1(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "a", false, weather.sunrise);
}
fixed_t widget_weather_sunrise_icon2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "a", true, weather.sunrise);
}
fixed_t widget_weather_sunset_icon0(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "", false, weather.sunset);
}
fixed_t widget_weather_sunset_icon1(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "A", false, weather.sunset);
}
fixed_t widget_weather_sunset_icon2(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
    return widget_weather_sunrise_sunset(fctx, draw, position, align, foreground_color, "A", true, weather.sunset);
}



// -- autogen
// -- ## for key in widgets
// -- ## if key["autogen"] == "icon_text"
// -- fixed_t {{ key["key"] | lower }}(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
// --   return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "{{ key["icontext"] }}", {{ key["text"] }}, {{ key["icon"] }}, false);
// -- }
// -- ## endif
// -- 
// -- 
// -- ## endfor
fixed_t widget_steps_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "A", format_unitless(health_service_sum_today(HealthMetricStepCount)), true, false);
}
fixed_t widget_steps(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "A", format_unitless(health_service_sum_today(HealthMetricStepCount)), false, false);
}
fixed_t widget_steps_short_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "A", format_thousands(health_service_sum_today(HealthMetricStepCount)), true, false);
}
fixed_t widget_steps_short(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "A", format_thousands(health_service_sum_today(HealthMetricStepCount)), false, false);
}
fixed_t widget_calories_resting_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricRestingKCalories)), true, false);
}
fixed_t widget_calories_resting(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricRestingKCalories)), false, false);
}
fixed_t widget_calories_active_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricActiveKCalories)), true, false);
}
fixed_t widget_calories_active(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricActiveKCalories)), false, false);
}
fixed_t widget_calories_all_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricRestingKCalories)+health_service_sum_today(HealthMetricActiveKCalories)), true, false);
}
fixed_t widget_calories_all(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_unitless(health_service_sum_today(HealthMetricRestingKCalories)+health_service_sum_today(HealthMetricActiveKCalories)), false, false);
}
fixed_t widget_calories_resting_short_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricRestingKCalories)), true, false);
}
fixed_t widget_calories_resting_short(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricRestingKCalories)), false, false);
}
fixed_t widget_calories_active_short_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricActiveKCalories)), true, false);
}
fixed_t widget_calories_active_short(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricActiveKCalories)), false, false);
}
fixed_t widget_calories_all_short_icon(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricRestingKCalories)+health_service_sum_today(HealthMetricActiveKCalories)), true, false);
}
fixed_t widget_calories_all_short(FContext* fctx, bool draw, FPoint position, GTextAlignment align, uint8_t foreground_color, uint8_t background_color) {
  return draw_icon_number_widget(fctx, draw, position, align, foreground_color, background_color, "K", format_thousands(health_service_sum_today(HealthMetricRestingKCalories)+health_service_sum_today(HealthMetricActiveKCalories)), false, false);
}
// -- end autogen
