#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "shingeta.h"

// clang-format off

enum layers{
  MAC_BASE,
  WIN_BASE,
 _SHINGETA,
 _NUMPAD,
    MAC_FN1,
  WIN_FN1,
  FN2

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_jis_71(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_INT3,  KC_BSPC, KC_DEL,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                     KC_QUOT,
     KC_LCMD,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_BSPC,  KC_BSLS,  KC_ENT,            KC_PGUP,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_INT1,            KC_UP,   KC_PGDN,
     NK_TOGGLE,  KC_LOPT, KC_LCTL, NG_OFF,                     KC_SPC,                       KC_ENT,   NG_ON,MO(MAC_FN1),MO(FN2), KC_LEFT,  KC_DOWN, KC_RGHT),

[WIN_BASE] = LAYOUT_jis_71(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_INT3,  KC_BSPC, KC_DEL,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                     KC_QUOT,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_BSPC,  KC_BSLS,  KC_ENT,            KC_PGUP,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_INT1,            KC_UP,   KC_PGDN,
     NK_TOGGLE,  KC_LGUI,  KC_LALT,  NG_OFF,                    KC_SPC,                       KC_ENT,   NG_ON, MO(WIN_FN1),MO(FN2), KC_LEFT,  KC_DOWN, KC_RGHT),

[MAC_FN1] = LAYOUT_jis_71(
     KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  BL_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KC_END,
     _______,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,            _______,  BL_DOWN,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  _______,  _______,  _______),

[WIN_FN1] = LAYOUT_jis_71(
     KC_GRV,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  BL_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KC_END,
     _______,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,            _______,  BL_DOWN,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  _______,  _______,  _______),

[FN2] = LAYOUT_jis_71(
     KC_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  _______,  _______,                      _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  _______,  _______,  _______),

[_SHINGETA] = LAYOUT_jis_71(
    _______, NG_1,     NG_2,     NG_3,     NG_4,     NG_5,     NG_6,     NG_7,     NG_8,     NG_9,     NG_0,     NG_MINS,  KC_EQL,   KC_INT3,  KC_BSPC,  _______,
    KC_TAB,  NG_Q,     NG_W,     NG_E,     NG_R,     NG_T,     NG_Y,     NG_U,     NG_I,     NG_O,     NG_P,     NG_X1,    KC_RBRC,                      _______,
    _______, NG_A,     NG_S,     NG_D,     NG_F,     NG_G,     NG_H,     NG_J,     NG_K,     NG_L,     NG_SCLN,  KC_BSPC,  _______,  _______,            _______,
    _______,           NG_Z,     NG_X,     NG_C,     NG_V,     NG_B,     NG_N,     NG_M,     NG_COMM,  NG_DOT,   NG_SLSH,  _______,            _______,  _______,
    _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  _______,  _______,  _______),

[_NUMPAD] = LAYOUT_jis_71(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,     KC_NO,     KC_NO,   KC_NO,    KC_NO,     KC_NO,      KC_NO,     KC_NO,    KC_NO,   _______, _______,
    KC_TAB,  KC_PSLS, KC_P7,   KC_P8,     KC_P9,   KC_KP_MINUS,   KC_PSLS,     KC_P7,   KC_P8,    KC_P9,     KC_KP_MINUS,  KC_TAB,  KC_NO,                       KC_PSCR,
    _______, KC_PAST, KC_P4,   KC_P5,   KC_P6,     KC_PPLS,   KC_PAST,     KC_P4,   KC_P5,    KC_P6,   KC_PPLS,   _______,   KC_NO,   KC_NO,               KC_NO,
    _______,          KC_P0,   KC_P1,   KC_P2,      KC_P3,    KC_PDOT,     KC_P0,   KC_P1,    KC_P2,   KC_P3,    KC_PDOT,     KC_NO,             _______,    KC_NO,
    NK_TOGGLE, _______, _______,  KC_NO,                      _______,                        KC_ENT,   KC_NO,    KC_NO,     KC_NO,  _______,   _______,   _______)
//   ┌────────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬─────┬────┐
//   │        │    │    │    │    │    │    │    │    │    │    │    │    │    │     │    │
//   ├────────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴─────┼────┤
//   │ Tab     │  / │ 7  │ 8  │ 9  │ -  │ /  │  7 │ 8  │ 9  │ -  │TAB │    │         │PScr│
//   ├─────────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬────────┼────┤
//   │ Ctrl/Cmd │  * │ 4  │ 5  │ 6  │ +  │  * │ 4  │ 5  │ 6  │ +  │ BS │    │        │    │
//   ├──────────┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴──┬─┴────┬───┼────┤
//   │ Shift       │ 0  │ 1  │ 2  │ 3  │ .  │ 0  │ 1  │ 2  │ 3  │ .  │    │      │   │    │
//   ├────┬────┬───┴┬───┴────┴────┴────┴────┴────┴────┴────┴────┴────┼────┼─  ───┼───┼────┤
//   │NumT│    │    │                                          │ ent │    │    │    │   │   │   │
//   └────┴────┴────┴──────────────────────────────────────────┴─── ─┴────┴────┴────┴────┴────┘
//   左手: QWERTY配置でテンキー | 右手: Mキー=1でテンキー | モディファイア透過でショートカット可
};

// clang-format on

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

void matrix_init_user(void) {
    set_shingeta(_SHINGETA);
}

void matrix_scan_user(void) {
    shingeta_timer_task();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint8_t previous_layer = MAC_BASE; // 前のレイヤーを記憶

    switch (keycode) {
        case NG_TOGGLE:
            if (record->event.pressed) {
                if (shingeta_state()) {
                    shingeta_off();
                } else {
                    shingeta_on();
                }
            }
            return false;
            break;

        case NG_ON:
            if (record->event.pressed) {
                if (!shingeta_state()) {
                    shingeta_on();
                }
            }
            return false;
            break;

        case NG_OFF:
            if (record->event.pressed) {
                if (shingeta_state()) {
                    shingeta_off();
                }
            }
            return false;
            break;

        case NK_TOGGLE: // テンキートグル
            if (record->event.pressed) {
                if (IS_LAYER_ON(_NUMPAD)) {
                    // テンキーレイヤーがONなら、前のレイヤーに戻る
                    layer_off(_NUMPAD);

                    // 薙刀式の状態を復元
                    if (previous_layer == _SHINGETA) {
                        if (!shingeta_state()) {
                            shingeta_on();
                        }
                    } else {
                        if (shingeta_state()) {
                            shingeta_off();
                        }
                    }
                } else {
                    // 現在のレイヤーを記憶
                    if (IS_LAYER_ON(_SHINGETA)) {
                        previous_layer = _SHINGETA;
                    } else if (IS_LAYER_ON(WIN_BASE) || get_highest_layer(default_layer_state) == WIN_BASE) {
                        previous_layer = WIN_BASE;
                    } else {
                        previous_layer = MAC_BASE;
                    }

                    // 薙刀式がONなら一旦OFF
                    if (shingeta_state()) {
                        shingeta_off();
                    }

                    // テンキーレイヤーをON
                    layer_on(_NUMPAD);
                }
            }
            return false;
            break;
    }

    if (shingeta_state()) {
        shingeta_mode(keycode, record);
        if (!process_shingeta(keycode, record)) {
            return false;
        }
    }

    return true;
}
