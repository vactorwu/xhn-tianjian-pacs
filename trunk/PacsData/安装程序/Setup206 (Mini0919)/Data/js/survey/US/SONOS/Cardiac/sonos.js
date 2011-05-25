
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////    CARDIAC    MEASUREMENTS   /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
////////////  Function VTI//////////////////////////////////////////////////////
////////////  Calculate VTI,mean velocity,mean PG,and max velocity/   //////////
/////////////////////////////////////////////////////////////////////////////////

function VTI(StrVTI,StrMean,StrMPG,StrMax)
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
				var a5=0;
				var pg=0;
				for(n=1;n<a1-1;n++)
				{
				   a10=Math.abs(HIWORD(GetROIData(n+1))-a2);
				   a11=Math.abs(HIWORD(GetROIData(n+2))-a2);

				   a3=a9*(LOWORD(GetROIData(n+1))-LOWORD(GetROIData(n+2)));
				   a4=a8*(a11+a10);
				   a6=a6+Math.abs(a3*a4/2);

				   if(a5<a11)
				      a5=a11;
				   if(a5<a10)
				      a5=a10;

				   pg=pg+Math.abs(4*a10*a10*a8*a8*a3);

				}
				pg=pg+4*a11*a11*a8*a8*a3;
				OutData(StrVTI,a6,6);
				var dx=Math.abs(a9*(LOWORD(GetROIData(a1+1))-LOWORD(GetROIData(2))));
				var mean=0,mpg=0;;
				if(dx)
				{
				   mean=a6/dx,mpg=pg/dx;
				}
				OutData(StrMean,mean,1);
				OutData(StrMPG,mpg,8);
				OutData(StrMax,a5*a8,1);
            }
        }
	}
}
/////////////////////////////////////////////////////////////////////////////////
////////////  Function input    /////////////////////////////////////////////////
////////////  Receive an input string as the parameter of the function  /////////
/////////////////////////////////////////////////////////////////////////////////
function input(str)
{
	var textstr;
	var db1;
	if(GetString(str,'50'))
	{
		textstr=ResultString();
		db1=eval(textstr);
		OutData(str,db1);
	}
}
    
///////////////////////////////////////////////////////////////////////////////////////////////////
////// NOTE: Info 1~9 to measure                   ////////////////////////////////////////////////
///////////  Info 11~15 to receive input data      ////////////////////////////////////////////////
///////////  Info 20~25    change mm2 to cm2       ////////////////////////////////////////////////
///////////  Info 30~86 87~                     ////////////////////////////////////////////////    
///////////////////////////////////////////////////////////////////////////////////////////////////


