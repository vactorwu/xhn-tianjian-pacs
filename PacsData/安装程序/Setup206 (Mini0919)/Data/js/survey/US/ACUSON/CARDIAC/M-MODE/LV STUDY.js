var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	Declare('RVWd',5,12,0,0);
	Declare('RVDd',5,12,0,0);
	Declare('IVSd',5,12,0,0);
	Declare('LVDd',5,12,0,1);
	Declare('LVPWd',5,12,0,0);
	Declare('RVWs',5,12,0,0);
	Declare('RVDs',5,12,0,0);
	Declare('IVSs',5,12,0,0);
	Declare('LVDs',5,12,0,2);
	Declare('LVPWs',5,12,0,0);
	Declare('PE',5,12,0,0);
}
else
if(a0==1||a0==2)
{
	var a1=GetValue('LVDd',-1);
	var a3=a1*a1*a1*1.07;
	OutData('LVd V',a3,2); //左室舒张期容积
	
	var a2=GetValue('LVDs',-1);
	var a4=a2*a2*a2*1.07;
	OutData('LVs V',a4,2); //左室收缩期容积
	
	if(a3&&a4)
	{
		var sv=a3-a4;
		OutData('SV',sv,2); //每博射血量
		if(a3)
			sv=sv/a3;
		else
			sv=0;
		OutData('EF',sv,4); //射血分数
	}

	if(a1&&a2)
	{
		a3=(a1-a2)/a1;
		OutData('FS',a3,4); //缩短分数
	}
}
