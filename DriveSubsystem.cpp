#include "DriveSubsystem.h"


DriveSubsystem::DriveSubsystem()
{
	m_leftDriveMotorController = new VictorSP(c_leftDriveMotorControllerPort);
	m_rightDriveMotorController = new VictorSP(c_rightDriveMotorControllerPort);
	m_leftDriveEncoder = new Encoder(c_leftDriveEncoderPortA,c_leftDriveEncoderPortB);
	m_rightDriveEncoder = new Encoder(c_rightDriveEncoderPortA,c_rightDriveEncoderPortB);
	m_leftJoystick = new Joystick(c_leftJoystickPort);
	m_rightJoystick = new Joystick(c_rightJoystickPort);
	m_driveShiftSolenoid = new Solenoid(c_driveShiftSolenoidPort);
	m_driveShiftButton = new OperatorButton(m_leftJoystick, c_stickTrigger);
	m_driveShiftState = false;

}

//SmartDashboard::PutBoolean("Low Gear", m_driveShiftSolenoid->Get());

void DriveSubsystem::TeleopPeriodic()
{
	Drive(m_leftJoystick->GetY(), m_rightJoystick->GetY());

	if (m_driveShiftButton->Get() == OperatorButton::kJustPressed)
	{
		m_driveShiftState = !m_driveShiftState;
	}

	DriveShift();

	SmartDashboard::PutBoolean("Low Gear", m_driveShiftSolenoid->Get());
}

void DriveSubsystem::Drive(float lvalue,float rvalue)
{
	if (lvalue >= 0.1 || lvalue <= -0.1)
	{
		m_leftDriveMotorController->Set(lvalue);
	}
	else
	{
		m_leftDriveMotorController->Set(0);
	}

	if (rvalue >= 0.1 || rvalue <= -0.1)
	{
		m_rightDriveMotorController->Set(-rvalue);
	}
	else
	{
		m_rightDriveMotorController->Set(0);
	}
}

void DriveSubsystem::DriveShift()
{
	m_driveShiftSolenoid->Set(m_driveShiftState);


}
