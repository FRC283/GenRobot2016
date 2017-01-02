/*
 * CameraTurret.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: FRC283
 */

#include <CameraTurret.h>

CameraTurret::CameraTurret(uint32_t xPort, uint32_t yPort)
{
	m_xAxisServo = new Servo(xPort);
	m_yAxisServo = new Servo(yPort);
	m_rescaler = new Rescaler();

	//Camera Items
	m_frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);

	m_imaqError = IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &m_session);
	if(m_imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)m_imaqError) + "\n");
	}
	m_imaqError = IMAQdxConfigureGrab(m_session);
	if(m_imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)m_imaqError) + "\n");
	}
}

CameraTurret::~CameraTurret() {}

void CameraTurret::Periodic()
{
	SmartDashboard::PutNumber("m_xAxisServo->Get()", m_xAxisServo->Get());
	SmartDashboard::PutNumber("m_yAxisServo->Get()", m_yAxisServo->Get());
	SmartDashboard::PutNumber("Camera: X Axis Degrees", GetXDegrees());
	SmartDashboard::PutNumber("Camera: Y Axis Degrees", GetYDegrees());

	IMAQdxStartAcquisition(m_session);

	IMAQdxGrab(m_session, m_frame, true, NULL); //Take image from session (the stream) and turn it into a single frame
	if(m_imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)m_imaqError) + "\n");
	}
	else
	{
		//Draw reticle on camera image
		//imaqDrawShapeOnImage(frame, frame, { 275, 160, 3, 320}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_RECT, 200.0f);
		imaqDrawShapeOnImage(m_frame, m_frame, { 239, 160, 3, 320}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_RECT, 200.0f);
		imaqDrawShapeOnImage(m_frame, m_frame, { 187, 319, 107, 3}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_RECT, 200.0f);

		CameraServer::GetInstance()->SetImage(m_frame);
	}
}

void CameraTurret::SetXSpeed(float magnitude, bool faster)
{
	magnitude = m_rescaler->Rescale(c_deadzone, 1.0, 0.0, 1.0, magnitude); //Account for deadzone
	magnitude = faster ? magnitude : magnitude / 2; //If faster is true, dont run at half speed
	//printf("Magnitude used to Modify X Axis: %f \n", magnitude);
	SetXPoint(m_xAxisServo->Get() + (-magnitude * c_xSpeedDelta)); //Add or subtract a distance proportional to the magnitude
	//printf("Current X Axis Servo Value: %f \n", m_xAxisServo->Get());
}

void CameraTurret::SetYSpeed(float magnitude, bool faster)
{
	magnitude = m_rescaler->Rescale(c_deadzone, 1.0, 0.0, 1.0, magnitude); //Account for deadzone
	//printf("Magnitude used to Modify Y Axis: %f \n", magnitude);
	magnitude = faster ? magnitude : magnitude / 2; //If faster is true, dont run at half speed
	SetYPoint(m_yAxisServo->Get() + (-magnitude * c_ySpeedDelta)); //Add or subtract a distance proportional to the magnitude
	//printf("Current Y Axis Servo Value: %f \n", m_yAxisServo->Get());
}

void CameraTurret::SetXPoint(float point)
{
	m_xAxisServo->Set(point);
}

void CameraTurret::SetYPoint(float point)
{
	float setPoint = point;
	setPoint = (point > c_maxYPoint) ? c_maxYPoint : point;
	setPoint = (point < c_minYPoint) ? c_minYPoint : point;
	m_yAxisServo->Set(setPoint);
}

float CameraTurret::GetX()
{
	return m_xAxisServo->Get();
}

float CameraTurret::GetXDegrees()
{
	//return ((261.13 * (m_xAxisServo->Get() * m_xAxisServo->Get())) + (765.64 * m_xAxisServo->Get()) - 402.81); //Derived from testing, stolen from old code
	//^ This is used to directly map to swivel values - never enable this
	return (((m_xAxisServo->Get() / 0.005205) - 90) * -1) + 15; //Conversion taken from old code
}

float CameraTurret::GetY()
{
	return m_yAxisServo->Get();
}

float CameraTurret::GetYDegrees()
{
	return ((m_yAxisServo->Get() -.45)/0.00434) + 4; //conversions are ripped from the old code
}

void CameraTurret::Home()
{
	m_xAxisServo->Set(c_xAxisServoHomePoint);
	m_yAxisServo->Set(c_yAxisServoHomePoint);
}

void CameraTurret::ClearLowbar()
{
	m_yAxisServo->Set(c_lowbarClearPoint);
}
