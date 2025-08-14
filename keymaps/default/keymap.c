#include QMK_KEYBOARD_H
#include "process_tap_dance.h"

enum layer_names {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NAV,
	_NUM,
};

enum {
    TD_LSFT_CAPS,
    TD_TLD_GRV,
};

// --- State tracking variables ---
bool is_alt_tab_active = false;

// --- Custom keycodes ---
enum custom_keycodes {
    NAV_FWD = SAFE_RANGE, // alt-tab forward
    NAV_BACK               // alt-tab backward
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)
#define NAV MO(_NAV)
#define UNLOCK LALT(LCTL(KC_DEL))
#define TAB_RT LCTL(KC_TAB)
#define TAB_LT LCTL(LSFT(KC_TAB))
#define TAB_NW LCTL(KC_T)
#define TAB_CL LCTL(KC_F4)
#define D_NW   LGUI(LCTL(KC_D))
#define D_CL   LGUI(KC_TAB)
#define D_LT   LCTL(LGUI(KC_LEFT))
#define D_RT   LCTL(LGUI(KC_RIGHT))

// --- Layer change hook ---
layer_state_t layer_state_set_user(layer_state_t state) {
    if (!layer_state_cmp(state, _NAV)) {  // If NAV layer is no longer active
        if (is_alt_tab_active) {
            unregister_code(KC_LALT);    // Release alt
            is_alt_tab_active = false;
        }
    }
    return state;
}

// --- Key handling ---
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case NAV_FWD:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                tap_code(KC_TAB);
            }
            return false;

        case NAV_BACK:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                tap_code16(S(KC_TAB));
            }
            return false;
    }
    return true;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
      KC_TAB, 	  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,	  		KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      KC_ESC, 	  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,	  		KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
TD(TD_LSFT_CAPS), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   		KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_RSFT, KC_ENT),
                                      MT(MOD_LCTL, KC_LGUI), LOWER, LT(NAV, KC_SPC), KC_SPC, RAISE, MT(MOD_RGUI, KC_RALT)
  ),

  [_RAISE] = LAYOUT_split_3x6_3(
      KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_NUM),
                                     XXXXXXX, MO(ADJUST), XXXXXXX,     XXXXXXX, _______, XXXXXXX
  ),

  [_LOWER] = LAYOUT_split_3x6_3(
      TD(TD_TLD_GRV), KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,              KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
      UNLOCK, 		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   			 XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
      XXXXXXX, 	XXXXXXX,   XXXXXXX,   KC_F10,  KC_F11,  KC_F12,  			 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
										  XXXXXXX, _______, XXXXXXX, XXXXXXX, MO(ADJUST), XXXXXXX
  ),

  [_ADJUST] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, XXXXXXX,                  XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                           XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX
  ),

  [_NAV] = LAYOUT_split_3x6_3(
	  LALT(KC_F4), XXXXXXX, TAB_CL, TAB_LT, TAB_RT, TAB_NW,    				 _______, KC_PGUP, KC_UP, KC_PGDN, KC_PSCR, KC_DEL,
	  XXXXXXX, LCTL(KC_A), LCTL(KC_S), NAV_BACK, NAV_FWD, XXXXXXX,          KC_HOME,    KC_LEFT, KC_DOWN, KC_RGHT, KC_INS,  _______,
      XXXXXXX, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), XXXXXXX,                KC_END,     D_NW,    D_LT,    D_RT,    D_CL,    XXXXXXX,
                                             XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, KC_RGUI
  ),

  [_NUM] = LAYOUT_split_3x6_3(
	  KC_TAB , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PAST, KC_BSPC,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PMNS, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  KC_NUM, KC_P1,   KC_P2,   KC_P3,   KC_PPLS, KC_PENT,
                                          XXXXXXX, _______, XXXXXXX, TO(_QWERTY), KC_P0, KC_PDOT
  ),


};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Left Shift tap, double tap Caps Lock
    [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    // Tilde tap, Grave double tap
    [TD_TLD_GRV] = ACTION_TAP_DANCE_DOUBLE(KC_TILD, KC_GRV),

};


