/*
 * Robot.h
 *
 *  Created on: Jul 16, 2016
 *      Author: FRC283
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "WPILib.h"
#include "ShooterSubsystem.h"
#include "DriveSubsystem.h"
#include "LiftSubsystem.h"


class Robot: public IterativeRobot
{
	public:
		Robot();
		void RobotInit();
		void TeleopInit();
		void TeleopPeriodic();
		void AutonomousInit();
		void AutonomousPeriodic();
		void Periodic(); //Runs all the periodic functions of the subsystems
		//^ Should be ran in both other periodics
	private:
		Joystick *m_xbox;
		ShooterSubsystem *m_shooterSubsystem;
		DriveSubsystem *m_driveSubsystem;
		LiftSubsystem *m_liftSubsystem;
		Timer *m_autoTimer;
		enum AutoState{
			kFireCheck,
			kHoodSet,
			kSpinUp,
			kFire,
			kEnd
		};
		AutoState m_autoState;

};



#endif /* SRC_ROBOT_H_ */
