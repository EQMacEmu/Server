#include "../global_define.h"
#include "../eqemu_logsys.h"
#include "../eq_packet.h"
#include "../eq_packet_translator.h"
#include "trilogy.h"
#include "../opcodemgr.h"
#include "../crc32.h"

#include "../eq_packet_structs.h"
#include "../misc_functions.h"
#include "../packet_functions.h"
#include "../strings.h"
#include "../inventory_profile.h"
#include "trilogy_structs.h"
#include "trilogy_strings.h"
#include "../rulesys.h"
#include "../zone_numbers.h"
#include "../zone_store.h"
#include "../content/world_content_service.h"
#include "../deity.h"
#include "../classes.h"
#include "../races.h"
namespace Trilogy {

	static Strategy struct_strategy;


	structs::Item_Struct* TrilogyItem(const EQ::ItemInstance *inst, int16 slot_id_in, int type = 0);
	structs::Spawn_Struct* TrilogySpawns(struct Spawn_Struct*, int type);

	static inline int16 ServerToTrilogySlot(uint32 ServerSlot);
	static inline int16 ServerToTrilogyCorpseSlot(uint32 ServerCorpse);

	static inline uint32 TrilogyToServerSlot(int16 TrilogySlot);
	static inline uint32 TrilogyToServerCorpseSlot(int16 TrilogyCorpse);

	static std::string CleanItemLinks(const std::string& str)
	{
		std::string result;
		result.reserve(str.length());
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (str[i] == '\x12')
			{
				size_t close_pos = str.find('\x12', i + 1);
				if (close_pos != std::string::npos && (close_pos - i - 1) >= 7)
				{
					size_t text_start = i + 1 + 7;
					size_t text_len = close_pos - text_start;
					result.append(str, text_start, text_len);
					i = close_pos;
					continue;
				}
				continue;
			}
			result += str[i];
		}
		return result;
	}

	void Register(EQPacketTranslator &translator)
	{
		struct_strategy.Register(translator);
	}

	void Strategy::Register(EQPacketTranslator &translator) const
	{
		#define E(x) translator.SetEncoder(x, Encode_##x);
		#define D(x) translator.SetDecoder(x, Decode_##x);
		#include "trilogy_ops.h"
	}

	#include "ss_define.h"

