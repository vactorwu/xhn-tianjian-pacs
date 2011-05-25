if(GetInfo(0)==0)
{
	SetInfo(7,0);
	Clear();
	Declare('LVDd',5,12,0,1);
	Declare('LVDs',5,12,0,1);
	Declare('R-R',4,11,3,1);
	Declare('LVET',4,11,3,1);
}
else
if(GetInfo(0)==1)
{
	var a1=GetValue('LVDd',-1);
	var a2=GetValue('LVDs',-1);
	var a3=GetValue('LVET',-1);
	if(a3&&a1&&a2)
	{
		var a4=a1-a2;
		var a5=a3*a1;
		a4=a4/a5;
		OutData('VCF',a4,5); 
	}
}