/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

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
#ifndef EQEMU_OPTIONS_H
#define EQEMU_OPTIONS_H

/**
 * Collects options on one object, because having a bunch of global variables floating around is
 * really ugly and just a little dangerous.
 */
class Options {
public:
	Options() :
		m_allow_unregistered(true),
		m_encryption_mode(5),
		m_local_network("127.0.0.1"),
		m_network_ip("127.0.0.1"),
		m_reject_duplicate_servers(false),
		m_allow_password_login(true),
		m_allow_token_login(false),
		m_auto_create_accounts(false) { }

	inline void AllowUnregistered(bool b) { m_allow_unregistered = b; }
	inline bool IsUnregisteredAllowed() const { return m_allow_unregistered; }
	inline void EncryptionMode(int m) { m_encryption_mode = m; }
	inline int GetEncryptionMode() const { return m_encryption_mode; }
	inline void LocalNetwork(std::string n) { m_local_network = n; }
	inline void NetworkIP(std::string n) { m_network_ip = n; }
	inline std::string GetLocalNetwork() const { return m_local_network; }
	inline std::string GetNetworkIP() const { return m_network_ip; }
	inline void AccountTable(std::string t) { m_account_table = t; }
	inline std::string GetAccountTable() const { return m_account_table; }
	inline void WorldRegistrationTable(std::string t) { m_world_registration_table = t; }
	inline std::string GetWorldRegistrationTable() const { return m_world_registration_table; }
	inline void WorldAdminRegistrationTable(std::string t) { m_world_admin_registration_table = t; }
	inline std::string GetWorldAdminRegistrationTable() const { return m_world_admin_registration_table; }
	inline void WorldServerTypeTable(std::string t) { m_world_server_type_table = t; }
	inline std::string GetWorldServerTypeTable() const { return m_world_server_type_table; }
	inline void LoginPasswordSalt(std::string t) { m_login_password_salt = t; }
	inline std::string GetLoginPasswordSalt() const { return m_login_password_salt; }
	inline void RejectDuplicateServers(bool b) { m_reject_duplicate_servers = b; }
	inline bool IsRejectingDuplicateServers() { return m_reject_duplicate_servers; }
	inline void AllowTokenLogin(bool b) { m_allow_token_login = b; }
	inline bool IsTokenLoginAllowed() const { return m_allow_token_login; }
	inline void AllowPasswordLogin(bool b) { m_allow_password_login = b; }
	inline bool IsPasswordLoginAllowed() const { return m_allow_password_login; }
	inline void AutoCreateAccounts(bool b) { m_auto_create_accounts = b; }
	inline bool AllowPcClient(bool b) { return m_allow_pc_client = b; }
	inline bool IsPcClientAllowed() const { return m_allow_pc_client; }
	inline bool AllowIntelClient(bool b) { return m_allow_intel_client = b; }
	inline bool IsIntelClientAllowed() const { return m_allow_intel_client; }
	inline bool AllowTicketClient(bool b) { return m_allow_ticket_client = b; }
	inline bool IsTicketClientAllowed() const { return m_allow_ticket_client; }
	inline bool CanAutoCreateAccounts() const { return m_auto_create_accounts; }
	inline bool IsShowPlayerCountEnabled() const { return m_show_player_count; }
	inline void SetShowPlayerCount(bool show_player_count) { m_show_player_count = show_player_count; }
	inline std::string GetBannerTicker() const { return m_banner_ticker; }
	inline void BannerTicker(std::string t) { m_banner_ticker = t; }

private:
	bool        m_allow_unregistered;
	bool        m_reject_duplicate_servers;
	bool        m_allow_token_login;
	bool        m_allow_password_login;
	bool        m_auto_create_accounts;
	bool        m_show_player_count;
	bool        m_allow_pc_client;
	bool        m_allow_intel_client;
	bool        m_allow_ticket_client;
	int         m_encryption_mode;
	std::string m_local_network;
	std::string m_network_ip;
	std::string m_account_table;
	std::string m_world_registration_table;
	std::string m_world_admin_registration_table;
	std::string m_world_server_type_table;
	std::string m_login_password_salt;
	std::string m_banner_ticker;
};

#endif

