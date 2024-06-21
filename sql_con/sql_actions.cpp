#include "sql_actions.h"
#include <iostream>
#include <iomanip>
#include "../hash_generate/HashGenerator.h"

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
	unsigned long long sequence;
	return txn.exec_prepared("get_sequence_for_public_key")[0][0].as(sequence);
}

/**
 * @brief Returns the next value in the sequence private key
 * 
 * @param txn Reference to current transaction.
 * @return unsigned long long Next value in the sequence
 */
unsigned long long sql_actions::execute_get_sequence_for_private_key (pqxx::transaction_base& txn) {
	unsigned long long sequence;
	return txn.exec_prepared("get_sequence_for_private_key")[0][0].as(sequence);
}

/**
 * @brief Prepares a query that returns private key by public key
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_private_key (pqxx::connection_base& conn) {
	conn.prepare (
		"get_private_key",
		"SELECT private_key FROM pastes WHERE public_key = $1");
}

/**
 * @brief Returns private key of the paste by public key
 * 
 * @param txn Reference to current transaction.
 * @param public_key Public key for this paste
 * @return std::string Private key
 */
std::string sql_actions::execute_get_private_key (pqxx::transaction_base& txn, const std::string& public_key) {
	pqxx::result pr_key = txn.exec_prepared("get_private_key", public_key);
	return pr_key.empty() ? "" : pr_key[0][0].c_str();
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
		"INSERT INTO pastes (public_key, private_key, login) \
		VALUES ($1, $2, $3)");
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
						const std::string& private_key) {
	txn.exec_prepared0("add_paste", public_key, private_key, login);
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
keys sql_actions::new_paste (pqxx::dbtransaction& txn, int64_t login) {
	unsigned long long seq_pb_key = sql_actions::execute_get_sequence_for_public_key(txn);
	unsigned long long seq_pr_key = sql_actions::execute_get_sequence_for_private_key(txn);
	paste_hash::Base64 hash_seq_public_key(seq_pb_key);
	paste_hash::Base64 hash_seq_private_key(seq_pr_key);

	pqxx::subtransaction txn1(txn, "insert");

	sql_actions::execute_add_user(txn1, login);
		
	sql_actions::execute_add_paste(txn1, login, hash_seq_public_key.hash, hash_seq_private_key.hash);
	txn1.commit();

	return {hash_seq_public_key.hash, hash_seq_private_key.hash};
}

// Work with users states
/**
 * @brief Prepares check if login exist in user_state table
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_check_state (pqxx::connection_base& conn) {
	conn.prepare(
		"check_state",
		"SELECT new_paste FROM user_state WHERE login = $1");
}

/**
 * @brief Prepares add user login in table users_state
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_add_user_state (pqxx::connection_base& conn) {
	conn.prepare (
		"add_user_state",
		"INSERT INTO user_state (login, new_paste, watch_paste) VALUES ($1, false, false)");
}

/**
 * @brief Adds user login in table users_state if it doesn't exist there
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_add_user_state (pqxx::transaction_base& txn, int64_t login) {
	if (txn.exec_prepared("check_state", login).size() == 0)
		txn.exec_prepared0("add_user_state", login);
}

/**
 * @brief Prepares a query that returns current user state
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_get_user_state (pqxx::connection_base& conn) {
	conn.prepare (
		"get_user_state",
		"SELECT new_paste, watch_paste FROM user_state WHERE login = $1");
}

/**
 * @brief Return current user state
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 * @return user_state Pair of new_paste and watch_paste flags
 */
user_state sql_actions::execute_get_user_state (pqxx::transaction_base& txn, int64_t login) {
	auto result = txn.exec_prepared("get_user_state", login);
	bool boolean_type;
	return {result[0][0].as(boolean_type), result[0][1].as(boolean_type)};
}

/**
 * @brief Prepares change flag new paste in users_state table to true
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_set_flag_new_paste_true (pqxx::connection_base& conn) {
	conn.prepare (
		"flag_new_paste_true",
		"UPDATE user_state SET new_paste = TRUE WHERE login = $1");
}

/**
 * @brief Prepares change flag watch paste in users_state table to true
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_set_flag_watch_paste_true (pqxx::connection_base& conn) {
	conn.prepare (
		"flag_watch_paste_true",
		"UPDATE user_state SET watch_paste = TRUE WHERE login = $1");
}

/**
 * @brief Сhange flag new paste in users_state table to true
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_set_flag_new_paste_true (pqxx::transaction_base& txn, int64_t login) {
	txn.exec_prepared0("flag_new_paste_true", login);
}

/**
 * @brief Сhange flag watch paste in users_state table to true
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_set_flag_watch_paste_true (pqxx::transaction_base& txn, int64_t login) {
	txn.exec_prepared0("flag_watch_paste_true", login);
}

/**
 * @brief Prepares change flags new paste and watch paste in users_state table to false
 * 
 * @param conn Reference to current connection
 */
void sql_actions::prepare_set_flags_false (pqxx::connection_base& conn) {
	conn.prepare (
		"flags_false",
		"UPDATE user_state SET watch_paste = FALSE, new_paste = FALSE WHERE login = $1");
}

/**
 * @brief Change flags new paste and watch paste in users_state table to false
 * 
 * @param txn Reference to current transaction.
 * @param login Id telegram chat 
 */
void sql_actions::execute_set_flags_false (pqxx::transaction_base& txn, int64_t login) {
	txn.exec_prepared0("flags_false", login);
}