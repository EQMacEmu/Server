/*
	Copyright (C) 2005 Michael S. Finger

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "crc16.h"
#include "global_define.h"
#include "crc32.h"
#include "eq_packet.h"
#include "eq_stream.h"
#include "misc.h"
#include "op_codes.h"
#include "platform.h"

#include <sstream>
#ifndef STATIC_OPCODE
#include "opcodemgr.h"
#endif
#include "packet_dump.h"
#include "packet_functions.h"

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdio.h>

EQPacket::EQPacket(EmuOpcode op, const unsigned char *buf, uint32 len)
:	BasePacket(buf, len),
	emu_opcode(op)
{
}

void EQPacket::build_raw_header_dump(char *buffer, uint16 seq) const {
	BasePacket::build_raw_header_dump(buffer, seq);
	buffer += strlen(buffer);

	buffer += sprintf(buffer, "[EmuOpCode 0x%04x Size=%u]\n", emu_opcode, size);
}

void EQPacket::DumpRawHeader(uint16 seq, FILE *to) const
{
	char buff[196];
	build_raw_header_dump(buff, seq);
	fprintf(to, "%s", buff);
}

void EQPacket::build_header_dump(char *buffer) const {
	sprintf(buffer, "[EmuOpCode 0x%04x Size=%u]", emu_opcode, size);
}

void EQPacket::DumpRawHeaderNoTime(uint16 seq, FILE *to) const
{
	if (src_ip) {
		std::string sIP,dIP;
		sIP=long2ip(src_ip);
		dIP=long2ip(dst_ip);
		fprintf(to, "[%s:%d->%s:%d] ",sIP.c_str(),src_port,dIP.c_str(),dst_port);
	}
	if (seq != 0xffff)
		fprintf(to, "[Seq=%u] ",seq);

	fprintf(to, "[EmuOpCode 0x%04x Size=%lu]\n",emu_opcode,(unsigned long)size);
}

void EQProtocolPacket::build_raw_header_dump(char *buffer, uint16 seq) const
{
	BasePacket::build_raw_header_dump(buffer, seq);
	buffer += strlen(buffer);

	buffer += sprintf(buffer, "[ProtoOpCode 0x%04x Size=%u]\n",opcode,size);
}

void EQProtocolPacket::DumpRawHeader(uint16 seq, FILE *to) const
{
	char buff[196];
	build_raw_header_dump(buff, seq);
	fprintf(to, "%s", buff);
}

void EQProtocolPacket::build_header_dump(char *buffer) const
{
	sprintf(buffer, "[ProtoOpCode 0x%04x Size=%u]",opcode,size);
}

void EQProtocolPacket::DumpRawHeaderNoTime(uint16 seq, FILE *to) const
{
	if (src_ip) {
		std::string sIP,dIP;
		sIP=long2ip(src_ip);
		dIP=long2ip(dst_ip);
		fprintf(to, "[%s:%d->%s:%d] ",sIP.c_str(),src_port,dIP.c_str(),dst_port);
	}
	if (seq != 0xffff)
		fprintf(to, "[Seq=%u] ",seq);

	fprintf(to, "[ProtoOpCode 0x%04x Size=%lu]\n",opcode,(unsigned long)size);
}

void EQApplicationPacket::build_raw_header_dump(char *buffer, uint16 seq) const
{
	BasePacket::build_raw_header_dump(buffer, seq);
	buffer += strlen(buffer);

#ifdef STATIC_OPCODE
	buffer += sprintf(buffer, "[OpCode 0x%04x Size=%u]\n", emu_opcode,size);
#else
	buffer += sprintf(buffer, "[OpCode %s Size=%u]\n",OpcodeManager::EmuToName(emu_opcode),size);
#endif
}

void EQApplicationPacket::DumpRawHeader(uint16 seq, FILE *to) const
{
	char buff[196];
	build_raw_header_dump(buff, seq);
	fprintf(to, "%s", buff);
}

void EQApplicationPacket::build_header_dump(char *buffer) const
{
#ifdef STATIC_OPCODE
	sprintf(buffer, "[OpCode 0x%04x Size=%u]\n", emu_opcode,size);
#else
	sprintf(buffer, "[OpCode %s(0x%04x) Size=%u]", OpcodeManager::EmuToName(emu_opcode), GetProtocolOpcode(), size);
#endif
}

void EQApplicationPacket::DumpRawHeaderNoTime(uint16 seq, FILE *to) const
{
	if (src_ip) {
		std::string sIP,dIP;
		sIP=long2ip(src_ip);
		dIP=long2ip(dst_ip);
		fprintf(to, "[%s:%d->%s:%d] ",sIP.c_str(),src_port,dIP.c_str(),dst_port);
	}
	if (seq != 0xffff)
		fprintf(to, "[Seq=%u] ",seq);

#ifdef STATIC_OPCODE
	fprintf(to, "[OpCode 0x%04x Size=%u]\n", emu_opcode,size);
#else
	fprintf(to, "[OpCode %s Size=%lu]\n",OpcodeManager::EmuToName(emu_opcode),(unsigned long)size);
#endif
}

void EQRawApplicationPacket::build_raw_header_dump(char *buffer, uint16 seq) const
{
	BasePacket::build_raw_header_dump(buffer, seq);
	buffer += strlen(buffer);

#ifdef STATIC_OPCODE
	buffer += sprintf(buffer, "[OpCode 0x%04x (0x%04x) Size=%u]\n", emu_opcode, opcode,size);
#else
	buffer += sprintf(buffer, "[OpCode %s (0x%04x) Size=%u]\n", OpcodeManager::EmuToName(emu_opcode), opcode,size);
#endif
}

void EQRawApplicationPacket::DumpRawHeader(uint16 seq, FILE *to) const
{
	char buff[196];
	build_raw_header_dump(buff, seq);
	fprintf(to, "%s", buff);
}

void EQRawApplicationPacket::build_header_dump(char *buffer) const
{
#ifdef STATIC_OPCODE
	sprintf(buffer, "[OpCode 0x%04x (0x%04x) Size=%u]\n", emu_opcode, opcode,size);
#else
	sprintf(buffer, "[OpCode %s (0x%04x) Size=%u]", OpcodeManager::EmuToName(emu_opcode), opcode,size);
#endif
}

void EQRawApplicationPacket::DumpRawHeaderNoTime(uint16 seq, FILE *to) const
{
	if (src_ip) {
		std::string sIP,dIP;
		sIP=long2ip(src_ip);
		dIP=long2ip(dst_ip);
		fprintf(to, "[%s:%d->%s:%d] ",sIP.c_str(),src_port,dIP.c_str(),dst_port);
	}
	if (seq != 0xffff)
		fprintf(to, "[Seq=%u] ",seq);

#ifdef STATIC_OPCODE
	fprintf(to, "[OpCode 0x%04x (0x%04x) Size=%u]\n", emu_opcode, opcode,size);
#else
	fprintf(to, "[OpCode %s (0x%04x) Size=%lu]\n", OpcodeManager::EmuToName(emu_opcode), opcode,(unsigned long)size);
#endif
}

uint32 EQProtocolPacket::serialize(unsigned char *dest) const
{
	if (opcode>0xff) {
		*(uint16 *)dest=opcode;
	} else {
		*(dest)=0;
		*(dest+1)=opcode;
	}
	memcpy(dest+2,pBuffer,size);

	return size+2;
}

uint32 EQApplicationPacket::serialize(uint16 opcode, unsigned char *dest) const
{
	uint8 OpCodeBytes = app_opcode_size;

	if (app_opcode_size==1)
		*(unsigned char *)dest = opcode;
	else
	{
		// Application opcodes with a low order byte of 0x00 require an extra 0x00 byte inserting prior to the opcode.
		if((opcode & 0x00ff) == 0)
		{
			*(uint8 *)dest = 0;
			*(uint16 *)(dest + 1) = opcode;
			++OpCodeBytes;
		}
		else
			*(uint16 *)dest = opcode;
	}
	memcpy(dest+OpCodeBytes,pBuffer,size);

	return size+OpCodeBytes;
}

/*EQProtocolPacket::EQProtocolPacket(uint16 op, const unsigned char *buf, uint32 len)
:	BasePacket(buf, len),
	opcode(op)
{

uint32 offset;
	opcode=ntohs(*(const uint16 *)buf);
	offset=2;

	if (len-offset) {
		pBuffer= new unsigned char[len-offset];
		memcpy(pBuffer,buf+offset,len-offset);
		size=len-offset;
	} else {
		pBuffer=nullptr;
		size=0;
	}
	OpMgr=&RawOpcodeManager;
}*/

