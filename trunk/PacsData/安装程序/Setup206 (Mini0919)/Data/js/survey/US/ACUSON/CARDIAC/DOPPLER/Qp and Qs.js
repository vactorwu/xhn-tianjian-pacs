var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	Declare('SYS Diam',1,5,0,3);
	Declare('SYS VTI',6,14,6,1);
	Declare('PUL Diam',1,5,0,4);
	Declare('PUL VTI',6,14,6,2);
}
else
if(a0==1||a0==2)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==1)
		{
			var a2=GetInfo(240);
			var a1=GetROIData(1);
			if(a1>2)
			{
				var a8=GetInfo(6);
				var a9=GetInfo(5);
				var a3=LOWORD(GetROIData(2));
				var a4=LOWORD(GetROIData(2+a1-1));
				var a6=0;

				for(a3=a3;a3<=a4;a3++)
				{
					a1=(a2-Event(5,a3,0,0,0))*a8;
					a6=a6+a1;
				}

				a6=a6*a9;
				a6=Math.abs(a6);

				if(a0==1)
					OutData('SYS VTI',a6,6); 
				else
					OutData('PUL VTI',a6,6); 

				a1=GetValue('SYS Diam',-1);
				a2=GetValue('PUL Diam',-1);
				a3=GetValue('SYS VTI',-1);
				a4=GetValue('SYS VTI',-1);

				if(a1&&a2&&a3&&a4)
				{
					a1=3.1415926*a1*a1/4*a3;
					a2=3.1415926*a2*a2/4*a4;
					OutData('Qs',a1,2); 
					OutData('Qp',a2,2); 
					if(a1&&a2)
						OutData('Qp/Qs',a2/a1,-1); 
				}
			}
		}
	}
}
else
if(a0==3||a0==4)
{
	var a1=GetValue('SYS Diam',-1);
	var a2=GetValue('PUL Diam',-1);
	var a3=GetValue('SYS VTI',-1);
	var a4=GetValue('SYS VTI',-1);
	if(a1&&a2&&a3&&a4)
	{
		a1=3.1415926*a1*a1/4*a3;
		a2=3.1415926*a2*a2/4*a4;
		OutData('Qs',a1,2); 
		OutData('Qp',a2,2); 
		if(a1&&a2)
			OutData('Qp/Qs',a2/a1,-1); 
	}
}