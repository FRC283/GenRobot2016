/*
 * Rescaler.h
 *
 *  Created on: Jun 25, 2016
 *      Author: FRC283
 */

#ifndef SRC_RESCALER_H_
#define SRC_RESCALER_H_


class Rescaler
{
	public:
		Rescaler();
		virtual ~Rescaler();
		float Rescale(float lowero, float uppero, float lowern, float uppern, float value);
};

#endif /* SRC_RESCALER_H_ */