bool EQProtocolPacket::combine(const EQProtocolPacket *rhs)
{
bool result=false;
	if (opcode==OP_Combined && size+rhs->size+5<256) {
		auto tmpbuffer = new unsigned char[size + rhs->size + 3];
		memcpy(tmpbuffer,pBuffer,size);
		uint32 offset=size;
		tmpbuffer[offset++]=rhs->Size();
		offset+=rhs->serialize(tmpbuffer+offset);
		size=offset;
		delete[] pBuffer;
		pBuffer=tmpbuffer;
		result=true;
	} else if (size+rhs->size+7<256) {
		auto tmpbuffer = new unsigned char[size + rhs->size + 6];
		uint32 offset=0;
		tmpbuffer[offset++]=Size();
		offset+=serialize(tmpbuffer+offset);
		tmpbuffer[offset++]=rhs->Size();
		offset+=rhs->serialize(tmpbuffer+offset);
		size=offset;
		delete[] pBuffer;
		pBuffer=tmpbuffer;
		opcode=OP_Combined;
		result=true;
	}

	return result;

}

/*
this is the code to do app-layer combining, instead of protocol layer.
this was taken out due to complex interactions with the opcode manager,
and will require a bit more thinking (likely moving into EQStream) to
get running again... but might be a good thing some day.

bool EQApplicationPacket::combine(const EQApplicationPacket *rhs)
{
uint32 newsize=0, offset=0;
unsigned char *tmpbuffer=nullptr;

	if (opcode!=OP_AppCombined) {
		newsize=app_opcode_size+size+(size>254?3:1)+app_opcode_size+rhs->size+(rhs->size>254?3:1);
		tmpbuffer=new unsigned char [newsize];
		offset=0;
		if (size>254) {
			tmpbuffer[offset++]=0xff;
			*(uint16 *)(tmpbuffer+offset)=htons(size);
			offset+=1;
		} else {
			tmpbuffer[offset++]=size;
		}
		offset+=serialize(tmpbuffer+offset);
	} else {
		newsize=size+app_opcode_size+rhs->size+(rhs->size>254?3:1);
		tmpbuffer=new unsigned char [newsize];
		memcpy(tmpbuffer,pBuffer,size);
		offset=size;
	}

	if (rhs->size>254) {
		tmpbuffer[offset++]=0xff;
		*(uint16 *)(tmpbuffer+offset)=htons(rhs->size);
		offset+=1;
	} else {
		tmpbuffer[offset++]=rhs->size;
	}
	offset+=rhs->serialize(tmpbuffer+offset);

	size=offset;
	opcode=OP_AppCombined;

	delete[] pBuffer;
	pBuffer=tmpbuffer;

	return true;
}
*/

