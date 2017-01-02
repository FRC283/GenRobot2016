/*
 * Swivel.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#include "InnerSwivel.h"

InnerSwivel::InnerSwivel(uint32_t leftLimPort, uint32_t rightLimPort, uint32_t encoderPortA, uint32_t encoderPortB, uint32_t controllerPort)
{
	m_controller = new VictorSP(controllerPort);
	m_encoder = new Encoder(encoderPortA, encoderPortB);
	//CURRENTLY DISABLED leftLimitSwitch = new DigitalInput(leftLimPort);
	//CURRENTLY DISABLED rightLimitSwitch = new DigitalInput(rightLimPort);
	m_setPointTimer = new Timer();
	m_rescaler = new Rescaler();

	m_setPoint = 0;
	m_setPointRunning = false;
}

InnerSwivel::~InnerSwivel() {}

void InnerSwivel::Periodic()
{
	SmartDashboard::PutNumber("Inner Swivel: Current Point", m_encoder->Get());
	SmartDashboard::PutNumber("Inner Swivel: Current Power", m_controller->Get());
	SmartDashboard::PutBoolean("Inner Swivel: Is Home", IsHome());

	SetPointPeriodic();
}

float InnerSwivel::Get()
{
	return m_encoder->Get();
}

void InnerSwivel::SetSpeed(float magnitude)
{
	//First, rescale the values for the deadzone
	magnitude = m_rescaler->Rescale(c_deadzone, 1.0, 0 , 1.0, magnitude);
	printf("Value being sent to Swivel: %f \n", magnitude * c_speedCoeff);
	printf("Current Encoder Value of Swivel: %i \n", m_encoder->Get());
	//Then, check the limits

	if(m_encoder->Get() <= c_lowerLimit) //At or beyond the left limit
	{
		printf("At left limit \n");
		if(magnitude < 0) //Moving to the right
		{
			printf("but swivel allowing to move right\n");
			m_controller->Set(-magnitude * c_speedCoeff); //Allow it to move
		}
		else
		{
			m_controller->Set(0); //Otherwise stop it
		}
	}
	if(m_encoder->Get() >= c_upperLimit) //Touching the switch that limits rightward movement
	{
		printf("At right limit \n");
		if(magnitude > 0) //Moving to the left
		{
			printf("but swivel allowing to move left\n");
			m_controller->Set(-magnitude * c_speedCoeff); //Allow it to move
		}
		else
		{
			m_controller->Set(0); //Otherwise stop it
		}
	}
	if(m_encoder->Get() < c_upperLimit && m_encoder->Get() > c_lowerLimit) //If neither switch is being pressed
	{
		printf("Regular Movement \n");
		m_controller->Set(-magnitude * c_speedCoeff);
	}
}

void InnerSwivel::SetPointInit(float point, bool override)
{
	//Due to a design decision, all SetPointInits are effectively called with true
	override = true;
	printf("Swivel: SetPointInit() \n");
	if(m_setPointRunning == false || override == true)
	{
		m_setPointRunning = true;
		m_setPoint = point;
		printf("Swivel: Setting point to: %f \n", point);
		m_setPointTimer->Start();
	}
}

void InnerSwivel::SetPointPeriodic()
{
	if (m_setPointRunning == true) //If we're trying to p-control and we havent timed out
	{
		if (m_setPointTimer->Get() > c_setPointTimeout)
		{
			printf("Swivel: Timed out \n");
			m_controller->Set(0);
			m_setPointRunning = false;
		}
		m_setPoint = m_setPoint > c_upperLimit ? c_upperLimit : m_setPoint;
		m_setPoint = m_setPoint < c_lowerLimit ? c_lowerLimit : m_setPoint;
		float err = m_setPoint - m_encoder->Get(); //Difference between the target point and current point, must be in this order to preserve sign
		printf("Swivel: P-Control Error: %f \n", err);
		if(err < c_minErr && err > -c_minErr) //If we're really close to the target
		{
			printf("Swivel: PControl to point %f is now Finished \n", m_setPoint);
			m_controller->Set(0); //Stop
			m_setPointRunning = false; //Stop running p-control
		}
		else
		{
			m_controller->Set(err * c_pCoeff); //Allow to move
		}
		printf("Swivel: Controller being set to %f \n", m_controller->Get());



		/* Depreciated temporarily
		 * else
		{
			//Below Logic: Making sure we don't try to move past limits
			if(encoder->Get() <= c_lowerLimit) //At left limit
			{
				if(err * c_pCoeff > 0) //Moving to the right
				{
					controller->Set(err * c_pCoeff); //Allow to move
				}
				else
				{
					controller->Set(0); //Otherwise stop it
				}
			}
			if(encoder->Get() >= c_upperLimit) //Touching right switch
			{
				if(err * c_pCoeff < 0) //Moving to the left
				{
					controller->Set(err * c_pCoeff); //Allow to move
				}
				else
				{
					controller->Set(0); //Otherwise stop it
				}
			}
		}*/
	}
}

bool InnerSwivel::IsHome()
{
	return (m_encoder->Get() < c_minErr) || (m_encoder->Get() > -c_minErr); //Since home is 0, minErr can be used as tolerance
}

void InnerSwivel::Home()
{
	SetPointInit(0, true); //0 is center of turret
}

float InnerSwivel::DegreesToTicks(float degrees)
{
	printf("InnerSwivel: Degree %f ", degrees); printf("was converted to tick: %f \n", degrees * -5.55);
	return degrees * -5.55; //Ripped from old code
}









/* Calibrate() is disabled because without limit switches being wired in, it is not possible
 *
 * Please manually home the swivel before start up while
 *
 *
 *
 *
 * void InnerSwivel::Calibrate() //Nothing else should go on while calibrating - this is not a split function it locks you out of controls while executing
{

	 * Here's how calibration works:
	 * Encoders forget their value when you restart the robot.
	 * This means if the swivel ends on something other than the center the code will be messed up.
	 * We can't set a value for the encoder: Only reset it
	 * SO:
	 * First we reset the encoder. Then we creep to the left limit switch.
	 * Once we hit the left Limit switch, we can move slightly faster to the right limit switch because we know the distance
	 * Then we know the distance between these two limit switches: Halfway is the center, so we move to the center.
	 * Finally, we reset the encoder one more time and we're finished.
	 * We have an turret set to 0 in the center


	encoder->Reset(); //Set the encoder value to zero
	while (leftLimitSwitch->Get() == true) //While the left limit switch is not pressed
		controller->Set(-0.3); //Start slowly moving to the left until the left limit switch is pressed
	while (rightLimitSwitch->Get() == true) //While the right limit switch is not pressed
		controller->Set(0.3); //Start moving to the right until the right limit switch is pressed
	SetPointInit(encoder->Get()/2, true); //Go to halfway between
	encoder->Reset(); //Reset the encoder once more
}*/
