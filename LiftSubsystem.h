#ifndef LIFT_SUBSYSTEM_H
#define LIFT_SUBSYSTEM_H

#include "Constants.h"
#include "OperatorButton.h"

class LiftSubsystem
{
public:
	LiftSubsystem();
	void DoTeleopInit();
	void TeleopPeriodic();
	void Unpin();
	void ReleaseHook();
	void Unspool();
	void Lift(float speed);
	void Lock();
	void Fire();
	void Unlock();

private:
	Solenoid *m_unpinSolenoid;
	Solenoid *m_releaseHookSolenoid;
	Solenoid *m_lockingSolenoid;

	VictorSP *m_liftMotor;

	Joystick *m_leftJoystick;
	Joystick *m_rightJoystick;

	OperatorButton *m_unpinningButtonA;
	OperatorButton *m_unpinningButtonB;
	OperatorButton *m_shiftingButton;
	OperatorButton *m_lockingButton;
	OperatorButton *m_fireButton;

	bool m_unpinState = false;
	bool m_lockState = false;
	bool m_fireState = false;

};

#endif
