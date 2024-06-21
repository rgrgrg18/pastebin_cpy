#ifndef __SQL_ACTIONS_H__
#define __SQL_ACTIONS_H__

#include <pqxx/pqxx>

using keys = std::pair<std::string, std::string>; // pair of public and private keys

class sql_actions {
public:
	static void prepare_get_sequence_for_public_key (pqxx::connection_base& conn);
	static void prepare_get_sequence_for_private_key (pqxx::connection_base& conn);
	static void prepare_get_private_key (pqxx::connection_base& conn);

	static void prepare_check_login (pqxx::connection_base& conn);
	static void prepare_add_user (pqxx::connection_base& conn);
	static void prepare_add_paste (pqxx::connection_base& conn);
public:
	static unsigned long long execute_get_sequence_for_public_key (pqxx::transaction_base& txn);
	static unsigned long long execute_get_sequence_for_private_key (pqxx::transaction_base& txn);
	static std::string execute_get_private_key (pqxx::transaction_base& txn, const std::string& public_key);

	static void execute_add_user (pqxx::transaction_base& txn, int64_t login);
	static void execute_add_paste (pqxx::transaction_base& txn, int64_t login, 
								   const std::string& public_key, const std::string& private_key);
	static keys new_paste (pqxx::dbtransaction& txn, int64_t login); 
};

#endif