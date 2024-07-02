/*
 * KalmanFilter.cpp
 *
 *  Created on: 2024Äê4ÔÂ19ÈÕ
 *      Author: Administrator
 */

#include <KalmanFilter.h>

u32 KalmanFilter::run(u32 Value) {
	float LastData;
	float NowData;
	float kalman_value;
	static float kalman_value_old= 0;
	static float P1;
	static float Q=0.0003;
	static float R=9;
	static float Kg = 0;
	static float P=1;
	NowData = Value;
	LastData =kalman_value_old;
	P = P1 +Q;
	Kg=P/(P+R);
	kalman_value = LastData + Kg*(NowData - kalman_value_old);
	P1=(1-Kg)*P;
	P = P1;
	kalman_value_old=Value;
	return (unsigned long)(kalman_value);
}
