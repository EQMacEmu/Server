#ifndef EQCRYPTO_H
#define EQCRYPTO_H

#include <openssl/des.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include "../common/unix.h"
#endif

class EQCrypto 
{
public:
	EQCrypto();

	void DoEQDecrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size);
	void DoEQEncrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size);

private:
	void DoDESCrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size, int enc);

	static DES_cblock verant_key;
	DES_key_schedule key_schedule;
};

#endif