//////////////////////////////////////////////////////////////////////////
/// Function PIRI     ////////////////////////////////////////////////////
/// Calculate mean,max and min velocity, PI and RI  with VTI   ///////////
//////////////////////////////////////////////////////////////////////////

function PIRI(StrMean,StrMax,StrMin,StrPI,StrRI)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==1)
		{
			var a2=GetInfo(240);//纵轴基线
			var a1=GetROIData(1);//点个数
			if(a1>2)
			{
				var a6=0;
				var a8=GetInfo(6);//纵轴标尺
				var a9=GetInfo(5);//横轴标尺
				var n=0;
				var a3=0;
				var a4=0;
				var a10=0;
				var a11=0;
				var min=Math.abs(HIWORD(GetROIData(2))-a2)*a8;
				var max=min;
				for(n=1;n<a1-1;n++)
				{
				   a10=Math.abs(HIWORD(GetROIData(n+1))-a2)*a8;
				   a11=Math.abs(HIWORD(GetROIData(n+2))-a2)*a8;

				   a3=a9*(LOWORD(GetROIData(n+1))-LOWORD(GetROIData(n+2)));
				   a4=a10+a11;
				   a6=a6+Math.abs(a3*a4/2);
				   max=a11>max?a11:max;
				   min=a11<min?a11:min;

				}
				var dx=Math.abs(a9*(LOWORD(GetROIData(a1+1))-LOWORD(GetROIData(2))));
				var mean=0;
				if(dx)
				{
				   mean=a6/dx;
				}
				OutData(StrMean,mean,1);
				OutData(StrMax,max,1);
				OutData(StrMin,min,1);
				if(mean)
				{
				   pi=Math.abs(max-min)/mean;
       				   OutData(StrPI,pi);
				}  
				if(max)
				{
				   ri=Math.abs(max-min)/max;
       				   OutData(StrRI,ri);
				}  

	                 }
                  }

          }
}

///////////////////////////////////////////////////////////////////////
/// Function MeanVel  /////////////////////////////////////////////////
/// Calculate the mean velocity with VTI   ////////////////////////////
///////////////////////////////////////////////////////////////////////


function MeanVel(StrMean)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==1)
		{
			var a2=GetInfo(240);//纵轴基线
			var a1=GetROIData(1);//点个数
			if(a1>2)
			{
				var a6=0;
				var a8=GetInfo(6);//纵轴标尺
				var a9=GetInfo(5);//横轴标尺
				var n=0;
				var a3=0;
				var a4=0;
				var a10=0;
				var a11=0;
				var min=Math.abs(HIWORD(GetROIData(2))-a2)*a8;
				var max=min;
				for(n=1;n<a1-1;n++)
				{
				   a10=Math.abs(HIWORD(GetROIData(n+1))-a2)*a8;
				   a11=Math.abs(HIWORD(GetROIData(n+2))-a2)*a8;
				   a3=a9*(LOWORD(GetROIData(n+1))-LOWORD(GetROIData(n+2)));
				   a4=a10+a11;
				   a6=a6+Math.abs(a3*a4/2);
				}
				var dx=Math.abs(a9*(LOWORD(GetROIData(a1+1))-LOWORD(GetROIData(2))));
				var mean=0;
				if(dx)
				{
				   mean=a6/dx;
				}
				OutData(StrMean,mean,1);

	                 }
                  }

          }
}
//////////////////////////////////////////////////////////////////////////////
///////////////   Measurements    ////////////////////////////////////////////
///////////  Info:1~2 PI,RI; 5 mean vel;10~12  change mm2 to cm2  ////////////
///////////       30~                                             ////////////
//////////////////////////////////////////////////////////////////////////////
var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
//B-Mode
	Declare('Area Resid',2,8,10,10);
	Declare('Area True',2,8,10,11);
	Declare('Diam Resid',1,5,0,30);
	Declare('Diam True',1,5,0,30);
	Declare('Renal Height',1,5,0,0);
	Declare('Renal Length',1,5,0,0);
	Declare('Renal Width',1,5,0,0);

