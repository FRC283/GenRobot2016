/*
 * Flywheel.h
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#ifndef SRC_FLYWHEEL_H_
#define SRC_FLYWHEEL_H_
#include "WPILib.h"
class Flywheel
{
	public:
		Flywheel(uint32_t encoderPortA, uint32_t encoderPortB, uint32_t controllerPort);
		virtual ~Flywheel();
		void Periodic(); //Contains all periodics and smart dashboards stuff
		void BrakeInit(); //Carefully slows down the speed of the flywheel so as not to feedback into controller
		void BrakePeriodic(); //
		void SetRPMInit(float RPM); //Sends a target RPM to the flywheel
		void SetRPMPeriodic();
		void SpinUp(); //Idles the flywheel at normal speed, quite a violent "idle"
		float GetRPM(); //Gets the RPM Of the flywheel
	private:
		Encoder *m_encoder;
		VictorSP *m_controller; //Needs to run in a negative direction for firing

		/*
		 * Polarity Fed to Motor | RPM is changing towards
		 * Positive				 | Negative
		 * Negative				 | Positive
		 */

		bool m_brakeRunning; //Tracks if wheel is currently trying to slow to a stop
		float m_targetRPM; //Used by SetRPM as a target

		//Constants
		const float c_flywheelPCoeff = 0.001; //Used by SetRPM as a scaling factor
		const float c_brakingConstant = 0.0065; //The amount jumped down each cycle of the robot
		const float c_nominalRPM = 4400; //Stolen from last years
		const float m_nominalPower = -0.9; //The power to maintain a constant nominal rpm formwards
};

#endif /* SRC_FLYWHEEL_H_ */
