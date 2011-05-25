if(GetInfo(0)==0)
{
	SetInfo(7,0);
	Clear();
	Declare('LVd V',7,13,2,1);
	Declare('LVs V',7,13,2,1);
}
else
if(GetInfo(0)==1)
{
	var a1=GetValue('LVd V',-1);
	var a2=GetValue('LVs V',-1);
	if(a1!=0&&a2!=0)
	{
		var a3=a1-a2;
		OutData('SV',a3,2); //每博射血量
		if(a1)
			a3=a3/a1;
		else
			a3=0;
		OutData('EF',a3,4); //射血分数
	}
}