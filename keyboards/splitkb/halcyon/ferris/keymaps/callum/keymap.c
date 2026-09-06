#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TABL G(S(KC_LBRC))
#define TABR G(S(KC_RBRC))
#define SPCL C(G(KC_LEFT))
#define SPC_R C(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define LA_PWR LT(4,KC_SPC)

enum layers {
    DEF,
    SYM,
    NAV,
    FUN,
    PWR,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN,  // Switch to next window         (alt-tab)
    SW_LANG, // Switch to next input language (gui-spc)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_ferris_hlc(
        KC_Q   , KC_W   , KC_E   , KC_R  ,  KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   ,
        KC_A   , KC_S   , KC_D   , KC_F  ,  KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_QUOT,
        KC_Z   , KC_X   , KC_C   , KC_V  ,  KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH,
	                           LA_NAV,  OS_LSFT, LA_PWR , LA_SYM ,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [SYM] = LAYOUT_ferris_hlc(
        KC_DLR , KC_AT  , KC_LBRC, KC_RBRC, QK_REP , KC_CIRC, KC_UNDS, KC_EXLM, KC_QUES, KC_GRV ,
        KC_PERC, KC_ASTR, KC_LPRN, KC_RPRN, KC_ENT , KC_EQL , OS_SHFT, OS_ALT , OS_CTRL, OS_CMD ,
        KC_TILD, KC_HASH, KC_LCBR, KC_RCBR, KC_ESC , KC_AMPR, KC_MINS, KC_SCLN, KC_COLN, KC_BSLS,
                                   _______, KC_TAB , _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [NAV] = LAYOUT_ferris_hlc(
	XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC, KC_DEL , XXXXXXX, KC_PGDN, KC_PGUP, XXXXXXX, XXXXXXX,
        OS_CMD , OS_CTRL, OS_ALT , OS_SHFT, KC_BSPC, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_END , XXXXXXX, XXXXXXX,
                                   _______, _______, KC_TAB , _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),    

    
    [FUN] = LAYOUT_ferris_hlc(
        KC_BRID, KC_BRIU, KC_VOLU, KC_VOLD, KC_MUTE, KC_F12 , KC_F7  , KC_F8  , KC_F9  , KC_NUM ,
        OS_CMD , OS_CTRL, OS_ALT , OS_SHFT, SW_LANG, KC_F10 , KC_F4  , KC_F5  , KC_F6  , KC_CAPS,
        RM_TOGG, RM_NEXT, KC_MPRV, KC_MNXT, KC_MPLY, KC_F11 , KC_F1  , KC_F2  , KC_F3  , CW_TOGG,
	                           _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [PWR] = LAYOUT_ferris_hlc(
	KC_DOT , KC_9   , KC_8   , KC_7   , QK_REP , XXXXXXX, KC_WBAK, KC_WFWD, SPCL   , SPC_R  ,
        KC_MINS, KC_6   , KC_5   , KC_4   , KC_ENT , XXXXXXX, OS_SHFT, OS_ALT , OS_CTRL, OS_CMD ,
        KC_COMM, KC_3   , KC_2   , KC_1   , KC_ESC , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   KC_0   , KC_TAB , XXXXXXX, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
        keycode, record
    );
    update_swapper(
        &sw_lang_active, KC_LGUI, KC_SPC, SW_LANG,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, FUN);
}
