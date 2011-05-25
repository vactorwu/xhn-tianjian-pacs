var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	LoadConfig('us.config');
	Declare('时间轴颜色',6,15,-1,1);
	Declare('距离/速度轴颜色',6,15,-1,2);
	Declare('2维图(2D)定标线颜色',6,15,-1,3);
	Declare('修改识别参数',0,0,-1,4);
	Declare('特征识别',6,6,-1,5);

	Declare('2维图(2D)定标(区域法)',6,6,-1,6);

	Declare('DOPPLER速度轴定标(左边)',6,12,-1,7);
	Declare('DOPPLER速度轴定标(右边)',6,12,-1,8);
	
	Declare('M-MODE距离定标(左边)',6,12,-1,9);

	//Declare('DOPPLER刻度识别区域(左边)',6,6,-1,10);
	//Declare('DOPPLER刻度识别区域(右边)',6,6,-1,11);

	Declare('学习字体',6,6,-1,12);
	Declare('重新学习',0,0,-1,13);
	
	Declare('保存设置',0,0,-1,14);
	Declare('装入设置',0,0,-1,15);
	Declare('测试自动定标',0,0,-1,16);
}
else
if(a0<=3)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=GetROIData(2);
			var db=Event(6,a1,0,0,0);
			OutData('颜色',db);
			if(a0==1)
				SetInfo(200,db);
			else
			if(a0==2)
				SetInfo(201,db);
			else
			if(a0==3)
				SetInfo(202,db);
		}
	}
}
else
if(a0==4)
{
	if(GetString('指定色差动态范围',GetInfo(203)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('范围1',db);
		SetInfo(203,db);
	}
	if(GetString('指定特征变化范围(DOPPLER标尺)',GetInfo(204)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('范围2',db);
		SetInfo(204,db);
	}
	if(GetString('指定特征变化范围(2D标尺)',GetInfo(219)))
	{
		var textstr=ResultString();
		var db=eval(textstr);
		OutData('范围3',db);
		SetInfo(219,db);
	}
}
else
if(a0==5)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(9,a1,a2,a3,0);
			OutData('类型',(LOWORD(a4)%4));
			OutData('基线',(LOWORD(a4)/4));
			OutData('标尺',(HIWORD(a4)));
			SetInfo(205,a1);//第一特征区域
			SetInfo(206,a2);

			a1=(LOWORD(a4)/4);
			a4=Event(13,0,a1,a3,0);
			OutData('起点',(LOWORD(a4)));
			OutData('终点',(HIWORD(a4)));
		}
	}
}
else
if(a0==6)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(219),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(202)
			var a4=Event(12,a1,a2,a3,10);
			OutData('2D标尺',a4);
			if(a4)
			{
				SetInfo(207,a1);
				SetInfo(208,a2);
			}
		}
	}
}
else
if(a0>=7&&a0<=9)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==4||GetROIData(0)==8||GetROIData(0)==9)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201)
			var a4=Event(1,a1,a2,a3,0);
			if(a0==8||a0==7)
				OutData('DOPPLER标尺',a4);
			else
				OutData('M-MODE标尺',a4);
			
			if(a4>=0)
			{
				if(a0==7)
				{
					SetInfo(209,a1);
					SetInfo(210,a2);
				}
				else
				if(a0==8)
				{
					SetInfo(211,a1);
					SetInfo(212,a2);
				}
				else
				if(a0==9)
				{
					SetInfo(213,a1);
					SetInfo(214,a2);
				}
			}
		}
	}
}
else
if(a0>=10&&a0<=11)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201)
			Event(7,GetInfo(203),GetInfo(204),0,0);
			Event(3,a1,a2,a3,0);
			var textstr=ResultString();
			if(textstr.length>1)
			{
				Message(textstr);
				OutData('刻度',atoiStr(textstr,'CAL='));
				if(a0==10)
				{
					SetInfo(215,a1);
					SetInfo(216,a2);
				}
				else
				if(a0==11)
				{
					SetInfo(217,a1);
					SetInfo(218,a2);
				}
			}
		}
	}
}
else
if(a0==12)
{
	if(GetInfo(8))
	{
		Event(7,GetInfo(203),GetInfo(204),0,0);
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(201);
			Event(10,a1,a2,a3,0);
		}
	}	
}
else
if(a0==13)
{
	Event(11,0,0,0,0);
	Message('字体模板已清除！请开始重新学习。');
}
else
if(a0==14)
	SaveConfig('us.config');
else
if(a0==15)
	LoadConfig('us.config');