//Doppler Velocity
	Declare('accel T',4,11,3,31);
	Declare('DV',5,12,1,31);
	Declare('SV',5,12,1,31);
	Declare('Max vel',5,12,1,0);


//Generic Doppler Velocity Measurements
	Declare('DV1',5,12,1,32);
	Declare('DV2',5,12,1,33);
	Declare('SV1',5,12,1,34);
	Declare('SV2',5,12,1,35);
	Declare('SV3',5,12,1,0);
	Declare('V1 accel T',4,11,3,36);
	Declare('V1 delay T',4,11,3,0);
	Declare('V2 accel T',4,11,3,37);
	Declare('V2 delay T',4,11,3,0);

//Flow Volume
	Declare('Area(FloV)',2,8,10,12);
	Declare('Diam(FloV)',1,5,0,0);
	Declare('Mean vel(FloV)',6,14,1,5);

//Frequence
	Declare('DF1',4,11,11,38);
	Declare('DF2',4,11,11,39);
	Declare('SF1',4,11,11,40);
	Declare('SF2',4,11,11,41);
	Declare('SF3',4,11,11,0);
	Declare('Max freq',4,11,11,0);

//Pulsatility and Resistivity
	Declare('MxV(PI,RI)',6,14,1,1);
	Declare('MeanV(PI)',6,14,1,1);
	Declare('MnV(PI,RI)',6,14,1,1);


//Genetic Pulsatility and Resistivity Measurements
	Declare('Max vel(PI,RI)',6,14,1,2);
	Declare('Mean vel(PI)',6,14,1,2);
	Declare('Min vel(PI,RI)',6,14,1,2);

}
else
if(a0==1)
    PIRI('MeanV(PI)','MxV(PI,RI)','MnV(PI,RI)','PI','RI');
else
if(a0==2)
    PIRI('Mean vel(PI)','Max vel(PI,RI)','Min vel(PI,RI)','PI','RI');
else
if(a0==5)
{
    MeanVel('Mean vel(FloV)');
    RunJavaScript('AbCalcu_5.cal',1);
}
else
if(a0==10)
{
   var Result=GetValue('Area Resid',-1);
   OutData('Area Resid',Result/100,10);
    RunJavaScript('AbCalcu_10.cal',1);
}
else
if(a0==11)
{
   var Result=GetValue('Area True',-1);
   OutData('Area True',Result/100,10);
    RunJavaScript('AbCalcu_11.cal',1);
}
else
if(a0==12)
{
   var Result=GetValue('Area(FloV)',-1);
   OutData('Area(FloV)',Result/100,10);
    RunJavaScript('AbCalcu_12.cal',1);
}
else
if(a0==30)
{
   RunJavaScript('AbCalcu_30.cal',1);
}
else
if(a0==31)
{
   RunJavaScript('AbCalcu_31.cal',1);
}
else
if(a0==32)
{
   RunJavaScript('AbCalcu_32.cal',1);
}
if(a0==33)
{
   RunJavaScript('AbCalcu_33.cal',1);
}
else
if(a0==34)
{
   RunJavaScript('AbCalcu_34.cal',1);
}
else
if(a0==35)
{
   RunJavaScript('AbCalcu_35.cal',1);
}
if(a0==36)
{
   RunJavaScript('AbCalcu_36.cal',1);
}
else
if(a0==37)
{
   RunJavaScript('AbCalcu_37.cal',1);
}
else
if(a0==38)
{
   RunJavaScript('AbCalcu_38.cal',1);
}
if(a0==39)
{
   RunJavaScript('AbCalcu_39.cal',1);
}
else
if(a0==40)
{
   RunJavaScript('AbCalcu_40.cal',1);
}
else
if(a0==41)
{
   RunJavaScript('AbCalcu_41.cal',1);
}
//////////////////////////////////////////////////////////////////////////////
////////////   Obdorminal Vascular Calculations      /////////////////////////
//////////////////////////////////////////////////////////////////////////////

//RunJavaScript('AbCalcu.cal',1);

