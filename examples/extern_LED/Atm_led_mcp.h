/*
 * Atm_led_mcp.h
 *
 *  Created on: 09.12.2017
 *      Author: ian
 */

#ifndef SRC_ATM_LED_MCP_H_
#define SRC_ATM_LED_MCP_H_

#include "Adafruit_MCP23017.h"
#include <Atm_led.hpp>

class Atm_led_mcp: public Atm_led {
public:
	Atm_led_mcp(Adafruit_MCP23017& _gpio);

private:
	Adafruit_MCP23017& gpio;

protected:
	virtual void initLED();
	virtual void switchOn();
	virtual void switchOff();
    virtual void setBrightness(int value);
};

#endif /* SRC_ATM_LED_MCP_H_ */
