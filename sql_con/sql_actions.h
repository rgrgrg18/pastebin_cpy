#ifndef __SQL_ACTIONS_H__
#define __SQL_ACTIONS_H__

#include <pqxx/pqxx>

class sql_actions {
public:
	static void add_user (pqxx::work& txn, const std::string& login);
	static void add_paste (pqxx::work& txn, const std::string& login, const std::string& amazon_link, const std::string& hash_link);
	static std::string get_amazon_link (pqxx::work& txn, const std::string& hash);
	static unsigned long long get_sequence_for_hash (pqxx::work& txn);
	static void print_sql_tables(pqxx::work& txn);
};


#endif