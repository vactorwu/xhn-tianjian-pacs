var a0=GetInfo(0);
Event(7,40,2,0,0);
if(a0==0)
{
	SetInfo(7,0);
	
	Clear();
	LoadConfig('us.config');

	Declare('Ŀ��ͼ��ֵ',6,15,-1,1);
	Declare('2άͼ(2D)����(���)',6,5,-1,2);
	Declare('2άͼ(2D)����(�ұ�)',6,5,-1,13);
	Declare('DOPPLER�ٶ��ᶨ��',6,12,-1,3);
	Declare('DOPPLERʱ���ᶨ��',6,11,-1,4);
	Declare('M-MODE���붨��',6,12,-1,5);
	Declare('M-MODEʱ���ᶨ��',6,11,-1,6);
	Declare('����ʶ��',6,6,-1,7);
	Declare('DOPPLER�̶�ʶ������',6,6,-1,8);
	Declare('M-MODE�̶�ʶ������',6,6,-1,9);
	Declare('ѧϰ����',6,6,-1,10);
	Declare('����ѧϰ',0,0,-1,11);
	Declare('��������',0,0,-1,12);
	Declare('װ������',0,0,-1,13);
	Declare('�����Զ�����',0,0,-1,14);
}
else
if(a0==1)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=GetROIData(2);
			var db=Event(6,a1,0,0,0);
			OutData('��ֵ',db);
			SetInfo(200,db);
		}
	}
}
else
if((a0>=2&&a0<=6)||(a0==13))
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==4||GetROIData(0)==8||GetROIData(0)==9)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(1,a1,a2,a3,0);
			if(a0==2||a0==13)
				OutData('2D���',a4);
			else
			if(a0==3||a0==4)
				OutData('DOPPLER���',a4);
			else
				OutData('M-MODE���',a4);
			if(a4>=0)
			{
				if(a0==2)
				{
					SetInfo(201,a1);
					SetInfo(202,a2);
				}
				else
				if(a0==13)
				{
					SetInfo(217,a1);
					SetInfo(218,a2);
				}
				else
				if(a0==3)
				{
					SetInfo(203,a1);
					SetInfo(204,a2);
				}
				else
				if(a0==4)
				{
					SetInfo(205,a1);
					SetInfo(206,a2);
				}
				else
				if(a0==5)
				{
					SetInfo(207,a1);
					SetInfo(208,a2);
				}
				else
				if(a0==6)
				{
					SetInfo(209,a1);
					SetInfo(210,a2);
				}
			}
		}
	}
}
else
if(a0==7)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(2,a1,a2,a3,0);
			OutData('����',LOWORD(a4));
			OutData('����',HIWORD(a4));
			SetInfo(211,a1);
			SetInfo(212,a2);
		}
	}
}
else
if(a0==8||a0==9)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			Event(3,a1,a2,a3,0);
			var textstr=ResultString();
			if(textstr.length>1)
			{
				Message(textstr);
				OutData('�̶�',atoiStr(textstr,'CAL'));
				if(a0==8)
				{
					SetInfo(213,a1);
					SetInfo(214,a2);
				}
				else
				{
					SetInfo(215,a1);
					SetInfo(216,a2);
				}
			}
		}
	}
}
else
if(a0==10)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200);
			Event(10,a1,a2,a3,0);
		}
	}	
}
else
if(a0==11)
{
	Event(11,0,0,0,0);
	Message('����ģ����������뿪ʼ����ѧϰ��');
}
else
if(a0==12)
	SaveConfig('us.config');
else
if(a0==13)
	LoadConfig('us.config');
