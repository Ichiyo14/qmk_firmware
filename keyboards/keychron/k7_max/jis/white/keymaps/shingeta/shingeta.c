/* =========================================================================
 * shingeta.c — 新下駄配列エンジン for QMK
 *
 * 同時押し（コンボ）判定つきのかな入力エンジン。
 * COMBO_WINDOW 以内に2キー押されたら同時押し、超えたら単打として処理する。
 *
 * 公開API（shingeta.h）:
 *   set_shingeta()       レイヤー番号の登録
 *   shingeta_on/off()    ON/OFF（IME同期キー送信つき）
 *   shingeta_force_on/off()  同上（互換用エイリアス）
 *   shingeta_state()     現在の状態
 *   shingeta_mode()      モディファイアキーの監視（レイヤー退避/復帰）
 *   process_shingeta()   NGキーの入力処理本体
 *   shingeta_timer_task() 単打確定タイマー（matrix_scan_userから呼ぶ）
 * ========================================================================= */

#include "shingeta.h"
#include <string.h>

/* ------------------------------------------------------------------------
 * 設定
 * --------------------------------------------------------------------- */
#define NGBUFFER 5           // 入力バッファ長（同時押しは最大3キー想定）
#define FIRST_OUTPUT_DELAY 50 // 単打確定までの待ち時間 [ms]
#define COMBO_WINDOW 50       // 同時押し判定ウィンドウ [ms]

#define NGMAP_SIZE (sizeof(ngmap) / sizeof(shingeta_keymap))

/* ------------------------------------------------------------------------
 * 状態
 * --------------------------------------------------------------------- */
static bool     is_shingeta    = false;      // エンジンON/OFF
static uint8_t  shingeta_layer = 0;          // かなレイヤー番号
static bool     layer_was_on   = false;      // モディファイア退避時のレイヤー状態

static uint16_t ninputs[NGBUFFER];           // 押されたNGキーコードの列
static uint8_t  ng_chrcount    = 0;          // ninputsの要素数
static uint64_t keycomb        = 0;          // 押下中キーのビットマスク（コンボ検索用）

static uint16_t first_key_time = 0;          // 1打目の押下時刻
static bool     timer_active   = false;      // 単打確定タイマーの有効フラグ

/* ------------------------------------------------------------------------
 * キー → ビット対応（43キーを64bitに割り当て）
 * --------------------------------------------------------------------- */
#define B_Q ((uint64_t)1 << 0)
#define B_W ((uint64_t)1 << 1)
#define B_E ((uint64_t)1 << 2)
#define B_R ((uint64_t)1 << 3)
#define B_T ((uint64_t)1 << 4)

#define B_Y ((uint64_t)1 << 5)
#define B_U ((uint64_t)1 << 6)
#define B_I ((uint64_t)1 << 7)
#define B_O ((uint64_t)1 << 8)
#define B_P ((uint64_t)1 << 9)

#define B_A ((uint64_t)1 << 10)
#define B_S ((uint64_t)1 << 11)
#define B_D ((uint64_t)1 << 12)
#define B_F ((uint64_t)1 << 13)
#define B_G ((uint64_t)1 << 14)

#define B_H ((uint64_t)1 << 15)
#define B_J ((uint64_t)1 << 16)
#define B_K ((uint64_t)1 << 17)
#define B_L ((uint64_t)1 << 18)
#define B_SCLN ((uint64_t)1 << 19)

#define B_Z ((uint64_t)1 << 20)
#define B_X ((uint64_t)1 << 21)
#define B_C ((uint64_t)1 << 22)
#define B_V ((uint64_t)1 << 23)
#define B_B ((uint64_t)1 << 24)

#define B_N ((uint64_t)1 << 25)
#define B_M ((uint64_t)1 << 26)
#define B_COMM ((uint64_t)1 << 27)
#define B_DOT ((uint64_t)1 << 28)
#define B_SLSH ((uint64_t)1 << 29)

#define BB_1 ((uint64_t)1 << 30)
#define BB_2 ((uint64_t)1 << 31)
#define BB_3 ((uint64_t)1 << 32)
#define BB_4 ((uint64_t)1 << 33)
#define BB_5 ((uint64_t)1 << 34)
#define BB_6 ((uint64_t)1 << 35)
#define BB_7 ((uint64_t)1 << 36)
#define BB_8 ((uint64_t)1 << 37)
#define BB_9 ((uint64_t)1 << 38)
#define BB_0 ((uint64_t)1 << 39)
#define B_MINS ((uint64_t)1 << 40)

