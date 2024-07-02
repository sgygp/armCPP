#include <HeartPID.h>
float HeartPID::pid(float pv,float sv)
{
	float ff;
	if(pv<0)
	{
		m_HeatOut=0;
		return m_HeatOut;
	}
		
//	m_piancha3=m_piancha2;
	m_piancha2=m_piancha1;
	m_piancha1=pv-sv;
	ff=PPPP*m_piancha1+IIII*(m_piancha1-m_piancha2);
//	ff=m_xiuzheng[0]*m_piancha1+m_xiuzheng[1]*(m_piancha1-m_piancha2)+m_xiuzheng[2]*(m_piancha1-2*m_piancha2+m_piancha3);
	ff=m_HeatOut-ff*0.6*5;

	if(ff>=1000)
		m_HeatOut=1000;
	else if(ff<=0)
		m_HeatOut=0;
	else
		m_HeatOut=ff;

	return m_HeatOut;
}

float HeartPID::getHeartOut() const
{
	return m_HeatOut;
}

/*

	参数整定找最佳，从小到大顺序查
	先是比例后积分，最后再把微分加
	曲线振荡很频繁，比例度盘要放大
	曲线漂浮绕大湾，比例度盘往小扳
	曲线偏离回复慢，积分时间往下降
	曲线波动周期长，积分时间再加长
	曲线振荡频率快，先把微分降下来
	动差大来波动慢。微分时间应加长
	理想曲线两个波，前高后低四比一
	一看二调多分析，调节质量不会低



P是解决幅值震荡，P大了会出现幅值震荡的幅度大，但震荡频率小，系统达到稳定时间长；
I是解决动作响应的速度快慢的，I大了响应速度慢，反之则快；
D是消除静态误差的，一般D设置都比较小，而且对系统影响比较小。
对于温度控制系统P在5-10%之间；I在180-240s之间；D在30以下。
对于压力控制系统P在30-60%之间；I在30-90s之间；D在30以下。 
*/
