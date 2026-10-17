#ifndef TRILOGY_H_
#define TRILOGY_H_

class EQApplicationPacket;
class EQPacketEncodeResult;
class EQPacketTranslator;

namespace Trilogy {

	// Register the Trilogy packet encoders and decoders with the application packet translator.
	extern void Register(EQPacketTranslator &translator);

	class Strategy {
	public:
		void Register(EQPacketTranslator &translator) const;

	private:
		//magic macro to declare our opcodes
		#include "ss_declare.h"
		#include "trilogy_ops.h"
	};

};

#endif /*TRILOGY_H_*/