#define B_X1 ((uint64_t)1 << 41)

#define B_SHFT ((uint64_t)1 << 42)

const uint64_t ng_key[] = {
    [NG_Q - NG_Q] = B_Q, [NG_W - NG_Q] = B_W, [NG_E - NG_Q] = B_E, [NG_R - NG_Q] = B_R, [NG_T - NG_Q] = B_T, [NG_Y - NG_Q] = B_Y, [NG_U - NG_Q] = B_U, [NG_I - NG_Q] = B_I, [NG_O - NG_Q] = B_O, [NG_P - NG_Q] = B_P, [NG_A - NG_Q] = B_A, [NG_S - NG_Q] = B_S, [NG_D - NG_Q] = B_D, [NG_F - NG_Q] = B_F, [NG_G - NG_Q] = B_G, [NG_H - NG_Q] = B_H, [NG_J - NG_Q] = B_J, [NG_K - NG_Q] = B_K, [NG_L - NG_Q] = B_L, [NG_SCLN - NG_Q] = B_SCLN, [NG_Z - NG_Q] = B_Z, [NG_X - NG_Q] = B_X, [NG_C - NG_Q] = B_C, [NG_V - NG_Q] = B_V, [NG_B - NG_Q] = B_B, [NG_N - NG_Q] = B_N, [NG_M - NG_Q] = B_M, [NG_COMM - NG_Q] = B_COMM, [NG_DOT - NG_Q] = B_DOT, [NG_SLSH - NG_Q] = B_SLSH, [NG_1 - NG_Q] = BB_1, [NG_2 - NG_Q] = BB_2, [NG_3 - NG_Q] = BB_3, [NG_4 - NG_Q] = BB_4, [NG_5 - NG_Q] = BB_5, [NG_6 - NG_Q] = BB_6, [NG_7 - NG_Q] = BB_7, [NG_8 - NG_Q] = BB_8, [NG_9 - NG_Q] = BB_9, [NG_0 - NG_Q] = BB_0, [NG_MINS - NG_Q] = B_MINS, [NG_X1 - NG_Q] = B_X1, [NG_SHFT - NG_Q] = B_SHFT,
};

/* ------------------------------------------------------------------------
 * かな定義テーブル
 * --------------------------------------------------------------------- */
typedef struct {
    uint64_t key;
    char     kana[5];
} shingeta_keymap;

