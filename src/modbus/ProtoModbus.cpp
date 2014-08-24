#include <string>
#include <string.h>
#include <stdio.h> 
#include <iostream>
#include <stdlib.h>
#include "common.h"
#include <stdlib.h>

#ifdef WIN32
	#include <windows.h>
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

#include "ProtoModbus.h"


/*
CRC-16码的计算步骤
1、	置16位寄存器为十六进制FFFF(即全为1)。称此寄存器为CRC寄存器。
2、	把一个8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器。
3、	把寄存器的内容右移一位(朝低位)，用0填补最高位，检查最低位(移出位)。
4、	如果最低位为0：复第3步(再次移位)。
如果最低位为1：CRC寄存器与多项式A001(1010 0000 0000 0001)进行异或。
5、	重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理。
6、	重复步骤2到步骤5，进行下一个8位的处理。
7、	最后得到的CRC寄存器即为CRC码，低字节在前，高字节在后。
*/

//CRC16，modbus RTU专用
uint16 calccrc(uint8 crcbuf, uint16 crc)
{
	int i; 
	crc=crc ^ crcbuf;
	for(i=0;i<8;i++)
	{
		uint8 chk;
		chk=crc&1;
		crc=crc>>1;
		crc=crc&0x7fff;
		if (chk==1)
			crc=crc^0xa001;
		crc=crc&0xffff;
	}
	return crc; 
}

uint16 CRC_16(const uint8 *buf, int len)//结构为INTEL顺序，低字节在前，高字节在后
{
	int  i;
	unsigned short crc;
	crc=0xFFFF;
	for (i=0;i<len;i++)
	{	
		crc=calccrc(*buf,crc);
		buf++;
	}
	return crc;
}



uint16 CProtoModbusCom::GetReqBufSize(uint8 funcode)
{
	if(funcode < 5)
	{
		return 8;//1+5+2
	}
	else
	{
		return 0;//其他功能码暂时不考虑
	}
}

uint16 CProtoModbusTcp::GetReqBufSize(uint8 funcode)
{
	if(funcode < 5)
	{
		return 12;//7+5
	}
	else
	{
		return 0;//其他功能码暂时不考虑
	}
}

uint16 CProtoModbusTcp::GetRspBufSize(uint8 funcode, uint16 cnt)
{	
	uint16 wLen=0;
	switch(funcode)//数据长度
	{	
	case 0x01:
	case 0x02:
		wLen = (cnt < 1) ? 9 : 9 + (cnt-1)/8 + 1;//7+2=9,后面是数据长度			
		break;
	case 0x03:
	case 0x04:
		wLen = 9 + cnt*2;
		break;
	default :
		wLen = 0;
	}
	return wLen;
}

uint16 CProtoModbusCom::GetRspBufSize(uint8 funcode, uint16 cnt)
{	
	uint16 wLen=0;
	switch(funcode)//数据长度
	{	
	case 0x01:
	case 0x02:
		wLen = (cnt < 1) ? 5 : 5 + (cnt-1)/8 + 1;			
		break;
	case 0x03:
	case 0x04:
		wLen = 5 + cnt*2;
		break;
	default :
		wLen = 0;
	}
	return wLen;
}

void CProtoModbusTcp::PackPollingReq(uint8 devaddr
										, uint8 funcode
										, uint16 startaddr
										, uint16 cnt
										, uint8* &out
										, uint16 &outlen)
{
	static uint16 wNo = 1;
	*(uint16*)(out) = htons(wNo++);//相当于包序号，暂时不用
	out += sizeof(uint16);
	*(uint16*)(out) = htons(0);//0 -> modbus
	out += sizeof(uint16);
	*(uint16*)(out) = htons(GetReqBufSize(funcode)-6);//此字段后面的数据长度
	out += sizeof(uint16);
	*(uint8*)out = devaddr; //设备地址
	out += sizeof(uint8);

	uint16 pdulen = 0;
	PackPdu(funcode, startaddr, cnt, out, pdulen);//PDU

	outlen = GetReqBufSize(funcode);
}

void CProtoModbusCom::PackPollingReq(uint8 devaddr
										, uint8 funcode
										, uint16 startaddr
										, uint16 cnt
										, uint8* &out
										, uint16 &outlen)
{
	uint8 *pTemp = out;

	*(uint8*)pTemp = devaddr; //设备地址
	pTemp += sizeof(uint8);

	uint16 wLen = 0;
	PackPdu(funcode, startaddr, cnt, pTemp, wLen);//PDU
	pTemp += wLen;
	
	*(uint16*)pTemp = CRC_16(out, GetReqBufSize(funcode)-2);//crc校验

	outlen = GetReqBufSize(funcode);

}

bool CProtoModbusTcp::ParsePollingRsp(uint8 *in
									 , uint16 inlen
									 , uint8 &devaddr
									 , uint8 &funcode
									 , uint8* &out
									 , uint16 &outlen)
{
	uint8 *pdu = NULL;
	uint16 pdulen = 0;
	if(!GetPdu(in, inlen, pdu, pdulen))
	{
		return false;
	}

	devaddr = *(uint8*)(in+6);

	ParsePdu(pdu, pdulen, funcode, out, outlen);

	return true;
}

bool CProtoModbusCom::ParsePollingRsp(uint8 *in
									  , uint16 inlen
									  , uint8 &devaddr
									  , uint8 &funcode
									  , uint8* &out
									  , uint16 &outlen)
{
	uint8 *pdu = NULL;
	uint16 pdulen = 0;

	if(!GetPdu(in, inlen, pdu, pdulen))
	{
		return false;
	}

	devaddr = *(uint8*)in;

	ParsePdu(pdu, pdulen, funcode, out, outlen);

	return true;
}

void CProtoModbus::PackPdu(uint8 funcode
			 , uint16 startaddr
			 , uint16 cnt
			 , uint8 *pdu
			 , uint16 &pdulen)
{
	pdulen = 0;
	*(uint8*)pdu = funcode;
	pdulen += sizeof(uint8);
	*(uint16*)(pdu+pdulen) = htons(startaddr);
	pdulen += sizeof(uint16);
	*(uint16*)(pdu+pdulen) = htons(cnt);
	pdulen += sizeof(uint16);
}

void CProtoModbus::ParsePdu(uint8 *pdu
			  , uint16 pdulen
			  , uint8 &funcode
			  , uint8* &out
		      , uint16 &outlen)
{
	uint8 *pTemp = pdu;
	funcode = *(uint8*)pTemp;
	pTemp += sizeof(uint8);
	outlen  = *(uint8*)pTemp;
	pTemp += sizeof(uint8);
	out = pTemp;
}

/*
tcp ADU格式：7(MBAP头)+pdu
*/
bool CProtoModbusTcp::GetPdu( uint8 *adu
								, uint16 adulen
								, uint8* &pdu
								, uint16 &pdulen)
{
	if(adu)
	{
		pdu = adu + 7;//取出5个MBAP头
		pdulen = adulen - 7;
		return true;
	}

	return false;
}

/*
串口ADU格式：1(设备地址)+PDU+2(校验)
*/

bool CProtoModbusCom::GetPdu( uint8 *adu
								, uint16 adulen
								, uint8* &pdu
								, uint16 &pdulen)
{
	if(adu)
	{
		uint16 wCrc = *(uint16*)(adu+adulen-2);
		if(CRC_16(adu, adulen-2) != wCrc)
		{
			return false;
		}
		pdu = adu + 1;//跳过1个头
		pdulen = adulen - 3;
		return true;
	}

	return false;
}
