//FULL HANDSHAKE
//-----------------------------������·��---------------------------------
//����SYN
//����SYN

//���տ���-->nTotalBlock(�����ֽ�)
//����ACK

//����BLOCK1(264�ֽڻ�136�ֽ�):���ָ���NULL��HT����SP
//����ACK
	//�������ݰ�
//��NAK
//��SYN

//����SYN
//�����������ݰ�lpData������ʾ��
	//�ɱ�ʾ�㷢��ACK


/*-----------------------------��ʾ��---------------------------------
���ܣ���������·�㴫�ݹ��������ݰ��ֽ�Ϊ�飬�ύ��Ӧ�ò㣨���뻺������
		���ûص�����
		(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);
���ṹ��
	PREAMBLE|GROUPx1|GROUPx2|......|GROUPxy|POSTAMBLE
---------------------------------------------------------------------*/

//1.��ȡ��ǰ׺Preamble
//2.�ҵ�DC1,��ʼ����(section)��
//3.ȡ��Field��nField(�����ֽ�)
	/*	if nField=0 then
			�ص���2��
		else
			nFieldCount=0
		endif */
//4.ȡ��field���뵽������OutResult��
	/*
		repeat
			��CR/LFΪ�ֽ��ȡ��Field,�����й���Ϣ����OutResult��
		until DC1 //����Postamble
	*/
//5.����ACK
//6.�����յ������ݴ��͵��ϲ㻺����������
		//(*lpResultProcessRoutine)(lpDevice->nDevNO, OutResult, nItemCount);

	
