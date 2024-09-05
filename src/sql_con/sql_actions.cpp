#include "sql_actions.hpp"

#include <iostream>
#include <iomanip>

#include "hash_generate/HashGenerator.hpp"


// Work with sequences public and private keys
/**
 * @brief Prepares a query that returns the next value in the sequence public key
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_sequence_for_public_key (pqxx::connection_base& conn) {
	conn.prepare (
		"get_sequence_for_public_key",
		"SELECT nextval('public_key_sq');");
}

/**
 * @brief Prepares a query that returns the next value in the sequence private key
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_sequence_for_private_key (pqxx::connection_base& conn) {
	conn.prepare (
		"get_sequence_for_private_key",
		"SELECT nextval('private_key_sq');");
}

/**
 * @brief Returns the next value in the sequence public key
 * 
 * @param txn Reference to current transaction.
 * @return unsigned long long Next value in the sequence
 */
unsigned long long sql_actions::execute_get_sequence_for_public_key (pqxx::transaction_base& txn) {
	unsigned long long sequence = 0;
	return txn.exec_prepared("get_sequence_for_public_key")[0][0].as(sequence);
}

/**
 * @brief Returns the next value in the sequence private key
 * 
 * @param txn Reference to current transaction.
 * @return unsigned long long Next value in the sequence
 */
unsigned long long sql_actions::execute_get_sequence_for_private_key (pqxx::transaction_base& txn) {
	unsigned long long sequence = 0;
	return txn.exec_prepared("get_sequence_for_private_key")[0][0].as(sequence);
}

/**
 * @brief Prepares a query that returns private key, login, password and title of the paste by public key
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_info_paste (pqxx::connection_base& conn) {
	conn.prepare (
		"get_info_about_paste",
		"SELECT private_key, login, password, title, created_at FROM pastes WHERE public_key = $1");
}

/**
 * @brief Returns private key, login, password and title of the paste by public key
 * 
 * If paste does not exist in the table, then the private key is empty.
 * 
 * @param txn Reference to current transaction.
 * @param public_key Public key for this paste
 * @return paste_info Private key, login, password, title of the paste
 */
paste_info sql_actions::execute_get_info_paste (pqxx::transaction_base& txn, const std::string& public_key) {
	pqxx::result pr_key = txn.exec_prepared("get_info_about_paste", public_key);
	if (pr_key.empty())
		return {"", "", "", "", ""};
	return {pr_key[0][0].c_str(), pr_key[0][1].c_str(), pr_key[0][2].c_str(), pr_key[0][3].c_str(), pr_key[0][4].c_str()};
} 

// Work with adding users and pastes
/**
 * @brief Prepares check if login exist in users table
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_check_login (pqxx::connection_base& conn) {
	conn.prepare(
		"check_login",
		"SELECT id FROM users WHERE login = $1");
}

/**
 * @brief Prepares add user login in table users
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_add_user (pqxx::connection_base& conn) {
	conn.prepare(
		"add_user",
		"INSERT INTO users (login) VALUES ($1)");
}

/**
 * @brief Adds user login in table users if it doesn't exist there
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * 
 * If user login already exist in the table, function won't do anything
 */
void sql_actions::execute_add_user (pqxx::transaction_base& txn, int64_t login) {
	if (txn.exec_prepared("check_login", login).size() == 0)
		txn.exec_prepared0("add_user", login);
}

/**
 * @brief Prepares add row into table pastes
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_add_paste (pqxx::connection_base& conn) {
	conn.prepare(
		"add_paste",
		"INSERT INTO pastes (public_key, private_key, login, password) \
		VALUES ($1, $2, $3, $4)");
}

/**
 * @brief Adds row into table pastes
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @param public_key Key with which people receive the file
 * @param private_key Name of file in yandex cloud
 */
void sql_actions::execute_add_paste (pqxx::transaction_base& txn, 
									 int64_t login, 
									 const std::string& public_key, 
									 const std::string& private_key,
									 const std::string& password) {
	txn.exec_prepared0("add_paste", public_key, private_key, login, password);
}

/**
 * @brief Creates public and private keys for paste and adds all info in tables pastes, users.
 * 
 * @exception Throws exception if connection was broken, transaction aborted or the request failed.
 * If it will throws nothing will be created. (Strong exception guarantee)
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @return keys The pair of public key and private key
 */
keys sql_actions::new_paste (pqxx::dbtransaction& txn, int64_t login, const std::string& password) {
	unsigned long long seq_pb_key = sql_actions::execute_get_sequence_for_public_key(txn);
	unsigned long long seq_pr_key = sql_actions::execute_get_sequence_for_private_key(txn);
	paste_hash::Base64 hash_seq_public_key(seq_pb_key);
	paste_hash::Base64 hash_seq_private_key(seq_pr_key);

	pqxx::subtransaction txn1(txn, "insert");

	sql_actions::execute_add_user(txn1, login);
		
	sql_actions::execute_add_paste(txn1, login, hash_seq_public_key.hash, hash_seq_private_key.hash, password);
	
	sql_actions::execute_increase_amount_pastes(txn1, login);
	
	txn1.commit();

	return {hash_seq_public_key.hash, hash_seq_private_key.hash};
}

