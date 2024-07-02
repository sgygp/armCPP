/*
 * pid.h
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef PID_H_
#define PID_H_

#define PPPP 3.0
#define IIII 300.0

class HeartPID {
private:

	float m_HeatOut;

	float m_piancha1;
	float m_piancha2 ;
public:
	HeartPID()
	{
		 m_HeatOut=0;
		 m_piancha1 = 0;
		 m_piancha2 = 0;
	};
	float getHeartOut()const;
	float pid(float pv, float sv);

};

#endif /* PID_H_ */