bool EQProtocolPacket::ValidateCRC(const unsigned char *buffer, int length, uint32 Key)
{
bool valid=false;
	// OP_SessionRequest, OP_SessionResponse, OP_OutOfSession are not CRC'd
	if (buffer[0]==0x00 && (buffer[1]==OP_SessionRequest || buffer[1]==OP_SessionResponse || buffer[1]==OP_OutOfSession)) {
		valid=true;
	} else {
		uint16 comp_crc=CRC16(buffer,length-2,Key);
		uint16 packet_crc=ntohs(*(const uint16 *)(buffer+length-2));
#ifdef EQN_DEBUG
		if (packet_crc && comp_crc != packet_crc) {
			std::cout << "CRC mismatch: comp=" << std::hex << comp_crc << ", packet=" << packet_crc << std::dec << std::endl;
		}
#endif
		valid = (!packet_crc || comp_crc == packet_crc);
	}
	return valid;
}

uint32 EQProtocolPacket::Decompress(const unsigned char *buffer, const uint32 length, unsigned char *newbuf, uint32 newbufsize)
{
uint32 newlen=0;
uint32 flag_offset=0;
	newbuf[0]=buffer[0];
	if (buffer[0]==0x00) {
		flag_offset=2;
		newbuf[1]=buffer[1];
	} else
		flag_offset=1;

	if (length>2 && buffer[flag_offset]==0x5a) {
		newlen=InflatePacket(buffer+flag_offset+1,length-(flag_offset+1)-2,newbuf+flag_offset,newbufsize-flag_offset)+2;
		newbuf[newlen++]=buffer[length-2];
		newbuf[newlen++]=buffer[length-1];
	} else if (length>2 && buffer[flag_offset]==0xa5) {
		memcpy(newbuf+flag_offset,buffer+flag_offset+1,length-(flag_offset+1));
		newlen=length-1;
	} else {
		memcpy(newbuf,buffer,length);
		newlen=length;
	}

	return newlen;
}

