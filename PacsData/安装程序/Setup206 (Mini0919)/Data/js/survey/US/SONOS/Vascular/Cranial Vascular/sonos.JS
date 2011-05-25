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

///////////////////////////////////////////////////////////////////////////////////////////
///////////////  Measurements  ////////////////////////////////////////////////////////////
//////////  Info: 1 PI,RI;5 mean vel;10~11 change mm2 to cm2  /////////////////////////////
//////////        30~                                         /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();

//B-Mode
	Declare('Area(FloV)',2,8,10,10);
        Declare('Diam(FloV)',1,5,0,30);
	Declare('Area Resid',2,8,10,11);
	Declare('Area True',2,8,10,12);
	Declare('diam(FloV)',2,8,10,51);
	Declare('Diam Resid',1,5,0,31);
	Declare('Diam True',1,5,0,31);

//VAD Measurements （动脉血管测量）
	Declare('accel T',4,11,3,32);
	Declare('DF',4,11,11,0);
	Declare('DV',5,12,11,32);
	Declare('Mean V (PI)',6,14,1,1);//calcualte PI or RI
	Declare('MnV (PI,RI)',6,14,1,1);
	Declare('MxV (PI,RI)',6,14,1,1);
	Declare('Pk-Pk',4,11,3,0);
	Declare('SF',4,11,11,0);
	Declare('SV',5,12,1,32);

//VVD Measurements（静脉血管测量）
	Declare('delay T',4,11,3,0);
	Declare('Max Freq',4,11,3,0);
	Declare('Max vel',5,12,1,0);

//Doppler Measurements
	Declare('Accel Time1',4,11,3,0);
	Declare('Accel Time2',4,11,3,0);
	Declare('Accel Time3',4,11,3,0);
	Declare('Delay Time1',4,11,3,0);
	Declare('Delay Time2',4,11,3,0);
	Declare('Delay Time3',4,11,3,0);
	Declare('DF1',4,11,11,33);
	Declare('DF2',4,11,11,34);
	Declare('DF3',4,11,11,0);
	Declare('DV1',5,12,1,35);
	Declare('DV2',5,12,1,36);
	Declare('DV3',5,12,1,0);
	Declare('max freq1',4,11,11,41);
	Declare('max freq2',4,11,11,42);
	Declare('max freq3',4,11,11,43);
	Declare('max vel1',5,12,1,44);
	Declare('max vel2',5,12,1,45);
	Declare('max vel3',5,12,1,46);
	Declare('Mean V(FloV)',6,14,1,5);
	Declare('min freq1',4,11,11,47);
	Declare('min freq2',4,11,11,48);
	Declare('min freq3',4,11,11,0);
	Declare('min vel1',5,12,1,49);
	Declare('min vel2',5,12,1,50);
	Declare('min vel3',5,12,1,0);
	Declare('SF1',4,11,11,37);
	Declare('SF2',4,11,11,38);
	Declare('SF3',4,11,11,0);
	Declare('SV1',5,12,1,39);
	Declare('SV2',5,12,1,40);
	Declare('SV3',5,12,1,0);

}
else
if(a0==1)
    PIRI('Mean V(PI)','MxV (PI,RI)','MnV (PI,RI)','PI','RI');
else
if(a0==5)
{
    MeanVel('Mean vel(FloV)');
   RunJavaScript('CraCalcu_51.cal',1);
}
else
if(a0==10)
{
   var Result=GetValue('Area(FloV)',-1);
   OutData('Area(FloV)',Result/100,10);
   RunJavaScript('CraCalcu_10.cal',1);
}
else
if(a0==11)
{
   var Result=GetValue('Area Resid',-1);
   OutData('Area Resid',Result/100,10);
   RunJavaScript('CraCalcu_11.cal',1);
}
else
if(a0==12)
{
   var Result=GetValue('Area True',-1);
   OutData('Area True',Result/100,10);
   RunJavaScript('CraCalcu_11.cal',1);
}
else
if(a0==30)
{
   RunJavaScript('CraCalcu_30.cal',1);
}
else
if(a0==31)
{
   RunJavaScript('CraCalcu_31.cal',1);
}
else
if(a0==32)
{
   RunJavaScript('CraCalcu_32.cal',1);
}
else
if(a0==33)
{
   RunJavaScript('CraCalcu_33.cal',1);
}
else
if(a0==34)
{
   RunJavaScript('CraCalcu_34.cal',1);
}
else
if(a0==35)
{
   RunJavaScript('CraCalcu_35.cal',1);
}
else
if(a0==36)
{
   RunJavaScript('CraCalcu_36.cal',1);
}
else
if(a0==37)
{
   RunJavaScript('CraCalcu_37.cal',1);
}
else
if(a0==38)
{
   RunJavaScript('CraCalcu_38.cal',1);
}
else
if(a0==39)
{
   RunJavaScript('CraCalcu_39.cal',1);
}
else
if(a0==40)
{
   RunJavaScript('CraCalcu_40.cal',1);
}
else
if(a0==41)
{
   RunJavaScript('CraCalcu_41.cal',1);
}
else
if(a0==42)
{
   RunJavaScript('CraCalcu_42.cal',1);
}
else
if(a0==43)
{
   RunJavaScript('CraCalcu_43.cal',1);
}
else
if(a0==44)
{
   RunJavaScript('CraCalcu_44.cal',1);
}
else
if(a0==45)
{
   RunJavaScript('CraCalcu_45.cal',1);
}
else
if(a0==46)
{
   RunJavaScript('CraCalcu_46.cal',1);
}
else
if(a0==47)
{
   RunJavaScript('CraCalcu_47.cal',1);
}
else
if(a0==48)
{
   RunJavaScript('CraCalcu_48.cal',1);
}
else
if(a0==49)
{
   RunJavaScript('CraCalcu_49.cal',1);
}
else
if(a0==50)
{
   RunJavaScript('CraCalcu_50.cal',1);
}
else
if(a0==51)
{
   RunJavaScript('CraCalcu_51.cal',1);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////// Carotid,Upper and Lower Extremity and Cranial Vascular Calculations      ////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//RunJavaScript('CraCalcu.cal',1);

