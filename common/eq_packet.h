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
#ifndef _EQPACKET_H
#define _EQPACKET_H

#include "base_packet.h"
#include "platform.h"
#include <iostream>

#ifdef STATIC_OPCODE
	typedef unsigned short EmuOpcode;
	static const EmuOpcode OP_Unknown = 0;
#else
#include "emu_opcodes.h"
#endif

class EQOldStream;

/************ PACKETS ************/
struct EQPACKET_HDR_INFO
{
	uint8 a0_Unknown  :   1; //TODO: What is this one?
	uint8 a1_ARQ      :   1; //TODO: What is this one?
	uint8 a2_Closing  :   1; //TODO: What is this one?
	uint8 a3_Fragment :   1; //TODO: What is this one?
	uint8 a4_ASQ      :   1; //TODO: What is this one?
	uint8 a5_SEQStart :   1; //TODO: What is this one?
	uint8 a6_Closing  :   1; //TODO: What is this one?
	uint8 a7_SEQEnd   :   1; //TODO: What is this one?
	uint8 b0_SpecARQ  :   1; //TODO: What is this one?
	uint8 b1_Unknown  :   1; //TODO: What is this one?
	uint8 b2_ARSP     :   1; //TODO: What is this one?
	uint8 b3_Unknown  :   1; //TODO: What is this one?
	uint8 b4_Unknown  :   1; //TODO: What is this one?
	uint8 b5_Unknown  :   1; //TODO: What is this one?
	uint8 b6_Unknown  :   1; //TODO: What is this one?
	uint8 b7_Unknown  :   1; //TODO: What is this one?
};

struct FRAGMENT_INFO
{
	uint16 dwSeq;	//TODO: What is this one?
	uint16 dwCurr;	//TODO: What is this one?
	uint16 dwTotal;	//TODO: What is this one?
};

class EQPacket : public BasePacket {
	friend class EQStream;
	friend class EQOldStream;
public:
	virtual ~EQPacket() {}

	uint32 Size() const { return size+2; }

	virtual void build_raw_header_dump(char *buffer, uint16 seq=0xffff) const;
	virtual void build_header_dump(char *buffer) const;
	virtual void DumpRawHeader(uint16 seq=0xffff, FILE *to = stdout) const;
	virtual void DumpRawHeaderNoTime(uint16 seq=0xffff, FILE *to = stdout) const;

	void SetOpcode(EmuOpcode op) { emu_opcode = op; }
	const EmuOpcode GetOpcode() const { return(emu_opcode); }
//	const char *GetOpcodeName() const;

protected:
	//this is just a cache so we dont look it up several times on Get()
	//and it is mutable so we can store the cached copy even on a const object
	EmuOpcode emu_opcode;

	EQPacket(EmuOpcode opcode, const unsigned char *buf, const uint32 len);
//	EQPacket(const EQPacket &p) { }
	EQPacket() { emu_opcode=OP_Unknown; pBuffer=nullptr; size=0; }

};

class EQRawApplicationPacket;

class EQProtocolPacket : public BasePacket {
	friend class EQStream;
	friend class EQOldStream;
	friend class EQStreamPair;
public:
	EQProtocolPacket(uint16 op, const unsigned char *buf, uint32 len) : BasePacket(buf,len), opcode(op) { acked = false; }
//	EQProtocolPacket(const unsigned char *buf, uint32 len);
	bool combine(const EQProtocolPacket *rhs);
	uint32 serialize (unsigned char *dest) const;
	EQProtocolPacket *Copy() { return new EQProtocolPacket(opcode,pBuffer,size); }
	EQRawApplicationPacket *MakeAppPacket() const;

	bool acked;

	virtual void build_raw_header_dump(char *buffer, uint16 seq=0xffff) const;
	virtual void build_header_dump(char *buffer) const;
	virtual void DumpRawHeader(uint16 seq=0xffff, FILE *to = stdout) const;
	virtual void DumpRawHeaderNoTime(uint16 seq=0xffff, FILE *to = stdout) const;

	uint16 GetRawOpcode() const { return(opcode); }

protected:

	static bool ValidateCRC(const unsigned char *buffer, int length, uint32 Key);
	static uint32 Decompress(const unsigned char *buffer, const uint32 length, unsigned char *newbuf, uint32 newbufsize);
	static uint32 Compress(const unsigned char *buffer, const uint32 length, unsigned char *newbuf, uint32 newbufsize);
	static void ChatDecode(unsigned char *buffer, int size, int DecodeKey);
	static void ChatEncode(unsigned char *buffer, int size, int EncodeKey);

	uint32 Size() const { return size+2; }

	//the actual raw EQ opcode
	uint16 opcode;
};

//Old (2001-era) packet
class EQOldPacket {
	friend class EQStream;
	friend class EQStreamPair;
public:
	EQOldPacket();
	EQOldPacket(const unsigned char *buf, uint32 len);
	~EQOldPacket();
//	EQProtocolPacket(const unsigned char *buf, uint32 len);
	EQOldPacket *Copy() { return new EQOldPacket(); }