const PROGMEM shingeta_keymap ngmap[] = {
    // 単独
    {.key = BB_1, .kana = "1"},
    {.key = BB_2, .kana = "2"},
    {.key = BB_3, .kana = "3"},
    {.key = BB_4, .kana = "4"},
    {.key = BB_5, .kana = "5"},
    {.key = BB_6, .kana = "6"},
    {.key = BB_7, .kana = "7"},
    {.key = BB_8, .kana = "8"},
    {.key = BB_9, .kana = "9"},
    {.key = BB_0, .kana = "0"},
    {.key = B_MINS, .kana = "-"},
    {.key = B_SHFT, .kana = " "},

    {.key = B_Q, .kana = "-"},
    {.key = B_W, .kana = "ni"},
    {.key = B_E, .kana = "ha"},
    {.key = B_R, .kana = ","},
    {.key = B_T, .kana = "ti"},
    {.key = B_Y, .kana = "gu"},
    {.key = B_U, .kana = "ba"},
    {.key = B_I, .kana = "ko"},
    {.key = B_O, .kana = "ga"},
    {.key = B_P, .kana = "hi"},
    {.key = B_X1, .kana = "ge"},

    {.key = B_A, .kana = "no"},
    {.key = B_S, .kana = "to"},
    {.key = B_D, .kana = "ka"},
    {.key = B_F, .kana = "nn"},
    {.key = B_G, .kana = "ltu"},
    {.key = B_H, .kana = "ku"},
    {.key = B_J, .kana = "u"},
    {.key = B_K, .kana = "i"},
    {.key = B_L, .kana = "si"},
    {.key = B_SCLN, .kana = "na"},

    {.key = B_Z, .kana = "su"},
    {.key = B_X, .kana = "ma"},
    {.key = B_C, .kana = "ki"},
    {.key = B_V, .kana = "ru"},
    {.key = B_B, .kana = "tu"},
    {.key = B_N, .kana = "te"},
    {.key = B_M, .kana = "ta"},
    {.key = B_COMM, .kana = "de"},
    {.key = B_DOT, .kana = "."},
    {.key = B_SLSH, .kana = "bu"},

    // 中指シフト
    {.key = B_K | B_Q, .kana = "fa"},
    {.key = B_K | B_W, .kana = "go"},
    {.key = B_K | B_E, .kana = "hu"},
    {.key = B_K | B_R, .kana = "fi"},
    {.key = B_K | B_T, .kana = "fe"},
    {.key = B_D | B_Y, .kana = "wi"},
    {.key = B_D | B_U, .kana = "pa"},
    {.key = B_D | B_I, .kana = "yo"},
    {.key = B_D | B_O, .kana = "mi"},
    {.key = B_D | B_P, .kana = "we"},
    {.key = B_D | B_X1, .kana = "ulo"},

    {.key = B_K | B_A, .kana = "ho"},
    {.key = B_K | B_S, .kana = "ji"},
    {.key = B_K | B_D, .kana = "re"},
    {.key = B_K | B_F, .kana = "mo"},
    {.key = B_K | B_G, .kana = "yu"},
    {.key = B_D | B_H, .kana = "he"},
    {.key = B_D | B_J, .kana = "a"},
    {.key = B_D | B_K, .kana = ""},
    {.key = B_D | B_L, .kana = "o"},
    {.key = B_D | B_SCLN, .kana = "e"},

    {.key = B_K | B_Z, .kana = "du"},
    {.key = B_K | B_X, .kana = "zo"},
    {.key = B_K | B_C, .kana = "bo"},
    {.key = B_K | B_V, .kana = "mu"},
    {.key = B_K | B_B, .kana = "fo"},
    {.key = B_D | B_N, .kana = "se"},
    {.key = B_D | B_M, .kana = "ne"},
    {.key = B_D | B_COMM, .kana = "be"},
    {.key = B_D | B_DOT, .kana = "pu"},
    {.key = B_D | B_SLSH, .kana = "vu"},

    {.key = B_K | BB_1, .kana = "la"},
    {.key = B_K | BB_2, .kana = "li"},
    {.key = B_K | BB_3, .kana = "lu"},
    {.key = B_K | BB_4, .kana = "le"},
    {.key = B_K | BB_5, .kana = "lo"},

    // 薬指シフト
    {.key = B_L | B_Q, .kana = "di"},
    {.key = B_L | B_W, .kana = "me"},
    {.key = B_L | B_E, .kana = "ke"},
    {.key = B_L | B_R, .kana = "teli"},
    {.key = B_L | B_T, .kana = "deli"},
    {.key = B_S | B_Y, .kana = "sye"},
    {.key = B_S | B_U, .kana = "pe"},
    {.key = B_S | B_I, .kana = "do"},
    {.key = B_S | B_O, .kana = "ya"},
    {.key = B_S | B_P, .kana = "je"},

    {.key = B_L | B_A, .kana = "wo"},
    {.key = B_L | B_S, .kana = "sa"},
    {.key = B_L | B_D, .kana = "o"},
    {.key = B_L | B_F, .kana = "ri"},
    {.key = B_L | B_G, .kana = "zu"},
    {.key = B_S | B_H, .kana = "bi"},
    {.key = B_S | B_J, .kana = "ra"},
    {.key = B_S | B_K, .kana = ""},
    {.key = B_S | B_L, .kana = ""},
    {.key = B_S | B_SCLN, .kana = "so"},

    {.key = B_L | B_Z, .kana = "ze"},
    {.key = B_L | B_X, .kana = "za"},
    {.key = B_L | B_C, .kana = "gi"},
    {.key = B_L | B_V, .kana = "ro"},
    {.key = B_L | B_B, .kana = "nu"},
    {.key = B_S | B_N, .kana = "wa"},
    {.key = B_S | B_M, .kana = "da"},
    {.key = B_S | B_COMM, .kana = "pi"},
    {.key = B_S | B_DOT, .kana = "po"},
    {.key = B_S | B_SLSH, .kana = "tile"},

    {.key = B_L | BB_1, .kana = "xya"},
    {.key = B_L | BB_2, .kana = "mya"},
    {.key = B_L | BB_3, .kana = "myu"},
    {.key = B_L | BB_4, .kana = "myo"},
    {.key = B_L | BB_5, .kana = "xwa"},

    // 拗音（+I）
    {.key = B_I | B_E, .kana = "sho"},
    {.key = B_I | B_W, .kana = "shu"},
    {.key = B_I | B_R, .kana = "kyu"},
    {.key = B_I | B_F, .kana = "kyo"},
    {.key = B_I | B_V, .kana = "kya"},
    {.key = B_I | B_C, .kana = "sha"},
    {.key = B_I | B_Q, .kana = "hyu"},
    {.key = B_I | B_A, .kana = "hyo"},
    {.key = B_I | B_Z, .kana = "hya"},
    {.key = B_I | B_T, .kana = "chu"},
    {.key = B_I | B_G, .kana = "cho"},
    {.key = B_I | B_B, .kana = "cha"},

    {.key = B_I | BB_1, .kana = "xyu"},
    {.key = B_I | BB_2, .kana = "bya"},
    {.key = B_I | BB_3, .kana = "byu"},
    {.key = B_I | BB_4, .kana = "byo"},

    // 拗音（+O）
    {.key = B_O | B_E, .kana = "jo"},
    {.key = B_O | B_W, .kana = "ju"},
    {.key = B_O | B_R, .kana = "gyu"},
    {.key = B_O | B_F, .kana = "gyo"},
    {.key = B_O | B_V, .kana = "gya"},
    {.key = B_O | B_C, .kana = "ja"},
    {.key = B_O | B_Q, .kana = "ryu"},
    {.key = B_O | B_A, .kana = "ryo"},
    {.key = B_O | B_Z, .kana = "rya"},
    {.key = B_O | B_T, .kana = "nyu"},
    {.key = B_O | B_G, .kana = "nyo"},
    {.key = B_O | B_B, .kana = "nya"},

    {.key = B_O | BB_1, .kana = "xyo"},
    {.key = B_O | BB_2, .kana = "pya"},
    {.key = B_O | BB_3, .kana = "pyu"},
    {.key = B_O | BB_4, .kana = "pyo"},

    // その他の同時押し（記号）
    {.key = B_R | B_F, .kana = "/"},
    {.key = B_R | B_G, .kana = "/"},
    {.key = B_F | B_G, .kana = "()"},
    {.key = B_F | B_V, .kana = "!"},
    {.key = B_F | B_B, .kana = "!"},
    {.key = B_N | B_J, .kana = "?"},
    {.key = B_H | B_J, .kana = "[]"},
    {.key = B_H | B_U, .kana = "/"},
};

