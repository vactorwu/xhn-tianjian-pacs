LoadConfig('us.config');

var a1=GetInfo(211);
var a2=GetInfo(212);
var a3=GetInfo(200)
var a4=Event(2,a1,a2,a3,0);
var a5=LOWORD(a4);

if(a5==0)
{
	a1=GetInfo(201);
	a2=GetInfo(202);
	a4=Event(1,a1,a2,a3,0);
	a1=GetInfo(217);
	a2=GetInfo(218);
	a5=Event(1,a1,a2,a3,0);
	if(a4>0&&a5>0)
		if(a4<a5)
			a4=a5;
	if(a4<=0)
		a4=a5;

	if(a4>0)
	{
		a4=10/a4;
		var strtext='2D:每点等于'+a4+'mm';
		Message(strtext);
		Event(4,a4,a4,0,0);
	}
	else
		Message('2D自动定标失败！！！');
}
else
if(a5==1)
{
	a1=GetInfo(207);//M-MODE
	a2=GetInfo(208);
	a5=Event(1,a1,a2,a3,0);
	a1=GetInfo(215);
	a2=GetInfo(216);
	Event(3,a1,a2,a3,0);

	var strtext=ResultString();

	a1=GetInfo(209);//M-MODE
	a2=GetInfo(210);
	a4=Event(1,a1,a2,a3,0);
	
	a1=atoiStr(strtext,'CAL');
	
	if(a5>0)
		a5=a1/a5;
	else
		a5=0;

	if(a4>0&&a5>0)
	{
		a4=0.2/a4;
		strtext='M-MODE距离:每点等于'+a5+'mm\n';
		strtext=strtext+'M-MODE时间:每点等于'+a4+'s';
		Message(strtext);
		Event(4,a4,a5,0,0);
	}
	else
		Message('M-MODE自动定标失败！！！');
}
else
if(a5==2)
{
	SetInfo(240,HIWORD(a4));//基线

	a1=GetInfo(203);//DOPPLER
	a2=GetInfo(204);
	a4=Event(1,a1,a2,a3,0);

	a1=GetInfo(213);
	a2=GetInfo(214);
	Event(3,a1,a2,a3,0);
	
	var strtext=ResultString();

	a1=GetInfo(205);
	a2=GetInfo(206);
	a5=Event(1,a1,a2,a3,0);
	
	a1=atoiStr(strtext,'CAL');
	if(a4>0&&a1>0)
		a4=a1/a4;
	else
		a4=0;

	if(a4>0&&a5>0)
	{
		a5=0.2/a5;
		Event(4,a5,a4,0,0);
		strtext='基线'+GetInfo(240)+'\n';
		strtext=strtext+'DOPPLER速度:每点等于'+a4+'m/s\n';
		strtext=strtext+'DOPPLER时间:每点等于'+a5+'s';
		Message(strtext);
	}
	else
		Message('DOPPLER自动定标失败！！！');
}
