#include "LiftSubsystem.h"

LiftSubsystem::LiftSubsystem()
{
	m_leftJoystick = new Joystick(c_leftJoystickPort);
	m_rightJoystick = new Joystick(c_rightJoystickPort);

	m_unpinSolenoid = new Solenoid(c_unpinSolenoidPort);
	m_releaseHookSolenoid = new Solenoid(c_hookReleaseSolenoidPort);
	m_lockingSolenoid = new Solenoid(c_lockingSolenoidPort);

	m_liftMotor = new VictorSP(c_liftMotorPort);

	m_leftJoystick = new Joystick(c_leftJoystickPort);
	m_rightJoystick = new Joystick(c_rightJoystickPort);

	m_unpinningButtonA = new OperatorButton(m_leftJoystick, 3); //3
	m_unpinningButtonB = new OperatorButton(m_rightJoystick, 4); //4
	m_shiftingButton = new OperatorButton(m_leftJoystick, 5); //5
	m_lockingButton = new OperatorButton(m_rightJoystick, 2); //2
	m_fireButton = new OperatorButton(m_rightJoystick,1);

	m_unpinState = false;
	m_lockState = false;

}

void LiftSubsystem::DoTeleopInit()
{
	Unlock();
}
void LiftSubsystem::TeleopPeriodic()
{
	if (m_unpinningButtonA->Get() == OperatorButton::kJustPressed && m_unpinningButtonB->Get() == OperatorButton::kJustPressed)
	{
		Unpin();
	}

	if (m_unpinState == true && m_fireButton->Get() == OperatorButton::kJustPressed)
	{
		Fire();
	}
	if(m_unpinState == true)
	{
		if (m_leftJoystick->GetRawButton(6))
		{
			Lift(1);
		}
		else if (m_leftJoystick->GetRawButton(2))
		{
			Lift(-1);
		}
		else
		{
			Lift(0);
		}

		if(m_lockingButton->Get() == OperatorButton::kJustPressed && m_lockState == true)
		{
			Lock();
		}
		if(m_lockingButton->Get() == OperatorButton::kJustPressed && m_lockState == false)

		{
			Unlock();
		}
	}
}

void LiftSubsystem::Unpin()
{
	m_unpinState = !m_unpinState;
	m_unpinSolenoid->Set(m_unpinState);
}

void LiftSubsystem::Fire()
{
	m_fireState = !m_fireState;
	m_releaseHookSolenoid->Set(true);

}
void LiftSubsystem::Lock()
{
	m_lockState = false;
	m_lockingSolenoid->Set(m_lockState);
}

void LiftSubsystem::Unlock()
{
	m_lockState = true;
	m_lockingSolenoid->Set(m_lockState);
}
void LiftSubsystem::Lift(float speed)
{
	m_liftMotor->Set((speed*0.75));
}
