var a0=GetInfo(0);
if(a0==0)
{
	SetInfo(7,0);
	Clear();
	Declare('测量标尺',6,5,-1,1);
}
else
if(a0==1)
{
	if(GetInfo(8))
	{
		if(GetROIData(0)==4)
		{
			var a1=GetROIData(2);
			var a2=GetROIData(3);
			var x0=LOWORD(a1);
			var y0=HIWORD(a1);
			var x1=LOWORD(a2);
			var y1=HIWORD(a2);

			a1=(x1-x0)*(x1-x0);
			a2=(y1-y0)*(y1-y0);
			
			a1=Math.sqrt(a1+a2);

			if(GetString('标尺长度(毫米)','10'))
			{
				var textstr=ResultString();
				var db=eval(textstr);
				if(db&&a1)
				{
					db=db/a1;
					Event(4,db,db,0,0);
					Message('标尺长度='+db+'(毫米/点)');
				}
				else
					Message('定标失败!!!');
			}
		}
	}
}
