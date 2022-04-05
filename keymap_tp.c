#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP_ANSI(
        ESC ,2   ,3   ,4   ,5   ,6   ,NO  ,7   ,8   ,9   ,0   ,1   ,BSPC,\
        TAB ,Q   ,W   ,E   ,R   ,T   ,Y   ,U   ,I   ,O   ,P   ,LBRC,RBRC,\
        LCTL,A   ,S   ,D   ,F   ,G   ,NO  ,H   ,J   ,K   ,L   ,SCLN,RCTL,\
        MINS,QUOT,Z   ,X   ,C   ,V   ,B   ,N   ,M   ,COMM,DOT ,SLSH,EQL ,\
        LSFT,NO  ,GRV ,LGUI,NO  ,FN0 ,ESC ,FN1 ,NO  ,RALT,BSLS,NO  ,RSFT),
    KEYMAP_ANSI(
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,NO  ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,\
        F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,TRNS,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,\
        TRNS,2   ,3   ,4   ,5   ,6   ,NO  ,7   ,8   ,9   ,0   ,1   ,TRNS,\
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,\
        TRNS,NO  ,TRNS,TRNS,NO  ,TRNS,DEL ,BSPC,NO  ,TRNS,TRNS,NO  ,TRNS),
    KEYMAP_ANSI(
        F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,NO  ,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,\
        CAPS,TRNS,TRNS,TRNS,PGUP,WH_L,MS_U,WH_U,UP  ,TRNS,TRNS,TRNS,TRNS,\
        TRNS,MS_L,HOME,PGDN,END ,WH_R,NO  ,WH_D,LEFT,DOWN,RGHT,MS_R,TRNS,\
        TRNS,TRNS,TRNS,BTN3,BTN2,BTN1,MS_D,VOLD,VOLU,TRNS,TRNS,TRNS,TRNS,\
        TRNS,NO  ,TRNS,TRNS,NO  ,BSPC,DEL ,TRNS,NO  ,TRNS,TRNS,NO  ,TRNS),
};

const action_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_TAP_KEY(1, KC_ENT),
    [1] = ACTION_LAYER_TAP_KEY(2, KC_SPC),
};
