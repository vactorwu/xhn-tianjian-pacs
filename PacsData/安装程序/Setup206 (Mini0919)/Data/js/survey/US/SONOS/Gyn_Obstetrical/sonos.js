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

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////  Gynecologic_Obstetrical Measurements   ///////////////////////////////
/////////////////// Info:  1~14  PI,RI     20~21   mean velocity   ////////////////////////
//////////////////          30~73                                  ////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();

	Declare('2 beats Pk-to-Pk',4,11,3,0);
	Declare('AC traced',3,8,0,30);
	Declare('ADap',1,5,0,30);
	Declare('ADtrv',1,5,0,30);
	Declare('A root diam',1,5,0,0);
	Declare('BD',1,5,0,0);
	Declare('BPD',1,5,0,31);
	Declare('CD',1,5,0,32);
	Declare('CRL',1,5,0,33);
	Declare('CxH',1,5,0,0);
	Declare('CxL',1,5,0,0);
	Declare('CxW',1,5,0,0);
        //Desc. Aorta
	Declare('DescAo accel T',4,11,3,34);
	Declare('DescAo DV',5,12,1,35);
	Declare('DescAo Mean V(PI)',6,14,1,1);//Caculate mean value
	Declare('DescAo MnV PI,RI',6,14,1,1);//
	Declare('DescAo MxV PI,RI',6,14,1,1);//
	Declare('DescAo SV',5,12,1,35);
	Declare('Ductus Accel T',4,11,3,36);
	Declare('Ductus DV',5,12,1,37);
	Declare('Ductus MnV PI,RI',6,14,1,2);//
	Declare('Ductus MxV PI,RI',6,14,1,2);//
	Declare('Ductus SV',5,12,1,37);
	Declare('DV1',5,12,1,0);
	Declare('DV2',5,12,1,0);


	Declare('ET',1,5,0,0);
	Declare('Fetal Ao DV',5,12,1,38);
	Declare('Fetal Ao meanV(PI)',6,14,1,3);//Caculate mean value
	Declare('Fetal Ao SV',5,12,1,38);
	Declare('Fetal Ao MnV PI,RI',6,14,1,3);//
	Declare('Fetal Ao MxV PI,RI',6,14,1,3);//
	Declare('FL',1,5,0,39);
	Declare('FTL',1,5,0,0);
	Declare('GSD1',1,5,0,40);
	Declare('GSD2',1,5,0,40);
	Declare('GSD3',1,5,0,40);
	Declare('HC(traced)',3,8,0,0);
	Declare('HL',1,5,0,41);


	Declare('L Ov A DV',5,12,1,42);
	Declare('L Ov A meanV(PI)',6,14,1,4);//Caculate mean value
	Declare('L Ov A MnV PI,RI',6,14,1,4);//
	Declare('L Ov A MxV PI,RI',6,14,1,4);//
	Declare('L Ov A SV',5,12,1,42);
	Declare('L Ut A DV',5,12,1,43);
	Declare('L Ut A meanV(PI)',6,14,1,5);//Caculate mean value
	Declare('L Ut A MnV PI,RI',6,14,1,5);//
	Declare('L Ut A MxV PI,RI',6,14,1,5);//
	Declare('L Ut A SV',5,12,1,43);
	Declare('L Follic Diam',1,5,0,0);
	Declare('LLQ',1,5,0,44);
	Declare('LOH',1,5,0,0);
	Declare('LOL',1,5,0,0);
	Declare('LOV',7,13,2,0);
	Declare('LOW',1,5,0,0);
	Declare('LPA Accel T',4,11,3,45);
	Declare('LPA DV',5,12,1,46);
	Declare('LPA mean vel',6,14,1,6);//Caculate mean value
	Declare('LPA MnV PI,RI',6,14,1,6);//
	Declare('LPA MxV PI,RI',6,14,1,6);//
	Declare('LPA SV',5,12,1,46);
	Declare('LRH',1,5,0,0);
	Declare('LRL',1,5,0,0);
	Declare('LRPD',1,5,0,0);
	Declare('LRV',7,13,2,0);
	Declare('LRW',1,5,0,0);
	Declare('LUQ',1,5,0,44);
	Declare('LVIDd',1,5,0,0);


	Declare('Mean vel 1',6,14,1,20);//caculate mean value
	Declare('Mean vel 2',6,14,1,21);//caculate mean value
	Declare('MPA accel T',4,11,3,47);
	Declare('MPA diam',1,5,0,0);
	Declare('MPA DV',5,12,1,48);
	Declare('MPA mean vel',6,14,1,7);//caculate mean value
	Declare('MPA MnV PI,RI',6,14,1,7);//
	Declare('MPA MxV PI,RI',6,14,1,7);//
	Declare('MPA SV',5,12,1,48);
	Declare('NT',1,5,0,0);
	Declare('OFD',1,5,0,49);


	Declare('R Ov A DV',5,12,1,50);
	Declare('R Ov A mean V(PI)',6,14,1,8);//Caculate mean value
	Declare('R Ov A MnV PI,RI',6,14,1,8);
	Declare('R Ov A MxV PI,RI',6,14,1,8);
	Declare('R Ov A SV',5,12,1,50);
	Declare('R Ut A DV',5,12,1,51);
	Declare('R Ut A mean V(PI)',6,14,1,9);//Caculate mean value
	Declare('R Ut A MnV PI,RI',6,14,1,9);
	Declare('R Ut A MxV PI,RI',6,14,1,9);
	Declare('R Ut A SV',5,12,1,51);
	Declare('R Follic Diam',1,5,0,0);
	Declare('Renal L A accel T',4,11,3,0);
	Declare('Renal L A DV',5,12,1,52);
	Declare('Renal L A mean V(PI)',6,14,1,10);//Caculate mean value
	Declare('Renal L A MnV PI,RI',6,14,1,10);
	Declare('Renal L A MxV PI,RI',6,14,1,10);
	Declare('Renal L A SV',5,12,1,52);

        //Renal L Artery
	Declare('Renal R A accel T',4,11,3,0);
	Declare('Renal R A DV',5,12,1,53);
	Declare('Renal R A mean V(PI)',6,14,1,11);//Caculate mean value
	Declare('Renal R A MnV PI,RI',6,14,1,11);
	Declare('Renal R A MxV PI,RI',6,14,1,11);
	Declare('Renal R A SV',5,12,1,53);
        //Renal R Artery
	Declare('RLQ',1,5,0,44);
	Declare('ROH',1,5,0,0);
	Declare('ROL',1,5,0,0);
	Declare('ROV',7,13,2,0);
	Declare('ROW',1,5,0,0);
	Declare('RPA accel T',4,11,3,0);
	Declare('RPA DV',5,12,1,54);
	Declare('RPA mean vel',6,14,1,12);//Caculate mean value
	Declare('RPA MnV PI,RI',6,14,1,12);
	Declare('RPA MxV PI,RI',6,14,1,12);
	Declare('RPA SV',5,12,1,54);
	Declare('RRH',1,5,0,0);
	Declare('RRL',1,5,0,0);
	Declare('RRPD',1,5,0,0);
	Declare('RRV',7,13,2,0);
	Declare('RRW',1,5,0,0);
	Declare('RUQ',1,5,0,44);
	Declare('RVDd',1,5,0,0);

	Declare('SV1',5,12,1,0);
	Declare('SV2',5,12,1,0);
	Declare('TC traced',3,8,57);
	Declare('TDap',1,5,0,55);
	Declare('TDtrv',1,5,0,55);
	//Thoracic Aorta
	Declare('ThorAo accel T',4,11,3,0);
	Declare('ThorAo DV',5,12,1,56);
	Declare('ThorAo mean vel',6,14,1,13);//Caculate mean value
	Declare('ThorAo MnV PI,RI',6,14,1,13);
	Declare('ThorAo MxV PI,RI',6,14,1,13);
	Declare('ThorAo SV',5,12,1,56);
	Declare('TL',1,5,0,57);
	Declare('UL',1,5,0,58);
	Declare('Um A accel T',4,11,3,0);
	Declare('Um A DV',5,12,1,59);
	Declare('Um A mean V(PI)',6,14,1,14);//Caculate mean value
	Declare('Um A MnV PI,RI',6,14,1,14);
	Declare('Um A MxV PI,RI',6,14,1,14);
	Declare('Um A SV',5,12,1,59);

        //Umbilical Artery
	Declare('UTH',1,5,0,0);
	Declare('UTL',1,5,0,0);
	Declare('UTV',7,13,2,0);
	Declare('UTW',1,5,0,0);
	Declare('VAD',1,5,0,0);
	Declare('YSD',1,5,0,0);
}
else
if(a0==1)
    PIRI('DescAo meanV(PI)','DescAo MxV PI,RI','DescAo MnV PI,RI','DescAo PI','DescAo RI');
