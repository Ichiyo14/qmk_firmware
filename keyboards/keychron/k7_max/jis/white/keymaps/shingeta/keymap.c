/* =========================================================================
 * keymap.c — Keychron K7 Max (JIS) 新下駄配列カスタム
 *
 * レイヤー構成:
 *   MAC_BASE / WIN_BASE : 通常のQWERTY（英数入力）
 *   _SHINGETA           : かなレイヤー（shingeta.cエンジンが使用）
 *   MAC_FN1 / WIN_FN1   : メディア・BT接続など
 *   FN2                 : ファンクションキー列＋Winファイル操作
 *
 * 右側ナビゲーションキー:
 *   旧PgUp位置 → End（単打）
 *   旧PgDn位置 → PageUp（単打） ／ Shift+同キー → PageDown
 *   Homeは右上に復帰済み。Home/End単打・PgUp/PgDnは1キー集約
 *
 * FN2のファイル操作ショートカット（Windowsエクスプローラー用）:
 *   FN2 + W     → Alt+F4        ウィンドウを閉じる
 *   FN2 + R     → F2            リネーム
 *   FN2 + N     → Ctrl+Shift+N  新規フォルダ
 *   FN2 + Q     → Win+Shift+S   スクリーンショット（範囲切り取り）
 *   FN2 + E     → Win+E         エクスプローラー起動
 *   FN2 + L     → Win+L         画面ロック
 *   FN2 + V     → Win+V         クリップボード履歴
 *   FN2 + M     → メニューキー  右クリックメニュー（続けてアクセスキー）
 *   FN2 + ↓    → メニューキー  同上（矢印移動から連続で使える）
 *   FN2 + ↑    → Alt+↑        親フォルダへ
 *   FN2 + ←/→ → Alt+←/→     戻る / 進む
 *   FN2 + PgUpキー → PageDown（Shift+PgUpキーでも可）
 *   FN1 + ←/→ → Ctrl+←/→    単語ジャンプ（文章編集用）
 *
 * FN2の擬似Ctrl（同じ指位置でコピペ系が完結）:
 *   FN2 + C/X/Z → コピー / 切り取り / 元に戻す
 *   FN2 + A/F/S → 全選択 / 検索 / 上書き保存
 *   FN2 + V     → Win+V（クリップボード履歴。通常の貼り付けは親指Ctrl+V）
 *
 * 新下駄のON/OFF:
 *   NG_OFF（スペース左）→ タップ: 常にOFF ／ ホールド: Win=Ctrl, Mac=Cmd
 *   NG_ON （スペース右）→ 常にON（かなへ）
 *   ※トグルではなく明示指定なので、IMEとズレても1キーで再同期できる
 *
 * モディファイア配置:
 *   Win: Aの左=FN2 ／ Ctrlは親指（NG_OFFホールド）のみ
 *   Mac: Aの左=Ctrl ／ 下段=Cmd ／ Cmdは親指ホールドでも可
 * ========================================================================= */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "shingeta.h"

/* NG_OFFのタップ/ホールド判定時間 [ms]（これより短い押下＝タップ） */
#define NG_OFF_TAPPING_TERM 200

// clang-format off

