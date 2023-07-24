
#include <iostream>
#include "../common/types.h"
#include "../common/database.h"

using namespace std;

#include "eq_crypto.h"

/* verant's DES key in our client: 13 D9 13 6D D0 34 15 FB - neorab */
DES_cblock EQCrypto::verant_key = { 19, 217, 19, 109, 208, 52, 21, 251 };

EQCrypto::EQCrypto()
{
	DES_key_sched(&verant_key, &key_schedule);
}

void EQCrypto::DoEQDecrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size)
{
	DoDESCrypt(in_buffer, out_buffer, buffer_size, 0);
}

void EQCrypto::DoEQEncrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size)
{
	DoDESCrypt(in_buffer, out_buffer, buffer_size, 1);
}

void EQCrypto::DoDESCrypt(const unsigned char *in_buffer, unsigned char *out_buffer, int buffer_size, int enc)
{
	/* The initialization vector (iv) is the key.
	   This is manipulated through the encryption, so I put it
	   here again, just to be safe. -neorab */
	DES_cblock verant_iv = { 19, 217, 19, 109, 208, 52, 21, 251 };

	DES_ncbc_encrypt(in_buffer, out_buffer, buffer_size, &key_schedule, &verant_iv, enc);
}