else
if(a0==2)
    PIRI('Ductus meanV(PI)','Ductus MxV PI,RI','Ductus MnV PI,RI','Ductus PI','Ductus RI');
else
if(a0==3) 
   PIRI('Fetal Ao meanV(PI)','Fetal Ao MxV PI,RI','Fetal Ao MnV PI,RI','Fetal Ao PI','Fetal RI');
else
if(a0==4)
    PIRI('L Ov A meanV(PI)','L Ov A MxV PI,RI','L Ov A MnV PI,RI','L Ov A PI','L Ov A RI');
else
if(a0==5)
    PIRI('L Ut A meanV(PI)','L Ut A MxV PI,RI','L Ut A MnV PI,RI','L Ut A PI','L Ut A RI');
else
if(a0==6)
    PIRI('LPA mean vel','MPA MxV PI,RI','LPA MnV PI,RI','LPA PI','LPA RI');
else
if(a0==7)
    PIRI('MPA mean vel','MPA MxV PI,RI','MPA MnV PI,RI','MPA PI','MPA RI');
else
if(a0==8)
    PIRI('R Ov A mean V(PI)','R Ov A MxV PI,RI','R Ov A MnV PI,RI','R Ov A PI','R Ov A RI');
else
if(a0==9)
    PIRI('R Ut A mean V(PI)','R Ut A MxV PI,RI','R Ut A MnV PI,RI','R Ut A PI','R Ut A RI');
