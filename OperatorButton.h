#ifndef  _OPERATOR_BUTTON_H
#define _OPERATOR_BUTTON_H

#include <WPILib.h>

class OperatorButton {
public:
	enum ButtonState { kReleased=0, kPressed=1, kJustPressed, kJustReleased };

	OperatorButton(Joystick *js, uint32_t number);
	OperatorButton(uint32_t js, uint32_t number);
	//OperatorButton(uint32_t number);
	~OperatorButton();
	ButtonState Get(void);
	ButtonState SetDelay(int delay);
	bool GetBool(void);
	bool GetDelayOccurred(double delay);

private:
	Joystick *m_js;
	int m_buttonNum;
	bool m_lastState;
	Timer *m_pressedTimer; //Keeps track of how long it has been since the button was released
	ButtonState DetermineState(bool current, bool previous);

};

#endif
