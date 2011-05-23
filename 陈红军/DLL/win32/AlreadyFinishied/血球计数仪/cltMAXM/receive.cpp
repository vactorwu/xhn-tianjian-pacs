//FULL HANDSHAKE
//-----------------------------数据链路层---------------------------------
//接收SYN
//发送SYN

//接收块数-->nTotalBlock(两个字节)
//发送ACK

//接收BLOCK1(264字节或136字节):将分隔符NULL或HT换成SP
//发送ACK
	//填入数据包
//或NAK
//或SYN

//接收SYN
//将完整的数据包lpData交给表示层
	//由表示层发送ACK


/*-----------------------------表示层---------------------------------
功能：将数据链路层传递过来的数据包分解为组，提交给应用层（放入缓冲区）
		调用回调函数
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
包结构：
	PREAMBLE|GROUPx1|GROUPx2|......|GROUPxy|POSTAMBLE
---------------------------------------------------------------------*/

//1.提取包前缀Preamble
//2.找到DC1,开始“节(section)”
//3.取出Field数nField(两个字节)
	/*	if nField=0 then
			回到第2步
		else
			nFieldCount=0
		endif */
//4.取出field放入到缓冲区OutResult中
	/*
		repeat
			以CR/LF为分界符取出Field,并将有关信息放入OutResult中
		until DC1 //遇到Postamble
	*/
//5.发送ACK
//6.将接收到的数据传送到上层缓冲区，即：
		//(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);

	
