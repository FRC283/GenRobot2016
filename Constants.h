/*
n * Constants.h
 *
 *  Created on: Jun 20, 2016
 *      Author: FRC283
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_
#include "WPILib.h"
//====Computer Ports====
	//Joystick ports
		const uint32_t c_leftJoystickPort					= 0;
		const uint32_t c_rightJoystickPort 					= 1;
		const uint32_t c_xboxPort 							= 2;
	//Joystick Channels
		//Axes
			const uint32_t c_leftXAxis								= 0;
			const uint32_t c_leftYAxis								= 1;
			const uint32_t c_leftTrigger							= 2;
			const uint32_t c_rightTrigger							= 3;
			const uint32_t c_rightXAxis 							= 4;
			const uint32_t c_rightYAxis 							= 5;
		//Buttons
			//X-Box
				const uint32_t c_a										= 1;
				const uint32_t c_b										= 2;
				const uint32_t c_x										= 3;
				const uint32_t c_y										= 4;
				const uint32_t c_leftBumper								= 5;
				const uint32_t c_rightBumper							= 6;
				const uint32_t c_rightAxisButton						= 10;
			//True Joysticks
				const uint32_t c_stickTrigger								= 1;


//====Robot Ports====
	//Digital
		const uint32_t c_turretSwivelLeftLimSwitch 				= 12;
		const uint32_t c_turretSwivelRightLimSwitch 			= 13;
		const uint32_t c_turretSwivelEncoderPortA 				= 0;
		const uint32_t c_turretSwivelEncoderPortB				= 1;
		const uint32_t c_flywheelEncoderPortA					= 2;
		const uint32_t c_flywheelEncoderPortB					= 3;
		const uint32_t c_leftDriveEncoderPortA 					= 4;
		const uint32_t c_leftDriveEncoderPortB 					= 5;
		const uint32_t c_rightDriveEncoderPortA					= 6;
		const uint32_t c_rightDriveEncoderPortB					= 7;

	//PWM
		const uint32_t c_turretSwivelControllerPort				= 6;
		const uint32_t c_turretHoodControllerPort				= 7;
		const uint32_t c_flywheelControllerPort					= 8;
		const uint32_t c_cameraXAxisServoPort					= 4;
		const uint32_t c_cameraYAxisServoPort					= 5;
		const uint32_t c_rollerIntakeControllerPort				= 2;
		const uint32_t c_leftDriveMotorControllerPort			= 0;
		const uint32_t c_rightDriveMotorControllerPort			= 1;
		const uint32_t c_liftMotorPort			 				= 3;

	//Analog
		const uint32_t c_turretHoodPotPort   					= 0;
		const uint32_t c_boulderUltrasonicPort					= 2;

	//Pneumatics Module
		const uint32_t c_basketSolenoidPort						= 7;
		const uint32_t c_driveShiftSolenoidPort					= 0;
		const uint32_t c_unpinSolenoidPort 						= 2;
		const uint32_t c_lockingSolenoidPort 					= 3;
		const uint32_t c_hookReleaseSolenoidPort 				= 1;
		const uint32_t c_intakeReleaseSolenoidPort				= 5;

//====Misc. Constants====
	//Physical Dimensions
	//Offsets
		//float c_hoodOffset        								= .0585;
	//Autonomous
		//float c_autoHoodPosition										= .95; //.95 is a test value
	//Buttons/Axes
		const float c_triggerPoint							= 0.9;

#endif /* SRC_CONSTANTS_H_ */
