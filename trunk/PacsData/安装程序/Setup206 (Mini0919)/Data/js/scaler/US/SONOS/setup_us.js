var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	LoadConfig('us.config');
	Declare('ʱ������ɫ',6,15,-1,1);
	Declare('����/�ٶ�����ɫ',6,15,-1,2);
	Declare('2άͼ(2D)��������ɫ',6,15,-1,3);
	Declare('�޸�ʶ�����',0,0,-1,4);
	Declare('����ʶ��',6,6,-1,5);

	Declare('2άͼ(2D)����(����)',6,6,-1,6);

	Declare('DOPPLER�ٶ��ᶨ��(���)',6,12,-1,7);
	Declare('DOPPLER�ٶ��ᶨ��(�ұ�)',6,12,-1,8);
	
	Declare('M-MODE���붨��(���)',6,12,-1,9);

	//Declare('DOPPLER�̶�ʶ������(���)',6,6,-1,10);
	//Declare('DOPPLER�̶�ʶ������(�ұ�)',6,6,-1,11);

	Declare('ѧϰ����',6,6,-1,12);
	Declare('����ѧϰ',0,0,-1,13);
	
	Declare('��������',0,0,-1,14);
	Declare('װ������',0,0,-1,15);
	Declare('�����Զ�����',0,0,-1,16);
}
else
if(a0<=3)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=GetROIData(2);
			var db=Event(6,a1,0,0,0);
			OutData('��ɫ',db);
			if(a0==1)
				SetInfo(200,db);
			else
			if(a0==2)
				SetInfo(201,db);
			else
			if(a0==3)
				SetInfo(202,db);
		}
	}
}
else
if(a0==4)
{
	if(GetString('ָ��ɫ�̬��Χ',GetInfo(203)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('��Χ1',db);
		SetInfo(203,db);
	}
	if(GetString('ָ�������仯��Χ(DOPPLER���)',GetInfo(204)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('��Χ2',db);
		SetInfo(204,db);
	}
	if(GetString('ָ�������仯��Χ(2D���)',GetInfo(219)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('��Χ3',db);
		SetInfo(219,db);
	}
}
else
if(a0==5)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(9,a1,a2,a3,0);
			OutData('����',(LOWORD(a4)%4));
			OutData('����',(LOWORD(a4)/4));
			OutData('���',(HIWORD(a4)));
			SetInfo(205,a1);//��һ��������
			SetInfo(206,a2);

			a1=(LOWORD(a4)/4);
			a4=Event(13,0,a1,a3,0);
			OutData('���',(LOWORD(a4)));
			OutData('�յ�',(HIWORD(a4)));
		}
	}
}
else
if(a0==6)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(219),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(202)
			var a4=Event(12,a1,a2,a3,10);
			OutData('2D���',a4);
			if(a4)
			{
				SetInfo(207,a1);
				SetInfo(208,a2);
			}
		}
	}
}
else
if(a0>=7&&a0<=9)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==4||GetROIData(0)==8||GetROIData(0)==9)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201)
			var a4=Event(1,a1,a2,a3,0);
			if(a0==8||a0==7)
				OutData('DOPPLER���',a4);
			else
				OutData('M-MODE���',a4);
			
			if(a4>=0)
			{
				if(a0==7)
				{
					SetInfo(209,a1);
					SetInfo(210,a2);
				}
				else
				if(a0==8)
				{
					SetInfo(211,a1);
					SetInfo(212,a2);
				}
				else
				if(a0==9)
				{
					SetInfo(213,a1);
					SetInfo(214,a2);
				}
			}
		}
	}
}
else
if(a0>=10&&a0<=11)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201)
			Event(7,GetInfo(203),GetInfo(204),0,0);
			Event(3,a1,a2,a3,0);
			var textstr=ResultString();
			if(textstr.length>1)
			{
				Message(textstr);
				OutData('�̶�',atoiStr(textstr,'CAL='));
				if(a0==10)
				{
					SetInfo(215,a1);
					SetInfo(216,a2);
				}
				else
				if(a0==11)
				{
					SetInfo(217,a1);
					SetInfo(218,a2);
				}
			}
		}
	}
}
else
if(a0==12)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201);
			Event(10,a1,a2,a3,0);
		}
	}	
}
else
if(a0==13)
{
	Event(11,0,0,0,0);
	Message('����ģ����������뿪ʼ����ѧϰ��');
}
else
if(a0==14)
	SaveConfig('us.config');
else
if(a0==15)
	LoadConfig('us.config');
