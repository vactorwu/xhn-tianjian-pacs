var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	Declare('Ao VTI',6,14,6,1);
	Declare('AoV Vmax',6,1,1,2);
	Declare('AI SLOPE',8,5,7,0);//µ¥Î»£¿
}
else
if(a0==1)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==1)
		{
			var a2=GetInfo(240);
			var a1=GetROIData(1);
			if(a1>2)
			{
				var a3=LOWORD(GetROIData(2));
				var a4=LOWORD(GetROIData(2+a1-1));
				var a5=0;
				var a6=0;
				var	a7=0;
				var a8=GetInfo(6);
				var a9=GetInfo(5);
				var a10=0;

				for(a3=a3;a3<=a4;a3++)
				{
					a1=(a2-Event(5,a3,0,0,0))*a8;
					a10=a10+4*a1*a1;
					if(a1>a5)
					{
						a5=a1;
						a7=a3;
					}
					a6=a6+a1;
				}

				a10=a10*a9;
				a6=a6*a9;

				a6=Math.abs(a6);
				OutData('Ao VTI',a6,6); 

				a5=Math.abs(a5);
				OutData('AoV Vmax',a5,1); 
				a7=(a4-a7+1)*a9;
				if(a7)
					a7=a5/a7;
				a7=Math.abs(a7);
				OutData('AI SLOPE',a7,7); 

				var ppg=4*a5*a5;
				OutData('PPG',ppg,8); 
				
				a3=LOWORD(GetROIData(2));
				a5=(a4-a3+1)*a9;
				if(a5!=0)
				{
					var MnGraD=a10/a5;
					OutData('MnGRAD',MnGraD,8); 
				}
			}
		}
	}
}
else
if(a0==2)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=HIWORD(GetROIData(2));
			var a2=GetInfo(240);
			var a8=GetInfo(6);
			a1=(a2-a1)*a8;
			a1=Math.abs(a1);

			OutData('AoV Vmax',a1,1); 
			var ppg=4*a1*a1;
			OutData('PPG',ppg,1); 
		}
	}
}
