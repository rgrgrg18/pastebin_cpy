#ifndef __SQL_ACTIONS_H__
#define __SQL_ACTIONS_H__

#include <pqxx/pqxx>


class sql_actions {
public:
	static void print_sql_tables(pqxx::work& txn);
public:
	static void prepare_add_user (pqxx::connection_base& conn);
	static void prepare_add_paste (pqxx::connection_base& conn);
	static void prepare_get_amazon_link (pqxx::connection_base& conn);
	static void prepare_get_sequence_for_hash (pqxx::connection_base& conn);
	static void prepare_check_login (pqxx::connection_base& conn);
	static void prepare_number_pastes (pqxx::connection_base& conn);
public:
	static void execute_add_user (pqxx::transaction_base& txn, const std::string& login);
	static void execute_add_paste (pqxx::transaction_base& txn, const std::string& login, 
								   const std::string& amazon_link, const std::string& hash_link);
	static std::string execute_get_amazon_link (pqxx::transaction_base& txn, const std::string& hash_link);
	static unsigned long long execute_get_sequence_for_hash (pqxx::transaction_base& txn);
	static int execute_number_pastes (pqxx::transaction_base& txn, const std::string& login);
public:
	static void new_paste (pqxx::dbtransaction& txn, const std::string& login, const std::string& amazon_link); 
};

#endif