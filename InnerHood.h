/*
 * InnerHood.h
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#ifndef SRC_INNERHOOD_H_
#define SRC_INNERHOOD_H_
#include "WPILib.h"
#include "Rescaler.h"
class InnerHood
{
	public:
		InnerHood(uint32_t potPort, uint32_t controllerPort);
		virtual ~InnerHood();
		void Periodic(); //Contains all periodics and Smartdashboards
		void SetPointInit(float point, bool override); //Override cancels any existing point in favor of the new value
		void SetPointPeriodic();
		float Get();
		void SetSpeed(float magnitude);
		void Clear(); //Clears the hood to where it can go under low bar
		bool IsClear(); //Determines if the hood is in a location that could go under the low bar
		void ClearForBall(); //Clears the hood so that a ball can be intaked
		bool IsClearForBall(); //Determines if the hood is high enough up to hold the bar
		float CameraYValuesToVoltage(float cameraYValue); //Converts values on the yAxis of the camera to valid and equivalent hood voltages
	private:
		VictorSP *m_controller; //Positive value moves hood down

		/*			POLARITY CHART (With No Corrections!)
		 *  Value Sent to Motors | Direction of Hood Movement | Voltage on Pot going... |
		 *  Positive	 		 | Up						  | Down					|
		 *	Negative			 | Down						  | Up						|
		 */

		AnalogInput *m_pot;
		Timer *m_setPointTimer; //Times the p-control to cancel after a certain time periodic
		Rescaler *m_rescaler; //Scales deadzone values to be smooth

		//Variables
		float m_setPoint; //Point that setPoint() p-controls to
		bool m_setPointRunning; //Whether or not p-control loop is running

		//Constants
		const float c_setPointTimeout = 20; //Time for the P-Control to Time out
		const float c_pCoeff = 5; //Multiplier of error
		const float c_minErr = 0.01; //Maximum volts that that we can be off
		const float c_speedCoeff = 0.3; //Modifier for the setSpeed speed
		const float c_deadzone = 0.1; //Maximum value to setSpeed
		const float c_ballPoint = 0.97; //TEMP SET TO UPPER PHYS LIMIT - Voltage (Point) at which the hood must be at or above to be able to hold a ball
		const float c_upperPhysLimit = 0.71; //Maximum Voltage (Highest Physical Point) to which the hood can move
		const float c_lowerPhysLimit = 1.10; //Minimum Voltage (Lowest Physical Point) to which the hood can move
		//^ Voltage (Point) at which the hood must be at or below to be clear of the low bar is also the lower limit

};

#endif /* SRC_INNERHOOD_H_ */
