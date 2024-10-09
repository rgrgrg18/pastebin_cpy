#pragma once

#include <pqxx/pqxx>

using Keys = std::pair<std::string, std::string>; // pair of public and private Keys
using PasteInfo = std::tuple<std::string, std::string, std::string, std::string, std::string>; // Private key, login, password, title, created at
using LastPastesInfo = std::vector<std::vector<std::string>>; // Last pastes with title, public key, created time

class SqlActions {
public:
	static void PrepareGetSequenceForPublicKey(	pqxx::connection_base& conn);
	static void PrepareGetSequenceForPrivateKey(pqxx::connection_base& conn);
	static void PrepareGetInfoPaste(pqxx::connection_base& conn);

	static void PrepareCheckLogin(pqxx::connection_base& conn);
	static void PrepareAddUser(pqxx::connection_base& conn);
	static void PrepareAddPaste(pqxx::connection_base& conn);

	static void PrepareReturnAmountPastes(pqxx::connection_base& conn);
	static void PrepareIncreaseAmountPastes(pqxx::connection_base& conn);
	static void PrepareDecreaseAmountPastes(pqxx::connection_base& conn);
	static void PrepareChangePasswordPaste(pqxx::connection_base& conn);
	static void PrepareChangeTitlePaste(pqxx::connection_base& conn);
	static void PrepareDeletePaste(pqxx::connection_base& conn);
	static void PrepareGetLastUserPastes(pqxx::connection_base& conn);
public:
	static uint64_t ExecuteGetSequenceForPublicKey(pqxx::transaction_base& txn);
	static uint64_t ExecuteGetSequenceForPrivateKey(pqxx::transaction_base& txn);
	static PasteInfo ExecuteGetInfoPaste(pqxx::transaction_base& txn, const std::string& public_key);
	
	static void ExecuteAddUser(pqxx::transaction_base& txn, uint64_t login);
	static void ExecuteAddPaste(pqxx::transaction_base& txn, uint64_t login, 
                                const std::string& public_key, const std::string& private_key, 
								const std::string& password);
	
	static Keys NewPaste(pqxx::dbtransaction& txn, uint64_t login, const std::string& password = "");
	
	static uint16_t ExecuteReturnAmountPastes(pqxx::transaction_base& txn, uint64_t login);
	static void ExecuteIncreaseAmountPastes(pqxx::transaction_base& txn, uint64_t login);
	static void ExecuteChangePasswordPaste(pqxx::transaction_base& txn, 
                                           const std::string& password, 
										   const std::string& public_key);
	static void ExecuteChangeTitlePaste(pqxx::transaction_base& txn, 
										const std::string& title, 
										const std::string& public_key);
	static void ExecuteDeletePaste(pqxx::transaction_base& txn, 
								   const std::string& public_key,
								   uint64_t login);
	static void ExecuteDecreaseAmountPastes(pqxx::transaction_base& txn, uint64_t login);
	static LastPastesInfo ExecuteGetLastUserPastes(pqxx::transaction_base& txn, 
	                                               uint64_t login, 
												   uint16_t limit);
};