/* ------------------------------------------------------------------------
 * 内部ヘルパー
 * --------------------------------------------------------------------- */

/* 入力状態を完全にリセットする（唯一のリセット経路） */
static void reset_input(void) {
    memset(ninputs, 0, sizeof(ninputs));
    ng_chrcount  = 0;
    keycomb      = 0;
    timer_active = false;
}

/* combo に一致するかなを検索して送信。見つかれば true */
static bool find_and_send_kana(uint64_t combo) {
    shingeta_keymap entry;
    for (uint8_t i = 0; i < NGMAP_SIZE; i++) {
        memcpy_P(&entry, &ngmap[i], sizeof(entry));
        if (combo == entry.key) {
            send_string(entry.kana);
            return true;
        }
    }
    return false;
}

/* 現在のバッファ内容を出力してリセットする。
 * まずコンボとして検索し、なければ1キーずつ単打として出力。 */
static void flush_input(void) {
    if (!find_and_send_kana(keycomb)) {
        for (uint8_t j = 0; j < ng_chrcount; j++) {
            if (ninputs[j] >= NG_Q && ninputs[j] <= NG_SHFT) {
                find_and_send_kana(ng_key[ninputs[j] - NG_Q]);
            }
        }
    }
    reset_input();
}

/* keycode をバッファに登録し、1打目ならタイマーを開始する */
static void push_input(uint16_t keycode) {
    ninputs[ng_chrcount] = keycode;
    ng_chrcount++;
    keycomb |= ng_key[keycode - NG_Q];

    if (ng_chrcount == 1) {
        first_key_time = timer_read();
        timer_active   = true;
    }
}

/* IME同期：日本語入力へ（Mac: かな / Win: 変換→カタカナひらがな） */
static void ime_set_japanese(void) {
    tap_code_delay(KC_LNG1, 10);
    wait_ms(30);
    tap_code_delay(KC_INT4, 10);
    wait_ms(30);
    // 全角英数になっていてもひらがなモードに強制復帰（Windows用）
    tap_code_delay(KC_INT2, 10);
}