	bool acked;

public:
	void  DecodePacket(uint16 length, uchar *pPacket);
	uint32 ReturnPacket(uchar** data, EQOldStream* netcon);
	EQRawApplicationPacket *MakeAppPacket() const;
	void Clear(void) 
	{  
		*((uint16*)&HDR)		   = 0;
		this->dwSEQ            = 0;        
		this->dwARSP           = 0;
		this->dwARQ            = 0;
		this->dbASQ_low        = 0;        
		this->dbASQ_high       = 0;
		this->dwOpCode         = 0;    
		this->fraginfo.dwCurr  = 0;
		this->fraginfo.dwSeq   = 0;
		this->fraginfo.dwTotal = 0;
		this->dwExtraSize      = 0;
		this->pExtra           = 0;
		this->ack_fields       = 0;
		this->resend_count	   = 0;
		this->dwLoopedOnce     = 0;
		this->LastSent		   = 0;
		this->Resend = false;
	}


	EQPACKET_HDR_INFO   HDR;				//TODO: What is this one?
	uint16				dwSEQ;				// Sequence number
	uint16				dwARSP;				//TODO: What is this one?
	uint16				b3ARSP;
	uint16				b4_size;
	uchar				*ack_fields;
	uint16				dwARQ;				//TODO: What is this one?
	uint16				dbASQ_high : 8;		//TODO: What is this one?
	uint16				dbASQ_low  : 8;		//TODO: What is this one?
	uint16				dwOpCode;			//Not all packet have opcodes. 
	FRAGMENT_INFO		fraginfo;			//Fragment info
	uint16				dwExtraSize;		//Size of additional info.
	uchar				*pExtra;			//Additional information
	uint16				resend_count;		// Quagmire: Moving resend count to a packet by packet basis
	uint16				dwLoopedOnce;		//Checks counter of times packet has looped. Basically a bool but kept multiples for debugging purposes
	uint32				LastSent; //Last time this packet was sent.
	bool				Resend;

	// Quagmire: Made the CRC stuff static and public. Makes things easier elsewhere.
	static uint32 GenerateCRC(uint32 b, uint32 bufsize, uchar *buf);
	uint16 GetRawOpcode() const { return(opcode); }
private:
	static uint32 RoL(uint32 in, uint32 bits);
	static uint32 CRCLookup(uchar idx);

	//the actual raw EQ opcode
	uint16 opcode;
};

class EQApplicationPacket : public EQPacket {
	friend class EQStream;
public:
	EQApplicationPacket() : EQPacket(OP_Unknown, nullptr, 0), opcode_bypass(0)
		{ app_opcode_size = GetExecutablePlatform() == ExePlatformUCS ? 1 : 2; }
	EQApplicationPacket(const EmuOpcode op) : EQPacket(op, nullptr, 0), opcode_bypass(0)
		{ app_opcode_size = GetExecutablePlatform() == ExePlatformUCS ? 1 : 2; }
	EQApplicationPacket(const EmuOpcode op, const uint32 len) : EQPacket(op, nullptr, len), opcode_bypass(0)
		{ app_opcode_size = GetExecutablePlatform() == ExePlatformUCS ? 1 : 2; }
	EQApplicationPacket(const EmuOpcode op, const unsigned char *buf, const uint32 len) : EQPacket(op, buf, len), opcode_bypass(0)
		{ app_opcode_size = GetExecutablePlatform() == ExePlatformUCS ? 1 : 2; }
	bool combine(const EQApplicationPacket *rhs);
	uint32 serialize (uint16 opcode, unsigned char *dest) const;
	uint32 Size() const { return size+app_opcode_size; }

	virtual EQApplicationPacket *Copy() const;

	virtual void build_raw_header_dump(char *buffer, uint16 seq=0xffff) const;
	virtual void build_header_dump(char *buffer) const;
	virtual void DumpRawHeader(uint16 seq=0xffff, FILE *to = stdout) const;
	virtual void DumpRawHeaderNoTime(uint16 seq=0xffff, FILE *to = stdout) const;

	uint16 GetOpcodeBypass() const { return opcode_bypass; }
	void SetOpcodeBypass(uint16 v) { opcode_bypass = v; }

	uint16 GetProtocolOpcode() const { return protocol_opcode; }
	void SetProtocolOpcode(uint16 v) { protocol_opcode = v; }
protected:
	uint16 protocol_opcode;
	uint8 app_opcode_size;
	uint16 opcode_bypass;
private:

	EQApplicationPacket(const EQApplicationPacket &p) : EQPacket(p.emu_opcode, p.pBuffer, p.size), opcode_bypass(p.opcode_bypass) { app_opcode_size = p.app_opcode_size; }

};

class EQRawApplicationPacket : public EQApplicationPacket {
	friend class EQStream;
public:
	EQRawApplicationPacket(uint16 opcode, const unsigned char *buf, const uint32 len);
	uint16 GetRawOpcode() const { return(opcode); }

	virtual void build_raw_header_dump(char *buffer, uint16 seq=0xffff) const;
	virtual void build_header_dump(char *buffer) const;
	virtual void DumpRawHeader(uint16 seq=0xffff, FILE *to = stdout) const;
	virtual void DumpRawHeaderNoTime(uint16 seq=0xffff, FILE *to = stdout) const;

protected:

	//the actual raw EQ opcode
	uint16 opcode;

	EQRawApplicationPacket(const unsigned char *buf, const uint32 len);
};

extern void DumpPacket(const EQApplicationPacket* app, bool iShowInfo = true);
extern std::string DumpPacketToString(const EQApplicationPacket* app);
extern std::string DumpProtocolPacketToString(const EQProtocolPacket* app);

#endif