uint32 EQProtocolPacket::Compress(const unsigned char *buffer, const uint32 length, unsigned char *newbuf, uint32 newbufsize) {
uint32 flag_offset=1,newlength;
	//dump_message_column(buffer,length,"Before: ");
	newbuf[0]=buffer[0];
	if (buffer[0]==0) {
		flag_offset=2;
		newbuf[1]=buffer[1];
	}
	if (length>30) {
		newlength=DeflatePacket(buffer+flag_offset,length-flag_offset,newbuf+flag_offset+1,newbufsize);
		*(newbuf+flag_offset)=0x5a;
		newlength+=flag_offset+1;
	} else {
		memmove(newbuf+flag_offset+1,buffer+flag_offset,length-flag_offset);
		*(newbuf+flag_offset)=0xa5;
		newlength=length+1;
	}
	//dump_message_column(newbuf,length,"After: ");

	return newlength;
}

void EQProtocolPacket::ChatDecode(unsigned char *buffer, int size, int DecodeKey)
{
	if ((size >= 2) && buffer[1]!=0x01 && buffer[0]!=0x02 && buffer[0]!=0x1d)
	{
		int Key=DecodeKey;
		unsigned char *test=(unsigned char *)malloc(size);
		buffer += 2;
		size -= 2;

		int i;
		for (i = 0; i + 4 <= size; i += 4)
		{
			int pt = (*(int*)&buffer[i]) ^ (Key);
			Key = (*(int*)&buffer[i]);
			*(int*)&test[i] = pt;
		}
		unsigned char KC = Key & 0xFF;
		for (; i < size; i++)
		{
			test[i] = buffer[i] ^ KC;
		}
		memcpy(buffer, test, size);
		free(test);
	}
}

void EQProtocolPacket::ChatEncode(unsigned char *buffer, int size, int EncodeKey)
{
	if (buffer[1]!=0x01 && buffer[0]!=0x02 && buffer[0]!=0x1d)
	{
		int Key=EncodeKey;
		char *test = (char*)malloc(size);
		int i;
		buffer += 2;
		size -= 2;
		for (i = 0; i + 4 <= size; i += 4)
		{
			int pt = (*(int*)&buffer[i]) ^ (Key);
			Key = pt;
			*(int*)&test[i] = pt;
		}
		unsigned char KC = Key & 0xFF;
		for (; i < size; i++)
		{
			test[i] = buffer[i] ^ KC;
		}
		memcpy(buffer, test, size);
		free(test);
	}
}

