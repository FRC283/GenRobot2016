/*
 * InnerHood.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#include "InnerHood.h"
#include "Constants.h"
InnerHood::InnerHood(uint32_t potPort, uint32_t controllerPort)
{
	m_controller = new VictorSP(controllerPort);
	m_pot = new AnalogInput(potPort);
	m_setPointTimer = new Timer();
	m_rescaler = new Rescaler();

	m_setPoint = 0;
	m_setPointRunning = false;
}

InnerHood::~InnerHood() {}

void InnerHood::Periodic()
{
	SmartDashboard::PutNumber("Inner Hood: Current Voltage", m_pot->GetVoltage());
	SmartDashboard::PutNumber("Inner Hood: Current Power", m_controller->Get());
	SmartDashboard::PutBoolean("Inner Hood: Clear for Low Bar", IsClear());
	SmartDashboard::PutBoolean("Inner Hood: Clear to Intake Ball", IsClearForBall());

	SetPointPeriodic();
}

void InnerHood::SetSpeed(float magnitude)
{
	//First, rescale the values for the deadzone
	magnitude = m_rescaler->Rescale(c_deadzone, 1.0, 0, 1.0, magnitude);
	printf("Value being sent to inner hood: %f \n", magnitude * c_speedCoeff);
	//Then, check the limits
	printf("Hood above Upper Limit? %i \n", m_pot->GetVoltage() < c_upperPhysLimit);
	printf("Hood below lower Limit? %i \n", m_pot->GetVoltage() > c_lowerPhysLimit);

	if(m_pot->GetVoltage() > c_upperPhysLimit) //If we're above the upper limit
	{
		printf("Hood above upper limit! \n");
		if(magnitude < 0) //Moving down
		{
			printf("Hood being allowed to move down \n");
			m_controller->Set(magnitude * c_speedCoeff); //Allow it to move
		}
		else
		{
			m_controller->Set(0); //Otherwise stop it
		}
	}
	if(m_pot->GetVoltage() < c_lowerPhysLimit) //If we're below the lower limit
	{
		printf("Hood below lower limit! \n");
		if(magnitude > 0) //Moving up
		{
			printf("Hood being allowed to move up \n");
			m_controller->Set(magnitude * c_speedCoeff); //Allow it to move
		}
		else
		{
			m_controller->Set(0); //Otherwise stop it
		}
	}
	if (m_pot->GetVoltage() < c_lowerPhysLimit && m_pot->GetVoltage() > c_upperPhysLimit) //If in a normal range of values
	{
		printf("Normal Hood Range \n");
		m_controller->Set(magnitude * c_speedCoeff);
	}
}

void InnerHood::SetPointInit(float point, bool override)
{
	//Due to a design decision, all SetPointInits are effectively called with true
	override = true;
	printf("Hood: SetPointInit() \n");
	if(m_setPointRunning == false || override == true) //Override forces the loop to start going to the new value
	{
		m_setPointRunning = true;
		m_setPoint = point;
		m_setPoint = m_setPoint - .0585; // up is down and down is up
		printf("Hood: P-Control Point set to: %f \n", m_setPoint);
		m_setPointTimer->Start();
	}
}

void InnerHood::SetPointPeriodic()
{
	if (m_setPointRunning == true) //If we're trying to p-control and we havent timed out
	{
		if((m_setPointTimer->Get() >= c_setPointTimeout))
		{
			printf("Hood: P-Control timed out \n");
			m_controller->Set(0);
			m_setPointRunning = false;
		}

		if (m_setPoint < c_upperPhysLimit) m_setPoint = c_upperPhysLimit; //If we try to go above max, set to max
		if (m_setPoint > c_lowerPhysLimit) m_setPoint = c_lowerPhysLimit; //If we try to go below min, set to min
		float err = m_setPoint - m_pot->GetVoltage(); //Difference between the target point and current point, must be in this order to preserve sign
		printf("Hood: P-Control error is %f \n", err);
		if(err < c_minErr && err > -c_minErr) //If we're really close to the target
		{
			printf("Hood: P-Control to point %f is now finished \n", m_setPoint);
			m_controller->Set(0); //Stop
			m_setPointRunning = false; //Stop running p-control
		}
		else
		{
			m_controller->Set(err * c_pCoeff); //If we're not close, recalculate power
			printf("Hood: P-Control is setting controller power to %f \n", m_controller->Get());
		}

	printf("Hood: Current Voltage: %f \n", m_pot->GetVoltage());
	printf("Hood: Current Power: %f \n", m_controller->Get());
	}
}

void InnerHood::Clear() //Clears for lowbar
{
	SetPointInit(c_lowerPhysLimit +  .0585, true); //Move to the point, but account for the min err, WITH OVERRIDE
}

bool InnerHood::IsClear()
{
	return m_pot->GetVoltage() > c_lowerPhysLimit + .0585; //If the hood is below the point for going under the low bar
}

void InnerHood::ClearForBall()
{
	SetPointInit(c_ballPoint + .0585, true); //Move to the point, but account for the min err, WITH OVERRIDE
}

bool InnerHood::IsClearForBall()
{
	return m_pot->GetVoltage() > c_ballPoint + .0585; //If the hood is above what it must be to carry a ball
}

float InnerHood::CameraYValuesToVoltage(float cameraYValue)
{
	float newValue = ((46.307 * (cameraYValue  * cameraYValue  * cameraYValue)) - (83.14 * (cameraYValue  * cameraYValue)) + (50.102 * cameraYValue) - 9.2031); //Derived from testing, stolen from old code
	printf("InnerHood: Camera Y value %f ", cameraYValue); printf("was converted to voltage: %f \n", newValue);
	return newValue;
}

