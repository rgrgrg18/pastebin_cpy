#include "sql_actions.hpp"

#include <iostream>
#include <iomanip>

#include "hash_generate/HashGenerator.hpp"


// Work with sequences public and private Keys
/**
 * @brief Prepares a query that returns the next value in the sequence public key
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareGetSequenceForPublicKey (pqxx::connection_base& conn) {
	conn.prepare (
		"get_sequence_for_public_key",
		"SELECT nextval('public_key_sq');");
}

/**
 * @brief Prepares a query that returns the next value in the sequence private key
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareGetSequenceForPrivateKey (pqxx::connection_base& conn) {
	conn.prepare (
		"get_sequence_for_private_key",
		"SELECT nextval('private_key_sq');");
}

/**
 * @brief Returns the next value in the sequence public key
 * 
 * @param txn Reference to current transaction.
 * @return uint64_t Next value in the sequence
 */
uint64_t SqlActions::ExecuteGetSequenceForPublicKey (pqxx::transaction_base& txn) {
	uint64_t sequence = 0;
	return txn.exec_prepared("get_sequence_for_public_key")[0][0].as(sequence);
}

/**
 * @brief Returns the next value in the sequence private key
 * 
 * @param txn Reference to current transaction.
 * @return uint64_t Next value in the sequence
 */
uint64_t SqlActions::ExecuteGetSequenceForPrivateKey (pqxx::transaction_base& txn) {
	uint64_t sequence = 0;
	return txn.exec_prepared("get_sequence_for_private_key")[0][0].as(sequence);
}

/**
 * @brief Prepares a query that returns private key, login, password and title of the paste by public key
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareGetInfoPaste (pqxx::connection_base& conn) {
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
 * @return PasteInfo Private key, login, password, title of the paste
 */
PasteInfo SqlActions::ExecuteGetInfoPaste (pqxx::transaction_base& txn, const std::string& public_key) {
	pqxx::result pr_key = txn.exec_prepared("get_info_about_paste", public_key);
	if (pr_key.empty()) {
		return {"", "", "", "", ""};
	}
	return {pr_key[0][0].c_str(), pr_key[0][1].c_str(), pr_key[0][2].c_str(), pr_key[0][3].c_str(), pr_key[0][4].c_str()};
} 

// Work with adding users and pastes
/**
 * @brief Prepares check if login exist in users table
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareCheckLogin (pqxx::connection_base& conn) {
	conn.prepare(
		"check_login",
		"SELECT id FROM users WHERE login = $1");
}

/**
 * @brief Prepares add user login in table users
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareAddUser (pqxx::connection_base& conn) {
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
void SqlActions::ExecuteAddUser (pqxx::transaction_base& txn, uint64_t login) {
	if (txn.exec_prepared("check_login", login).empty()) {
		txn.exec_prepared0("add_user", login);
	}
}

/**
 * @brief Prepares add row into table pastes
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareAddPaste (pqxx::connection_base& conn) {
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
void SqlActions::ExecuteAddPaste (pqxx::transaction_base& txn, 
									 uint64_t login, 
									 const std::string& public_key, 
									 const std::string& private_key,
									 const std::string& password) {
	txn.exec_prepared0("add_paste", public_key, private_key, login, password);
}

/**
 * @brief Creates public and private Keys for paste and adds all info in tables pastes, users.
 * 
 * @exception Throws exception if connection was broken, transaction aborted or the request failed.
 * If it will throws nothing will be created. (Strong exception guarantee)
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @return Keys The pair of public key and private key
 */
Keys SqlActions::NewPaste (pqxx::dbtransaction& txn, uint64_t login, const std::string& password) {
	uint64_t seq_pb_key = SqlActions::ExecuteGetSequenceForPublicKey(txn);
	uint64_t seq_pr_key = SqlActions::ExecuteGetSequenceForPrivateKey(txn);
	paste_hash::Base64 hash_seq_public_key(seq_pb_key);
	paste_hash::Base64 hash_seq_private_key(seq_pr_key);

	pqxx::subtransaction txn1(txn, "insert");

	SqlActions::ExecuteAddUser(txn1, login);
		
	SqlActions::ExecuteAddPaste(txn1, login, hash_seq_public_key.hash_, hash_seq_private_key.hash_, password);
	
	SqlActions::ExecuteIncreaseAmountPastes(txn1, login);
	
	txn1.commit();

	return {hash_seq_public_key.hash_, hash_seq_private_key.hash_};
}

