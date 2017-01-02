/*
 * CameraTurret.h
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#ifndef SRC_CAMERATURRET_H_
#define SRC_CAMERATURRET_H_
#include "WPILib.h"
#include "Rescaler.h"

class CameraTurret
{
	//TODO Put in Camera Stuff
	public:
		CameraTurret(uint32_t xPort, uint32_t yPort);
		virtual ~CameraTurret();
		void Periodic(); //Contains all periodics and smartdashboards
		void SetXSpeed(float magnitude, bool faster);
		void SetYSpeed(float magnitude, bool faster);
		//^ The above speed functions make the assumption that the function is called about once per cycle in robot. If this changes, they will need to be split
		void SetXPoint(float point);
		void SetYPoint(float point);
		float GetX(); //Returns raw pwm value
		float GetXDegrees(); //Returns the position of the X axis in terms a very specific angle set used for trig
		float GetY(); //Returns raw pwm value
		float GetYDegrees(); //Returns the position of the Y axis in terms of a very specific angle set used for trig
		void Home();
		void ClearLowbar(); //Move to a position so that the hood will be below the lowbar when it correlates
	private:
		Servo *m_xAxisServo; //180 Degree Servo that controls the horizontal position of the camera. Needs to be set to a negative to correlate with the xbox right x axis
		Servo *m_yAxisServo; //180 Degree Servo that controls the vertical position of the camera. Needs to be set to a negative to correlate with the xbox right y axis
		Rescaler *m_rescaler; //Used to deadzone manual control of axes

		//Items for Camera
		IMAQdxSession m_session;
		Image *m_frame;
		IMAQdxError m_imaqError;

		//Constants
		const float c_xAxisServoHomePoint = 0.5; //Point which is considered "home" for x axis
		const float c_yAxisServoHomePoint = 0.5; //Point which is considered "home" for y axis
		const float c_xSpeedDelta = .013; //Amount the axes jump each cycle
		const float c_ySpeedDelta = .013; //Amount the axes jump each cycle
		const float c_deadzone = 0.1; //Wiggle room on joystick
		const float c_lowbarClearPoint = 0.740; //Point at which, when run back through the formula, will put the hood below the lowbar. Aquired via testing
		const float c_minYPoint = 0.444; //The point at which the camera cannot move past if it wants the hood to follow
		const float c_maxYPoint = 0.732; //The point at which the camera cannot move past if it wants the hood to follow
		//^ It is worth noting that these are not needed for the X-Axis due to how the limits work out
};

#endif /* SRC_CAMERATURRET_H_ */
