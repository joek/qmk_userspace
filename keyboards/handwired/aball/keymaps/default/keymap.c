/* Copyright 2020 Richard Sutherland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum keycodes {
  KC_CYCLE_LAYERS = QK_USER,
};

enum layer_names {
    DEFAULT,
    VOLUMEN,
    RGB_LAYER,
    SHORTCUTS
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [DEFAULT] = LAYOUT(
            MS_BTN1,  MS_BTN3, MS_BTN2,
            KC_CYCLE_LAYERS),
        [VOLUMEN] = LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS,
            KC_CYCLE_LAYERS
        ),
        [RGB_LAYER] = LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS,
            KC_CYCLE_LAYERS
        ),
       [SHORTCUTS] = LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS,
            KC_CYCLE_LAYERS
        )
};


const uint16_t PROGMEM boot_combo[] = {MS_BTN1, MS_BTN3, MS_BTN2, COMBO_END};
combo_t key_combos[] = {
    COMBO(boot_combo, QK_BOOT),
};

#ifdef OLED_ENABLE

char layer_str[10];

bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);


    switch (get_highest_layer(layer_state)) {
        case DEFAULT:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case VOLUMEN:
            oled_write_P(PSTR("Volumen\n"), false);
            break;
        case RGB_LAYER:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case SHORTCUTS:
            oled_write_P(PSTR("Shortcuts\n"),false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }


    return false;
}
#endif



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [DEFAULT] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD)},
    [VOLUMEN] = { ENCODER_CCW_CW(KC_KB_VOLUME_UP, KC_KB_VOLUME_DOWN)},
    [RGB_LAYER] = { ENCODER_CCW_CW(UG_VALU, UG_VALD)},
    [SHORTCUTS] = { ENCODER_CCW_CW(UG_SPDU, UG_SPDD)}
};
#endif


// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   3

// Add the behaviour of this new keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_CYCLE_LAYERS:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) {
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return false;
      }

      uint8_t next_layer = current_layer + 1;
      if (next_layer > LAYER_CYCLE_END) {
          next_layer = LAYER_CYCLE_START;
      }
      layer_move(next_layer);
      return false;

    // Process other keycodes normally
    default:
      return true;
  }
}



// TODO: OLED
// - Annimation
// - km counter
// - scroll wheel counter