EQApplicationPacket *EQApplicationPacket::Copy() const {
	return(new EQApplicationPacket(*this));
}

EQRawApplicationPacket *EQProtocolPacket::MakeAppPacket() const {
	auto res = new EQRawApplicationPacket(opcode, pBuffer, size);
	res->copyInfo(this);
	return(res);
}

EQRawApplicationPacket::EQRawApplicationPacket(uint16 opcode, const unsigned char *buf, const uint32 len)
:	EQApplicationPacket(OP_Unknown, buf, len),
	opcode(opcode)
{
}
EQRawApplicationPacket::EQRawApplicationPacket(const unsigned char *buf, const uint32 len)
: EQApplicationPacket(OP_Unknown, buf+sizeof(uint16), len-sizeof(uint16))
{
	if(GetExecutablePlatform() != ExePlatformUCS) {
		opcode = *((const uint16 *) buf);
		if(opcode == 0x0000)
		{
			if(len >= 3)
			{
				opcode = *((const uint16 *) (buf + 1));
				const unsigned char *packet_start = (buf + 3);
				const int32 packet_length = len - 3;
				safe_delete_array(pBuffer);
				if(packet_length >= 0)
				{
					size = packet_length;
					pBuffer = new unsigned char[size];
					memcpy(pBuffer, packet_start, size);
				}
				else
				{
					size = 0;
				}
			}
			else
			{
				safe_delete_array(pBuffer);
				size = 0;
			}
		}
	} else {
		opcode = *((const uint8 *) buf);
	}
}

void DumpPacket(const EQApplicationPacket* app, bool iShowInfo) 
{
	#if EQDEBUG < 11
		return;
	#endif

	if (iShowInfo) {
		std::cout << "Dumping Applayer: 0x" << std::hex << std::setfill('0') << std::setw(4) << app->GetOpcode() << std::dec;
		std::cout << " size:" << app->size << std::endl;
	}
	DumpPacketHex(app->pBuffer, app->size);
//	DumpPacketAscii(app->pBuffer, app->size);
}

EQOldPacket::EQOldPacket(const unsigned char *buf, uint32 len)
{
	// Clear Fields
	pExtra = 0;
	ack_fields = 0;
	Clear();
}

EQOldPacket::EQOldPacket()
{
	// Clear Fields
	pExtra = 0;
	ack_fields = 0;
	Clear();
}

// Destructor
// deletes pExtra
EQOldPacket::~EQOldPacket()
{
	if (pExtra)
		delete[] pExtra;//delete pExtra;
	if (ack_fields)
		delete[] ack_fields;
	Clear();
}


// CRC Table generation code
uint32 EQOldPacket::RoL(uint32 in, uint32 bits) 
{
	uint32 temp, out;

	temp = in;
	temp >>= (32 - bits);
	out = in;
	out <<= bits;
	out |= temp;

	return out;
}
			
uint32 EQOldPacket::CRCLookup(uchar idx) 
{

	if (idx == 0)
	{
		return 0x00000000;
	}
			    
	if (idx == 1)
	{
		return 0x77073096;
	}
			    
	if (idx == 2)
	{
		return RoL(CRCLookup(1), 1);
	}
			     
	if (idx == 4)
	{
		return 0x076DC419;
	}
			    
	for (uchar b=7; b>0; b--)
	{
		uchar bv = 1 << b;
			    
		if (!(idx ^ bv)) 
		{
			/* bit is only one set */
			return ( RoL(CRCLookup (4), b - 2) );
		}

		if (idx&bv) 
		{
			/* bit is set */
			return( CRCLookup(bv) ^ CRCLookup(idx&(bv - 1)) );
		}
	}

	//Failure
	return false;
}

