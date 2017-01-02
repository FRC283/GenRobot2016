#include "Robot.h"

Robot::Robot()
{

}

void Robot::RobotInit()
{
	m_xbox = new Joystick(c_xboxPort);
	m_shooterSubsystem = new ShooterSubsystem();
	m_driveSubsystem = new DriveSubsystem();
	m_liftSubsystem = new LiftSubsystem();
	m_autoTimer = new Timer();
	m_autoState = kHoodSet;
	m_liftSubsystem->DoTeleopInit();



	SmartDashboard::init(); //Initialize Smartdashboard
}

void Robot::AutonomousInit()
{
	//m_shooterSubsystem->ReleaseIntakes();
	m_autoTimer->Reset();
	m_autoTimer->Start();
}

void Robot::AutonomousPeriodic()
{
	//m_driveSubsystem->AutoDrive();
	Periodic();
	switch(m_autoState)
	{
	case kHoodSet:
		printf("Entered HoodSet\n");
		m_shooterSubsystem->AutoHoodSet(.745 + .0585); // remeber to add c_hoodoffset to this value
		printf("Hood has been set\n");
		m_autoState = kSpinUp;
		break;
	case kSpinUp:
		printf("Entered SpinUp\n");
		m_shooterSubsystem->SpinUp();
		printf("SpinUp function run\n");
		m_autoState = kFireCheck;
		break;
	case kFireCheck:
		printf("Entered FireCheck\n");
		if (m_autoTimer->Get()>=10)
		{
			printf("FireCheck True\n");
			m_autoState=kFire;
		}
		printf("FireCheck False\n");
		break;
	case kFire:
		printf("Entered Fire\n");
		m_shooterSubsystem->FireInit();
		printf("FireInit Run\n");
		m_autoState = kEnd;
		break;
	case kEnd:
		printf("Entered End\n");
		m_shooterSubsystem->ClearLowbar();
		m_shooterSubsystem->Brake();
		break;
	}



}

void Robot::TeleopInit()
{
	m_shooterSubsystem->ReleaseIntakes();
	m_shooterSubsystem->HomeCamera();
}

void Robot::TeleopPeriodic()
{
	if (m_xbox->GetRawButton(c_x)) m_shooterSubsystem->HomeCamera();
	if (m_xbox->GetRawButton(c_leftBumper))
	{
		m_shooterSubsystem->Manual(m_xbox->GetRawAxis(c_rightXAxis), m_xbox->GetRawAxis(c_rightYAxis)); //Control the inner turret manually with the left stick
	}
	else
	{
		m_shooterSubsystem->Aim(m_xbox->GetRawAxis(c_rightXAxis), m_xbox->GetRawAxis(c_rightYAxis), m_xbox->GetRawButton(c_rightAxisButton)); //Control camera turret, and move inner turret with the camera turret value
	}
	if (m_xbox->GetRawAxis(c_rightTrigger) > c_triggerPoint) m_shooterSubsystem->FireInit();
	if (m_xbox->GetRawAxis(c_leftTrigger) > c_triggerPoint) m_shooterSubsystem->Brake();
	if (m_xbox->GetRawButton(c_rightBumper)) m_shooterSubsystem->SpinUp();

	if (m_xbox->GetRawButton(c_y)) m_shooterSubsystem->ClearLowbar();
	if (m_xbox->GetRawButton(c_a)) m_shooterSubsystem->ReleaseIntakes();
	m_shooterSubsystem->Intake(m_xbox->GetRawAxis(c_leftYAxis));

	printf("At liftsubsystem->TeleopPeriodic() line\n");
	m_driveSubsystem->TeleopPeriodic();
	m_liftSubsystem->TeleopPeriodic();

	Periodic();
}

void Robot::Periodic()
{
	SmartDashboard::PutNumber("Joystick Right X Axis", m_xbox->GetRawAxis(c_rightXAxis));
	SmartDashboard::PutNumber("Joystick Right Y Axis", m_xbox->GetRawAxis(c_rightYAxis));
	SmartDashboard::PutNumber("Joystick Left Y Axis", m_xbox->GetRawAxis(c_leftYAxis));

	m_shooterSubsystem->Periodic();

	printf("======= \n");
}

START_ROBOT_CLASS(Robot)