else
if(a0==10)
    PIRI('Renal L A mean V(PI)','Renal L A MxV PI,RI','Renal L A MnV PI,RI','Renal L A PI','Renal L A RI');
else
if(a0==11)
    PIRI('Renal R A mean V(PI)','Renal R A MxV PI,RI','Renal R A MnV PI,RI','Renal R A PI','Renal R A RI');
else
if(a0==12)
    PIRI('RPA mean vel','RPA MxV PI,RI','RPA MnV PI,RI','RPA PI','RPA RI');
else
if(a0==13)
    PIRI('ThorAo mean vel','ThorAo MxV PI,RI','ThorAo MnV PI,RI','ThorAo PI','ThorAo RI');
else
if(a0==14)
    PIRI('Um mean V(PI)','Um MxV PI,RI','Um MnV PI,RI','Um PI','Um RI');
else
if(a0==20)
    MeanVel('Mean vel 1');
else
if(a0==21)
    MeanVel('Mean vel 2');
else
if(a0==30)
{
   RunJavaScript('GOCalcu_30.cal',1);
}
else
if(a0==31)
{
   RunJavaScript('GOCalcu_31.cal',1);
}
else
if(a0==32)
{
   RunJavaScript('GOCalcu_32.cal',1);
}
else
if(a0==33)
{
   RunJavaScript('GOCalcu_33.cal',1);
}
else
if(a0==34)
{
   RunJavaScript('GOCalcu_34.cal',1);
}
else
if(a0==35)
{
   RunJavaScript('GOCalcu_35.cal',1);
}
else
if(a0==36)
{
   RunJavaScript('GOCalcu_36.cal',1);
}
else
if(a0==37)
{
   RunJavaScript('GOCalcu_37.cal',1);
}
else
if(a0==38)
{
   RunJavaScript('GOCalcu_38.cal',1);
}
else
if(a0==39)
{
   RunJavaScript('GOCalcu_39.cal',1);
}
else
if(a0==40)
{
   RunJavaScript('GOCalcu_40.cal',1);
}
else
if(a0==41)
{
   RunJavaScript('GOCalcu_41.cal',1);
}
else
if(a0==42)
{
   RunJavaScript('GOCalcu_42.cal',1);
}
else
if(a0==43)
{
   RunJavaScript('GOCalcu_43.cal',1);
}
else
if(a0==44)
{
   RunJavaScript('GOCalcu_44.cal',1);
}
else
if(a0==45)
{
   RunJavaScript('GOCalcu_45.cal',1);
}
else
if(a0==46)
{
   RunJavaScript('GOCalcu_46.cal',1);
}
else
if(a0==47)
{
   RunJavaScript('GOCalcu_47.cal',1);
}
else
if(a0==48)
{
   RunJavaScript('GOCalcu_48.cal',1);
}
else
if(a0==49)
{
   RunJavaScript('GOCalcu_49.cal',1);
}
else
if(a0==50)
{
   RunJavaScript('GOCalcu_50.cal',1);
}
else
if(a0==51)
{
   RunJavaScript('GOCalcu_51.cal',1);
}
else
if(a0==52)
{
   RunJavaScript('GOCalcu_52.cal',1);
}
else
if(a0==53)
{
   RunJavaScript('GOCalcu_53.cal',1);
}
else
if(a0==54)
{
   RunJavaScript('GOCalcu_54.cal',1);
}
else
if(a0==55)
{
   RunJavaScript('GOCalcu_55.cal',1);
}
else
if(a0==56)
{
   RunJavaScript('GOCalcu_56.cal',1);
}
else
if(a0==57)
{
   RunJavaScript('GOCalcu_57.cal',1);
}
else
if(a0==58)
{
   RunJavaScript('GOCalcu_58.cal',1);
}
else
if(a0==59)
{
   RunJavaScript('GOCalcu_59.cal',1);
}


//////////////////////////////////////////////////////////////////////////
////////  Gynecologic_Obstetrical Calculation            /////////////////
//////////////////////////////////////////////////////////////////////////

//RunJavaScript('GOCalcu1.cal',1);
//RunJavaScript('GOCalcu2.cal',1);