// Functions for features

/**
 * @brief Prepares increase the number of pastes by 1
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_increase_amount_pastes (pqxx::connection_base& conn) {
	conn.prepare (
		"increase_amount_pastes",
		"UPDATE users SET amount_pastes = amount_pastes + 1 WHERE login = $1");
}

/**
 * @brief Increases the number of pastes by 1
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_increase_amount_pastes (pqxx::transaction_base& txn, int64_t login) {
	txn.exec_prepared0("increase_amount_pastes", login);
}

/**
 * @brief Prepares decrease the number of pastes by 1
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_decrease_amount_pastes (pqxx::connection_base& conn) {
	conn.prepare (
		"decrease_amount_pastes",
		"UPDATE users SET amount_pastes = amount_pastes - 1 WHERE login = $1");
}

/**
 * @brief Decreases the number of pastes by 1
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_decrease_amount_pastes (pqxx::transaction_base& txn, int64_t login) {
	txn.exec_prepared0("decrease_amount_pastes", login);
}

/**
 * @brief Prepares return amount pastes of user
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_return_amount_pastes (pqxx::connection_base& conn) {
	conn.prepare (
		"return_amount_pastes",
		"SELECT amount_pastes FROM users WHERE login = $1");
}

/**
 * @brief Returns amount pastes of user
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @return size_t Amount pastes of user
 */
size_t sql_actions::execute_return_amount_pastes (pqxx::transaction_base& txn, int64_t login) {
	pqxx::result amount_pastes = txn.exec_prepared("return_amount_pastes", login);
	size_t example = 0;
	return amount_pastes.empty() ? 0 : amount_pastes[0][0].as(example);
}

/**
 * @brief Prepares change password of the paste
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_change_password_paste (pqxx::connection_base& conn) {
	conn.prepare (
		"change_password",
		"UPDATE pastes SET password = $1 WHERE public_key = $2");
}

/**
 * @brief Changes password of the paste
 * 
 * @param txn Reference to current transaction.
 * @param password New password
 * @param public_key Key with which people receive the file
 */
void sql_actions::execute_change_password_paste (pqxx::transaction_base& txn, 
												 const std::string& password, 
												 const std::string& public_key) {
	txn.exec_prepared0("change_password", password, public_key);
}

/**
 * @brief Prepares change title of the paste
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_change_title_paste (pqxx::connection_base& conn) {
	conn.prepare (
		"change_title",
		"UPDATE pastes SET title = $1 WHERE public_key = $2");
}

/**
 * @brief Changes title of the paste
 * 
 * @param txn Reference to current transaction.
 * @param title New title
 * @param public_key Key with which people receive the file
 */
void sql_actions::execute_change_title_paste (pqxx::transaction_base& txn, 
												 const std::string& title, 
												 const std::string& public_key) {
	txn.exec_prepared0("change_title", title, public_key);
}

/**
 * @brief Prepares delete paste by public key
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_delete_paste (pqxx::connection_base& conn) {
	conn.prepare (
		"delete_paste",
		"DELETE from pastes WHERE public_key = $1");
}

/**
 * @brief Deletes paste by public key
 * 
 * @param txn Reference to current transaction.
 * @param public_key Key with which people receive the file
 * @param login Id telegram chat 
 */
void sql_actions::execute_delete_paste (pqxx::transaction_base& txn, 
									    const std::string& public_key,
										int64_t login) {
	txn.exec_prepared0("delete_paste", public_key);
	execute_decrease_amount_pastes(txn, login);
}

/**
 * @brief Prepares return last pastes of user
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_last_user_pastes (pqxx::connection_base& conn) {
	conn.prepare(
		"get_user_pastes",
		"SELECT title, public_key, created_at FROM pastes WHERE login = $1 \
		ORDER BY created_at DESC  \
		LIMIT $2");
}

/**
 * @brief Returns last pastes of user
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @param limit Maximum number of output pastes
 * @return std::vector<std::vector<std::string>> Public key, title and created at of last pastes
 */
last_pastes_info sql_actions::execute_get_last_user_pastes (pqxx::transaction_base& txn, int64_t login, int64_t limit) {
	pqxx::result res = txn.exec_prepared("get_user_pastes", login, limit);
	size_t amount_columns = res.columns();
	size_t amount_lines = res.size();
	last_pastes_info user_pastes(amount_lines, std::vector<std::string>(amount_columns, ""));
	if (!res.empty()) {
		for (size_t line = 0; line < amount_lines; ++line) {
			for (int column = 0; column < amount_columns; ++column) {
				user_pastes[line][column] = std::move(res[line][column].c_str());
			}
		}
	}
	
	return user_pastes;
}