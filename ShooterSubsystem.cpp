/*
 * ShooterSubsystem.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#include <ShooterSubsystem.h>

ShooterSubsystem::ShooterSubsystem()
{
	m_intakeController = new VictorSP(c_rollerIntakeControllerPort); //Controls the roller intakes on the robot
	m_cameraTurret = new CameraTurret(c_cameraXAxisServoPort, c_cameraYAxisServoPort); //Smaller Camera Turret on the Robot
	m_innerHood = new InnerHood(c_turretHoodPotPort, c_turretHoodControllerPort); //The main shooter turret
	m_innerSwivel = new InnerSwivel(c_turretSwivelLeftLimSwitch, c_turretSwivelRightLimSwitch, c_turretSwivelEncoderPortA, c_turretSwivelEncoderPortB, c_turretSwivelControllerPort);
	m_basketSolenoid = new Solenoid(c_basketSolenoidPort); //Controls up and down motion of basket
	m_flywheel = new Flywheel(c_flywheelEncoderPortA, c_flywheelEncoderPortB, c_flywheelControllerPort); //Controls setting speed of flywheel
	m_boulderUltrasonic = new AnalogInput(c_boulderUltrasonicPort);
	m_intakeReleaseSolenoid = new Solenoid(c_intakeReleaseSolenoidPort);
	m_rescaler = new Rescaler();
	m_firingTimer = new Timer();
	m_readyToFire = true;
	m_intakesReleased = false;
}

ShooterSubsystem::~ShooterSubsystem() {}

void ShooterSubsystem::Periodic()
{
	SmartDashboard::PutBoolean("Ball In Basket", HasBoulder());
	SmartDashboard::PutBoolean("Firing Reset Period has passed, can fire", m_readyToFire);
	SmartDashboard::PutNumber("Ultrasonic Voltage (Not Wired)", m_boulderUltrasonic->GetVoltage());
	SmartDashboard::PutBoolean("Intakes Released", m_intakesReleased);
	SmartDashboard::PutBoolean("Flywheel Up to Speed", GetIsFlywheelUpToSpeed());

	printf("Intakes Released : %i\n", m_intakesReleased);
	m_innerHood->Periodic();
	m_innerSwivel->Periodic();
	m_flywheel->Periodic();
	m_cameraTurret->Periodic();
	FirePeriodic();
}

void ShooterSubsystem::Aim(float xMagnitude, float yMagnitude, bool faster)
{
	//First, set the speed of the camera axes
	m_cameraTurret->SetXSpeed(xMagnitude, faster);
	m_cameraTurret->SetYSpeed(yMagnitude, faster);
	//Fetch the degrees (in our degree system) from the camera axes
	float xCamAngle = m_cameraTurret->GetXDegrees();
	printf("xCamAngle: %f \n", xCamAngle);
	float yCamAngle = m_cameraTurret->GetYDegrees();
	printf("yCamAngle: %f \n", yCamAngle);

	//Correct angular offset of the x-axis - Temporarily disabled
	float correctXAngle = CorrectionalTrigonometry(xCamAngle, yCamAngle);

	//Finally, convert the x-angle back to ticks, and set the inner swivel to that tick target
	//In addition, convert Ycamera values to innerHood voltage, and then set the InnerHood to that voltage
	m_innerSwivel->SetPointInit(m_innerSwivel->DegreesToTicks(correctXAngle), false); //Convert angle to ticks, then set the swivel to go there
	m_innerHood->SetPointInit(m_innerHood->CameraYValuesToVoltage(m_cameraTurret->GetY()), false); //Convert camera values to volts, then set the hood to go there
}

void ShooterSubsystem::Manual(float xMagnitude, float yMagnitude)
{
	m_innerHood->SetSpeed(yMagnitude);
	m_innerSwivel->SetSpeed(xMagnitude);
}

void ShooterSubsystem::HomeCamera()
{
	m_cameraTurret->Home();
}

void ShooterSubsystem::ClearIntake()
{
	m_innerSwivel->Home();
	m_innerHood->ClearForBall();
}

void ShooterSubsystem::ClearLowbar()
{
	m_cameraTurret->ClearLowbar(); //Move the camera to a position at which the hood will be below the lowbar when it correlates
	m_innerHood->Clear();
}

void ShooterSubsystem::Intake(float magnitude)
{
	if (m_intakesReleased == true) //We can only intake if the motors are out
	{
		if (magnitude > c_intakeDeadzone || magnitude < -c_intakeDeadzone) ClearIntake(); //Only clear if we're actually intaking
		magnitude = m_rescaler->Rescale(c_intakeDeadzone, 1.0, 0.0, 1.0, magnitude);
		m_intakeController->Set(magnitude/2);
	}
}
void ShooterSubsystem::SpinUp()
{
	printf("Spinning the Flywheel up to Normal RPM");
	m_flywheel->SpinUp(); //Sets to run at regular RPM speed
}

void ShooterSubsystem::FireInit()
{
	printf("Moving the Firing Basket to Fire! \n");
	if (m_readyToFire == true)
	{
		m_readyToFire = false;
		m_firingTimer->Reset();
		m_firingTimer->Start(); //Start counting til next fire
		m_basketSolenoid->Set(true); //Moves the basket up
	}
}

void ShooterSubsystem::FirePeriodic()
{
	if (m_firingTimer->Get() >= c_basketUpPeriod)
	{
		m_basketSolenoid->Set(false); //Moves the basket down
	}
	if (m_firingTimer->Get() >= c_firingInterval)
	{
		m_firingTimer->Stop();
		m_firingTimer->Reset();
		m_readyToFire = true;
	}
}

void ShooterSubsystem::Brake()
{
	printf("Braking the Flywheel! \n");
	m_flywheel->BrakeInit();
}

void ShooterSubsystem::ReleaseIntakes()
{
	m_intakeReleaseSolenoid->Set(true); //Retract the solenoid so the intakes pop out
	m_intakesReleased = true;
}

void ShooterSubsystem::AutoHoodSet(float value)
{
	m_innerHood->SetPointInit(value,true);
}

bool ShooterSubsystem::GetIsFlywheelUpToSpeed()
{
	if(m_flywheel->GetRPM() > 4200)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*
void ShooterSubsystem::Calibrate()
{
	//innerSwivel->Calibrate();
}*/
