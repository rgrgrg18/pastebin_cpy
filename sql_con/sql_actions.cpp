#include "sql_actions.h"
#include <iostream>
#include <iomanip>


void sql_actions::prepare_get_sequence_for_hash (pqxx::connection_base& conn) {
	conn.prepare (
		"get_sequence_for_hash",
		"SELECT nextval('hash_generate');");
}

void sql_actions::prepare_get_amazon_link (pqxx::connection_base& conn) {
	conn.prepare (
		"get_amazon_link",
		"SELECT amazon_link FROM pastes WHERE hash = $1");
}

void sql_actions::prepare_add_user (pqxx::connection_base& conn) {
	conn.prepare(
		"add_user",
		"INSERT INTO users (id, login) VALUES (nextval('seq_person'), $1)");
}

void sql_actions::prepare_add_paste (pqxx::connection_base& conn) {
	conn.prepare(
		"add_paste",
		"INSERT INTO pastes (hash, amazon_link, user_id) \
		VALUES ($1, $2, (SELECT id FROM users WHERE login = $3))");
	
}

unsigned long long sql_actions::execute_get_sequence_for_hash (pqxx::transaction_base& txn) {
	unsigned long long sequence;
	return txn.exec_prepared("get_sequence_for_hash")[0][0].as(sequence);
}

std::string sql_actions::execute_get_amazon_link (pqxx::transaction_base& txn, const std::string& hash_link) {
	return (txn.exec_prepared("get_amazon_link", hash_link))[0][0].c_str();
}

void sql_actions::prepare_check_login (pqxx::connection_base& conn) {
	conn.prepare(
		"check_login",
		"SELECT id FROM users WHERE login = $1");
}

void sql_actions::execute_add_user (pqxx::transaction_base& txn, const std::string& login) {
	if (txn.exec_prepared("check_login", login).size() == 0)
		txn.exec_prepared0("add_user", login);
}

void sql_actions::execute_add_paste (pqxx::transaction_base& txn, 
						const std::string& login, 
						const std::string& amazon_link, 
						const std::string& hash_link) {
	txn.exec_prepared0("add_paste", hash_link, amazon_link, login);
}

void sql_actions::prepare_number_pastes (pqxx::connection_base& conn) {
	conn.prepare(
		"number_pastes",
		"SELECT COUNT(*) FROM pastes WHERE user_id = (SELECT id FROM users WHERE login = $1)");
}

int sql_actions::execute_number_pastes (pqxx::transaction_base& txn, const std::string& login) {
	int count;
	return txn.exec_prepared("number_pastes", login)[0][0].as(count);
}

void sql_actions::prepare_all_pastes (pqxx::connection_base& conn) {
	conn.prepare(
		"all_pastes",
		"COPY (SELECT * FROM pastes WHERE user_id = (SELECT id FROM users WHERE login = 'kirill')) TO '/var/lib/postgresql/test1.txt'");
}

void sql_actions::execute_all_pastes (pqxx::transaction_base& txn, const std::string& login) {
	txn.exec_prepared("all_pastes");
}		

void sql_actions::print_sql_tables (pqxx::work& txn) {
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