uint32 EQOldPacket::GenerateCRC(uint32 b, uint32 bufsize, uchar *buf) 
{
	uint32 CRC = (b ^ 0xFFFFFFFF), bufremain = bufsize;
	uchar  *bufptr = buf;

	while (bufremain--) 
	{
		CRC = CRCLookup((uchar)(*(bufptr++)^ (CRC&0xFF))) ^ (CRC >> 8);
	}
			  
	return (htonl (CRC ^ 0xFFFFFFFF));
}

void EQOldPacket::DecodePacket(uint16 length, uchar *pPacket)
{
	// Local variables
	uint16 *intptr   = (uint16*) pPacket;
	uint16  size     = 4;
			    
	// Start Processing

	if (length < 10) // Adding checks for illegal packets, so we dont read beyond the buffer
	{                //      Minimum normal packet length is 10
	//	cerr << "EQPacket.cpp: Illegal packet length" << endl;
		return;      // TODO: Do we want to check crc checksum on the incoming packets?
	}           

	HDR = *((EQPACKET_HDR_INFO*)intptr++);
	dwSEQ = ntohs(*intptr++);

	/************ CHECK ACK FIELDS ************/
	//Common ACK Response
	if(HDR.b2_ARSP)
	{
		dwARSP = ntohs(*intptr++);
		size+=2;
	}
	//  HDR.b4_Unknown - b7 is sent when there are packets missed, and bitfield following b4-b7 list what is acked and not
	//  HDR.b3_Unknown is sent when we want a resend of all non-acked packets prior to value in b3ARSP
	bool bDumpPacket = false;
	if (HDR.b3_Unknown)
	{
		b3ARSP = ntohs(*intptr++);
		size+=2;
	}
	b4_size = (HDR.b7_Unknown << 3) | (HDR.b6_Unknown << 2) | (HDR.b5_Unknown << 1) | HDR.b4_Unknown;

	if (b4_size) {
		size += b4_size;
		// if we are not long enough, then return
		if (length < size)
			return;
		ack_fields = new uchar[b4_size];
		memset(ack_fields, 0, b4_size);
		memcpy((void*)ack_fields, (void*)intptr, b4_size);
		intptr = (uint16*)&pPacket[size];
		bDumpPacket = true;
	}
			    
	//Common  ACK Request
	if(HDR.a1_ARQ)
	{
		size += 2;
		if (length < size)
			return;
		dwARQ = ntohs(*intptr++);
	}
	/************ END CHECK ACK FIELDS ************/



	/************ CHECK FRAGMENTS ************/
	if(HDR.a3_Fragment)
	{ 
		size += 6;
		if (length < size)
			return;
		fraginfo.dwSeq    = ntohs(*intptr++);
		fraginfo.dwCurr   = ntohs(*intptr++);
		fraginfo.dwTotal  = ntohs(*intptr++);
	}
	/************ END CHECK FRAGMENTS ************/
			    


	/************ CHECK ACK SEQUENCE ************/
	if(HDR.a4_ASQ && HDR.a1_ARQ)
	{
		size += 2;
		if (length < size)
			return;
		dbASQ_high = ((char*)intptr)[0];
		dbASQ_low  = ((char*)intptr)[1];
		intptr++;
	}
	else
	{
		if(HDR.a4_ASQ)
		{
			size++;
			if (length < size)
				return;
			dbASQ_high = ((char*)intptr)[0]; 
			intptr = (uint16*)&pPacket[size];
		}
	}
	/************ END CHECK ACK SEQUENCE ************/
			    
	/************ GET OPCODE/EXTRACT DATA ************/
				
	if(length-size > 4 && !(HDR.a2_Closing && HDR.a6_Closing))
	{
		/************ Extract applayer ************/
		if(!fraginfo.dwCurr) //OPCODE only included in first fragment!
		{
			dwOpCode = ntohs(*intptr++);
			size += 2;
		}

		dwExtraSize = length - size - 4;
		if (length < size + 4)
		{
			dwExtraSize = 0;    
		}
		if (dwExtraSize > 0)    
		{
			pExtra      = new uchar[dwExtraSize];
			memcpy((void*)pExtra, (void*) intptr, dwExtraSize);
		}
	}
	else
	{   /************ PURE ACK ************/
		dwOpCode    = 0;

		pExtra      = 0;
		dwExtraSize = 0;
	}
	/************ END GET OPCODE/EXTRA DATA ************/


/************ END PROCESSING ************/
}