/* IME同期：英数入力へ（Mac: 英数 / Win: 無変換） */
static void ime_set_ascii(void) {
    tap_code_delay(KC_LNG2, 10);
    wait_ms(30);
    tap_code_delay(KC_INT5, 10);
}

/* ------------------------------------------------------------------------
 * 公開API
 * --------------------------------------------------------------------- */

void set_shingeta(uint8_t layer) {
    shingeta_layer = layer;
}

bool shingeta_state(void) {
    return is_shingeta;
}

void shingeta_on(void) {
    is_shingeta = true;
    reset_input();
    layer_on(shingeta_layer);
    ime_set_japanese();
}

void shingeta_off(void) {
    is_shingeta = false;
    reset_input();
    layer_off(shingeta_layer);
    ime_set_ascii();
}

/* 互換用エイリアス（旧: 状態チェックなし強制切替。現実装は同一処理） */
void shingeta_force_on(void) {
    shingeta_on();
}

void shingeta_force_off(void) {
    shingeta_off();
}

/* モディファイアキーの監視。
 * 最初のモディファイア押下でかなレイヤーを退避し、
 * 最後のモディファイア解放で復帰する。
 * カウンタではなく get_mods() を参照するため状態がズレない。 */
void shingeta_mode(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LCTL:
        case KC_LSFT:
        case KC_LALT:
        case KC_LGUI:
        case KC_RCTL:
        case KC_RSFT:
        case KC_RALT:
        case KC_RGUI:
            if (record->event.pressed) {
                /* 押下時点では自分はまだ get_mods() に反映されていない。
                 * 他のモディファイアが無ければ「最初の1個」 */
                if ((get_mods() & ~MOD_BIT(keycode)) == 0) {
                    if (ng_chrcount > 0) {
                        flush_input(); // 未確定入力を先に確定させる
                    }
                    layer_was_on = IS_LAYER_ON(shingeta_layer);
                    if (layer_was_on) {
                        layer_off(shingeta_layer);
                    }
                }
            } else {
                /* リリース時点では自分はまだ get_mods() に残っている。
                 * 自分を除いてゼロなら「最後の1個」 */
                if ((get_mods() & ~MOD_BIT(keycode)) == 0) {
                    if (layer_was_on && is_shingeta) {
                        layer_on(shingeta_layer);
                    }
                    layer_was_on = false;
                }
            }
            break;
    }
}

/* NGキーの入力処理本体。false を返すと QMK 側の処理を止める。 */
bool process_shingeta(uint16_t keycode, keyrecord_t *record) {
    /* モディファイア押下中は通常キーとして透過（Ctrl+C等のショートカット用） */
    if (get_mods() != 0) {
        return true;
    }

    if (keycode < NG_Q || keycode > NG_SHFT) {
        return true; // NGキー以外は関与しない
    }

    if (record->event.pressed) {
        if (ng_chrcount >= NGBUFFER) {
            reset_input(); // バッファ溢れ防止
        }

        push_input(keycode);

        if (ng_chrcount == 2) {
            uint16_t elapsed = timer_elapsed(first_key_time);

            if (elapsed <= COMBO_WINDOW) {
                /* ウィンドウ内 → 同時押しコンボとして確定 */
                flush_input();
            } else {
                /* ウィンドウ外 → 1打目を単打で確定し、
                 * 2打目を新たな1打目として仕切り直す */
                uint16_t second = ninputs[1];

                ng_chrcount = 1;
                keycomb     = ng_key[ninputs[0] - NG_Q];
                flush_input();

                push_input(second);
            }
        } else if (ng_chrcount >= 3) {
            /* 3キー同時押し（拗音など）→ 直ちに確定 */
            flush_input();
        }
        return false;
    } else {
        /* キーリリース → 未確定分があれば確定してリセット */
        if (ng_chrcount > 0) {
            flush_input();
        } else {
            reset_input();
        }
        return false;
    }
}

/* 単打確定タイマー。matrix_scan_user() から毎スキャン呼ばれる。
 * 1打目から FIRST_OUTPUT_DELAY 経過しても2打目が来なければ単打で確定。 */
void shingeta_timer_task(void) {
    if (timer_active && ng_chrcount == 1) {
        if (timer_elapsed(first_key_time) > FIRST_OUTPUT_DELAY) {
            flush_input();
        }
    }
}