var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();

	Declare('2-D area',2,8,10,20);
	Declare('2-D diam',1,5,0,30);
	Declare('Accel slope',8,5,7,0);
	Declare('Accel time',4,11,3,31);
	Declare('ACS',1,5,0,0);
	Declare('AI end-d vel',5,12,1,32);
	Declare('AI max vel',5,12,1,33);
	Declare('Ao acc slope',8,5,7,0);
	Declare('Ao acc time',4,11,3,0);
	Declare('Ao dec slope',8,5,7,34);
	Declare('Ao dec time',4,11,3,0);
	Declare('Ao isthmus',1,5,0,0);
	Declare('Ao root area',2,8,10,21);
	Declare('Ao root diam',1,5,0,35);
	Declare('Ao arch',1,5,0,0);
	Declare('Aortic R-R',4,11,3,36);
	Declare('Ao V2 max',6,14,1,1);//最大速度
	Declare('Ao V2 mean',6,14,1,1);//平均值
	Declare('Ao V2 trace',6,14,8,1);//计算梯度
	Declare('Ao V2 VTI',6,14,0,1);//计算积分
	Declare('Area 1',2,8,10,22);
	Declare('Area 2',2,8,10,23);
	Declare('asc Aorta',1,5,0,0);
	Declare('AS max vel',5,12,1,37);
    Declare('BP diastolic',0,0,-1,15);
    Declare('BP end sys',0,0,-1,14);
    Declare('BP peak sys',0,0,-1,11);
	Declare('BSA',0,0,-1,12);
	Declare('Circum',3,8,0,0);
	Declare('Dec max vel',5,12,1,38);
	Declare('Decel slope',8,5,7,0);
	Declare('Decel time',4,11,3,0);
	Declare('desc Aorta',1,5,0,0);
	Declare('Eject time',4,11,3,39);
	Declare('EPSS',1,5,0,0);
	Declare('IVSd',1,5,0,40);
	Declare('IVSs',1,5,0,41);
	Declare('LA dimension',1,5,0,42);
	Declare('LPA diam',1,5,0,0);
	Declare('LVAd ap2 MOD',2,8,10,43);
	Declare('LVAd ap4 MOD',2,8,10,44);
	Declare('LVAd apical',2,8,10,87);
	Declare('LVAd sax epi',2,8,10,45);
	Declare('LVAd sax MV',2,8,10,88);
	Declare('LVAd sax PM',2,8,10,46);
	Declare('LVAs ap2 MOD',2,8,10,47);
	Declare('LVAs ap4 MOD',2,8,10,48);
        Declare('LVAs apical',2,8,10,89);
        Declare('LVAs sax epi',2,8,10,90);
	Declare('LVAs sax MV',2,8,10,91);
	Declare('LVAs sax PM',2,8,10,49);
	Declare('LVET',4,11,3,50);
	Declare('LVIDd',1,5,0,51);
	Declare('LVIDs',1,5,0,52);
	Declare('LVLd apical',1,5,0,53);
	Declare('LVLs apical',1,5,0,54);
	Declare('LVOT area',2,8,10,24);
	Declare('LVOT diam',1,5,0,55);
	Declare('LVPEP',4,11,3,56);
	Declare('LVPWd',1,5,0,57);
	Declare('LVPWs',1,5,0,58);
	Declare('LV V1 max',6,14,1,2);//
	Declare('LV V1 mean',6,14,1,2);//caculate mean value
	Declare('LV V1 trace',6,14,8,2);//caculate PG
	Declare('LV V1 VTI',6,14,1,2);//


	Declare('Max vel(TR)',5,12,1,59);
	Declare('Mean PG',6,14,8,3);//Caculate pressure gradient
	Declare('Mitral R-R',4,11,3,60);
	Declare('MM R-R int',4,11,3,61);
	Declare('MM slope',1,5,0,0);
	Declare('MPA diam',1,5,0,62);
	Declare('MR max vel',5,12,1,63);
	Declare('MV acc slope',8,5,7,0);
	Declare('MV acc time',4,11,3,0);
	Declare('MV annu diam',1,5,0,64);
	Declare('MV A point',5,12,1,65);
	Declare('MVA(traced)',2,8,10,92);
	Declare('MV dec slope',8,5,7,66);
	Declare('MV dec time',4,11,3,0);
	Declare('MV DFP',4,11,3,0);
	Declare('MV diam1',1,5,0,67);
	Declare('MV diam2',1,5,0,67);
	Declare('MV E point',5,12,1,68);
	Declare('MV E-F slope',8,5,7,0);
	Declare('MV excursion',1,5,0,0);
	Declare('MV P1/2max v',5,12,1,69);
	Declare('MV peak vel',5,12,1,70);
	Declare('MV V2 mean',6,14,1,4);//计算平均值
	Declare('MV V2 trace',6,14,8,4);//计算梯度
	Declare('MV V2 VTI',6,14,1,4);//计算积分


	Declare('PA acc slope',8,5,7,0);
	Declare('PA acc time',4,11,3,0);
	Declare('PA dec slope',8,5,7,71);
	Declare('PA dec time',4,11,3,0);
	Declare('PA V2 max',6,14,1,5);//Max velocity
	Declare('PA V2 mean',6,14,1,5)//计算平均值
	Declare('PA V2 trace',6,14,8,5);//计算梯度
	Declare('PA V2 VTI',6,14,0,5);//计算积分
	Declare('PEP',4,11,3,72);
	Declare('PI end-d vel',5,12,1,73);
	Declare('PI max vel',5,12,1,74);
	Declare('Pulm R-R',4,11,3,75);
	Declare('Q-to-PV close',4,11,3,76);
	Declare('Q-to-TV open',4,11,3,77);


    Declare('RAP diastole',0,0,-1,13);
	Declare('RPA diam',1,5,0,0);
	Declare('R-R interval',4,11,3,78);
	Declare('RVAW',1,5,0,0);
	Declare('RVDd',1,5,0,0);
	Declare('RVDd major',1,5,0,0);
	Declare('RVDd minor',1,5,0,0);
	Declare('RVDs',1,5,0,0);
	Declare('RVET',4,11,3,79);
    Declare('RVOT area',2,8,10,25);
	Declare('RVOT diam',1,5,0,80);
	Declare('RVPEP',4,11,3,81);
	Declare('RV V1 max',6,14,1,6);//max velocity
	Declare('RV V1 mean',6,14,1,6);//计算平均值
	Declare('RV V1 trace',6,14,8,6);//计算梯度
	Declare('RV V1 VTI',6,14,0,6);//积分


	Declare('Tricusp R-R',4,11,3,82);
	Declare('TV acc slope',8,5,7,0);
	Declare('TV acc time',4,11,3,0);
	Declare('TV annu diam',1,5,0,83);
	Declare('TV dec slope',8,5,7,84);
	Declare('TV dec time',4,11,3,0);
	Declare('TV DFP',4,11,3,0);
	Declare('TV flow diam',1,5,0,85);
	Declare('TV P1/2max V',5,12,1,0);
	Declare('TV V2 max',6,14,1,7);//max velocity
	Declare('TV V2 mean',6,14,1,7);//计算平均值
	Declare('TV V2 trace',6,14,8,7);//计算梯度
	Declare('TV V2 VTI',6,14,6,7);//计算积分


	Declare('V1 max',6,14,1,8);
	Declare('V1 mean',6,14,1,8);//caculate the mean value
	Declare('V1 trace',6,14,8,8);//caculate PG
	Declare('V1 VTI',6,14,6,8);//v-t integral
	Declare('V2 mean',6,14,1,9);//caculate the mean value
	Declare('V2 trace',6,14,8,9);//caculate PG
	Declare('V2 VTI',6,14,6,9);//v-t integral
	Declare('Vel slope',8,5,7,0);
	Declare('Volume',7,13,2,0);
	Declare('VSD max vel',5,12,1,86);
}
else
if(a0==1)
{
	VTI('Ao V2 VTI','Ao V2 mean','Ao V2 trace','Ao V2 max');
	RunJavaScript('CarCalcu_1.cal',1);
}
else
if(a0==2)
{
    VTI('LV V1 VTI','LV V1 mean','LV V1 trace','LV V1 max');
    RunJavaScript('CarCalcu_2.cal',1);
}
else
if(a0==3)
{
    VTI('VTI','V mean','Mean PG','V max');
}
else
if(a0==4)
{
	VTI('MV V2 VTI','MV V2 mean','MV V2 trace','MV V2 max');
	RunJavaScript('CarCalcu_4.cal',1);
}
else
if(a0==5)
{
    VTI('PA V2 VTI','PA V2 mean','PA V2 trace','PA V2 max');
    RunJavaScript('CarCalcu_5.cal',1);
}
else
if(a0==6)
{
	VTI('RV V1 VTI','RV V1 mean','RV V1 trace','RV V1 max');
	RunJavaScript('CarCalcu_1.cal',6);
}
else
if(a0==7)
{
    VTI('TV V2 VTI','TV V2 mean','TV V2 trace','TV V2 max');
    RunJavaScript('CarCalcu_7.cal',1);
}
else
if(a0==8)
{
	VTI('V1 VTI','V1 mean','V1 trace','V1 max');
	RunJavaScript('CarCalcu_8.cal',1);
}
else
if(a0==9)
{
    VTI('V2 VTI','V2 mean','V2 trace','V2 max');
    RunJavaScript('CarCalcu_9.cal',1);
}
else
if(a0==11)
{
	input('BP peak sys');
	RunJavaScript('CarCalcu_11.cal',1);
}
else
if(a0==12)
{
	var dw,dh,result;
	input('BSA W (kg)');
	input('BSA H (cm)');
	dw=GetValue('BSA W (kg)',-1);
	dh=GetValue('BSA H (cm)',-1);
	result=0.007184*Math.pow(dw,0.425)*Math.pow(dh,0.725);
	OutData('BSA',result,14);	
	RunJavaScript('CarCalcu_12.cal',1);
}
else
if(a0==13)  
{
	input('RAP diastole');
	RunJavaScript('CarCalcu_13.cal',1);
}
else
if(a0==14)
{
	input('BP end sys');
	RunJavaScript('CarCalcu_14.cal',1);
}
else
if(a0==15)
{
	input('BP diastolic');
	RunJavaScript('CarCalcu_15.cal',1);
}
else
if(a0==20)
{
	Result=GetValue('2-D area',-1);
	OutData('2-D area',Result/100,10);
	RunJavaScript('CarCalcu_20.cal',1);
}  
else
if(a0==21)
{
	Result=GetValue('Ao root area',-1);
	OutData('Ao root area',Result/100,10);
	RunJavaScript('CarCalcu_21.cal',1);
} 
else
if(a0==22)
{
	Result=GetValue('Area 1',-1);
	OutData('Area 1',Result/100,10);
	RunJavaScript('CarCalcu_22.cal',1);
} 
else
if(a0==23)
{
	Result=GetValue('Area 2',-1);
	OutData('Area 2',Result/100,10);
	RunJavaScript('CarCalcu_23.cal',1);
} 
else
if(a0==24)
{
	Result=GetValue('LVOT area',-1);
	OutData('LVOT area',Result/100,10);
	RunJavaScript('CarCalcu_24.cal',1);
} 
else
if(a0==25)
{
	Result=GetValue('RVOT area',-1);
	OutData('RVOT area',Result/100,10);
	RunJavaScript('CarCalcu_25.cal',1);
}
else
if(a0==30)
{
	RunJavaScript('CarCalcu_30.cal',1);
}
else
if(a0==31)
{
	RunJavaScript('CarCalcu_31.cal',1);
}
else
if(a0==32)
{
	RunJavaScript('CarCalcu_32.cal',1);
}
else
if(a0==33)
{
	RunJavaScript('CarCalcu_33.cal',1);
}
else
if(a0==34)
{
	RunJavaScript('CarCalcu_34.cal',1);
}
else
if(a0==35)
{
	RunJavaScript('CarCalcu_35.cal',1);
}
else
if(a0==36)
{
	RunJavaScript('CarCalcu_36.cal',1);
}
else
if(a0==37)
{
	RunJavaScript('CarCalcu_37.cal',1);
}
else
if(a0==38)
{
	RunJavaScript('CarCalcu_38.cal',1);
}
else
if(a0==39)
{
	RunJavaScript('CarCalcu_39.cal',1);
}
else
if(a0==40)
{
	RunJavaScript('CarCalcu_40.cal',1);
}
else
if(a0==41)
{
	RunJavaScript('CarCalcu_41.cal',1);
}
else
if(a0==42)
{
	RunJavaScript('CarCalcu_42.cal',1);
}
else
if(a0==43)
{
	Result=GetValue('LVAd ap2 MOD',-1);
	OutData('LVAd ap2 MOD',Result/100,10);
	RunJavaScript('CarCalcu_43.cal',1);
}
else
if(a0==44)
{
	Result=GetValue('LVAd ap4 MOD',-1);
	OutData('LVAd ap4 MOD',Result/100,10);
	RunJavaScript('CarCalcu_44.cal',1);
}
else
if(a0==45)
{
	Result=GetValue('LVAd sax epi',-1);
	OutData('LVAd sax epi',Result/100,10);
	RunJavaScript('CarCalcu_45.cal',1);
}
else
if(a0==46)
{
	Result=GetValue('LVAd sax PM',-1);
	OutData('LVAd sax PM',Result/100,10);
	RunJavaScript('CarCalcu_46.cal',1);
}
else
if(a0==47)
{
	Result=GetValue('LVAs ap2 MOD',-1);
	OutData('LVAs ap2 MOD',Result/100,10);
	RunJavaScript('CarCalcu_47.cal',1);
}
else
if(a0==48)
{
	Result=GetValue('LVAs ap4 MOD',-1);
	OutData('LVAs ap4 MOD',Result/100,10);
	RunJavaScript('CarCalcu_48.cal',1);
}
else
if(a0==49)
{
	Result=GetValue('LVAs sax PM',-1);
	OutData('LVAs sax PM',Result/100,10);
	RunJavaScript('CarCalcu_49.cal',1);
}
else
if(a0==50)
{
	RunJavaScript('CarCalcu_50.cal',1);
}
else
if(a0==51)
{
	RunJavaScript('CarCalcu_51.cal',1);
}
else
if(a0==52)
{
	RunJavaScript('CarCalcu_52.cal',1);
}
else
if(a0==53)
{
	RunJavaScript('CarCalcu_53.cal',1);
}
else
if(a0==54)
{
	RunJavaScript('CarCalcu_54.cal',1);
}
else
if(a0==55)
{
	RunJavaScript('CarCalcu_55.cal',1);
}
else
if(a0==56)
{
	RunJavaScript('CarCalcu_56.cal',1);
}
else
if(a0==57)
{
	RunJavaScript('CarCalcu_57.cal',1);
}
else
if(a0==58)
{
	RunJavaScript('CarCalcu_58.cal',1);
}
else
if(a0==59)
{
	RunJavaScript('CarCalcu_59.cal',1);
}
else
if(a0==60)
{
	RunJavaScript('CarCalcu_60.cal',1);
}
else
if(a0==61)
{
	RunJavaScript('CarCalcu_61.cal',1);
}
else
if(a0==62)
{
	RunJavaScript('CarCalcu_62.cal',1);
}
else
if(a0==63)
{
	RunJavaScript('CarCalcu_63.cal',1);
}
else
if(a0==64)
{
	RunJavaScript('CarCalcu_64.cal',1);
}
else
if(a0==65)
{
	RunJavaScript('CarCalcu_65.cal',1);
}
else
if(a0==66)
{
	RunJavaScript('CarCalcu_66.cal',1);
}
else
if(a0==67)
{
	RunJavaScript('CarCalcu_67.cal',1);
}
else
if(a0==68)
{
	RunJavaScript('CarCalcu_68.cal',1);
}
else
if(a0==69)
{
	RunJavaScript('CarCalcu_69.cal',1);
}
else
if(a0==70)
{
   RunJavaScript('CarCalcu_70.cal',1);
}
else
if(a0==71)
{
   RunJavaScript('CarCalcu_71.cal',1);
}
else
if(a0==72)
{
   RunJavaScript('CarCalcu_72.cal',1);
}
else
if(a0==73)
{
   RunJavaScript('CarCalcu_73.cal',1);
}
else
if(a0==74)
{
   RunJavaScript('CarCalcu_74.cal',1);
}
else
if(a0==75)
{
   RunJavaScript('CarCalcu_75.cal',1);
}
else
if(a0==76)
{
   RunJavaScript('CarCalcu_76.cal',1);
}
else
if(a0==77)
{
   RunJavaScript('CarCalcu_77.cal',1);
}
else
if(a0==78)
{
   RunJavaScript('CarCalcu_78.cal',1);
}
else
if(a0==79)
{
   RunJavaScript('CarCalcu_79.cal',1);
}
else
if(a0==80)
{
   RunJavaScript('CarCalcu_80.cal',1);
}
else
if(a0==81)
{
   RunJavaScript('CarCalcu_81.cal',1);
}
else
if(a0==82)
{
   RunJavaScript('CarCalcu_82.cal',1);
}
else
if(a0==83)
{
   RunJavaScript('CarCalcu_83.cal',1);
}
else
if(a0==84)
{
   RunJavaScript('CarCalcu_84.cal',1);
}
else
if(a0==85)
{
   RunJavaScript('CarCalcu_85.cal',1);
}
else
if(a0==86)
{
   RunJavaScript('CarCalcu_86.cal',1);
}
else
if(a0==87)
{
	Result=GetValue('LVAd apical',-1);
	OutData('LVAd apical',Result/100,10);
}
else
if(a0==88)
{
	Result=GetValue('LVAd sax MV',-1);
	OutData('LVAd sax MV',Result/100,10);
}
else
if(a0==89)
{
	Result=GetValue('LVAs apical',-1);
	OutData('LVAs apical',Result/100,10);
}
else
if(a0==90)
{
	Result=GetValue('LVAs sax epi',-1);
	OutData('LVAs sax epi',Result/100,10);
        RunJavaScript('CarCalcu_49.cal',1);
}
else
if(a0==91)
{
	Result=GetValue('LVAs sax MV',-1);
	OutData('LVAs sax MV',Result/100,10);
}
else
if(a0==92)
{	
        Result=GetValue('MVA(traced)',-1);
	OutData('MVA(traced)',Result/100,10);       
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////  CARDIAC CALCULATION   ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


//RunJavaScript('CarCalcu1.cal',1);
//RunJavaScript('CarCalcu2.cal',1);
//RunJavaScript('CarCalcu3.cal',1);
//RunJavaScript('CarCalcu4.ca.',1);