// Functions for features

/**
 * @brief Prepares increase the number of pastes by 1
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareIncreaseAmountPastes (pqxx::connection_base& conn) {
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
void SqlActions::ExecuteIncreaseAmountPastes (pqxx::transaction_base& txn, uint64_t login) {
	txn.exec_prepared0("increase_amount_pastes", login);
}

/**
 * @brief Prepares decrease the number of pastes by 1
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareDecreaseAmountPastes (pqxx::connection_base& conn) {
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
void SqlActions::ExecuteDecreaseAmountPastes (pqxx::transaction_base& txn, uint64_t login) {
	txn.exec_prepared0("decrease_amount_pastes", login);
}

/**
 * @brief Prepares return amount pastes of user
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareReturnAmountPastes (pqxx::connection_base& conn) {
	conn.prepare (
		"return_amount_pastes",
		"SELECT amount_pastes FROM users WHERE login = $1");
}

/**
 * @brief Returns amount pastes of user
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @return uint16_t Amount pastes of user
 */
uint16_t SqlActions::ExecuteReturnAmountPastes (pqxx::transaction_base& txn, uint64_t login) {
	pqxx::result amount_pastes = txn.exec_prepared("return_amount_pastes", login);
	uint16_t example = 0;
	return amount_pastes.empty() ? 0 : amount_pastes[0][0].as(example);
}

/**
 * @brief Prepares change password of the paste
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareChangePasswordPaste (pqxx::connection_base& conn) {
	conn.prepare (
		"ChangePassword",
		"UPDATE pastes SET password = $1 WHERE public_key = $2");
}

/**
 * @brief Changes password of the paste
 * 
 * @param txn Reference to current transaction.
 * @param password New password
 * @param public_key Key with which people receive the file
 */
void SqlActions::ExecuteChangePasswordPaste (pqxx::transaction_base& txn, 
												 const std::string& password, 
												 const std::string& public_key) {
	txn.exec_prepared0("ChangePassword", password, public_key);
}

/**
 * @brief Prepares change title of the paste
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareChangeTitlePaste (pqxx::connection_base& conn) {
	conn.prepare (
		"ChangeTitle",
		"UPDATE pastes SET title = $1 WHERE public_key = $2");
}

/**
 * @brief Changes title of the paste
 * 
 * @param txn Reference to current transaction.
 * @param title New title
 * @param public_key Key with which people receive the file
 */
void SqlActions::ExecuteChangeTitlePaste (pqxx::transaction_base& txn, 
												 const std::string& title, 
												 const std::string& public_key) {
	txn.exec_prepared0("ChangeTitle", title, public_key);
}

/**
 * @brief Prepares delete paste by public key
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareDeletePaste (pqxx::connection_base& conn) {
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
void SqlActions::ExecuteDeletePaste (pqxx::transaction_base& txn, 
									    const std::string& public_key,
										uint64_t login) {
	txn.exec_prepared0("delete_paste", public_key);
	ExecuteDecreaseAmountPastes(txn, login);
}

/**
 * @brief Prepares return last pastes of user
 * 
 * @param conn Reference to current connection
 */
void SqlActions::PrepareGetLastUserPastes (pqxx::connection_base& conn) {
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
LastPastesInfo SqlActions::ExecuteGetLastUserPastes (pqxx::transaction_base& txn, uint64_t login, uint16_t limit) {
	pqxx::result res = txn.exec_prepared("get_user_pastes", login, limit);
	uint8_t amount_columns = res.columns();
	uint16_t amount_lines = res.size();
	LastPastesInfo user_pastes(amount_lines, std::vector<std::string>(amount_columns, ""));
	if (!res.empty()) {
		for (uint16_t line = 0; line < amount_lines; ++line) {
			for (uint8_t column = 0; column < amount_columns; ++column) {
				user_pastes[line][column] = std::move(res[line][column].c_str());
			}
		}
	}
	
	return user_pastes;
}