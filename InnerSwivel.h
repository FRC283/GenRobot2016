/*
 * Swivel.h
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#ifndef SRC_INNERSWIVEL_H_
#define SRC_INNERSWIVEL_H_
#include "WPILib.h"
#include "Rescaler.h"
class InnerSwivel
{
	public:
		InnerSwivel(uint32_t leftLimPort, uint32_t rightLimPort, uint32_t encoderPortA, uint32_t encoderPortB, uint32_t controllerPort);
		virtual ~InnerSwivel();
		void Periodic(); //Contains all periodics and smartdashboards
		float Get();
		void SetSpeed(float magnitude);
		void SetPointInit(float point, bool override); //Override forces the value to change to that one
		void SetPointPeriodic();
		bool IsHome(); //Determines if the x swiveling of the turret is aligned forwards
		void Home(); //Homes the turret
		float DegreesToTicks(float degrees); //Converts a very specific set of degrees into equivalent tick position
		//DEPRECIATED: void Calibrate(); //Calibrates the swivel encoder
	private:
		VictorSP *m_controller; //Input reversed with respect to xbox left joystick x

		/*			POLARITY CHART (With No Corrections!)
		 *  Value Sent to Motors | Direction of Swivel Movement | Encoder Ticks going... |Switch we are heading towards |Angle is becoming
		 *  Positive	 		 | Right						| Up					 |Left						    |Positive
		 *  Negative			 | Left							| Down					 |Right							|Negative
		 *	----------------------------------------------------------------------------
		 *
		 *	The left limit switch is FALSE when we are at the very LOWER values of the encoder,
		 *	The right limit switch is FALSE when we are at the very UPPER values of the encoder
		 *	Both limit switches are TRUE when we are in a allowed range of values
		 *	This is because they are wired NORMALLY CLOSED (NORMALLY TRUE)
		 */

		Encoder *m_encoder;
		//DEPRECIATED:	//DigitalInput *leftLimitSwitch, *rightLimitSwitch; //Wired Normally Closed. Limit the motions of the swivel to reasonable places.
						//^ The left Limit switch is triggered when we reach the very right side, and the right is trigger when we hit the left limit
		Timer *m_setPointTimer; //Times the p-control to cancel after a certain time periodic
		Rescaler *m_rescaler; //Scales deadzone values to be smooth

		//Variables
		float m_setPoint; //Point that setPoint() p-controls to
		bool m_setPointRunning; //Whether or not p-control loop is running

		//Constants
		const float c_setPointTimeout = 20; //Time for the P-Control to Time out
		const float c_pCoeff = 0.01; //Multiplier of error
		const float c_minErr = 13; //Maximum Number of ticks that we can be off
		const float c_speedCoeff = 0.2; //-Normally 0.45- Modifies the value of the manual control speed
		const float c_deadzone = 0.1; //Minimum value to setSpeed
		const float c_upperLimit = 498; //Stolen from old code. Rightmost it is safe to go
		const float c_lowerLimit = -493; //Stolen from old code. Leftmost it is safe to go
};

#endif /* SRC_INNERSWIVEL_H_ */
