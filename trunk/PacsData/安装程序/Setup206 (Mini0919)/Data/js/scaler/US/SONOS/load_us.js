LoadConfig('us.config');
Event(7,GetInfo(203),GetInfo(204),0,0);

var a1=GetInfo(205);
var a2=GetInfo(206);
var a3=GetInfo(200)
var a4=Event(9,a1,a2,a3,0);
var a5=(LOWORD(a4)%4);

if(a5==0)
{
	Event(7,GetInfo(203),GetInfo(219),0,0);
	
	a1=GetInfo(207);
	a2=GetInfo(208);
	a3=GetInfo(202)
	a4=Event(12,a1,a2,a3,10);

	if(a4>0)
	{
		a4=10/a4;
		var strtext='2D:ÿ�����'+a4+'mm';
		Message(strtext);
		Event(4,a4,a4,0,0);
	}
	else
		Message('2D�Զ�����ʧ�ܣ�����');
}
else
if(a5==1)
{
	var	a6=HIWORD(a4);

	a1=GetInfo(213);//M-MODE
	a2=GetInfo(214);
	a3=GetInfo(201)
	
	a5=Event(1,a1,a2,a3,0);

	if(a6>0&&a5>0)
	{
		var strtext;
		a6=0.2/a6;
		a5=10/a5;
		strtext='M-MODE����:ÿ�����'+a5+'mm\n';
		strtext=strtext+'M-MODEʱ��:ÿ�����'+a6+'s';
		Message(strtext);
		Event(4,a6,a5,0,0);
	}
	else
		Message('M-MODE�Զ�����ʧ�ܣ�����');
}
else
if(a5==2)
{
	var	a6=HIWORD(a4);
	var	a7=LOWORD(a4)/4;

	SetInfo(240,a7);//����

	a3=GetInfo(200);
	a4=Event(13,0,a7,a3,0);

	if(HIWORD(a4)>(GetInfo(9)/2))
	{
		a1=GetInfo(211);//DOPPLER
		a2=GetInfo(212);
	}
	else
	{
		a1=GetInfo(209);//DOPPLER
		a2=GetInfo(210);
	}
	a3=GetInfo(201);
	a4=Event(1,a1,a2,a3,0);

	if(a4>0&&a6>0)
	{
		a6=0.2/a6;
		if(GetString('����DOPPLER�ٶ��ᵥλֵ','0'))
		{
			var textstr=ResultString();
			var db=eval(textstr);
			if(db>0)
			{
				a4=db/a4;
				Event(4,a6,a4,0,0);
				strtext='����'+a7+'\n';
				strtext=strtext+'DOPPLER�ٶ�:ÿ�����'+a4+'m/s\n';
				strtext=strtext+'DOPPLERʱ��:ÿ�����'+a6+'s';
				Message(strtext);
			}
			else
				Message('DOPPLER�Զ�����ʧ��1������');
		}
		else
			Message('DOPPLER�Զ�����ʧ��2������');
	}
	else
		Message('DOPPLER�Զ�����ʧ��3������');
}