enum layers {
    MAC_BASE,
    WIN_BASE,
    _SHINGETA,
    MAC_FN1,
    WIN_FN1,
    FN2
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[MAC_BASE] = LAYOUT_jis_71(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_INT3,  KC_BSPC, KC_DEL,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                     KC_HOME,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_BSPC,  KC_BSLS,  KC_ENT,            KC_END,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_INT1,            KC_UP,   KC_PGUP,
     KC_QUOT,  KC_LOPT,  KC_LCMD,  NG_OFF,                       KC_SPC,                       KC_ENT,   NG_ON,    MO(MAC_FN1), MO(FN2), KC_LEFT, KC_DOWN, KC_RGHT),

[WIN_BASE] = LAYOUT_jis_71(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_INT3,  KC_BSPC, KC_DEL,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                     KC_HOME,
     MO(FN2),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_BSPC,  KC_BSLS,  KC_ENT,            KC_END,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_INT1,            KC_UP,   KC_PGUP,
     KC_QUOT,  KC_LGUI,  KC_LALT,  NG_OFF,                       KC_SPC,                       KC_ENT,   NG_ON,    MO(WIN_FN1), MO(FN2), KC_LEFT, KC_DOWN, KC_RGHT),

[_SHINGETA] = LAYOUT_jis_71(
     _______,  NG_1,     NG_2,     NG_3,     NG_4,     NG_5,     NG_6,     NG_7,     NG_8,     NG_9,     NG_0,     NG_MINS,  KC_EQL,   KC_INT3,  KC_BSPC, _______,
     KC_TAB,   NG_Q,     NG_W,     NG_E,     NG_R,     NG_T,     NG_Y,     NG_U,     NG_I,     NG_O,     NG_P,     NG_X1,    KC_RBRC,                     _______,
     _______,  NG_A,     NG_S,     NG_D,     NG_F,     NG_G,     NG_H,     NG_J,     NG_K,     NG_L,     NG_SCLN,  KC_BSPC,  _______,  _______,           _______,
     _______,            NG_Z,     NG_X,     NG_C,     NG_V,     NG_B,     NG_N,     NG_M,     NG_COMM,  NG_DOT,   NG_SLSH,  _______,            _______, _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  _______,  _______, _______),

[MAC_FN1] = LAYOUT_jis_71(
     KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______, BL_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,
     _______,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,
     _______,            _______,  BL_DOWN,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,            _______, _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  C(KC_LEFT), _______, C(KC_RGHT)),

[WIN_FN1] = LAYOUT_jis_71(
     KC_GRV,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______, BL_TOGG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,
     _______,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,
     _______,            _______,  BL_DOWN,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,            _______, _______,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  C(KC_LEFT), _______, C(KC_RGHT)),

[FN2] = LAYOUT_jis_71(
     KC_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______, _______,
     _______,  G(S(KC_S)), A(KC_F4), G(KC_E), KC_F2,   _______,  _______,  _______,  _______,  _______,  KC_PSCR,  _______,  _______,                     _______,
     _______,  C(KC_A),  C(KC_S),  _______,  C(KC_F),  _______,  _______,  _______,  _______,  G(KC_L),  _______,  _______,  _______,  _______,           _______,
     _______,            C(KC_Z),  C(KC_X),  C(KC_C),  G(KC_V),  BAT_LVL,  C(S(KC_N)), KC_APP, _______, _______,  _______,  _______,            A(KC_UP), KC_PGDN,
     _______,  _______,  _______,  _______,                      _______,                      _______,  _______,  _______,  _______,  A(KC_LEFT), KC_APP, A(KC_RGHT)),
};

// clang-format on

/* ------------------------------------------------------------------------
 * QMKフック
 * --------------------------------------------------------------------- */

void matrix_init_user(void) {
    set_shingeta(_SHINGETA);
}

void matrix_scan_user(void) {
    shingeta_timer_task();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t ng_off_timer       = 0;
    static bool     ng_off_held        = false;
    static bool     ng_off_interrupted = false;

    /* NG_OFFホールド中に他のキーが押されたら「Ctrlとして使った」と判定 */
    if (ng_off_held && record->event.pressed && keycode != NG_OFF) {
        ng_off_interrupted = true;
    }

    switch (keycode) {
        case NG_ON:
            // 常にON（IMEとズレてもこのキー1つで再同期）
            if (record->event.pressed) {
                shingeta_on();
            }
            return false;

        case KC_PGUP: {
            /* 単打 → PageUp ／ Shift押しながら → PageDown */
            static uint16_t pg_sent = KC_PGUP;
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    uint8_t saved = get_mods();
                    del_mods(MOD_MASK_SHIFT); // Shift選択にならないよう一時解除
                    pg_sent = KC_PGDN;
                    register_code(pg_sent);
                    set_mods(saved);
                } else {
                    pg_sent = KC_PGUP;
                    register_code(pg_sent);
                }
            } else {
                unregister_code(pg_sent);
            }
            return false;
        }

        case NG_OFF: {
            /* タップ → 新下駄OFF ／ ホールド → Win: Ctrl, Mac: Cmd */
            static uint16_t held_mod = KC_LCTL;
            if (record->event.pressed) {
                ng_off_timer       = timer_read();
                ng_off_held        = true;
                ng_off_interrupted = false;
                held_mod = (get_highest_layer(default_layer_state) == WIN_BASE)
                               ? KC_LCTL
                               : KC_LGUI; // Mac は Cmd
                if (shingeta_state()) {
                    shingeta_mode(held_mod, record); // かなレイヤーを退避
                }
                register_code(held_mod); // 即有効（素早いCtrl+C / Cmd+Cにも対応）
            } else {
                unregister_code(held_mod);
                if (shingeta_state()) {
                    shingeta_mode(held_mod, record); // かなレイヤーを復帰
                }
                ng_off_held = false;
                /* 短押し＆他キー未使用ならタップ＝新下駄OFF */
                if (!ng_off_interrupted && timer_elapsed(ng_off_timer) < NG_OFF_TAPPING_TERM) {
                    shingeta_off();
                }
            }
            return false;
        }
    }

    if (shingeta_state()) {
        shingeta_mode(keycode, record);
        if (!process_shingeta(keycode, record)) {
            return false;
        }
    }

    return true;
}
