var a0=GetInfo(0);
Event(7,40,2,0,0);
if(a0==0)
{
	SetInfo(7,0);
	
	Clear();
	LoadConfig('us.config');

	Declare('目标图像阀值',6,15,-1,1);
	Declare('2维图(2D)定标(左边)',6,5,-1,2);
	Declare('2维图(2D)定标(右边)',6,5,-1,13);
	Declare('DOPPLER速度轴定标',6,12,-1,3);
	Declare('DOPPLER时间轴定标',6,11,-1,4);
	Declare('M-MODE距离定标',6,12,-1,5);
	Declare('M-MODE时间轴定标',6,11,-1,6);
	Declare('特征识别',6,6,-1,7);
	Declare('DOPPLER刻度识别区域',6,6,-1,8);
	Declare('M-MODE刻度识别区域',6,6,-1,9);
	Declare('学习字体',6,6,-1,10);
	Declare('重新学习',0,0,-1,11);
	Declare('保存设置',0,0,-1,12);
	Declare('装入设置',0,0,-1,13);
	Declare('测试自动定标',0,0,-1,14);
}
else
if(a0==1)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==7)
		{
			var a1=GetROIData(2);
			var db=Event(6,a1,0,0,0);
			OutData('阀值',db);
			SetInfo(200,db);
		}
	}
}
else
if((a0>=2&&a0<=6)||(a0==13))
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==4||GetROIData(0)==8||GetROIData(0)==9)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(1,a1,a2,a3,0);
			if(a0==2||a0==13)
				OutData('2D标尺',a4);
			else
			if(a0==3||a0==4)
				OutData('DOPPLER标尺',a4);
			else
				OutData('M-MODE标尺',a4);
			if(a4>=0)
			{
				if(a0==2)
				{
					SetInfo(201,a1);
					SetInfo(202,a2);
				}
				else
				if(a0==13)
				{
					SetInfo(217,a1);
					SetInfo(218,a2);
				}
				else
				if(a0==3)
				{
					SetInfo(203,a1);
					SetInfo(204,a2);
				}
				else
				if(a0==4)
				{
					SetInfo(205,a1);
					SetInfo(206,a2);
				}
				else
				if(a0==5)
				{
					SetInfo(207,a1);
					SetInfo(208,a2);
				}
				else
				if(a0==6)
				{
					SetInfo(209,a1);
					SetInfo(210,a2);
				}
			}
		}
	}
}
else
if(a0==7)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			var a4=Event(2,a1,a2,a3,0);
			OutData('类型',LOWORD(a4));
			OutData('基线',HIWORD(a4));
			SetInfo(211,a1);
			SetInfo(212,a2);
		}
	}
}
else
if(a0==8||a0==9)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200)
			Event(3,a1,a2,a3,0);
			var textstr=ResultString();
			if(textstr.length>1)
			{
				Message(textstr);
				OutData('刻度',atoiStr(textstr,'CAL'));
				if(a0==8)
				{
					SetInfo(213,a1);
					SetInfo(214,a2);
				}
				else
				{
					SetInfo(215,a1);
					SetInfo(216,a2);
				}
			}
		}
	}
}
else
if(a0==10)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==2)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var a3=GetInfo(200);
			Event(10,a1,a2,a3,0);
		}
	}	
}
else
if(a0==11)
{
	Event(11,0,0,0,0);
	Message('字体模板已清除！请开始重新学习。');
}
else
if(a0==12)
	SaveConfig('us.config');
else
if(a0==13)
	LoadConfig('us.config');
