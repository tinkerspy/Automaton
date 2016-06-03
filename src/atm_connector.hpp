#pragma once

#include "Arduino.h"

typedef void ( *atm_cb_push_t )( int idx, int v, int up );
typedef bool ( *atm_cb_pull_t )( int idx );

class atm_connector {
 public:
  enum { MODE_NULL, MODE_PUSHCB, MODE_PULLCB, MODE_MACHINE };            // bits 0, 1, 2 - Mode
  enum { LOG_AND, LOG_OR, LOG_XOR };                                     // bits 3, 4    - Logical operator
  enum { REL_NULL, REL_EQ, REL_NEQ, REL_LT, REL_GT, REL_LTE, REL_GTE };  // bits 5, 6, 7 - Relational operator
  uint8_t mode_flags;
  union {
    struct {
      union {
        atm_cb_push_t push_callback;
        atm_cb_pull_t pull_callback;
      };
      int callback_idx;  // +2 = 5 bytes per connector/object
    };
    struct {
      union {
        Machine* machine;
      };
      int event;
    };
  };
  void set( Machine* m, int evt, int8_t logOp = 0, int8_t relOp = 0 );
  void set( atm_cb_push_t callback, int idx, int8_t logOp = 0, int8_t relOp = 0 );
  void set( atm_cb_pull_t callback, int idx, int8_t logOp = 0, int8_t relOp = 0 );
  bool push( int v = 0, int up = 0, bool overrideCallback = false );  // returns false (only) if callback is set!
  int pull( int v = 0, int up = 0, bool def_value = false );
  int8_t logOp( void );
  int8_t relOp( void );
  int8_t mode( void );
};