DECODE(OP_SendLoginInfo)
	{
		DECODE_LENGTH_ATLEAST(structs::LoginInfo_Struct);
		SETUP_DIRECT_DECODE(LoginInfo_Struct, structs::LoginInfo_Struct);
		memcpy(emu->login_info, eq->AccountName, sizeof(emu->login_info));
		emu->zoning = (eq->zoning == 1 || __eq_buffer[188] == 1) ? 1 : 0;
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_ApproveName)
	{
		DECODE_LENGTH_EXACT(structs::NameApproval_Struct);
		SETUP_DIRECT_DECODE(NameApproval_Struct, structs::NameApproval_Struct);
		strn0cpy(emu->name, eq->charname, sizeof(emu->name));
		emu->race = eq->race;
		emu->class_ = eq->class_;
		emu->unknown066 = 0;
		emu->unknown070[0] = 0;
		emu->unknown070[1] = 0;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ApproveName)
	{
		SETUP_DIRECT_ENCODE(NameApprovalReply_Struct, structs::NameApprovalReply_Struct);
		OUT(approval);
		FINISH_ENCODE();
	}

	DECODE(OP_EnterWorld)
	{
		unsigned char *__eq_buffer = __packet->pBuffer;
		if (!__eq_buffer)
		{
			__packet->SetOpcode(OP_Unknown);
			return;
		}

		__packet->size = sizeof(EnterWorld_Struct);
		__packet->pBuffer = new unsigned char[__packet->size];
		memset(__packet->pBuffer, 0, __packet->size);
		EnterWorld_Struct *emu = (EnterWorld_Struct*)__packet->pBuffer;
		structs::EnterWorld_Struct *eq = (structs::EnterWorld_Struct *) __eq_buffer;
		strn0cpy(emu->name, eq->charname, sizeof(emu->name));
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ZoneServerInfo)
	{
		SETUP_DIRECT_ENCODE(ZoneServerInfo_Struct, structs::ZoneServerInfo_Struct);
		strcpy(eq->ip, emu->ip);
		eq->port = emu->port;

		FINISH_ENCODE();
	}

	EAT_ENCODE(OP_LogServer)


	ENCODE(OP_ApproveWorld)
	{
		SETUP_DIRECT_ENCODE(ApproveWorld_Struct, structs::ApproveWorld_Struct);
		eq->response = 0;
		FINISH_ENCODE();
	}

	ENCODE(OP_EnterWorld)
	{
		SETUP_DIRECT_ENCODE(ApproveWorld_Struct, structs::ApproveWorld_Struct);
		eq->response = 0;
		FINISH_ENCODE();
	}

	ENCODE(OP_ExpansionInfo)
	{
		SETUP_DIRECT_ENCODE(ExpansionInfo_Struct, structs::ExpansionInfo_Struct);
		eq->Expansions = (emu->Expansions > 0 && emu->Expansions <= 3) ? emu->Expansions : 3;
		FINISH_ENCODE();
	}

	ENCODE(OP_SendCharInfo)
	{
		int r;
		ENCODE_LENGTH_EXACT(CharacterSelect_Struct);
		SETUP_DIRECT_ENCODE(CharacterSelect_Struct, structs::CharacterSelect_Struct);
		for (r = 0; r < 10; r++)
		{
			if (emu->zone[r] != 0 && strcmp(emu->name[r], "<none>") != 0 && emu->name[r][0] != '\0')
			{
				const char* zn = StaticGetZoneName(emu->zone[r]);
				if (zn && strcmp(zn, "UNKNWN") != 0) {
					strncpy(eq->zone[r], zn, 20);
				} else {
					strncpy(eq->zone[r], "qeynos", 20);
				}
			}
			else
			{
				strncpy(eq->zone[r], "qeynos", 20);
			}

			if (emu->race[r] > 300)
				eq->race[r] = 1;
			else
				eq->race[r] = emu->race[r];
			OUT(class_[r]);
			strncpy(eq->name[r], emu->name[r], 30);
			OUT(gender[r]);
			OUT(level[r]);
			OUT(face[r]);
			int k;
			for (k = 0; k < 9; k++)
			{
				eq->equip[r][k] = (int8)emu->equip[r].Slot[k].Material;
				eq->cs_colors[r][k].color = emu->cs_colors[r].Slot[k].Color;
			}
			OUT(deity[r]);
		}

		for (int i = 0; i < 8; i++) {
			memset((char*)eq->primary + 4 * i, i + 1, 4);
			memset((char*)eq->secondary + 4 * i, i + 1, 4);
		}

		FINISH_ENCODE();
	}

	DECODE(OP_ZoneEntry)
	{
		SETUP_DIRECT_DECODE(ClientZoneEntry_Struct, structs::ClientZoneEntry_Struct)
			strn0cpy(emu->char_name, eq->char_name, 30);

		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ZoneEntry)
	{
		SETUP_DIRECT_ENCODE(ServerZoneEntry_Struct, structs::ServerZoneEntry_Struct);

		strn0cpy(eq->name, emu->name, 30);
		strn0cpy(eq->Surname, emu->Surname, 20);
		strn0cpy(eq->zone, StaticGetZoneName(emu->zoneID), 15);
		eq->class_ = emu->class_;
		eq->race = emu->race;
		eq->gender = emu->gender;
		eq->level = emu->level;
		eq->deity = emu->deity;
		eq->x = emu->x_pos;
		eq->y = emu->y_pos;
		eq->z = emu->z_pos;
		eq->heading = emu->heading;
		eq->walkspeed = emu->walkspeed;
		eq->runspeed = emu->runspeed;
		eq->anon = emu->anon;
		eq->face = emu->face;
		eq->guildeqid = (int16)emu->GuildID;
		eq->pvp = emu->pvp;
		eq->helmet = (int8)emu->helm;
		eq->helmcolor = emu->equipcolors.Slot[EQ::textures::armorHead].Color;
		eq->npc_armor_graphic = (int8)0xff;
		eq->left_hand = (int8)emu->equipment[EQ::textures::weaponPrimary];
		eq->right_hand = (int8)emu->equipment[EQ::textures::weaponSecondary];
		eq->flymode = emu->flymode;
		eq->GM = emu->GM;

		LogInfo("[Trilogy ENCODE OP_ZoneEntry] name={} zone={} x={:.2f} y={:.2f} z={:.2f} heading={:.2f}",
			eq->name, eq->zone, eq->x, eq->y, eq->z, eq->heading);

		CRC32::SetEQChecksum(__packet->pBuffer, sizeof(structs::ServerZoneEntry_Struct));

		FINISH_ENCODE();
	}

	ENCODE(OP_PlayerProfile)
	{
		SETUP_DIRECT_ENCODE(PlayerProfile_Struct, structs::PlayerProfile_Struct);

		const char* bind_zone_name = StaticGetZoneName(emu->binds[0].zoneId ? emu->binds[0].zoneId : emu->zone_id);
		if (bind_zone_name) {
			strn0cpy(eq->bind_point_zone, bind_zone_name, 20);
			for (int b = 0; b < 4; b++) {
				strn0cpy(eq->start_point_zone[b], bind_zone_name, 20);
			}
		}
		float default_bind_y = emu->binds[0].y != 0.0f ? emu->binds[0].y : emu->y;
		float default_bind_x = emu->binds[0].x != 0.0f ? emu->binds[0].x : emu->x;
		float default_bind_z = emu->binds[0].z != 0.0f ? emu->binds[0].z : emu->z;
		for (int b = 0; b < 5; b++) {
			if (emu->binds[b].zoneId != 0) {
				eq->bind_location[0][b] = emu->binds[b].y;
				eq->bind_location[1][b] = emu->binds[b].x;
				eq->bind_location[2][b] = emu->binds[b].z;
			} else {
				eq->bind_location[0][b] = default_bind_y;
				eq->bind_location[1][b] = default_bind_x;
				eq->bind_location[2][b] = default_bind_z;
			}
		}

		int r = 0;
		strn0cpy(eq->current_zone, StaticGetZoneName(emu->zone_id), 15);
		OUT(gender);
		OUT(race);
		OUT(class_);
		OUT(level);
		if (emu->deity == Deity::Agnostic2) {
			eq->deity = Deity::Agnostic1;
		} else {
			OUT(deity);
		}
		OUT(intoxication);
		OUT(points);
		OUT(mana);
		OUT(cur_hp);
		OUT(STR);
		OUT(STA);
		OUT(CHA);
		OUT(DEX);
		OUT(INT);
		OUT(AGI);
		OUT(WIS);
		OUT(face);
		OUT_array(spell_book, 256);
		OUT_array(mem_spells, 8);
		OUT(platinum);
		OUT(gold);
		OUT(silver);
		OUT(copper);
		OUT(platinum_cursor);
		OUT(gold_cursor);
		OUT(silver_cursor);
		OUT(copper_cursor);
		OUT_array(skills, structs::MAX_PP_SKILL);  // 1:1 direct copy (100 dword)

		int value = 6000;

		float tpercent = (float)emu->thirst_level / (float)value;
		float tlevel = 127.0f*tpercent;
		eq->thirst_level = (int8)(tlevel + 0.5);

		float hpercent = (float)emu->hunger_level / (float)value;
		float hlevel = 127.0f*hpercent;
		eq->hunger_level = (int8)(hlevel + 0.5);

		for (r = 0; r < 15; r++)
		{
			if (emu->buffs[r].spellid == 0xFFFFFFFF || emu->buffs[r].spellid > 2999)
				eq->buffs[r].spellid = 0;
			else
				eq->buffs[r].spellid = emu->buffs[r].spellid;
			if (eq->buffs[r].spellid == 0)
			{
				eq->buffs[r].visable = 0;
				eq->buffs[r].level = 0;
				eq->buffs[r].bard_modifier = 0;
				eq->buffs[r].duration = 0;
				eq->buffs[r].activated = 0;
			}
			else {
				eq->buffs[r].visable = 2;
				eq->buffs[r].level = emu->buffs[r].level;
				eq->buffs[r].bard_modifier = emu->buffs[r].bard_modifier;
				eq->buffs[r].spellid = emu->buffs[r].spellid;
				eq->buffs[r].duration = emu->buffs[r].duration;
			}
		}
		strn0cpy(eq->name, emu->name, 30);
		strncpy(eq->Surname, emu->last_name, 20);
		if (emu->guild_id == 0 || emu->guild_id == 0xFFFFFFFF)
			eq->guild_id = 0xFFFF;
		else
			eq->guild_id = emu->guild_id;
		OUT(pvp);
		OUT(anon);
		OUT(gm);
		OUT(guildrank);
		OUT(exp);
		OUT_array(languages, 24);
		OUT(x);
		OUT(y);
		OUT(z);
		OUT(heading);
		LogInfo("[Trilogy ENCODE OP_PlayerProfile] name={} cur_zone={} x={:.2f} y={:.2f} z={:.2f} heading={:.2f} bind_zone={} bind_loc=({:.2f}, {:.2f}, {:.2f})",
			eq->name, eq->current_zone, eq->x, eq->y, eq->z, eq->heading,
			eq->bind_point_zone, eq->bind_location[1][0], eq->bind_location[0][0], eq->bind_location[2][0]);
		OUT(platinum_bank);
		OUT(gold_bank);
		OUT(silver_bank);
		OUT(copper_bank);
		OUT(autosplit);
		if (emu->expansions > 3)
			eq->expansions = 3;
		else
			OUT(expansions);
		for (r = 0; r < 6; r++)
		{
			OUT_str(groupMembers[r]);
		}
		OUT(abilitySlotRefresh);
		OUT_array(spellSlotRefresh, structs::MAX_PP_MEMSPELL);
		eq->eqbackground = 0;

		for (r = 0; r < 30; r++) {
			eq->inventory[r] = emu->inventory[r];
			eq->invItemProprieties[r].charges = emu->invItemProperties[r].charges;
			eq->inventoryitemPointers[r] = 0;
		}
		for (r = 0; r < 80; r++) {
			eq->containerinv[r] = emu->containerinv[r];
			eq->bagItemProprieties[r].charges = emu->bagItemProperties[r].charges;
		}
		for (r = 0; r < 10; r++) {
			eq->cursorbaginventory[r] = emu->cursorbaginventory[r];
			eq->cursorItemProprieties[r].charges = emu->cursorItemProperties[r].charges;
		}
		for (r = 0; r < 8; r++) {
			eq->bank_inv[r] = emu->bank_inv[r];
			eq->bankinvitemproperties[r].charges = emu->bankinvitemproperties[r].charges;
			eq->bankinvitemPointers[r] = 0;
		}
		for (r = 0; r < 80; r++) {
			eq->bank_cont_inv[r] = emu->bank_cont_inv[r];
			eq->bankbagitemproperties[r].charges = emu->bankbagitemproperties[r].charges;
		}

		LogNetcodeDetail("[STRUCTS] Player Profile Packet is {} bytes uncompressed", sizeof(structs::PlayerProfile_Struct));

		CRC32::SetEQChecksum(__packet->pBuffer, sizeof(structs::PlayerProfile_Struct));
		EQApplicationPacket* outapp = new EQApplicationPacket();
		outapp->SetOpcode(OP_PlayerProfile);
		outapp->pBuffer = new uchar[10000];
		outapp->size = DeflatePacket((unsigned char*)__packet->pBuffer, sizeof(structs::PlayerProfile_Struct), outapp->pBuffer, 10000);
		EncryptTrilogyProfilePacket(outapp->pBuffer, outapp->size);
		LogNetcodeDetail("[STRUCTS] Player Profile Packet is {} bytes compressed", outapp->size);
		result->SetPacket(&outapp, reliable);
		delete[] __emu_buffer;
		delete __packet;
	}

	DECODE(OP_CharacterCreate)
	{
		DECODE_LENGTH_EXACT(structs::CharCreate_Struct);
		SETUP_DIRECT_DECODE(CharCreate_Struct, structs::CharCreate_Struct);
		IN(class_);
		IN(gender);
		IN(race);
		emu->zone_id = 0;
		if (strlen(eq->current_zone) > 0) {
			emu->zone_id = ZoneID(eq->current_zone);
		}
		if (!emu->zone_id && strlen(eq->start_point_zone[0]) > 0) {
			emu->zone_id = ZoneID(eq->start_point_zone[0]);
		}
		if (!emu->zone_id && strlen(eq->bind_point_zone) > 0) {
			emu->zone_id = ZoneID(eq->bind_point_zone);
		}
		strn0cpy(emu->name, eq->name, 64);
		strn0cpy(emu->last_name, eq->Surname, 66);
		uint32 deity = eq->deity_;
		if (deity == 0 && eq->deity != 0) {
			deity = eq->deity;
		}
		if (deity == Deity::Agnostic1) {
			deity = Deity::Agnostic2;
		}
		emu->deity = deity;
		IN(STR);
		IN(STA);
		IN(AGI);
		IN(DEX);
		IN(WIS);
		IN(INT);
		IN(CHA);
		emu->oldface = eq->face;
		emu->x = 0.0f;
		emu->y = 0.0f;
		emu->z = 0.0f;
		emu->heading = 0.0f;
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_SetGuildMOTD)
	{
		SETUP_DIRECT_DECODE(GuildMOTD_Struct, structs::GuildMOTD_Struct);
		strcpy(emu->name, eq->name);
		strcpy(emu->motd, eq->motd);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GuildMOTD)
	{
		SETUP_DIRECT_ENCODE(GuildMOTD_Struct, structs::GuildMOTD_Struct);
		strcpy(eq->name, emu->name);
		strcpy(eq->motd, emu->motd);
		FINISH_ENCODE();
	}

	DECODE(OP_GuildInvite)
	{
		SETUP_DIRECT_DECODE(GuildCommand_Struct, structs::GuildCommand_Struct);
		strcpy(emu->myname, eq->myname);
		strcpy(emu->othername, eq->othername);
		IN(guildeqid);
		IN(officer);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GuildInvite)
	{
		SETUP_DIRECT_ENCODE(GuildCommand_Struct, structs::GuildCommand_Struct);
		strn0cpy(eq->myname, emu->myname, 30);
		strn0cpy(eq->othername, emu->othername, 30);
		OUT(guildeqid);
		OUT(officer);
		FINISH_ENCODE();
	}

	DECODE(OP_GuildRemove)
	{
		SETUP_DIRECT_DECODE(GuildCommand_Struct, structs::GuildCommand_Struct);
		strcpy(emu->myname, eq->myname);
		strcpy(emu->othername, eq->othername);
		IN(guildeqid);
		IN(officer);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GuildRemove)
	{
		SETUP_DIRECT_ENCODE(GuildCommand_Struct, structs::GuildCommand_Struct);
		strn0cpy(eq->myname, emu->myname, 30);
		strn0cpy(eq->othername, emu->othername, 30);
		OUT(guildeqid);
		OUT(officer);
		FINISH_ENCODE();
	}

	DECODE(OP_GuildInviteAccept)
	{
		SETUP_DIRECT_DECODE(GuildInviteAccept_Struct, structs::GuildInviteAccept_Struct);
		strcpy(emu->inviter, eq->inviter);
		strcpy(emu->newmember, eq->newmember);
		IN(response);
		emu->guildeqid = (int16)eq->guildeqid;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_NewZone)
	{
		SETUP_DIRECT_ENCODE(NewZone_Struct, structs::NewZone_Struct);
		OUT_str(char_name);
		OUT_str(zone_short_name);
		OUT_str(zone_long_name);
		OUT(ztype);
		OUT_array(fog_red, 4);
		OUT_array(fog_green, 4);
		OUT_array(fog_blue, 4);
		OUT_array(fog_minclip, 4);
		OUT_array(fog_maxclip, 4);
		OUT(gravity);
		OUT(time_type);
		OUT(sky);
		OUT(safe_y);
		OUT(safe_x);
		OUT(safe_z);
		OUT(max_z);
		eq->underworld = emu->underworld;
		OUT(minclip);
		OUT(maxclip);
		OUT(timezone);
		FINISH_ENCODE();
	}

	ENCODE(OP_ChannelMessage)
	{
		EQApplicationPacket *__packet = *p;
		*p = nullptr;
		unsigned char *__emu_buffer = __packet->pBuffer;
		ChannelMessage_Struct *emu = (ChannelMessage_Struct *)__emu_buffer;
		uint32 __i = 0;
		__i++; /* to shut up compiler */

		std::string clean_msg = CleanItemLinks(emu->message);
		int msglen = clean_msg.length() + 1;
		int len = sizeof(structs::ChannelMessage_Struct) + msglen + 4;
		__packet->pBuffer = new unsigned char[len];
		__packet->size = len;
		memset(__packet->pBuffer, 0, len);
		structs::ChannelMessage_Struct *eq = (structs::ChannelMessage_Struct *) __packet->pBuffer;
		strn0cpy(eq->targetname, emu->targetname, 32);
		strn0cpy(eq->sender, emu->sender, 32);
		eq->language = emu->language;
		eq->chan_num = emu->chan_num;
		eq->skill_in_language = emu->skill_in_language;
		strcpy(eq->message, clean_msg.c_str());
		FINISH_ENCODE();
	}

	ENCODE(OP_FormattedMessage)
	{
		EQApplicationPacket *__packet = *p;
		*p = nullptr;
		char *bufptr;
		unsigned char *__emu_buffer = __packet->pBuffer;
		FormattedMessage_Struct *emu = (FormattedMessage_Struct *)__emu_buffer;
		uint32 __i = 0;
		__i++; /* to shut up compiler */

		std::string message("");
		if (emu->string_id == 1032 || emu->string_id == 1034) {
			char sender[32] = { 0 };
			strn0cpy(sender, emu->message, 30);
			bufptr = emu->message + strlen(sender) + 1;
			std::string clean_channel_msg = CleanItemLinks(bufptr);
			int len = sizeof(structs::ChannelMessage_Struct) + clean_channel_msg.length() + 5;
			__packet->pBuffer = new unsigned char[len];
			__packet->size = len;
			memset(__packet->pBuffer, 0, len);
			structs::ChannelMessage_Struct *eq = (structs::ChannelMessage_Struct *) __packet->pBuffer;
			__packet->SetOpcode(OP_ChannelMessage);
			strn0cpy(eq->sender, sender, 30);
			strcpy(eq->message, clean_channel_msg.c_str());
			if (emu->string_id == 1032)
				eq->chan_num = 8;
			else
				eq->chan_num = 3;
			eq->skill_in_language = 100;
		}
		else {
			std::vector<std::string> args;
			if (__packet->size > sizeof(FormattedMessage_Struct)) {
				const char* ptr = emu->message;
				const char* end = (const char*)__emu_buffer + __packet->size;
				while (ptr < end && *ptr != '\0') {
					args.emplace_back(ptr);
					ptr += args.back().length() + 1;
				}
			}

			const char* fmt = GetTrilogyString(emu->string_id);
			if (fmt) {
				message = FormatTrilogyString(fmt, args);
			}
			else {
				message = "Unhandled string_id: ";
				message += itoa(emu->string_id);
			}
		}
		if (message.length() > 0) {
			message = CleanItemLinks(message);
			__packet->SetOpcode(OP_SpecialMesg);
			int len = sizeof(structs::SpecialMesg_Struct) + message.length() + 5;
			__packet->pBuffer = new unsigned char[len];
			__packet->size = len;
			memset(__packet->pBuffer, 0, len);
			structs::SpecialMesg_Struct *eq = (structs::SpecialMesg_Struct *) __packet->pBuffer;
			strcpy(eq->message, message.c_str());
			eq->msg_type = emu->type;
		}

		FINISH_ENCODE();
	}

	ENCODE(OP_InterruptCast)
	{
		EQApplicationPacket *__packet = *p;
		*p = nullptr;
		unsigned char *__emu_buffer = __packet->pBuffer;
		InterruptCast_Struct *emu = (InterruptCast_Struct *)__emu_buffer;
		std::string message = "";
		const char* str = GetTrilogyString(emu->messageid);
		if (str) {
			message = str;
		}
		else {
			switch (emu->messageid) {
			case 12687:
				message = "Your song ends.";
				break;
			case 439:
				message = "Your spell is interrupted.";
				break;
			case 12686:
				message = "Your song ends abruptly.";
				break;
			case 173:
				message = "Your spell fizzles!";
				break;
			default:
				break;
			}
		}
		int len = sizeof(structs::InterruptCast_Struct) + message.length() + 1;
		__packet->pBuffer = new unsigned char[len];
		__packet->size = len;
		memset(__packet->pBuffer, 0, len);
		structs::InterruptCast_Struct *eq = (structs::InterruptCast_Struct *) __packet->pBuffer;
		eq->spawnid = 0;
		strncpy(eq->message, message.c_str(), message.length());
		FINISH_ENCODE();
	}

	ENCODE(OP_SpecialMesg)
	{
		EQApplicationPacket *__packet = *p;
		*p = nullptr;
		unsigned char *__emu_buffer = __packet->pBuffer;
		SpecialMesg_Struct *emu = (SpecialMesg_Struct *)__emu_buffer;
		uint32 __i = 0;
		__i++; /* to shut up compiler */

		std::string clean_msg = CleanItemLinks(emu->message);
		int msglen = clean_msg.length() + 1;
		int len = sizeof(structs::SpecialMesg_Struct) + msglen + 4;
		__packet->pBuffer = new unsigned char[len];
		__packet->size = len;
		memset(__packet->pBuffer, 0, len);
		structs::SpecialMesg_Struct *eq = (structs::SpecialMesg_Struct *) __packet->pBuffer;
		eq->msg_type = emu->msg_type;
		strcpy(eq->message, clean_msg.c_str());
		FINISH_ENCODE();
	}

	DECODE(OP_ChannelMessage)
	{
		if (__packet->size < sizeof(structs::ChannelMessage_Struct))
		{
			__packet->SetOpcode(OP_Unknown);
			return;
		}
		unsigned char *__eq_buffer = __packet->pBuffer;
		structs::ChannelMessage_Struct *eq = (structs::ChannelMessage_Struct *) __eq_buffer;
		int msglen = __packet->size - sizeof(structs::ChannelMessage_Struct);
		if (msglen < 0)
			msglen = 0;
		int len = msglen + sizeof(ChannelMessage_Struct);
		__packet->size = len;
		__packet->pBuffer = new unsigned char[len];
		MEMSET_IN(ChannelMessage_Struct);
		ChannelMessage_Struct *emu = (ChannelMessage_Struct *)__packet->pBuffer;
		strn0cpy(emu->targetname, eq->targetname, 32);
		strn0cpy(emu->sender, eq->targetname, 32);
		emu->language = eq->language;
		emu->chan_num = eq->chan_num;
		emu->skill_in_language = eq->skill_in_language;
		strcpy(emu->message, eq->message);
		delete[] __eq_buffer;
	}

	DECODE(OP_TargetMouse)
	{
		SETUP_DIRECT_DECODE(ClientTarget_Struct, structs::ClientTarget_Struct);
		IN(new_target);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_TargetCommand)
	{
		SETUP_DIRECT_DECODE(ClientTarget_Struct, structs::ClientTarget_Struct);
		IN(new_target);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_Surname)
	{
		SETUP_DIRECT_DECODE(Surname_Struct, structs::Surname_Struct);
		strn0cpy(emu->name, eq->name, 32);
		emu->success = eq->unknown032;
		strn0cpy(emu->lastname, eq->lastname, 20);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Surname)
	{
		ENCODE_LENGTH_EXACT(Surname_Struct);
		SETUP_DIRECT_ENCODE(Surname_Struct, structs::Surname_Struct);
		strn0cpy(eq->name, emu->name, 32);
		eq->unknown032 = emu->success;
		strn0cpy(eq->lastname, emu->lastname, 20);
		FINISH_ENCODE();
	}

	DECODE(OP_Taunt)
	{
		SETUP_DIRECT_DECODE(ClientTarget_Struct, structs::ClientTarget_Struct);
		IN(new_target);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_SetServerFilter)
	{
		DECODE_LENGTH_EXACT(structs::SetServerFilter_Struct);
		SETUP_DIRECT_DECODE(SetServerFilter_Struct, structs::SetServerFilter_Struct);
		emu->filters[0] = eq->filters[5]; //GuildChat
		emu->filters[1] = eq->filters[6]; //Socials
		emu->filters[2] = eq->filters[7]; //GroupChat
		emu->filters[3] = eq->filters[8]; //Shouts
		emu->filters[4] = eq->filters[9]; //Auctions
		emu->filters[5] = eq->filters[10];//OOC
		emu->filters[6] = 1;				//BadWords (Handled by LogServer?)
		emu->filters[7] = eq->filters[2]; //PC Spells 0 is on
		emu->filters[8] = 0;				//NPC Spells Client has it but it doesn't work. 0 is on.
		emu->filters[9] = eq->filters[3]; //Bard Songs 0 is on
		emu->filters[10] = eq->filters[15]; //Spell Crits 0 is on
		int critm = eq->filters[16];
		if (critm > 0){ critm = critm - 1; }
		emu->filters[11] = critm;			//Melee Crits 0 is on. EQMac has 3 options, Emu only 2.
		emu->filters[12] = eq->filters[0]; //Spell Damage 0 is on
		emu->filters[13] = eq->filters[11]; //My Misses
		emu->filters[14] = eq->filters[12]; //Others Misses
		emu->filters[15] = eq->filters[13]; //Others Hit
		emu->filters[16] = eq->filters[14]; //Missed Me
		emu->filters[17] = 0;			  //Damage Shields
		emu->filters[18] = 0;			  //DOT
		emu->filters[19] = 0;			  //Pet Hits
		emu->filters[20] = 0;			  //Pet Misses
		emu->filters[21] = 0;			  //Focus Effects
		emu->filters[22] = 0;			  //Pet Spells
		emu->filters[23] = 0;			  //HoT	
		emu->filters[24] = 0;			  //Unknowns
		emu->filters[25] = 0;
		emu->filters[26] = 0;
		emu->filters[27] = 0;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ZoneSpawns)
	{

		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;


		LogNetcodeDetail("Got {}", sizeof(structs::Spawn_Struct));


		//store away the emu struct
		unsigned char *__emu_buffer = in->pBuffer;
		Spawn_Struct *emu = (Spawn_Struct *)__emu_buffer;

		//determine and verify length
		int entrycount = in->size / sizeof(Spawn_Struct);
		if (entrycount == 0 || (in->size % sizeof(Spawn_Struct)) != 0)
		{
			LogNetcode("[STRUCTS] Wrong size on outbound {}: Got {}, expected multiple of {}", OpcodeManager::EmuToName(in->GetOpcode()), in->size, sizeof(Spawn_Struct));
			delete in;
			return;
		}
		EQApplicationPacket* out = new EQApplicationPacket();
		out->SetOpcode(OP_ZoneSpawns);
		//make the EQ struct.
		out->size = sizeof(structs::Spawn_Struct)*entrycount;
		out->pBuffer = new unsigned char[out->size];
		structs::Spawn_Struct *eq = (structs::Spawn_Struct *) out->pBuffer;

		//zero out the packet. We could avoid this memset by setting all fields (including unknowns)
		memset(out->pBuffer, 0, out->size);

		//do the transform...
		for (int r = 0; r < entrycount; r++, eq++, emu++)
		{

			struct structs::Spawn_Struct* spawns = TrilogySpawns(emu, 0);
			memcpy(eq, spawns, sizeof(structs::Spawn_Struct));
			safe_delete(spawns);

		}
		EQApplicationPacket* outapp = new EQApplicationPacket(OP_ZoneSpawns, sizeof(structs::Spawn_Struct)*entrycount);
		outapp->size = DeflatePacket((unsigned char*)out->pBuffer, out->size, outapp->pBuffer, sizeof(structs::Spawn_Struct)*entrycount);
		EncryptTrilogyZoneSpawnPacket(outapp->pBuffer, outapp->size);
		delete in;
		delete out;
		result->SetPacket(&outapp, reliable);

	}

	ENCODE(OP_GuildsList)
	{
		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;

		//store away the emu struct
		unsigned char *__emu_buffer = in->pBuffer;
		GuildsList_Struct *old_guildlist_pkt = (GuildsList_Struct *)__emu_buffer;
		int num_guilds = (in->size - 4) / sizeof(GuildsListEntry_Struct);
		LogNetcodeDetail("GuildList size {}", num_guilds);

		if (num_guilds == 0) {
			delete in;
			return;
		}
		if (num_guilds > 511)
			num_guilds = 511;
		EQApplicationPacket* outapp = new EQApplicationPacket(OP_GuildsList, 4 + sizeof(structs::GuildsListEntry_Struct) * num_guilds);
		structs::GuildsList_Struct *new_list = (structs::GuildsList_Struct *)outapp->pBuffer;
		memset(new_list, 0, outapp->size);
		memcpy(new_list->head, old_guildlist_pkt->head, 4);
		for (int i = 0; i < num_guilds; i++)
		{
			new_list->Guilds[i].guildID = old_guildlist_pkt->Guilds[i].guildID;
			strn0cpy(new_list->Guilds[i].name, old_guildlist_pkt->Guilds[i].name, 32);
			new_list->Guilds[i].unknown1 = 0xFFFFFFFF;
			new_list->Guilds[i].unknown3 = 0xFFFFFFFF;
		};
		delete in;
		result->SetPacket(&outapp, reliable);

	}

	ENCODE(OP_GuildAdded)
	{
		SETUP_DIRECT_ENCODE(GuildUpdate_Struct, structs::GuildUpdate_Struct);
		OUT(guildID);
		eq->entry.exists = emu->entry.exists;
		eq->entry.guildID = emu->entry.guildID;
		strn0cpy(eq->entry.name, emu->entry.name, 32);
		eq->entry.unknown1 = 0xFFFFFFFF;
		eq->entry.unknown3 = 0xFFFFFFFF;
		FINISH_ENCODE();
	}

	ENCODE(OP_NewSpawn)
	{
		SETUP_DIRECT_ENCODE(Spawn_Struct, structs::Spawn_Struct);

		struct structs::Spawn_Struct* spawns = TrilogySpawns(emu, 0);
		memcpy(eq, spawns, sizeof(structs::Spawn_Struct));
		safe_delete(spawns);

		EQApplicationPacket* outapp = new EQApplicationPacket(OP_ZoneSpawns, sizeof(structs::Spawn_Struct));
		outapp->size = DeflatePacket((unsigned char*)__packet->pBuffer, __packet->size, outapp->pBuffer, sizeof(structs::Spawn_Struct));
		EncryptTrilogyZoneSpawnPacket(outapp->pBuffer, outapp->size);
		result->SetPacket(&outapp, reliable);
		delete[] __emu_buffer;
		safe_delete(__packet);

	}

	DECODE(OP_SpawnAppearance)
	{
		DECODE_LENGTH_EXACT(structs::SpawnAppearance_Struct);
		SETUP_DIRECT_DECODE(SpawnAppearance_Struct, structs::SpawnAppearance_Struct);
		IN(spawn_id);
		IN(type);
		IN(parameter);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_SpawnAppearance)
	{
		ENCODE_LENGTH_EXACT(SpawnAppearance_Struct);
		SETUP_DIRECT_ENCODE(SpawnAppearance_Struct, structs::SpawnAppearance_Struct);
		OUT(spawn_id);
		OUT(type);
		OUT(parameter);
		FINISH_ENCODE();
	}

	DECODE(OP_ZoneChange)
	{
		DECODE_LENGTH_EXACT(structs::ZoneChange_Struct);
		SETUP_DIRECT_DECODE(ZoneChange_Struct, structs::ZoneChange_Struct);
		memcpy(emu->char_name, eq->char_name, sizeof(emu->char_name));
		char shortname[16];
		strncpy(shortname, eq->short_name, 16);
		int i = 179;
		emu->zoneID = 0;
		if (strlen(eq->short_name) > 0)
		{
			while (emu->zoneID == 0 && i > 0)
			{
				if (!strcasecmp(eq->short_name, StaticGetZoneName(i)))
					emu->zoneID = i;
				i--;
			}
		}
		IN(zone_reason);
		IN(success);

		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ZoneChange)
	{
		ENCODE_LENGTH_EXACT(ZoneChange_Struct);
		SETUP_DIRECT_ENCODE(ZoneChange_Struct, structs::ZoneChange_Struct);

		strn0cpy(eq->char_name, emu->char_name, 30);
		strn0cpy(eq->short_name, StaticGetZoneName(emu->zoneID), 15);
		OUT(zone_reason);
		OUT(success);
		FINISH_ENCODE();
	}

	ENCODE(OP_CancelTrade)
	{
		ENCODE_LENGTH_EXACT(CancelTrade_Struct);
		SETUP_DIRECT_ENCODE(CancelTrade_Struct, structs::CancelTrade_Struct);
		OUT(fromid);
		eq->action = 1665;
		FINISH_ENCODE();
	}

	DECODE(OP_CancelTrade)
	{
		DECODE_LENGTH_EXACT(structs::TradeRequest_Struct);
		SETUP_DIRECT_DECODE(TradeRequest_Struct, structs::TradeRequest_Struct);
		IN(from_mob_id);
		IN(to_mob_id);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_TradeMoneyUpdate)
	{
		ENCODE_LENGTH_EXACT(TradeMoneyUpdate_Struct);
		SETUP_DIRECT_ENCODE(TradeMoneyUpdate_Struct, structs::TradeMoneyUpdate_Struct);
		OUT(trader);
		OUT(type);
		OUT(amount);
		FINISH_ENCODE();
	}

	DECODE(OP_TradeMoneyUpdate)
	{
		DECODE_LENGTH_EXACT(structs::TradeMoneyUpdate_Struct);
		SETUP_DIRECT_DECODE(TradeMoneyUpdate_Struct, structs::TradeMoneyUpdate_Struct);
		IN(trader);
		IN(type);
		IN(amount);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_MemorizeSpell)
	{
		SETUP_DIRECT_ENCODE(MemorizeSpell_Struct, structs::MemorizeSpell_Struct);
		OUT(slot);
		OUT(spell_id);
		OUT(scribing);
		FINISH_ENCODE();
	}

	DECODE(OP_MemorizeSpell)
	{
		DECODE_LENGTH_EXACT(structs::MemorizeSpell_Struct);
		SETUP_DIRECT_DECODE(MemorizeSpell_Struct, structs::MemorizeSpell_Struct);
		IN(slot);
		IN(spell_id);
		IN(scribing);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Buff)
	{
		SETUP_DIRECT_ENCODE(SpellBuffFade_Struct, structs::SpellBuffFade_Struct);
		eq->entityid = emu->entityid;
		eq->spellid = emu->spellid;
		eq->slotid = emu->slot_number;
		eq->bufffade = emu->bufffade;
		eq->duration = emu->duration;
		eq->slot = (uint8)emu->slot_number;
		eq->level = emu->level;
		eq->effect = emu->bufftype;
		FINISH_ENCODE();
	}

	DECODE(OP_Buff)
	{
		DECODE_LENGTH_EXACT(structs::Buff_Struct);
		SETUP_DIRECT_DECODE(SpellBuffFade_Struct, structs::Buff_Struct);
		emu->entityid = eq->visible;
		emu->spellid = eq->spellid;
		emu->slot_number = (uint16)eq->slotid;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_BeginCast)
	{
		SETUP_DIRECT_ENCODE(BeginCast_Struct, structs::BeginCast_Struct);
		if (emu->spell_id >= 2999)
			eq->spell_id = 0xFFFF;
		else
			eq->spell_id = emu->spell_id;
		OUT(caster_id);
		OUT(cast_time);
		FINISH_ENCODE();
	}

	DECODE(OP_BeginCast)
	{
		DECODE_LENGTH_EXACT(structs::BeginCast_Struct);
		SETUP_DIRECT_DECODE(BeginCast_Struct, structs::BeginCast_Struct);
		OUT(spell_id);
		OUT(caster_id);
		OUT(cast_time);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_CastSpell)
	{
		SETUP_DIRECT_ENCODE(CastSpell_Struct, structs::CastSpell_Struct);
		OUT(slot);
		OUT(spell_id);
		eq->inventoryslot = ServerToTrilogySlot(emu->inventoryslot);
		OUT(target_id);
		FINISH_ENCODE();
	}

	DECODE(OP_CastSpell) {
		DECODE_LENGTH_EXACT(structs::CastSpell_Struct);
		SETUP_DIRECT_DECODE(CastSpell_Struct, structs::CastSpell_Struct);
		IN(slot);
		IN(spell_id);
		emu->inventoryslot = TrilogyToServerSlot(eq->inventoryslot);
		IN(target_id);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Damage)
	{
		ENCODE_LENGTH_EXACT(Damage_Struct);
		SETUP_DIRECT_ENCODE(Damage_Struct, structs::CombatDamage_Struct);
		eq->target = emu->target;
		eq->source = emu->source;
		eq->type = static_cast<uint8>(emu->type);
		eq->unknown = 0;
		eq->spellid = emu->spellid;
		eq->damage = emu->damage;

		if (std::isnan(emu->force) || std::isinf(emu->force) || emu->force < 0.0f || emu->force > 10.0f)
			eq->force = 0.0f;
		else
			eq->force = emu->force;

		if (std::isnan(emu->sequence) || std::isinf(emu->sequence))
			eq->sequence = 0.0f;
		else
			eq->sequence = emu->sequence;

		if (std::isnan(emu->pushup_angle) || std::isinf(emu->pushup_angle))
			eq->pushup_angle = 0.0f;
		else
			eq->pushup_angle = emu->pushup_angle;

		LogInfo("Trilogy ENCODE(OP_Damage): Target: {}, Source: {}, Type: {}, Spell: {}, Dmg: {}, Force: {:.2f}, Seq: {:.2f}",
			eq->target, eq->source, static_cast<int>(eq->type), eq->spellid, eq->damage, eq->force, eq->sequence);

		FINISH_ENCODE();
	}

	ENCODE(OP_Action)
	{
		ENCODE_LENGTH_EXACT(Action_Struct);
		SETUP_DIRECT_ENCODE(Action_Struct, structs::Action_Struct);
		eq->target = emu->target;
		eq->source = emu->source;
		if (emu->level > 65)
			eq->level = 0x41;
		else
			eq->level = static_cast<uint8>(emu->level);
		eq->spell_level = 0x41;
		eq->instrument_mod = static_cast<uint8>(emu->instrument_mod);
		eq->sequence = emu->sequence;
		eq->type = emu->type;
		if (emu->spell > 2999)
			eq->spell = 0xFFFF;
		else
			eq->spell = emu->spell;
		eq->buff_unknown = emu->buff_unknown;

		LogInfo("Trilogy ENCODE(OP_Action): Target: {}, Source: {}, Level: {}, Type: {}, Spell: {}, BuffUnknown: {}, Seq: {:.2f}",
			eq->target, eq->source, eq->level, eq->type, eq->spell, eq->buff_unknown, eq->sequence);

		FINISH_ENCODE();
	}

	DECODE(OP_Damage) { DECODE_FORWARD(OP_EnvDamage); }
	DECODE(OP_EnvDamage)
	{
		DECODE_LENGTH_EXACT(structs::EnvDamage2_Struct);
		SETUP_DIRECT_DECODE(Damage_Struct, structs::EnvDamage2_Struct);
		emu->target = eq->id;
		emu->type = (uint8)eq->dmgtype;
		emu->damage = eq->damage;
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_ConsiderCorpse) { DECODE_FORWARD(OP_Consider); }
	DECODE(OP_Consider)
	{
		DECODE_LENGTH_EXACT(structs::Consider_Struct);
		SETUP_DIRECT_DECODE(Consider_Struct, structs::Consider_Struct);
		IN(playerid);
		IN(targetid);
		IN(faction);
		IN(level);
		IN(cur_hp);
		IN(max_hp);
		IN(pvpcon);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Consider)
	{
		ENCODE_LENGTH_EXACT(Consider_Struct);
		SETUP_DIRECT_ENCODE(Consider_Struct, structs::Consider_Struct);
		OUT(playerid);
		OUT(targetid);
		uint32 pass_faction = 0;
		switch (emu->faction)
		{
			case 1: pass_faction = 0x00000500; break; // Ally
			case 2: pass_faction = 0x00000300; break; // Warmly
			case 3: pass_faction = 0x00000200; break; // Kindly
			case 4: pass_faction = 0x00000100; break; // Amiably
			case 5: pass_faction = 0x00000000; break; // Indifferently
			case 6: pass_faction = 0xFFFFFD00; break; // Scowls
			case 7: pass_faction = 0xFFFFFE00; break; // Threateningly
			case 8: pass_faction = 0xFFFFFF00; break; // Dubiously
			case 9: pass_faction = 0xFFFFFFFF; break; // Apprehensively
			default: pass_faction = 0x00000000; break;
		}
		eq->faction = pass_faction;
		OUT(level);
		OUT(cur_hp);
		OUT(max_hp);
		OUT(pvpcon);
		FINISH_ENCODE();
	}

	ENCODE(OP_SpawnDoor)
	{
		ENCODE_LENGTH_EXACT(Door_Struct);
		SETUP_DIRECT_ENCODE(Door_Struct, structs::Door_Struct);
		memcpy(eq->name, emu->name, sizeof(eq->name));
		OUT(pos_y);
		OUT(pos_x);
		OUT(pos_z);
		OUT(heading);
		eq->incline = (float)emu->incline;
		eq->padding = 0.0f;
		OUT(doorid);
		OUT(opentype);
		OUT(doorIsOpen);
		OUT(inverted);
		OUT(parameter);
		LogInfo("Trilogy ENCODE(OP_SpawnDoor): name=[{}], doorid=[{}], pos=({:.2f}, {:.2f}, {:.2f}), hdg={:.2f}",
			eq->name, eq->doorid, eq->pos_x, eq->pos_y, eq->pos_z, eq->heading);
		FINISH_ENCODE();
	}

	DECODE(OP_ClickDoor)
	{
		DECODE_LENGTH_EXACT(structs::ClickDoor_Struct);
		SETUP_DIRECT_DECODE(ClickDoor_Struct, structs::ClickDoor_Struct);
		IN(doorid);
		IN(item_id);
		IN(player_id);
		LogInfo("Trilogy DECODE(OP_ClickDoor): raw hex [{:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x} {:02x}], doorid=[{}], item_id=[{}], player_id=[{}]",
			__eq_buffer[0], __eq_buffer[1], __eq_buffer[2], __eq_buffer[3],
			__eq_buffer[4], __eq_buffer[5], __eq_buffer[6], __eq_buffer[7],
			__eq_buffer[8], __eq_buffer[9], __eq_buffer[10], __eq_buffer[11],
			eq->doorid, eq->item_id, eq->player_id);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_GMEndTraining)
	{
		if (__packet->size == sizeof(structs::GMTrainee_Struct))
		{
			SETUP_DIRECT_DECODE(GMTrainEnd_Struct, structs::GMTrainee_Struct);
			emu->npcid = eq->npcid;
			emu->playerid = eq->playerid;
			FINISH_DIRECT_DECODE();
			return;
		}
		DECODE_LENGTH_EXACT(structs::GMTrainEnd_Struct);
		SETUP_DIRECT_DECODE(GMTrainEnd_Struct, structs::GMTrainEnd_Struct);
		IN(npcid);
		IN(playerid);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_GMTraining)
	{
		DECODE_LENGTH_EXACT(structs::GMTrainee_Struct);
		SETUP_DIRECT_DECODE(GMTrainee_Struct, structs::GMTrainee_Struct);
		MEMSET_IN(GMTrainee_Struct);
		emu->npcid = eq->npcid;
		emu->playerid = eq->playerid;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GMTraining)
	{
		ENCODE_LENGTH_EXACT(GMTrainee_Struct);
		SETUP_DIRECT_ENCODE(GMTrainee_Struct, structs::GMTrainee_Struct);
		eq->npcid = emu->npcid;
		eq->playerid = emu->playerid;
		for (int sk = 0; sk < 73; sk++) {
			eq->highesttrain[sk] = (uint8)emu->skills[sk];
		}
		memset(eq->unknown, 1, sizeof(eq->unknown));
		for (int l = 0; l < 24; l++) {
			eq->highesttrainLang[l] = emu->language[l] > 100 ? 100 : (uint8)emu->language[l];
		}
		memset(eq->unknown2, 0, sizeof(eq->unknown2));
		FINISH_ENCODE();
	}

	DECODE(OP_GMTrainSkill)
	{
		DECODE_LENGTH_EXACT(structs::GMSkillChange_Struct);
		SETUP_DIRECT_DECODE(GMSkillChange_Struct, structs::GMSkillChange_Struct);
		MEMSET_IN(GMSkillChange_Struct);
		emu->npcid = eq->npcid;
		emu->playerid = 0;
		emu->skillbank = eq->skillbank;
		emu->unknown2 = 0;
		emu->skill_id = eq->skill_id;
		emu->unknown3 = 0;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ItemLinkResponse) { ENCODE_FORWARD(OP_ItemPacket); }
	ENCODE(OP_ItemPacket)
	{
		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;

		//store away the emu struct
		unsigned char *__emu_buffer = in->pBuffer;
		ItemPacket_Struct *old_item_pkt = (ItemPacket_Struct *)__emu_buffer;
		EQ::InternalSerializedItem_Struct *int_struct = (EQ::InternalSerializedItem_Struct *)(old_item_pkt->SerializedItem);

		const EQ::ItemInstance * item = (const EQ::ItemInstance *)int_struct->inst;

		if (item)
		{
			structs::Item_Struct* trilogy_item = TrilogyItem((EQ::ItemInstance*)int_struct->inst, int_struct->slot_id);

			if (trilogy_item == 0)
			{
				delete in;
				return;
			}

			EQApplicationPacket* outapp = new EQApplicationPacket(OP_ItemPacket, sizeof(structs::Item_Struct));
			memcpy(outapp->pBuffer, trilogy_item, sizeof(structs::Item_Struct));

			outapp->SetOpcode(OP_Unknown);

			if (old_item_pkt->PacketType == ItemPacketSummonItem)
				outapp->SetOpcode(OP_SummonedItem);
			else if (old_item_pkt->PacketType == ItemPacketViewLink)
				outapp->SetOpcode(OP_ItemLinkResponse);
			else if (old_item_pkt->PacketType == ItemPacketTrade || old_item_pkt->PacketType == ItemPacketMerchant)
				outapp->SetOpcode(OP_MerchantItemPacket);
			else if (old_item_pkt->PacketType == ItemPacketLoot)
				outapp->SetOpcode(OP_LootItemPacket);
			else if (item->GetItem()->ItemClass == 1)
				outapp->SetOpcode(OP_ContainerPacket);
			else if (item->GetItem()->ItemClass == 2)
				outapp->SetOpcode(OP_BookPacket);
			else if (int_struct->slot_id == 0)
				outapp->SetOpcode(OP_SummonedItem);
			else
				outapp->SetOpcode(OP_ItemPacket);

			if (outapp->size != sizeof(structs::Item_Struct))
				LogNetcodeDetail("Invalid size on OP_ItemPacket packet. Expected: {}, Got: {}", sizeof(structs::Item_Struct), outapp->size);

			result->SetPacket(&outapp, reliable);
			safe_delete_array(trilogy_item);
		}
		delete in;
	}

	ENCODE(OP_TradeItemPacket)
	{
		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;

		//store away the emu struct
		unsigned char *__emu_buffer = in->pBuffer;
		ItemPacket_Struct *old_item_pkt = (ItemPacket_Struct *)__emu_buffer;
		EQ::InternalSerializedItem_Struct *int_struct = (EQ::InternalSerializedItem_Struct *)(old_item_pkt->SerializedItem);

		const EQ::ItemInstance * item = (const EQ::ItemInstance *)int_struct->inst;

		if (item)
		{
			structs::Item_Struct* trilogy_item = TrilogyItem((EQ::ItemInstance*)int_struct->inst, int_struct->slot_id);

			if (trilogy_item == 0)
			{
				delete in;
				return;
			}

			EQApplicationPacket* outapp = new EQApplicationPacket(OP_TradeItemPacket, sizeof(structs::TradeItemsPacket_Struct));
			structs::TradeItemsPacket_Struct* myitem = (structs::TradeItemsPacket_Struct*) outapp->pBuffer;
			myitem->fromid = old_item_pkt->fromid;
			myitem->slotid = int_struct->slot_id;
			memcpy(&myitem->item, trilogy_item, sizeof(structs::Item_Struct));

			if (outapp->size != sizeof(structs::TradeItemsPacket_Struct))
				LogNetcodeDetail("Invalid size on OP_TradeItemPacket packet. Expected: {}, Got: {}", sizeof(structs::TradeItemsPacket_Struct), outapp->size);

			result->SetPacket(&outapp, reliable);
			safe_delete_array(trilogy_item);
		}
		delete in;
	}

	ENCODE(OP_CharInventory)
	{
		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;

		if (in->size == 0)
		{
			auto outapp = new EQApplicationPacket(OP_CharInventory, 2);
			*((uint16 *)outapp->pBuffer) = 0;
			result->SetPacket(&outapp, reliable);
			delete in;
			return;
		}

		//store away the emu struct
		unsigned char *__emu_buffer = in->pBuffer;

		int16 itemcount = in->size / sizeof(EQ::InternalSerializedItem_Struct);
		if (itemcount == 0 || (in->size % sizeof(EQ::InternalSerializedItem_Struct)) != 0)
		{
			LogNetcode("[STRUCTS] Wrong size on outbound {}: Got {}, expected multiple of {}", OpcodeManager::EmuToName(in->GetOpcode()), in->size, sizeof(EQ::InternalSerializedItem_Struct));
			delete in;
			return;
		}

		EQ::InternalSerializedItem_Struct *eq = (EQ::InternalSerializedItem_Struct *)in->pBuffer;
		//do the transform...
		std::string trilogy_item_string;
		int valid_item_count = 0;
		for (int r = 0; r < itemcount; r++, eq++)
		{
			structs::Item_Struct* trilogy_item = TrilogyItem((EQ::ItemInstance*)eq->inst, eq->slot_id);

			if (trilogy_item != nullptr)
			{
				structs::PlayerItemsPacket_Struct item_pkt;
				memset(&item_pkt, 0, sizeof(structs::PlayerItemsPacket_Struct));
				item_pkt.opcode = 0x2164; // OP_ItemPacket
				memcpy(&item_pkt.item, trilogy_item, sizeof(structs::Item_Struct));

				char *trilogy_item_char = reinterpret_cast<char*>(&item_pkt);
				trilogy_item_string.append(trilogy_item_char, sizeof(structs::PlayerItemsPacket_Struct));
				valid_item_count++;

				LogInfo("Trilogy ENCODE(OP_CharInventory): Item [{}], Slot [{}], ID [{}]",
					trilogy_item->Name, trilogy_item->equipSlot, trilogy_item->ID);

				safe_delete_array(trilogy_item);
			}
		}

		int32 length = 16384;
		int buffer = 2;

		EQApplicationPacket* outapp = new EQApplicationPacket(OP_CharInventory, length);
		outapp->size = buffer + DeflatePacket((uchar*)trilogy_item_string.c_str(), trilogy_item_string.length(), &outapp->pBuffer[buffer], length - buffer);
		*((uint16*)outapp->pBuffer) = valid_item_count;

		result->SetPacket(&outapp, reliable);
		delete in;
	}

	ENCODE(OP_ShopInventoryPacket)
	{
		//consume the packet
		EQApplicationPacket *in = *p;
		*p = nullptr;

		int16 itemcount = in->size / sizeof(EQ::InternalSerializedItem_Struct);
		if (itemcount == 0 || (in->size % sizeof(EQ::InternalSerializedItem_Struct)) != 0)
		{
			LogNetcodeDetail("Wrong size on outbound {}: Got {}, expected multiple of {}", OpcodeManager::EmuToName(in->GetOpcode()), in->size, sizeof(EQ::InternalSerializedItem_Struct));
			delete in;
			return;
		}
		if (itemcount > 80)
			itemcount = 80;

		EQ::InternalSerializedItem_Struct *eq = (EQ::InternalSerializedItem_Struct *)in->pBuffer;
		//do the transform...
		for (int r = 0; r < itemcount; r++, eq++)
		{
			EQ::ItemInstance *cur = (EQ::ItemInstance*)eq->inst;
			structs::Item_Struct* trilogy_item = TrilogyItem(cur, eq->slot_id, 1);

			if (trilogy_item != nullptr)
			{
				int pisize = ITEM_STRUCT_SIZE;
				if (trilogy_item->ItemClass == EQ::item::ItemClassBook)
					pisize = SHORT_BOOK_ITEM_STRUCT_SIZE;
				else if (trilogy_item->ItemClass == EQ::item::ItemClassBag)
					pisize = SHORT_CONTAINER_ITEM_STRUCT_SIZE;

				EQApplicationPacket* outapp = new EQApplicationPacket();
				outapp->SetOpcode(OP_ShopInventoryPacket);
				outapp->size = pisize + 5;
				outapp->pBuffer = new unsigned char[outapp->size];
				memset(outapp->pBuffer, 0, outapp->size);
				structs::MerchantItems_Struct* pi = (structs::MerchantItems_Struct*) outapp->pBuffer;
				memcpy(&pi->item, trilogy_item, pisize);
				pi->itemtype = trilogy_item->ItemClass;
				result->SetPacket(&outapp, reliable);

				safe_delete_array(trilogy_item);
			}
			safe_delete(cur);
		}
		delete in;
	}

	DECODE(OP_DeleteCharge) { DECODE_FORWARD(OP_MoveItem); }
	DECODE(OP_MoveItem)
	{
		SETUP_DIRECT_DECODE(MoveItem_Struct, structs::MoveItem_Struct);

		emu->from_slot = TrilogyToServerSlot(eq->from_slot);
		emu->to_slot = TrilogyToServerSlot(eq->to_slot);
		IN(number_in_stack);

		LogInventory("TRILOGY DECODE OUTPUT to_slot: {}, from_slot: {}, number_in_stack: {}", emu->to_slot, emu->from_slot, emu->number_in_stack);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_DeleteCharge) { ENCODE_FORWARD(OP_MoveItem); }
	ENCODE(OP_MoveItem)
	{
		ENCODE_LENGTH_EXACT(MoveItem_Struct);
		SETUP_DIRECT_ENCODE(MoveItem_Struct, structs::MoveItem_Struct);

		eq->from_slot = ServerToTrilogySlot(emu->from_slot);
		eq->to_slot = ServerToTrilogySlot(emu->to_slot);
		OUT(number_in_stack);
		LogInventory("TRILOGY ENCODE OUTPUT to_slot: {}, from_slot: {}, number_in_stack: {}", eq->to_slot, eq->from_slot, eq->number_in_stack);

		FINISH_ENCODE();
	}

	ENCODE(OP_HPUpdate)
	{
		ENCODE_LENGTH_EXACT(SpawnHPUpdate_Struct);
		SETUP_DIRECT_ENCODE(SpawnHPUpdate_Struct, structs::SpawnHPUpdate_Struct);
		OUT(spawn_id);
		OUT(cur_hp);
		OUT(max_hp);

		LogInfo("Trilogy ENCODE(OP_HPUpdate): spawn_id: {}, cur_hp: {}, max_hp: {}",
			eq->spawn_id, eq->cur_hp, eq->max_hp);

		FINISH_ENCODE();
	}

	DECODE(OP_Consume)
	{
		DECODE_LENGTH_EXACT(structs::Consume_Struct);
		SETUP_DIRECT_DECODE(Consume_Struct, structs::Consume_Struct);

		emu->slot = TrilogyToServerSlot(eq->slot);
		IN(type);
		IN(auto_consumed);

		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ReadBook)
	{
		// no apparent slot translation needed -U
		EQApplicationPacket *in = *p;
		*p = nullptr;

		if (in->size < sizeof(BookText_Struct))
		{
			delete in;
			return;
		}

		unsigned char *__emu_buffer = in->pBuffer;
		BookText_Struct *emu_BookText_Struct = (BookText_Struct *)__emu_buffer;
		size_t txtlen = strlen(emu_BookText_Struct->booktext);

		// In Trilogy, text starts directly at offset 0 (no type byte)
		in->size = txtlen + 1;
		in->pBuffer = new unsigned char[in->size];
		structs::BookText_Struct *eq_BookText_Struct = (structs::BookText_Struct*)in->pBuffer;

		memcpy(eq_BookText_Struct->booktext, emu_BookText_Struct->booktext, txtlen + 1);

		LogInfo("ENCODE(OP_ReadBook): encoded {} bytes of book text starting at offset 0: \"{:.60}...\"", in->size, eq_BookText_Struct->booktext);

		delete[] __emu_buffer;
		result->SetPacket(&in, reliable);
	}

	DECODE(OP_ReadBook)
	{
		CHECK_DECODE_NULLPTR(__packet->pBuffer);
		if (__packet->size < 1)
		{
			__packet->SetOpcode(OP_Unknown);
			return;
		}

		unsigned char *__eq_buffer = __packet->pBuffer;
		size_t eq_size = __packet->size;

		__packet->size = sizeof(BookRequest_Struct);
		__packet->pBuffer = new unsigned char[__packet->size];
		memset(__packet->pBuffer, 0, sizeof(BookRequest_Struct));
		BookRequest_Struct *emu = (BookRequest_Struct *)__packet->pBuffer;

		// In Trilogy, txtfile starts at offset 0
		strn0cpy(emu->txtfile, (const char *)__eq_buffer, sizeof(emu->txtfile));

		// BookType is at the end of the packet, after the null-terminated string
		size_t txtlen = strlen(emu->txtfile);
		if (txtlen + 1 < eq_size)
		{
			emu->type = *(uint8 *)(__eq_buffer + txtlen + 1);
		}

		LogInfo("DECODE(OP_ReadBook): eq_size={}, txtfile='{}', type={}", eq_size, emu->txtfile, emu->type);

		delete[] __eq_buffer;
	}

	ENCODE(OP_Illusion)
	{
		ENCODE_LENGTH_EXACT(Illusion_Struct);
		SETUP_DIRECT_ENCODE(Illusion_Struct, structs::Illusion_Struct);
		OUT(spawnid);
		OUT(race);
		OUT(gender);
		OUT(texture);
		OUT(helmtexture);
		OUT(face);
		OUT(hairstyle);
		OUT(haircolor);
		OUT(beard);
		OUT(beardcolor);
		eq->size = (int16)(emu->size + 0.5);
		eq->unknown007 = 0xFF;
		eq->unknown_void = 0xFFFFFFFF;

		FINISH_ENCODE();
	}

	DECODE(OP_Illusion)
	{
		DECODE_LENGTH_EXACT(structs::Illusion_Struct);
		SETUP_DIRECT_DECODE(Illusion_Struct, structs::Illusion_Struct);
		IN(spawnid);
		IN(race);
		IN(gender);
		IN(texture);
		IN(helmtexture);
		IN(face);
		IN(hairstyle);
		IN(haircolor);
		IN(beard);
		IN(beardcolor);
		IN(size);

		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ShopRequest)
	{
		ENCODE_LENGTH_EXACT(Merchant_Click_Struct);
		SETUP_DIRECT_ENCODE(Merchant_Click_Struct, structs::Merchant_Click_Struct);
		eq->npcid = emu->npcid;
		OUT(playerid);
		OUT(command);
		eq->unknown[0] = 0x71;
		eq->unknown[1] = 0x54;
		eq->unknown[2] = 0x00;
		OUT(rate);
		FINISH_ENCODE();
	}

	DECODE(OP_ShopRequest)
	{
		DECODE_LENGTH_EXACT(structs::Merchant_Click_Struct);
		SETUP_DIRECT_DECODE(Merchant_Click_Struct, structs::Merchant_Click_Struct);
		emu->npcid = eq->npcid;
		IN(playerid);
		IN(command);
		IN(rate);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_ShopPlayerBuy)
	{
		DECODE_LENGTH_EXACT(structs::Merchant_Sell_Struct);
		SETUP_DIRECT_DECODE(Merchant_Sell_Struct, structs::Merchant_Sell_Struct);
		emu->npcid = eq->npcid;
		IN(playerid);
		emu->itemslot = TrilogyToServerSlot(eq->itemslot);
		IN(quantity);
		IN(price);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ShopPlayerBuy)
	{
		ENCODE_LENGTH_EXACT(Merchant_Sell_Struct);
		SETUP_DIRECT_ENCODE(Merchant_Sell_Struct, structs::Merchant_Sell_Struct);
		eq->npcid = emu->npcid;
		eq->playerid = emu->playerid;
		eq->itemslot = ServerToTrilogySlot(emu->itemslot);
		OUT(quantity);
		OUT(price);
		FINISH_ENCODE();
	}

	DECODE(OP_ShopPlayerSell)
	{
		DECODE_LENGTH_EXACT(structs::Merchant_Purchase_Struct);
		SETUP_DIRECT_DECODE(Merchant_Purchase_Struct, structs::Merchant_Purchase_Struct);
		emu->npcid = eq->npcid;
		//IN(playerid);
		emu->itemslot = TrilogyToServerSlot(eq->itemslot);
		IN(quantity);
		IN(price);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ShopPlayerSell)
	{
		ENCODE_LENGTH_EXACT(Merchant_Purchase_Struct);
		SETUP_DIRECT_ENCODE(Merchant_Purchase_Struct, structs::Merchant_Purchase_Struct);
		eq->npcid = emu->npcid;
		//eq->playerid=emu->playerid;
		eq->itemslot = ServerToTrilogySlot(emu->itemslot);
		OUT(quantity);
		OUT(price);
		FINISH_ENCODE();
	}

	ENCODE(OP_ShopDelItem)
	{
		ENCODE_LENGTH_EXACT(Merchant_DelItem_Struct);
		SETUP_DIRECT_ENCODE(Merchant_DelItem_Struct, structs::Merchant_DelItem_Struct);
		eq->npcid = emu->npcid;
		OUT(playerid);
		eq->itemslot = ServerToTrilogySlot(emu->itemslot);
		if (emu->type == 0)
			eq->type = 64;
		else
			OUT(type);
		FINISH_ENCODE();
	}

	ENCODE(OP_Animation)
	{
		ENCODE_LENGTH_EXACT(Animation_Struct);
		SETUP_DIRECT_ENCODE(Animation_Struct, structs::Animation_Struct);
		eq->spawnid = emu->entity_id;
		eq->pad02 = 0;
		eq->action = emu->animation_id;
		eq->speed = emu->speed;
		FINISH_ENCODE();
	}

	DECODE(OP_Animation)
	{
		DECODE_LENGTH_EXACT(structs::Animation_Struct);
		SETUP_DIRECT_DECODE(Animation_Struct, structs::Animation_Struct);
		emu->entity_id = eq->spawnid;
		emu->animation_id = eq->action;
		emu->speed = eq->speed;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_LootItem)
	{
		ENCODE_LENGTH_EXACT(LootingItem_Struct);
		SETUP_DIRECT_ENCODE(LootingItem_Struct, structs::LootingItem_Struct);
		OUT(lootee);
		OUT(looter);
		eq->slot_id = ServerToTrilogyCorpseSlot(emu->slot_id);
		OUT(auto_loot);

		FINISH_ENCODE();
	}

	DECODE(OP_LootItem)
	{
		DECODE_LENGTH_EXACT(structs::LootingItem_Struct);
		SETUP_DIRECT_DECODE(LootingItem_Struct, structs::LootingItem_Struct);
		IN(lootee);
		IN(looter);
		emu->slot_id = TrilogyToServerCorpseSlot(eq->slot_id);
		IN(auto_loot);

		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_AAExpUpdate)
	{
		ENCODE_LENGTH_EXACT(AltAdvStats_Struct);
		SETUP_DIRECT_ENCODE(AltAdvStats_Struct, structs::AltAdvStats_Struct);
		OUT(experience);
		OUT(unspent);
		OUT(percentage);
		FINISH_ENCODE();
	}

	ENCODE(OP_AAAction)
	{
		ENCODE_LENGTH_EXACT(UseAA_Struct);
		SETUP_DIRECT_ENCODE(UseAA_Struct, structs::UseAA_Struct);
		OUT(end);
		OUT(ability);
		OUT(begin);
		eq->unknown_void = 2154;

		FINISH_ENCODE();
	}

	ENCODE(OP_GroundSpawn)
	{
		ENCODE_LENGTH_EXACT(Object_Struct);
		SETUP_DIRECT_ENCODE(Object_Struct, structs::Object_Struct);
		eq->dropid = emu->drop_id;
		eq->itemid = emu->itemid;
		eq->xpos = emu->x;
		eq->ypos = emu->y;
		eq->zpos = emu->z;
		eq->heading = emu->heading;
		strncpy(eq->objectname, emu->object_name, 16);
		eq->type = (int16)emu->object_type;
		eq->stack_size = emu->charges;
		for (int g = 0; g < 10; g++)
		{
			if (emu->itemsinbag[g] > 0 && emu->itemsinbag[g] != 0xFFFF)
			{
				eq->itemsinbag[g] = emu->itemsinbag[g];
			}
			else
			{
				eq->itemsinbag[g] = 0xFFFF;
			}
		}
		FINISH_ENCODE();
	}

	DECODE(OP_GroundSpawn)
	{
		DECODE_LENGTH_EXACT(structs::Object_Struct);
		SETUP_DIRECT_DECODE(Object_Struct, structs::Object_Struct);
		emu->drop_id = eq->dropid;
		emu->itemid = eq->itemid;
		emu->x = eq->xpos;
		emu->y = eq->ypos;
		emu->z = eq->zpos;
		emu->heading = eq->heading;
		strncpy(emu->object_name, eq->objectname, 16);
		emu->charges = eq->stack_size;
		emu->object_type = eq->type;
		for (int i = 0; i < 10; i++)
		{
			emu->itemsinbag[i] = eq->itemsinbag[i];
		}
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ClickObjectAction)
	{

		ENCODE_LENGTH_EXACT(ClickObjectAction_Struct);
		SETUP_DIRECT_ENCODE(ClickObjectAction_Struct, structs::ClickObjectAction_Struct);
		OUT(player_id);
		OUT(drop_id);
		OUT(open);
		OUT(unknown9);
		OUT(type);
		OUT(unknown11);
		OUT(slot);
		memcpy(eq->unknown10, emu->unknown10, sizeof(eq->unknown10));
		OUT(icon);
		memcpy(eq->unknown16, emu->unknown16, sizeof(eq->unknown16));
		FINISH_ENCODE();
	}

	DECODE(OP_ClickObjectAction)
	{
		DECODE_LENGTH_EXACT(structs::ClickObjectAction_Struct);
		SETUP_DIRECT_DECODE(ClickObjectAction_Struct, structs::ClickObjectAction_Struct);
		IN(player_id);
		IN(drop_id);
		IN(open);
		IN(unknown9);
		IN(type);
		IN(unknown11);
		IN(slot);
		memcpy(emu->unknown10, eq->unknown10, sizeof(emu->unknown10));
		IN(icon);
		memcpy(emu->unknown16, eq->unknown16, sizeof(emu->unknown16));
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_TradeSkillCombine)
	{
		DECODE_LENGTH_EXACT(structs::Combine_Struct);
		SETUP_DIRECT_DECODE(Combine_Struct, structs::Combine_Struct);
		emu->container_slot = TrilogyToServerSlot(eq->container_slot);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_TradeRequest)
	{
		DECODE_LENGTH_EXACT(structs::TradeRequest_Struct);
		SETUP_DIRECT_DECODE(TradeRequest_Struct, structs::TradeRequest_Struct);
		IN(from_mob_id);
		IN(to_mob_id);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_TradeRequest)
	{

		ENCODE_LENGTH_EXACT(TradeRequest_Struct);
		SETUP_DIRECT_ENCODE(TradeRequest_Struct, structs::TradeRequest_Struct);
		OUT(from_mob_id);
		OUT(to_mob_id);
		FINISH_ENCODE();
	}

	DECODE(OP_TradeRequestAck)
	{
		DECODE_LENGTH_EXACT(structs::TradeRequest_Struct);
		SETUP_DIRECT_DECODE(TradeRequest_Struct, structs::TradeRequest_Struct);
		IN(from_mob_id);
		IN(to_mob_id);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_TradeRequestAck)
	{

		ENCODE_LENGTH_EXACT(TradeRequest_Struct);
		SETUP_DIRECT_ENCODE(TradeRequest_Struct, structs::TradeRequest_Struct);
		OUT(from_mob_id);
		OUT(to_mob_id);
		FINISH_ENCODE();
	}

	ENCODE(OP_ManaChange)
	{
		EQApplicationPacket *__packet = *p;
		*p = nullptr;
		unsigned char *__emu_buffer = __packet->pBuffer;
		ManaChange_Struct *emu = (ManaChange_Struct *)__emu_buffer;
		EQApplicationPacket* outapp = new EQApplicationPacket(OP_ManaChange);
		if (emu->spell_id > 0 && emu->spell_id < 3000)
		{
			outapp->size = sizeof(structs::ManaChange_Struct);
			outapp->pBuffer = new uchar[outapp->size];
			structs::ManaChange_Struct *eq = (structs::ManaChange_Struct *)outapp->pBuffer;
			eq->new_mana = emu->new_mana;
			eq->spell_id = emu->spell_id;

		}
		else {
			outapp->size = sizeof(structs::ManaChange_Struct2);
			outapp->pBuffer = new uchar[outapp->size];
			structs::ManaChange_Struct2 *eq = (structs::ManaChange_Struct2 *)outapp->pBuffer;
			eq->new_mana = emu->new_mana;
		}
		delete __packet;
		result->SetPacket(&outapp, reliable);
	}

	ENCODE(OP_DeleteSpawn)
	{
		SETUP_DIRECT_ENCODE(DeleteSpawn_Struct, structs::DeleteSpawn_Struct);
		OUT(spawn_id);
		eq->unknown02 = 0;
		LogInfo("Trilogy ENCODE(OP_DeleteSpawn): Spawn: {}", eq->spawn_id);
		FINISH_ENCODE();
	}

	DECODE(OP_DeleteSpawn)
	{
		if (__packet->size != 2 && __packet->size != sizeof(structs::DeleteSpawn_Struct)) {
			__packet->SetOpcode(OP_Unknown);
			return;
		}
		unsigned char *__eq_buffer = __packet->pBuffer;
		uint16 spawn_id = *(uint16*)__eq_buffer;
		__packet->size = sizeof(DeleteSpawn_Struct);
		__packet->pBuffer = new unsigned char[__packet->size];
		DeleteSpawn_Struct *emu = (DeleteSpawn_Struct *)__packet->pBuffer;
		emu->spawn_id = spawn_id;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_TimeOfDay)
	{
		SETUP_DIRECT_ENCODE(TimeOfDay_Struct, structs::TimeOfDay_Struct);
		OUT(hour);
		OUT(minute);
		OUT(day);
		OUT(month);
		OUT(year);
		FINISH_ENCODE();
	}

	DECODE(OP_WhoAllRequest)
	{
		DECODE_LENGTH_EXACT(structs::Who_All_Struct);
		SETUP_DIRECT_DECODE(Who_All_Struct, structs::Who_All_Struct);
		MEMSET_IN(Who_All_Struct);
		strn0cpy(emu->whom, eq->whom, sizeof(eq->whom));
		IN(wrace);
		IN(wclass);
		IN(lvllow);
		IN(lvlhigh);
		IN(gmlookup);
		if (strcasecmp(emu->whom, "lfg") == 0) {
			emu->guildid = -3;
			emu->whom[0] = '\0';
		} else {
			emu->guildid = -1;
		}
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_WhoAllResponse)
	{
		EQApplicationPacket *in = *p;
		*p = nullptr;

		if (!in || in->size < 58) {
			delete in;
			return;
		}

		const unsigned char* bufptr = in->pBuffer;
		const unsigned char* endptr = in->pBuffer + in->size;

		uint32 plid = *(const uint32*)bufptr;
		bufptr += 4;
		uint16 playerineqstring = *(const uint16*)bufptr;
		bufptr += 2;
		bufptr += 27; // line2: "---------------------------"
		uint8 unknown35 = *bufptr;
		bufptr += 1;
		bufptr += 2; // unknown36
		uint16 playersinzonestring = *(const uint16*)bufptr;
		bufptr += 2;
		bufptr += 10; // unknown44[5]
		uint32 unknown52 = *(const uint32*)bufptr;
		bufptr += 4;
		uint32 unknown56 = *(const uint32*)bufptr;
		bufptr += 4;
		uint16 totalusers = *(const uint16*)bufptr;
		bufptr += 2;

		auto send_line = [&](const std::string& line, uint32 msg_type = 10) {
			int len = sizeof(structs::SpecialMesg_Struct) + line.length() + 5;
			auto pkt = new EQApplicationPacket(OP_SpecialMesg, len);
			memset(pkt->pBuffer, 0, len);
			auto* eq = (structs::SpecialMesg_Struct*)pkt->pBuffer;
			eq->msg_type = msg_type;
			strcpy(eq->message, line.c_str());
			result->SetPacket(&pkt, reliable);
		};

		send_line("Players on server:");

		constexpr uint16 kStringIdWhoAllAnon = 5024;
		constexpr uint16 kStringIdWhoAllRole = 5023;
		constexpr uint16 kStringIdWhoAllGM   = 5022;
		constexpr uint16 kStringIdWhoAllCutShort = 5033;

		int displayed_count = 0;
		for (uint16 i = 0; i < totalusers && bufptr < endptr; ++i) {
			if (bufptr + sizeof(uint16) * 2 > endptr)
				break;
			uint16 formatstring = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 pidstring = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);

			const char* plname = (const char*)bufptr;
			size_t plname_len = strnlen(plname, endptr - bufptr);
			bufptr += plname_len + 1;
			if (bufptr >= endptr)
				break;

			if (bufptr + sizeof(uint16) > endptr)
				break;
			uint16 rankstring = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);

			const char* guildbuffer = (const char*)bufptr;
			size_t guild_len = strnlen(guildbuffer, endptr - bufptr);
			bufptr += guild_len + 1;
			if (bufptr >= endptr)
				break;

			// plgm(2) + unk80(2) + plflag(2) + zonestring(2) + plzone(4) + plclass_(2) + pllevel(2) + plrace(2) = 18 bytes
			if (bufptr + 18 > endptr)
				break;
			uint16 plgm = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 unk80 = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 plflag = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 zonestring = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint32 plzone = *(const uint32*)bufptr;
			bufptr += sizeof(uint32);
			uint16 plclass_ = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 pllevel = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);
			uint16 plrace = *(const uint16*)bufptr;
			bufptr += sizeof(uint16);

			const char* placcount = (const char*)bufptr;
			size_t acct_len = strnlen(placcount, endptr - bufptr);
			bufptr += acct_len + 1;
			if (bufptr + sizeof(uint16) <= endptr) {
				bufptr += sizeof(uint16); // ending
			}

			// Format GM or AFK tag
			std::string tmpgm = "";
			if (rankstring != 0 && rankstring != 0xFFFF) {
				if (rankstring == 12311) {
					tmpgm = "AFK ";
				}
				else {
					const char* gm_str = GetTrilogyString(rankstring);
					if (gm_str) {
						tmpgm = gm_str;
						tmpgm += " ";
					}
				}
			}

			// Guild
			std::string guild_str = "";
			if (guild_len > 0 && guildbuffer[0] != '\0') {
				guild_str = " ";
				guild_str += guildbuffer;
			}

			// LFG
			std::string lfg_str = "";
			if (plflag == 12314) {
				lfg_str = " LFG";
			}

			// Zone
			const char* zn = StaticGetZoneName(plzone);
			std::string zone_str = (zn && zn[0] != '\0') ? zn : "UNKNOWN";

			// Class & Race
			const char* class_name = GetClassIDName(static_cast<uint8>(plclass_), static_cast<uint8>(pllevel));
			const char* race_name = GetRaceIDName(plrace);

			char line[512] = { 0 };
			if (formatstring == kStringIdWhoAllAnon) {
				snprintf(line, sizeof(line), "  %s[ANONYMOUS] %s%s", tmpgm.c_str(), plname, lfg_str.c_str());
			}
			else if (formatstring == kStringIdWhoAllRole) {
				snprintf(line, sizeof(line), "  %s[ANONYMOUS] %s%s%s", tmpgm.c_str(), plname, guild_str.c_str(), lfg_str.c_str());
			}
			else if (formatstring == kStringIdWhoAllGM) {
				snprintf(line, sizeof(line), "  %s[ANON %d %s] %s (%s)%s zone: %s%s",
					tmpgm.c_str(), pllevel, class_name, plname, race_name, guild_str.c_str(), zone_str.c_str(), lfg_str.c_str());
			}
			else {
				snprintf(line, sizeof(line), "  %s[%d %s] %s (%s)%s zone: %s%s",
					tmpgm.c_str(), pllevel, class_name, plname, race_name, guild_str.c_str(), zone_str.c_str(), lfg_str.c_str());
			}

			send_line(line);
			displayed_count++;
		}

		if (playersinzonestring == kStringIdWhoAllCutShort) {
			send_line("Your who request was cut short..too many players.");
		}
		char count_line[64] = { 0 };
		snprintf(count_line, sizeof(count_line), "%d players online", displayed_count);
		send_line(count_line);

		delete in;
	}

	DECODE(OP_LFGCommand)
	{
		DECODE_LENGTH_EXACT(structs::LFG_Struct);
		SETUP_DIRECT_DECODE(LFG_Struct, structs::LFG_Struct);
		MEMSET_IN(LFG_Struct);
		strn0cpy(emu->name, eq->name, sizeof(eq->name));
		IN(value);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_LFGCommand)
	{
		delete *p;
		*p = nullptr;
	}

	ENCODE(OP_GroupInvite2) { ENCODE_FORWARD(OP_GroupInvite); }
	ENCODE(OP_GroupInvite)
	{
		ENCODE_LENGTH_EXACT(GroupInvite_Struct);
		SETUP_DIRECT_ENCODE(GroupInvite_Struct, structs::GroupInvite_Struct);
		strn0cpy(eq->invitee_name, emu->invitee_name, 30);
		strn0cpy(eq->inviter_name, emu->inviter_name, 30);
		FINISH_ENCODE();
	}

	DECODE(OP_GroupInvite2) { DECODE_FORWARD(OP_GroupInvite); }
	DECODE(OP_GroupInvite)
	{
		DECODE_LENGTH_EXACT(structs::GroupInvite_Struct);
		SETUP_DIRECT_DECODE(GroupInvite_Struct, structs::GroupInvite_Struct);
		strcpy(emu->invitee_name, eq->invitee_name);
		strcpy(emu->inviter_name, eq->inviter_name);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GroupFollow)
	{
		ENCODE_LENGTH_EXACT(GroupGeneric_Struct);
		SETUP_DIRECT_ENCODE(GroupGeneric_Struct, structs::GroupGeneric_Struct);
		strn0cpy(eq->name1, emu->name1, 30);
		strn0cpy(eq->name2, emu->name2, 30);
		FINISH_ENCODE();
	}

	DECODE(OP_GroupFollow)
	{
		DECODE_LENGTH_EXACT(structs::GroupGeneric_Struct);
		SETUP_DIRECT_DECODE(GroupGeneric_Struct, structs::GroupGeneric_Struct);
		strcpy(emu->name1, eq->name1);
		strcpy(emu->name2, eq->name2);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GroupUpdate)
	{
		ENCODE_LENGTH_ATLEAST(GroupJoin_Struct);
		SETUP_DIRECT_ENCODE(GroupUpdate_Struct, structs::GroupUpdate_Struct);
		strn0cpy(eq->yourname, emu->yourname, 32);

		if (emu->action == 8)
		{
			// groupActMakeLeader = 8
			strn0cpy(eq->othername, emu->leadersname, 32);
			eq->action = 1;
		}
		else if (emu->action == 6) {
			// groupActDisband = 6
			strn0cpy(eq->othername, emu->yourname, 32);
			eq->action = 4;
		}
		else {
			strn0cpy(eq->othername, emu->membername[0], 32);
		}
		// Trilogy
		// #define	ADD_MEMBER		0
		// #define  NEW_LEADER		1
		// #define	REMOVE_MEMBER	3
		// #define	GROUP_QUIT		4
		// Group Full 5

		// groupActLeave = 1
		if (emu->action == 1)
			eq->action = 3;


		FINISH_ENCODE();
	}

	ENCODE(OP_GroupCancelInvite)
	{
		ENCODE_LENGTH_EXACT(GroupCancel_Struct);
		SETUP_DIRECT_ENCODE(GroupCancel_Struct, structs::GroupCancel_Struct);
		strn0cpy(eq->name1, emu->name1, 30);
		strn0cpy(eq->name2, emu->name2, 30);
		OUT(toggle);
		FINISH_ENCODE();
	}

	DECODE(OP_GroupCancelInvite)
	{
		DECODE_LENGTH_EXACT(structs::GroupCancel_Struct);
		SETUP_DIRECT_DECODE(GroupCancel_Struct, structs::GroupCancel_Struct);
		strcpy(emu->name1, eq->name1);
		strcpy(emu->name2, eq->name2);
		IN(toggle);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_GroupDisband)
	{
		ENCODE_LENGTH_EXACT(GroupGeneric_Struct);
		SETUP_DIRECT_ENCODE(GroupGeneric_Struct, structs::GroupGeneric_Struct);
		strn0cpy(eq->name1, emu->name1, 30);
		strn0cpy(eq->name2, emu->name2, 30);
		FINISH_ENCODE();
	}

	DECODE(OP_GroupDisband)
	{
		DECODE_LENGTH_EXACT(structs::GroupGeneric_Struct);
		SETUP_DIRECT_DECODE(GroupGeneric_Struct, structs::GroupGeneric_Struct);
		strcpy(emu->name1, eq->name1);
		strcpy(emu->name2, eq->name2);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_TradeCoins)
	{
		SETUP_DIRECT_ENCODE(TradeCoin_Struct, structs::TradeCoin_Struct);
		OUT(trader);
		OUT(slot);
		OUT(amount);
		eq->unknown005 = 0x4fD2;
		FINISH_ENCODE();
	}

	DECODE(OP_ItemLinkResponse)
	{
		DECODE_LENGTH_EXACT(structs::ItemViewRequest_Struct);
		SETUP_DIRECT_DECODE(ItemViewRequest_Struct, structs::ItemViewRequest_Struct);
		IN(item_id);
		strcpy(emu->item_name, eq->item_name);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_RequestClientZoneChange)
	{
		SETUP_DIRECT_ENCODE(RequestClientZoneChange_Struct, structs::RequestClientZoneChange_Struct);
		strn0cpy(eq->zone_name, StaticGetZoneName(emu->zone_id), 30);
		OUT(x);
		OUT(y);
		eq->z = emu->z * 10;
		OUT(heading);
		FINISH_ENCODE();
	}

	ENCODE(OP_Trader)
	{
		if ((*p)->size == sizeof(Trader_Struct))
		{
			ENCODE_LENGTH_EXACT(Trader_Struct);
			SETUP_DIRECT_ENCODE(Trader_Struct, structs::Trader_Struct);
			OUT(Code);
			int k;
			for (k = 0; k < 80; k++)
			{
				eq->Items[k] = emu->Items[k];
			}
			for (k = 0; k < 80; k++)
			{
				eq->ItemCost[k] = emu->ItemCost[k];
			}
			FINISH_ENCODE();
		}
		else if ((*p)->size == sizeof(Trader_ShowItems_Struct))
		{
			ENCODE_LENGTH_EXACT(Trader_ShowItems_Struct);
			SETUP_DIRECT_ENCODE(Trader_ShowItems_Struct, structs::Trader_ShowItems_Struct);
			OUT(Code);
			OUT(TraderID);
			if (emu->SubAction == 0)
				eq->SubAction = emu->Code;
			else
				OUT(SubAction);
			eq->Items[0] = emu->Items[0];
			eq->Items[1] = emu->Items[1];
			eq->Items[2] = emu->Items[2];
			FINISH_ENCODE();
		}
		else if ((*p)->size == sizeof(TraderPriceUpdate_Struct))
		{
			ENCODE_LENGTH_EXACT(TraderPriceUpdate_Struct);
			SETUP_DIRECT_ENCODE(TraderPriceUpdate_Struct, structs::TraderPriceUpdate_Struct);
			OUT(Action);
			OUT(SubAction);
			eq->SerialNumber = emu->ItemID;
			OUT(NewPrice);
			FINISH_ENCODE();
		}
		else if ((*p)->size == sizeof(TraderBuy_Struct))
		{
			ENCODE_LENGTH_EXACT(TraderBuy_Struct);
			SETUP_DIRECT_ENCODE(TraderBuy_Struct, structs::TraderBuy_Struct);
			OUT(Action);
			OUT(TraderID);
			OUT(ItemID);
			OUT(Price);
			OUT(Quantity);
			OUT(Slot);
			strcpy(eq->ItemName, emu->ItemName);
			FINISH_ENCODE();
		}
		else
		{
			delete *p;
			*p = nullptr;
		}
	}

	DECODE(OP_Trader)
	{
		if (__packet->size == sizeof(structs::Trader_Struct))
		{
			DECODE_LENGTH_EXACT(structs::Trader_Struct);
			SETUP_DIRECT_DECODE(Trader_Struct, structs::Trader_Struct);
			IN(Code);
			int k;
			for (k = 0; k < 80; k++)
			{
				emu->Items[k] = eq->Items[k];
			}
			for (k = 0; k < 80; k++)
			{
				emu->ItemCost[k] = eq->ItemCost[k];
			}
			FINISH_DIRECT_DECODE();
		}
		else if (__packet->size == sizeof(structs::TraderStatus_Struct))
		{
			DECODE_LENGTH_EXACT(structs::TraderStatus_Struct);
			SETUP_DIRECT_DECODE(TraderStatus_Struct, structs::TraderStatus_Struct);
			IN(Code);
			IN(TraderID);
			FINISH_DIRECT_DECODE();
		}
		else if (__packet->size == sizeof(structs::TraderPriceUpdate_Struct))
		{
			DECODE_LENGTH_EXACT(structs::TraderPriceUpdate_Struct);
			SETUP_DIRECT_DECODE(TraderPriceUpdate_Struct, structs::TraderPriceUpdate_Struct);
			IN(Action);
			IN(SubAction);
			emu->ItemID = eq->SerialNumber;
			IN(NewPrice);
			FINISH_DIRECT_DECODE();
		}
		else
		{
			__packet->SetOpcode(OP_Unknown);
		}
	}

	ENCODE(OP_BecomeTrader)
	{
		ENCODE_LENGTH_EXACT(BecomeTrader_Struct);
		SETUP_DIRECT_ENCODE(BecomeTrader_Struct, structs::BecomeTrader_Struct);
		OUT(Code);
		OUT(ID);
		FINISH_ENCODE();
	}

	ENCODE(OP_TraderBuy)
	{
		ENCODE_LENGTH_EXACT(TraderBuy_Struct);
		SETUP_DIRECT_ENCODE(TraderBuy_Struct, structs::TraderBuy_Struct);
		OUT(Action);
		OUT(TraderID);
		OUT(ItemID);
		OUT(Price);
		OUT(Quantity);
		OUT(Slot);
		strcpy(eq->ItemName, emu->ItemName);
		FINISH_ENCODE();
	}

	DECODE(OP_TraderBuy)
	{
		DECODE_LENGTH_EXACT(structs::TraderBuy_Struct);
		SETUP_DIRECT_DECODE(TraderBuy_Struct, structs::TraderBuy_Struct);
		IN(Action);
		IN(TraderID);
		IN(ItemID);
		IN(Price);
		IN(Quantity);
		IN(Slot);
		strcpy(emu->ItemName, eq->ItemName);
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_BazaarSearch){
		if (__packet->size == sizeof(structs::BazaarSearch_Struct))
		{
			DECODE_LENGTH_EXACT(structs::BazaarSearch_Struct);
			SETUP_DIRECT_DECODE(BazaarSearch_Struct, structs::BazaarSearch_Struct);
			IN(Beginning.Action);
			IN(TraderID);
			IN(Class_);
			IN(Race);
			IN(ItemStat);
			IN(Slot);
			IN(Type);
			IN(MinPrice);
			IN(MaxPrice);
			strcpy(emu->Name, eq->Name);
			FINISH_DIRECT_DECODE();
		}
		else if (__packet->size == sizeof(structs::BazaarWelcome_Struct))
		{
			DECODE_LENGTH_EXACT(structs::BazaarWelcome_Struct);
			SETUP_DIRECT_DECODE(BazaarWelcome_Struct, structs::BazaarWelcome_Struct);
			emu->Beginning.Action = eq->Action;
			IN(Traders);
			IN(Items);
			FINISH_DIRECT_DECODE();
		}
		else
		{
			__packet->SetOpcode(OP_Unknown);
		}
	}

	ENCODE(OP_WearChange)
	{
		ENCODE_LENGTH_EXACT(WearChange_Struct);
		SETUP_DIRECT_ENCODE(WearChange_Struct, structs::WearChange_Struct);
		OUT(spawn_id);
		OUT(wear_slot_id);
		OUT(material);
		eq->color = (uint16)emu->color.Color;
		eq->blue = emu->color.Blue;
		eq->green = emu->color.Green;
		eq->red = emu->color.Red;
		eq->use_tint = emu->color.UseTint;
		FINISH_ENCODE();
	}

	DECODE(OP_WearChange)
	{
		DECODE_LENGTH_EXACT(structs::WearChange_Struct);
		SETUP_DIRECT_DECODE(WearChange_Struct, structs::WearChange_Struct);
		IN(spawn_id);
		IN(wear_slot_id);
		IN(material);
		emu->color.Color = eq->color;
		emu->color.Blue = eq->blue;
		emu->color.Green = eq->green;
		emu->color.Red = eq->red;
		emu->color.UseTint = eq->use_tint;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_ExpUpdate)
	{
		ENCODE_LENGTH_EXACT(ExpUpdate_Struct);
		SETUP_DIRECT_ENCODE(ExpUpdate_Struct, structs::ExpUpdate_Struct);
		OUT(exp);
		FINISH_ENCODE();
	}

	ENCODE(OP_LevelUpdate)
	{
		ENCODE_LENGTH_EXACT(LevelUpdate_Struct);
		SETUP_DIRECT_ENCODE(LevelUpdate_Struct, structs::LevelUpdate_Struct);
		eq->level = static_cast<uint8>(emu->level);
		eq->can_delevel = 1;
		FINISH_ENCODE();
	}

	ENCODE(OP_Death)
	{
		ENCODE_LENGTH_EXACT(Death_Struct);
		SETUP_DIRECT_ENCODE(Death_Struct, structs::Death_Struct);
		OUT(spawn_id);
		OUT(killer_id);
		OUT(corpseid);
		OUT(spell_id);
		OUT(attack_skill);
		eq->unknown015 = 0;
		eq->damage = static_cast<uint16>(emu->damage);
		OUT(is_PC);
		eq->unknown017 = 0;
		LogInfo("Trilogy ENCODE(OP_Death): Spawn: {}, Killer: {}, Corpse: {}, Spell: {}, Skill: {}, Dmg: {}, is_PC: {}",
			eq->spawn_id, eq->killer_id, eq->corpseid, eq->spell_id, eq->attack_skill, eq->damage, eq->is_PC);
		FINISH_ENCODE();
	}

	DECODE(OP_Bug)
	{
		DECODE_LENGTH_EXACT(structs::BugStruct);
		SETUP_DIRECT_DECODE(BugReport_Struct, structs::BugStruct);
		strcpy(emu->category_name, eq->chartype);
		strn0cpy(emu->reporter_name, eq->name, sizeof(emu->reporter_name));
		strn0cpy(emu->bug_report, eq->bug, sizeof(emu->bug_report));
		strcpy(emu->target_name, eq->target_name);
		emu->pos_x = eq->x;
		emu->pos_y = eq->y;
		emu->pos_z = eq->z;
		IN(heading);
		emu->optional_info_mask = eq->type;
		memcpy(emu->unknown256, eq->unknown256, sizeof(emu->unknown256));
		emu->unknown3336 = eq->unknown3336;
		FINISH_DIRECT_DECODE();
	}

	DECODE(OP_CombatAbility)
	{
		DECODE_LENGTH_EXACT(structs::CombatAbility_Struct);
		SETUP_DIRECT_DECODE(CombatAbility_Struct, structs::CombatAbility_Struct);
		IN(m_target);
		IN(m_atk);
		IN(m_skill);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Projectile)
	{
		ENCODE_LENGTH_EXACT(Arrow_Struct);
		SETUP_DIRECT_ENCODE(Arrow_Struct, structs::Arrow_Struct);
		OUT(type);
		OUT(src_x);
		OUT(src_y);
		OUT(src_z);
		OUT(velocity);
		OUT(launch_angle);
		OUT(tilt);
		OUT(arc);
		OUT(source_id);
		OUT(target_id);
		OUT(skill);
		OUT(object_id);
		OUT(effect_type);
		OUT(yaw);
		OUT(pitch);
		OUT(behavior);
		OUT(light);
		strcpy(eq->model_name, emu->model_name);
		FINISH_ENCODE();
	}

	ENCODE(OP_Charm)
	{
		ENCODE_LENGTH_EXACT(Charm_Struct);
		SETUP_DIRECT_ENCODE(Charm_Struct, structs::Charm_Struct);
		OUT(owner_id);
		OUT(pet_id);
		OUT(command);
		FINISH_ENCODE();
	}

	ENCODE(OP_Reward)
	{
		ENCODE_LENGTH_EXACT(QuestReward_Struct);
		SETUP_DIRECT_ENCODE(QuestReward_Struct, structs::QuestReward_Struct);
		OUT(mob_id);
		OUT(copper);
		OUT(silver);
		OUT(gold);
		OUT(platinum);
		FINISH_ENCODE();
	}

	ENCODE(OP_FaceChange)
	{
		ENCODE_LENGTH_EXACT(FaceChange_Struct);
		SETUP_DIRECT_ENCODE(FaceChange_Struct, structs::FaceChange_Struct);
		OUT(haircolor);
		OUT(beardcolor);
		OUT(eyecolor1);
		OUT(eyecolor2);
		OUT(hairstyle);
		OUT(beard);
		OUT(face);
		FINISH_ENCODE();
	}

	DECODE(OP_FaceChange)
	{
		DECODE_LENGTH_EXACT(structs::FaceChange_Struct);
		SETUP_DIRECT_DECODE(FaceChange_Struct, structs::FaceChange_Struct);
		IN(haircolor);
		IN(beardcolor);
		IN(eyecolor1);
		IN(eyecolor2);
		IN(hairstyle);
		IN(beard);
		IN(face);
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_Assist)
	{
		ENCODE_LENGTH_EXACT(EntityId_Struct);
		SETUP_DIRECT_ENCODE(EntityId_Struct, structs::EntityId_Struct);
		OUT(entity_id);
		FINISH_ENCODE();
	}

	DECODE(OP_Assist)
	{
		DECODE_LENGTH_EXACT(structs::EntityId_Struct);
		SETUP_DIRECT_DECODE(EntityId_Struct, structs::EntityId_Struct);
		IN(entity_id);
		FINISH_DIRECT_DECODE();
	}

	structs::Item_Struct* TrilogyItem(const EQ::ItemInstance *inst, int16 slot_id_in, int type)
	{

		if (!inst)
			return 0;

		const EQ::ItemData *item = inst->GetItem();

		if (item->ID > 32767)
			return 0;

		unsigned char *buffer = new unsigned char[sizeof(structs::Item_Struct)];
		structs::Item_Struct *trilogy_pop_item = (structs::Item_Struct *)buffer;
		memset(trilogy_pop_item, 0, sizeof(structs::Item_Struct));

		if (item->GMFlag == -1)
			LogInfo("Item {} is flagged for GMs.", item->Name);

		// General items
		if (type == 0)
		{
			trilogy_pop_item->equipSlot = ServerToTrilogySlot(slot_id_in);
			trilogy_pop_item->common.Charges = inst->GetCharges();
			if (item->NoDrop == 0)
				trilogy_pop_item->Price = 0;
			else
				trilogy_pop_item->Price = item->Price;
			trilogy_pop_item->common.SellRate = item->SellRate;
		}
		else
		{
			trilogy_pop_item->common.Charges = 1;
			trilogy_pop_item->equipSlot = inst->GetMerchantSlot();
			if (item->NoDrop == 0)
				trilogy_pop_item->Price = 0;
			else
				trilogy_pop_item->Price = inst->GetPrice();  //This handles sellrate for us. 
			trilogy_pop_item->common.SellRate = 1;
		}

		// Comment: Flag value indicating type of item:
		// Comment: 0x0000 - Readable scroll? few i've seen say "rolled up", i think they're like books
		// 	#define ITEM_NORMAL1                    0x0031
		// 	#define ITEM_NORMAL2                    0x0036
		// 	#define ITEM_NORMAL3                    0x315f
		// 	#define ITEM_NORMAL4                    0x3336
		// Comment: 0x0031 - Normal Item - Only seen once on GM summoned food
		// Comment: 0x0036 - Normal Item (all scribed spells, Velium proc weapons, and misc.)
		// Comment: 0x315f - Normal Item
		// Comment: 0x3336 - Normal Item
		// Comment: 0x3d00 - Container, racial tradeskills? or maybe non-consuming? i dunnno, something weirdo =p
		// Comment: 0x5400 - Container (Combine, Player made, Weight Reducing, etc...)
		// Comment: 0x5450 - Container, plain ordinary newbie containers
		// Comment: 0x7669 - Book item
		if (item->ItemClass == 2) {
			// books
			trilogy_pop_item->flag = 0x7669;
		}
		else if (item->ItemClass == 1) {
			if (item->BagType > 8)
				trilogy_pop_item->flag = 0x3d00;
			else
				trilogy_pop_item->flag = 0x5450;
		}
		else {
			if ((item->Worn.Effect > 0 && item->Worn.Effect < 3000) || (item->Click.Effect > 0 && item->Click.Effect < 3000) || (item->Scroll.Effect > 0 && item->Scroll.Effect < 3000) || (item->Proc.Effect > 0 && item->Proc.Effect < 3000))
				trilogy_pop_item->flag = 0x0036;
			else
				trilogy_pop_item->flag = 0x315f;
		}
		//trilogy_pop_item->flag = 0x0036;
		trilogy_pop_item->ItemClass = item->ItemClass;
		strn0cpy(trilogy_pop_item->Name, item->Name, 35);
		strn0cpy(trilogy_pop_item->Lore, item->Lore, 60);
		strn0cpy(trilogy_pop_item->IDFile, item->IDFile, 6);
		trilogy_pop_item->Weight = item->Weight;
		trilogy_pop_item->NoRent = item->NoRent;
		trilogy_pop_item->NoDrop = item->NoDrop;
		trilogy_pop_item->Size = item->Size;
		trilogy_pop_item->ID = item->ID;
		trilogy_pop_item->Icon = item->Icon;
		trilogy_pop_item->Slots = item->Slots;

		if (item->ItemClass == 2)
		{
			strncpy(trilogy_pop_item->book.Filename, item->Filename, 15);
			trilogy_pop_item->book.Book = item->Book;
			trilogy_pop_item->book.BookType = item->BookType;
		}
		else
		{
			trilogy_pop_item->common.unknown0282 = 0xFF;
			trilogy_pop_item->common.unknown0283 = 0XFF;
			trilogy_pop_item->common.CastTime = item->CastTime;
			trilogy_pop_item->common.SkillModType = item->SkillModType;
			trilogy_pop_item->common.SkillModValue = item->SkillModValue;
			trilogy_pop_item->common.BaneDmgRace = item->BaneDmgRace;
			trilogy_pop_item->common.BaneDmgBody = item->BaneDmgBody;
			trilogy_pop_item->common.BaneDmgAmt = item->BaneDmgAmt;
			trilogy_pop_item->common.RecLevel = item->RecLevel;
			trilogy_pop_item->common.RecSkill = item->RecSkill;
			trilogy_pop_item->common.ProcRate = item->ProcRate;
			trilogy_pop_item->common.ElemDmgType = item->ElemDmgType;
			trilogy_pop_item->common.ElemDmgAmt = item->ElemDmgAmt;
			trilogy_pop_item->common.FactionMod1 = item->FactionMod1;
			trilogy_pop_item->common.FactionMod2 = item->FactionMod2;
			trilogy_pop_item->common.FactionMod3 = item->FactionMod3;
			trilogy_pop_item->common.FactionMod4 = item->FactionMod4;
			trilogy_pop_item->common.FactionAmt1 = item->FactionAmt1;
			trilogy_pop_item->common.FactionAmt2 = item->FactionAmt2;
			trilogy_pop_item->common.FactionAmt3 = item->FactionAmt3;
			trilogy_pop_item->common.FactionAmt4 = item->FactionAmt4;
			trilogy_pop_item->common.Deity = item->Deity;

			if (item->ItemClass == 1)
			{
				trilogy_pop_item->common.container.BagType = item->BagType;
				trilogy_pop_item->common.container.BagSlots = item->BagSlots;
				trilogy_pop_item->common.container.IsBagOpen = 0;
				trilogy_pop_item->common.container.BagSize = item->BagSize;
				trilogy_pop_item->common.container.BagWR = item->BagWR;
			}
			else {
				trilogy_pop_item->common.normal.Races = item->Races;
				if (item->Click.Effect > 0 && item->Click.Effect < 3000) {
					if (item->Click.Type == 5) {
						trilogy_pop_item->common.normal.click_effect_type = 3;
					}
					else {
						trilogy_pop_item->common.normal.click_effect_type = item->Click.Type;
					}
				}
				else if (item->Worn.Effect > 0 && item->Worn.Effect < 3000) {
					trilogy_pop_item->common.normal.click_effect_type = item->Worn.Type;
				}
				else if (item->Scroll.Effect > 0 && item->Scroll.Effect < 3000) {
					trilogy_pop_item->common.normal.click_effect_type = item->Scroll.Type;
				}
				else if (item->Proc.Effect > 0 && item->Proc.Effect < 3000) {
					trilogy_pop_item->common.normal.click_effect_type = 2;
				}
			}
			trilogy_pop_item->common.AStr = item->AStr;
			trilogy_pop_item->common.ASta = item->ASta;
			trilogy_pop_item->common.ACha = item->ACha;
			trilogy_pop_item->common.ADex = item->ADex;
			trilogy_pop_item->common.AInt = item->AInt;
			trilogy_pop_item->common.AAgi = item->AAgi;
			trilogy_pop_item->common.AWis = item->AWis;
			trilogy_pop_item->common.MR = item->MR;
			trilogy_pop_item->common.FR = item->FR;
			trilogy_pop_item->common.CR = item->CR;
			trilogy_pop_item->common.DR = item->DR;
			trilogy_pop_item->common.PR = item->PR;
			trilogy_pop_item->common.HP = item->HP;
			trilogy_pop_item->common.Mana = item->Mana;
			trilogy_pop_item->common.AC = item->AC;
			//trilogy_pop_item->MaxCharges = item->MaxCharges;    
			trilogy_pop_item->common.Light = item->Light;
			trilogy_pop_item->common.Delay = item->Delay;
			trilogy_pop_item->common.Damage = item->Damage;
			trilogy_pop_item->common.Range = item->Range;
			trilogy_pop_item->common.ItemType = item->ItemType;
			trilogy_pop_item->common.Magic = item->Magic;
			trilogy_pop_item->common.Material = item->Material;
			trilogy_pop_item->common.Color = item->Color;
			//trilogy_pop_item->common.Faction = item->Faction;   
			trilogy_pop_item->common.Classes = item->Classes;
			trilogy_pop_item->common.Stackable = (item->Stackable_ == 1 ? 1 : 0);
		}

		//FocusEffect and BardEffect is already handled above. Now figure out click, scroll, proc, and worn.

		if (item->Click.Effect > 0 && item->Click.Effect < 3000)
		{
			trilogy_pop_item->common.Effect1 = item->Click.Effect;
			trilogy_pop_item->common.Effect2 = item->Click.Effect;
			trilogy_pop_item->common.EffectType2 = item->Click.Type;
			trilogy_pop_item->common.EffectType1 = item->Click.Type;
			if (item->Click.Level > 0)
			{
				trilogy_pop_item->common.EffectLevel1 = item->Click.Level;
				trilogy_pop_item->common.EffectLevel2 = item->Click.Level;
			}
			else
			{
				trilogy_pop_item->common.EffectLevel1 = item->Click.Level2;
				trilogy_pop_item->common.EffectLevel2 = item->Click.Level2;
			}
		}
		else if (item->Scroll.Effect > 0 && item->Scroll.Effect < 3000)
		{
			trilogy_pop_item->common.Effect1 = item->Scroll.Effect;
			trilogy_pop_item->common.Effect2 = item->Scroll.Effect;
			trilogy_pop_item->common.EffectType2 = item->Scroll.Type;
			trilogy_pop_item->common.EffectType1 = item->Scroll.Type;
			if (item->Scroll.Level > 0)
			{
				trilogy_pop_item->common.EffectLevel1 = item->Scroll.Level;
				trilogy_pop_item->common.EffectLevel2 = item->Scroll.Level;
			}
			else
			{
				trilogy_pop_item->common.EffectLevel1 = item->Scroll.Level2;
				trilogy_pop_item->common.EffectLevel2 = item->Scroll.Level2;
			}
		}
		//We have some worn effect items (Lodizal Shell Shield) as proceffect in db.
		else if (item->Proc.Effect > 0 && item->Proc.Effect < 3000)
		{
			trilogy_pop_item->common.Effect1 = item->Proc.Effect;
			trilogy_pop_item->common.Effect2 = item->Proc.Effect;
			if (item->Worn.Type > 0)
			{
				trilogy_pop_item->common.EffectType2 = item->Worn.Type;
				trilogy_pop_item->common.EffectType1 = item->Worn.Type;
			}
			else
			{
				trilogy_pop_item->common.EffectType2 = item->Proc.Type;
				trilogy_pop_item->common.EffectType1 = item->Proc.Type;
			}
			if (item->Proc.Level > 0)
			{
				trilogy_pop_item->common.EffectLevel1 = item->Proc.Level;
				trilogy_pop_item->common.EffectLevel2 = item->Proc.Level;
			}
			else
			{
				trilogy_pop_item->common.EffectLevel1 = item->Proc.Level2;
				trilogy_pop_item->common.EffectLevel2 = item->Proc.Level2;
			}
		}
		else if (item->Worn.Effect > 0 && item->Worn.Effect < 3000)
		{
			trilogy_pop_item->common.Effect1 = item->Worn.Effect;
			trilogy_pop_item->common.Effect2 = item->Worn.Effect;
			trilogy_pop_item->common.EffectType2 = item->Worn.Type;
			trilogy_pop_item->common.EffectType1 = item->Worn.Type;
			if (item->Worn.Level > 0)
			{
				trilogy_pop_item->common.EffectLevel1 = item->Worn.Level;
				trilogy_pop_item->common.EffectLevel2 = item->Worn.Level;
			}
			else
			{
				trilogy_pop_item->common.EffectLevel1 = item->Worn.Level2;
				trilogy_pop_item->common.EffectLevel2 = item->Worn.Level2;
			}
		}

		return trilogy_pop_item;
	}

	structs::Spawn_Struct* TrilogySpawns(struct Spawn_Struct* emu, int type)
	{

		if (sizeof(emu) == 0)
			return 0;

		structs::Spawn_Struct *eq = new structs::Spawn_Struct;
		memset(eq, 0, sizeof(structs::Spawn_Struct));
		if (type == 0)
			eq->deltaHeading = emu->deltaHeading;
		if (type == 1)
			eq->deltaHeading = 0;

		strn0cpy(eq->name, emu->name, 30);
		eq->deity = emu->deity;
		if ((emu->race == 42 || emu->race == 120) && emu->gender == 2)
			eq->size = emu->size + 4.0f;
		else
			eq->size = emu->size;
		eq->NPC = emu->NPC;

		eq->cur_hp = emu->curHp;
		eq->x_pos = (int16)emu->x;
		eq->y_pos = (int16)emu->y;
		eq->z_pos = (int16)emu->z * 10;
		eq->deltaY = 0;
		eq->deltaX = 0;
		eq->heading = (uint8)emu->heading;
		eq->deltaZ = 0;
		eq->level = emu->level;
		eq->petOwnerId = emu->petOwnerId;
		eq->guildrank = emu->guildrank;
		eq->bodytexture = emu->bodytexture;
		for (int k = 0; k < 9; k++)
		{
			eq->equipment[k] = (uint8)emu->equipment[k];
			eq->equipcolors[k].color = emu->colors.Slot[k].Color;
		}
		eq->runspeed = emu->runspeed;
		eq->LD = 0;					// 0=NotLD, 1=LD
		eq->GuildID = emu->guildID;
		if (emu->NPC == 1)
		{
			eq->guildrank = 0;
			eq->LD = 1;
			eq->GuildID = 0XFFFF;
		}
		if (eq->GuildID == 0)
			eq->GuildID = 0xFFFF;
		eq->helm = emu->helm;
		eq->face = emu->face;
		eq->gender = emu->gender;
		eq->bodytype = emu->bodytype;

		if (emu->race >= 209 && emu->race <= 212)
		{
			eq->race = 75;
			if (emu->race == 210)
				eq->bodytexture = 3;
			else if (emu->race == 211)
				eq->bodytexture = 2;
			else if (emu->race == 212)
				eq->bodytexture = 1;
			else
				eq->bodytexture = 0;
		}
		else
			eq->race = (int8)emu->race;

		strn0cpy(eq->Surname, emu->lastName, 20);
		eq->walkspeed = emu->walkspeed;

		if (emu->class_ > 19 && emu->class_ < 35)
			eq->class_ = emu->class_ - 3;
		else if (emu->class_ == 40)
			eq->class_ = 16;
		else if (emu->class_ == 41)
			eq->class_ = 32;
		else
			eq->class_ = emu->class_;
		eq->anon = 0;
		eq->spawn_id = emu->spawnId;

		eq->invis = emu->invis;			// 0=visable, 1=invisable
		eq->sneaking = 0;
		eq->pvp = 0;
		eq->anim_type = emu->StandState;
		eq->light = emu->light;
		eq->anon = emu->anon;			// 0=normal, 1=anon, 2=RP
		eq->AFK = emu->afk;				// 0=off, 1=on
		eq->GM = emu->gm;

		eq->flymode = emu->flymode;
		return eq;
	}

	DECODE(OP_MBRetrievalRequest)
	{
		DECODE_LENGTH_EXACT(structs::MBRetrieveMessages_Struct);
		SETUP_DIRECT_DECODE(MBRetrieveMessages_Struct, structs::MBRetrieveMessages_Struct);
		emu->entityID = eq->entityID;
		emu->category = eq->category;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_MBRetrievalResponse)
	{
		ENCODE_LENGTH_EXACT(MBMessageRetrievalGen_Struct);
		SETUP_DIRECT_ENCODE(MBMessageRetrievalGen_Struct, structs::MBMessage_Struct);
		eq->id = emu->id;
		strn0cpy(eq->date, emu->date, sizeof(eq->date));
		strn0cpy(eq->author, emu->author, sizeof(eq->author));
		eq->language = emu->language;
		strn0cpy(eq->subject, emu->subject, sizeof(eq->subject));
		eq->category = emu->category;
		FINISH_ENCODE();
	}

	ENCODE(OP_MBRetrievalFin)
	{
		EQApplicationPacket *in = *p;
		*p = nullptr;
		auto outapp = new EQApplicationPacket(OP_MBRetrievalFin, 1);
		outapp->pBuffer[0] = 0;
		delete in;
		result->SetPacket(&outapp, reliable);
	}

	DECODE(OP_MBRetrievalDetailRequest)
	{
		DECODE_LENGTH_EXACT(structs::MBRetrieveMessage_Struct);
		SETUP_DIRECT_DECODE(MBModifyRequest_Struct, structs::MBRetrieveMessage_Struct);
		emu->EntityID = (uint16)eq->entityID;
		emu->id = (uint16)eq->id;
		emu->category = (uint16)eq->category;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_MBRetrievalDetailResponse)
	{
		EQApplicationPacket *in = *p;
		*p = nullptr;
		if (in->size > 0 && in->pBuffer) {
			auto outapp = new EQApplicationPacket(OP_MBRetrievalDetailResponse, 2048);
			memset(outapp->pBuffer, 0, 2048);
			uint32 to_copy = (in->size < 2048) ? in->size : 2048;
			memcpy(outapp->pBuffer, in->pBuffer, to_copy);
			delete in;
			result->SetPacket(&outapp, reliable);
		} else {
			auto outapp = new EQApplicationPacket(OP_MBRetrievalDetailResponse, 1);
			outapp->pBuffer[0] = 0;
			delete in;
			result->SetPacket(&outapp, reliable);
		}
	}

	DECODE(OP_MBRetrievalPostRequest)
	{
		constexpr size_t kHeaderSize = offsetof(structs::MBMessage_Struct, message);
		if (__packet->size < kHeaderSize) {
			__packet->SetOpcode(OP_Unknown);
			return;
		}

		uint32 incoming_size = __packet->size;
		unsigned char *__eq_buffer = __packet->pBuffer;
		structs::MBMessage_Struct *eq = (structs::MBMessage_Struct *)__eq_buffer;

		__packet->size = sizeof(MBMessageRetrievalGen_Struct);
		__packet->pBuffer = new unsigned char[__packet->size];
		memset(__packet->pBuffer, 0, __packet->size);
		MBMessageRetrievalGen_Struct *emu = (MBMessageRetrievalGen_Struct *)__packet->pBuffer;

		emu->id = eq->id;
		strn0cpy(emu->date, eq->date, sizeof(emu->date));
		strn0cpy(emu->author, eq->author, sizeof(emu->author));
		emu->language = eq->language;
		strn0cpy(emu->subject, eq->subject, sizeof(emu->subject));
		emu->category = eq->category;

		uint32 msg_len = incoming_size - kHeaderSize;
		if (msg_len > 0) {
			if (msg_len >= sizeof(emu->message))
				msg_len = sizeof(emu->message) - 1;
			memcpy(emu->message, eq->message, msg_len);
			emu->message[msg_len] = '\0';
		}

		delete[] __eq_buffer;
	}

	DECODE(OP_MBRetrievalEraseRequest)
	{
		DECODE_LENGTH_EXACT(structs::MBEraseMessage_Struct);
		SETUP_DIRECT_DECODE(MBEraseRequest_Struct, structs::MBEraseMessage_Struct);
		emu->EntityID = (uint16)eq->entityID;
		emu->id = (uint16)eq->id;
		emu->category = (uint16)eq->category;
		emu->unknown = 0;
		FINISH_DIRECT_DECODE();
	}

	ENCODE(OP_RaidJoin) { ENCODE_FORWARD(OP_Unknown); }
	ENCODE(OP_RaidUpdate) { ENCODE_FORWARD(OP_Unknown); }
	ENCODE(OP_RaidInvite) { ENCODE_FORWARD(OP_Unknown); }
	ENCODE(OP_SendAAStats) { ENCODE_FORWARD(OP_Unknown); }
	ENCODE(OP_Unknown)
	{
		EQApplicationPacket *in = *p;
		*p = nullptr;

		LogPacketClientServer("Dropped an invalid packet: {}", OpcodeManager::EmuToName(in->GetOpcode()));

		delete in;
		return;

	}

	static inline int16 ServerToTrilogySlot(uint32 ServerSlot)
	{
		//int16 TrilogySlot;
		if (ServerSlot == INVALID_INDEX)
			return INVALID_INDEX;

		return ServerSlot; // deprecated
	}

	static inline int16 ServerToTrilogyCorpseSlot(uint32 ServerCorpse)
	{
		return ServerCorpse;
	}

	static inline uint32 TrilogyToServerSlot(int16 TrilogySlot)
	{
		//uint32 ServerSlot;
		if (TrilogySlot == INVALID_INDEX)
			return INVALID_INDEX;

		return TrilogySlot; // deprecated
	}

	static inline uint32 TrilogyToServerCorpseSlot(int16 TrilogyCorpse)
	{
		return TrilogyCorpse;
	}

} //end namespace Trilogy
