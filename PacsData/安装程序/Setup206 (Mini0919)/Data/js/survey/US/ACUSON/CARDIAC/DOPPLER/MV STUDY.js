var a0=GetInfo(0);
if(a0==0)
{
	Clear(); 
	Declare('MV VTI',6,14,6,1);
	Declare('Peak E',6,1,1,2);
	Declare('Peak A',6,1,1,3);
	Declare('MV IVRT',4,11,3,4);
	Declare('MV PHT',6,5,3,5);
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
				OutData('MV VTI',a6,6); 
				
				a5=Math.abs(a5);
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
if(a0==2||a0==3)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=HIWORD(GetROIData(2));
			var a2=GetInfo(240);
			var a8=GetInfo(6);
			a1=Math.abs((a2-a1)*a8);
			if(a0==2)
			{
				OutData('Peak E',a1,1); 
				var a7=GetValue('Peak A',-1);
				if(a7)
					OutData('E/A',a1/a7,-1); 
			}
			else
			{
				OutData('Peak A',a1,1); 
				var a7=GetValue('Peak E',-1);
				if(a1)
					OutData('E/A',a7/a1,-1); 
			}
		}
	}
}
else
if(a0==5)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==4)
		{
			var a8=GetInfo(6);
			var a9=GetInfo(5);
			var y0=HIWORD(GetROIData(2))*a8;
			var x0=LOWORD(GetROIData(2))*a9;
			var y1=HIWORD(GetROIData(3))*a8;
			var x1=LOWORD(GetROIData(3))*a9;
			if(x0!=x1)
			{
				var k=(y0-y1)/(x0-x1);
				if(k)
				{
					var a2=GetInfo(240);
					var	a1=(a2-HIWORD(GetROIData(2)))*a8;
					a1=a1/1.414;
					a1=Math.abs(a1*0.29/k);
					OutData('MV PHT',a1,3); 
					if(a1)
						OutData('MV area',Math.abs(0.22/a1),10); 
				}
			}
		}
	}
}
