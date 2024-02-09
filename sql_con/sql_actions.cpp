#include "sql_actions.h"
#include <iostream>
#include <iomanip>

unsigned long long sql_actions::get_sequence_for_hash (pqxx::work& txn) {
	pqxx::result res(txn.exec("SELECT nextval('hash_generate');"));
	int hash_s = 0;
	res[0][0].to(hash_s);

	txn.commit();

	return hash_s;
}

std::string sql_actions::get_amazon_link (pqxx::work& txn, const std::string& hash) {
	std::string get_link = "SELECT amazon_link FROM pastes WHERE hash = '";
	get_link += hash;
	get_link.push_back('\'');

	pqxx::result res(txn.exec(get_link.c_str()));

	return res[0][0].c_str();
}

void sql_actions::add_user (pqxx::work& txn, const std::string& login) {
	std::string insert_users = "INSERT INTO users (id, login) VALUES (nextval('seq_person'), '";
	insert_users += login;

	insert_users.push_back('\'');
	insert_users.push_back(')');

	txn.exec0(insert_users.c_str());

	txn.commit();
}

void sql_actions::add_paste (pqxx::work& txn, const std::string& login, const std::string& amazon_link, const std::string& hash_link) {

	std::string insert_paste = "INSERT INTO pastes (hash, amazon_link, user_id) VALUES ('";

	insert_paste += hash_link;
	insert_paste += "', '";
	insert_paste += amazon_link;

	std::string user_id = "', (SELECT id FROM users WHERE login = '";
	user_id += login;
	user_id.push_back('\'');
	user_id.push_back(')');
	user_id.push_back(')');

	insert_paste += user_id;

	txn.exec0(insert_paste.c_str());

	txn.commit();
}

void sql_actions::print_sql_tables(pqxx::work& txn) {
	pqxx::result res(txn.exec("SELECT * FROM users"));

	if (!res.empty()) {
		int totCol = res.columns();
		std::cout << "-------------------------------------------------------------------" << std::endl;

		std::array<int, 2> wCol = {28, 28};

		std::cout << "|";
		for (int i = 0; i < totCol; ++i) {
			std::cout << std::setfill(' ') << std::setw(wCol[i]) << std::left << res.column_name(i) << "|";
		}

		std::cout << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		for (auto row : res) {
			std::cout << "|";
			for (int i = 0; i < totCol; ++i) {
				std::cout << std::setfill(' ') << std::setw(wCol[i]) << std::left << row[i].c_str() << "|";
			}
			std::cout << std::endl;
		}

		std::cout << "-------------------------------------------------------------------" << std::endl;
	}

	std::cout << std::endl;

	pqxx::result resu(txn.exec("SELECT * FROM pastes"));

	if (!resu.empty()) {
		int totColu = resu.columns();
		std::cout << "-------------------------------------------------------------------" << std::endl;

		std::array<int, 4> wCol = {10, 15, 26, 5};

		std::cout << "|";
		for (int i = 0; i < totColu; ++i) {
			std::cout << std::setfill(' ') << std::setw(wCol[i]) << std::left << resu.column_name(i) << "|";
		}

		std::cout << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		for (const auto& row : resu) {
			std::cout << "|";
			for (int i = 0; i < totColu; ++i) {
				std::cout << std::setfill(' ') << std::setw(wCol[i]) << std::left << row[i].c_str() << "|";
			}
			std::cout << std::endl;
		}

		std::cout << "-------------------------------------------------------------------" << std::endl;
	}
}