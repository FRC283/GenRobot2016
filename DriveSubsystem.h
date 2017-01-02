#ifndef DRIVE_SUBSYSTEM_H
#define DRIVE_SUBSYSTEM_H


#include "WPILib.h"
#include "Constants.h"
#include "OperatorButton.h"

class DriveSubsystem
{
public:
	DriveSubsystem();
	void Drive(float,float);
	void TeleopPeriodic();
	void DriveShift();

private:
	VictorSP *m_leftDriveMotorController;
	VictorSP *m_rightDriveMotorController;

	Joystick *m_leftJoystick;
	Joystick *m_rightJoystick;

	Encoder *m_leftDriveEncoder;
	Encoder *m_rightDriveEncoder;

	Solenoid *m_driveShiftSolenoid;

	OperatorButton *m_driveShiftButton;

	bool m_driveShiftState;


};

#endif
