/*
 * Cunchu.h
 *
 *  Created on: 2021��3��13��
 *      Author: Administrator
 */

#ifndef CUNCHU_H_
#define CUNCHU_H_
#include "W25X16.h"
#define __DEVICES__ 100
//#include "AntData_Device.h"
//#include <vector>
/*
 W25 �洢�ṹ˵��
 ����ռ�õ�һ��4K��������������С��λ
�ܹ�16ҳ��256�ֽ�һ��ҳ��
 ��0-10ҳ���96���豸+1����������
  ÿҳ��10���豸
   *
ҳ�����ǲ���4K�ֽڣ�ÿҳ256*16ҳ
 */

typedef struct   //ռ��32���ֽ�
{
	u16 qiyong;//�Ƿ�����
	u16 inNo1;//��λ����վ�ţ�ʮ��λ�����ĸ�����㣨��1��ʼ��
	u16 inNo2;//��λ����վ�ţ�ʮ��λ�����ĸ�����㣨��1��ʼ��
	u16 inNo3;//��λ����վ�ţ�ʮ��λ�����ĸ�����㣨��1��ʼ��
	u16 outNo1;//��λ����վ�ţ�ʮ��λ�����ĸ�����㣨��1��ʼ��
	u16 outNo2;//��λ����վ�ţ�ʮ��λ�����ĸ�����㣨��1��ʼ��
	u16 m_ctrType;///���ֽڴ����Ƿ�Ϊ���߼���ֻӰ���¿ظ����ģʽ�������ֽ�0���ֶ���1ʱ�أ�2�¿أ�3���
	u16 m_huancaiNO;//0-4 ʹ���ĸ�վ���� -1����ϵͳ����
	u16 m_zhengzhuanShijian;//д�˼Ĵ���ʱ��д����Сʱ*100+����
	u16 m_fanzhuanShijian;//д�˼Ĵ���ʱ��д����Сʱ*100+����
	u16 m_gaoShuju;
	u16 m_diShuju;
	u16 m_yunxingSecs;
	u16 m_fenduan;
	u16 m_tmpZhuangtai;//д��Ч
	u16 m_tmpZhengzaiyunxingSec;//д��Ч
} device_Canshu;

typedef struct   //ռ��32���ֽ�
{
	u16 m_addr;//��Ϊ��վʱ��������ַΪ1
	u16 m_qiyongZhanhao;//������һ· �������ݲɼ� bit0-4�ֱ�����Ƿ���1-5վ,Ĭ��ֻ��վ1
	u16 m_huancaiAddr0;//��������վ1��ַ
	u16 m_huancaiAddr1;
	u16 m_huancaiAddr2;
	u16 m_huancaiAddr3;
	u16 m_huancaiAddr4;
	u16 m_huancaiNO;//0-4 Ĭ��ʹ��վ1����
	u16 m_caijiTime; //5��ɼ�1��
	u16 m_yuxue;//��ѩ����������ţ�0������
	u16 m_gensuiFengji;//ˮ�ø���������0�������档
	u16 m_riqiLock;
	u16 m_fenduanJiange;
	u16 m_beiyongU16_2;
	u16 m_beiyongU16_3;
	u16 m_FLAG;//0X55AA
} sys_Canshu;

class Cunchu
{
public:
	Cunchu();
public:
	static u8 cpuUsed;
	static u16 benjiZhuangtai;

	static u8 YuanKong;
	static device_Canshu m_Canshu[__DEVICES__];//100���豸����32�ֽڽṹ
	static sys_Canshu  m_sysCanshu;
public :
	static void WriteCunchu(u32 shoudizhi);
	static void ReadCunchu(u32 shoudizhi);
	static void MorenCanshu();
	static void write();
	static void read();
	static void save();
	static void load();
};

#endif /* CUNCHU_H_ */
