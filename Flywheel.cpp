/*
 * Flywheel.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#include "Flywheel.h"

Flywheel::Flywheel(uint32_t encoderPortA, uint32_t encoderPortB, uint32_t controllerPort)
{
	m_encoder = new Encoder(encoderPortA, encoderPortB);
	m_controller = new VictorSP(controllerPort);
	m_brakeRunning = false;
	m_targetRPM = 0;
}

Flywheel::~Flywheel() {}

void Flywheel::Periodic()
{
	SmartDashboard::PutNumber("Flywheel RPM", GetRPM());
	SmartDashboard::PutNumber("Flywheel Power", m_controller->Get());

	BrakePeriodic();
	SetRPMPeriodic();
}

void Flywheel::BrakeInit()
{
	if (m_brakeRunning == false)
	{
		m_brakeRunning = true;
		m_targetRPM = 0;
	}
}

void Flywheel::BrakePeriodic()
{
	if (m_brakeRunning == true)
	{
		printf("Current Flywheel POWER: %f \n", m_controller->Get());
		printf("Current Flywheel RPM: %f \n", GetRPM());
		m_controller->Set(m_controller->Get() + c_brakingConstant); //Each cycle of the robot, decrease the power of the flywheel
		if (m_controller->Get() >= 0)
		{
			m_controller->Set(0);
			m_brakeRunning = false;
			printf("FINISHED BRAKING! \n");
		}
	}
}

void Flywheel::SetRPMInit(float RPM)
{
	m_targetRPM = RPM;
}

void Flywheel::SetRPMPeriodic() //This function will essentially always run
{
	if(m_brakeRunning == false)
	{
		printf("Current Flywheel POWER: %f \n", m_controller->Get());
		printf("Current Flywheel RPM: %f \n", GetRPM());

		//Trying to get to 0 rpm requires a special case
		float basePower = (m_targetRPM == 0) ? 0 : m_nominalPower;
		//^ If we are trying to go to 0 rpm, we need to work off a base power of 0 instead of the nominal rpm power
		//Most other RPMs work ok off of the nominal power, but the system is designed to run at the normal rpm at the nominal power when 0 error

		m_controller->Set(basePower + (-1 * (m_targetRPM - GetRPM()) * c_flywheelPCoeff)); //See chart on header for sign explanation

	}
	//Do not add an else setting to 0
}

void Flywheel::SpinUp()
{
	m_brakeRunning = false;
	SetRPMInit(c_nominalRPM);


}

float Flywheel::GetRPM()
{
	return m_encoder->GetRate() * 3; //According to the old code, this will return the rpm of the encoder
}
