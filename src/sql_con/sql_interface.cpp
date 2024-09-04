#include "sql_interface.hpp"
#include "../config.hpp"

void postgres::db_connection::prepare_queries() {
	sql_actions::prepare_get_sequence_for_public_key(*conn_);
	sql_actions::prepare_get_sequence_for_private_key(*conn_);
	sql_actions::prepare_get_info_paste(*conn_);
	sql_actions::prepare_check_login(*conn_);
	sql_actions::prepare_add_user(*conn_);
	sql_actions::prepare_add_paste(*conn_);
	sql_actions::prepare_return_amount_pastes(*conn_);
	sql_actions::prepare_increase_amount_pastes(*conn_);
	sql_actions::prepare_decrease_amount_pastes(*conn_);
	sql_actions::prepare_change_password_paste(*conn_);
	sql_actions::prepare_delete_paste(*conn_);
	sql_actions::prepare_change_title_paste(*conn_);
	sql_actions::prepare_get_last_user_pastes(*conn_);
}

pqxx::connection& postgres::db_connection::get() const noexcept {
	return *conn_;
}

postgres::postgres_conn postgres::getConnection() {
    postgres_pool& pool = postgres_pool::getInstance(5, Config::Conn);
    return pool.getConnection();
}

paste_info postgres::get_paste_info(const std::string& public_key) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	paste_info res = sql_actions::execute_get_info_paste(txn, public_key);
	txn.commit();
	return res;
}

void postgres::change_password(const std::string& public_key, const std::string& new_password) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	sql_actions::execute_change_password_paste(txn, new_password, public_key);
	txn.commit();
}

void postgres::change_title(const std::string& public_key, const std::string& new_name) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	sql_actions::execute_change_title_paste(txn, new_name, public_key);
	txn.commit();
}

keys postgres::create_new_paste(uint64_t login) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	keys res = sql_actions::new_paste(txn, login);
	txn.commit();
	return res;
}

void postgres::del_paste(const std::string& public_key, uint64_t login) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	sql_actions::execute_delete_paste(txn, public_key, login);
	txn.commit();
}

last_pastes_info postgres::get_last_user_pastes(uint64_t login, uint16_t limit) {
	postgres_conn conn = postgres::getConnection();
	pqxx::work txn(conn->get());
	last_pastes_info res = sql_actions::execute_get_last_user_pastes(txn, login, limit);
	txn.commit();
	return res;
}