uint32 EQOldPacket::ReturnPacket(uchar** data, EQOldStream* netcon) {
	*data = new uchar[dwExtraSize + 39];
	uchar* buf = *data;
	uint32 o = 4;
	bool clearFlags = false;
	this->dwSEQ = netcon->SACK.dwGSQ;
	*((uint16*)&buf[2]) = ntohs(netcon->SACK.dwGSQ++);

	//if(netcon->SACK.dwGSQ == 0)
	//{
	//	netcon->SACK.dwGSQ = 120; //Magic number. Seems to cause the client to recover. I don't think this is right now /shrug
	//	this->dwSEQ = 120;
	//}

	netcon->SACK.dwGSQcount++;

	if (netcon->CACK.dwARQ) {
		dwARSP = netcon->CACK.dwARQ;
		HDR.b2_ARSP = 1;				//Set ack response field
		*((uint16*)&buf[o]) = ntohs(netcon->CACK.dwARQ);
		o += 2;
	}
	else if (dwOpCode == 0xFFFF) {
		dwARSP = netcon->dwLastCACK;
		HDR.b2_ARSP = 1;				//Set ack response field
		*((uint16*)&buf[o]) = ntohs(netcon->dwLastCACK);
		o += 2;
	}
	else {
		HDR.b2_ARSP = 0;
	}

	if (HDR.a1_ARQ) {
		netcon->SACK.dwGSQcount = 0;
		*((uint16*)&buf[o]) = ntohs(dwARQ);
		o += 2;

	}
	if(HDR.a3_Fragment) {
        *((uint16*)&buf[o]) = ntohs(fraginfo.dwSeq);
		o += 2;
        *((uint16*)&buf[o]) = ntohs(fraginfo.dwCurr);
		o += 2;
        *((uint16*)&buf[o]) = ntohs(fraginfo.dwTotal);
		o += 2;
	}
	if(HDR.a4_ASQ && HDR.a1_ARQ) {
		*&buf[o++] = dbASQ_high;
		*&buf[o++] = dbASQ_low;
	}
	else if(HDR.a4_ASQ) {
		*&buf[o++] = dbASQ_high;
	}
	if (dwOpCode) {
        *((uint16*)&buf[o]) = ntohs(dwOpCode);
		o += 2;
	}
	if(pExtra) {
		memcpy(&buf[o], pExtra, dwExtraSize);
		o += dwExtraSize;
	}
	memcpy(buf, &HDR, 2);
	*((uint32*)&buf[o]) = htonl(CRC32::Generate(buf, o));
	o += 4;
	netcon->sent_Start = true;
	return o;
}


EQRawApplicationPacket *EQOldPacket::MakeAppPacket() const {


	if(pExtra == 0 && dwExtraSize > 0)
	{
		return nullptr;
	}

	EQRawApplicationPacket *res = new EQRawApplicationPacket(dwOpCode, pExtra, dwExtraSize);
	return(res);
}

std::string DumpPacketToString(const EQApplicationPacket* app){
	std::ostringstream out;
	out << DumpPacketHexToString(app->pBuffer, app->size);
	return out.str();
}

std::string DumpProtocolPacketToString(const EQProtocolPacket *app){
	std::ostringstream out;
	out << DumpPacketHexToString(app->pBuffer, app->size);
	return out.str();
}