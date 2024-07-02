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

	������������ѣ���С����˳���
	���Ǳ�������֣�����ٰ�΢�ּ�
	�����񵴺�Ƶ������������Ҫ�Ŵ�
	����Ư���ƴ��壬����������С��
	����ƫ��ظ���������ʱ�����½�
	���߲������ڳ�������ʱ���ټӳ�
	������Ƶ�ʿ죬�Ȱ�΢�ֽ�����
	���������������΢��ʱ��Ӧ�ӳ�
	����������������ǰ�ߺ���ı�һ
	һ��������������������������



P�ǽ����ֵ�𵴣�P���˻���ַ�ֵ�𵴵ķ��ȴ󣬵���Ƶ��С��ϵͳ�ﵽ�ȶ�ʱ�䳤��
I�ǽ��������Ӧ���ٶȿ����ģ�I������Ӧ�ٶ�������֮��죻
D��������̬���ģ�һ��D���ö��Ƚ�С�����Ҷ�ϵͳӰ��Ƚ�С��
�����¶ȿ���ϵͳP��5-10%֮�䣻I��180-240s֮�䣻D��30���¡�
����ѹ������ϵͳP��30-60%֮�䣻I��30-90s֮�䣻D��30���¡� 
*/
