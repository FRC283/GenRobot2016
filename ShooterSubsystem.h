/*
 * ShooterSubsystem.h
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#ifndef SRC_SHOOTERSUBSYSTEM_H_
#define SRC_SHOOTERSUBSYSTEM_H_
#include <cmath>
#include "CameraTurret.h"
#include "InnerHood.h"
#include "InnerSwivel.h"
#include "Flywheel.h"
#include "Constants.h"
#include "Rescaler.h"

/*
 * TODO: (Master)
 * Link between camera and turret
 * Note:
 * 		All Splitfs[with lockout possibilities]:
 *	 	 SetRPM[braking](done),
 *	 	 Brake[none],
 *	 	 Setpoint
 *	 	 	hood[clears]
 *	 	 	swivel[home]
 *
 * Last Things:
 * Discuss what should be lockout'd, what should be automatic (Intake()?)
 * New Coding Conventions
 * 	-change in class c_'s to m_'s - have shaq help, show him how this code works - waiting on convention clarification
 *
 * Ask driveteam if they want to fire basket on release of button
 * Make sure ultrasonic ball detection works
 *
 */

//Pickup Spot:

class ShooterSubsystem
{
	public:
		ShooterSubsystem();
		virtual ~ShooterSubsystem();
		void Periodic();
		void Aim(float xMagnitude, float yMagnitude, bool faster); //Allows you to aim the camera turret and sync the inner turret with it
		void Manual(float xMagnitude, float yMagnitude); //Lets you take manual control of inner turret
		void HomeCamera(); //Homes the camera turret
		void ClearIntake(); //Clears the hood and swivel into good positions for intaking
		void ClearLowbar(); //Clears the hood for the lowbar
		void Intake(float magnitude); //Allows you to control the intakes
		void SpinUp(); //Spins the flywheel up to normal speed
		void FireInit(); //Temporarily moves the basket up, launching the ball
		void FirePeriodic(); //Checks to see when the timer has passed the firing interval. IS RAN IN PERIODIC()
		void Brake(); //Carefully stops the flywheel to prevent feedback
		void ReleaseIntakes(); //Releases the intakes from their bound start position
		void AutoHoodSet(float value);
		bool GetIsFlywheelUpToSpeed();
		///DEPRECIATED void Calibrate(); //Calibrates the encoders of the inner turret  // It's deprecated...
	private:
		bool HasBoulder() //Checks if a boulder is present or not
		{
			return m_boulderUltrasonic->GetVoltage() < c_ballUltrasonicVoltage;
		}

		float CorrectionalTrigonometry(float xCameraAngle, float yCameraAngle) //Horizontal Trig math encapsulated
		{
			//Get everything to rads
			printf("CorrectionalTrig: Incoming xCameraAngle: %f \n", xCameraAngle);
			xCameraAngle = xCameraAngle * c_degToRads;
			yCameraAngle = yCameraAngle * c_degToRads;
			//Calculate the current distance from the goal
			float dist = c_heightOfGoal / (tan(yCameraAngle)); //Distance in a direct line between the camera turret to the goal
			//Then calculate the proper offset for the x axis
			float b = abs(sin(xCameraAngle) * dist); //The horizontal length of the whole box. See the original trig drawing for reference.
			printf("CorrectionalTrig: b: %f \n", b);
			float r = abs(cos(xCameraAngle) * dist); //The purely forward distance from the camera turret to the goal
			float correctedAngle = 0; //new angle for the inner turret
			if (xCameraAngle < 0 && b > c_c2) //BOUND 1; Aiming to the left of the inner turret
			{
				printf("CorrectionalTrig: Angles are in  Bound 1 \n");
				correctedAngle = -1 * atan((b - c_c2) / (r + c_c3)); //Should be a negative angle
			}
			else if (xCameraAngle < 0 && b <= c_c2) //BOUND 2; Aiming to an area between the camera and inner turret
			{
				printf("CorrectionalTrig: Angles are in Bound 2 \n");
				correctedAngle = atan((c_c2 - b) / (r + c_c3)); //Should be a slightly positive angle
			}
			else if (xCameraAngle >= 0) //BOUND 3; Aiming to the right of the camera
			{
				printf("CorrectionalTrig: Angles are in Bound 3 \n");
				correctedAngle = atan((b + c_c2) / (r + c_c3)); //Should be a positive angle
			}
			else
			{
				printf("CorrectionalTrig: ERROR!: Camera angles do not fall within any bounds \n");
				printf("CorrectionalTrig: because of this, the corrected Angle is being set to 0 \n");
			}
			//Then return the corrected angle, in degrees
			correctedAngle = correctedAngle * c_radsToDeg;
			printf("CorrectionalTrig: The new angle for the inner turret is %f \n", correctedAngle);
			return correctedAngle;
		}

		VictorSP *m_intakeController; //Controls the roller intakes on the robot
		CameraTurret *m_cameraTurret; //Smaller Camera Turret on the Robot
		InnerHood *m_innerHood; //The y-rotational of the inner turret
		InnerSwivel *m_innerSwivel; //The x-rotational of the inner turret
		Solenoid *m_basketSolenoid; //Controls up and down motion of basket
		Solenoid *m_intakeReleaseSolenoid; //Holds the intakes in at the start of the game
		Flywheel *m_flywheel; //Controls setting speed of flywheel
		AnalogInput *m_boulderUltrasonic; //Checks if boulder is in basket - NOTE : Currently not wired

		Rescaler *m_rescaler; //Used to deadzone intake rollers
		Timer *m_firingTimer; //Used to space out firing
		bool m_readyToFire; //Whether or not we are ready to fire again
		bool m_intakesReleased; //Whether or not the intakes have been released - true is out, false is in

		//Constants
		const float c_ballUltrasonicVoltage = 0.3; //Value stolen from old code in "Robot.cpp"
		const float c_intakeDeadzone = 0.1;
		const float c_basketUpPeriod = 0.75; //Time that the basket stays up
		const float c_firingInterval = 1; //Time between fires

			//Trig Constants - Values stolen from old code
			const double c_c1  								= 11.458; //Inches directly from the camera to inner turret
			const double c_c2  								= 5.775; //Inches horizontally between the camera and inner turret
			const double c_c3 								= 9.896; //Inches vertically between the camera and inner turret
			const float c_heightOfGoal						= 70.331; //Ripped from old code, presumably inches
			const float c_radsToDeg							= 180 / M_PI; //Multiply by this
			const float c_degToRads							= M_PI / 180; //Multiply by this
};

#endif /* SRC_SHOOTERSUBSYSTEM_H_ */
