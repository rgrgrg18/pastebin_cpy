#include "sql_interface.hpp"

#include "config.hpp"

Postgres::DbConnection::DbConnection(const std::string& conn_str)
		: conn_(std::make_shared<pqxx::connection>(conn_str)) {
	PrepareQueries();
}

Postgres::DbConnection::DbConnection(const DbConnection&& other) 
		: conn_(std::move(other.conn_)) {
	conn_ = nullptr; 	
}

Postgres::DbConnection& Postgres::DbConnection::operator=(const Postgres::DbConnection&& other) noexcept {
	conn_ = std::move(other.conn_);
	return *this;
}

void Postgres::DbConnection::PrepareQueries() {
	SqlActions::PrepareGetSequenceForPublicKey(*conn_);
	SqlActions::PrepareGetSequenceForPrivateKey(*conn_);
	SqlActions::PrepareGetInfoPaste(*conn_);
	SqlActions::PrepareCheckLogin(*conn_);
	SqlActions::PrepareAddUser(*conn_);
	SqlActions::PrepareAddPaste(*conn_);
	SqlActions::PrepareReturnAmountPastes(*conn_);
	SqlActions::PrepareIncreaseAmountPastes(*conn_);
	SqlActions::PrepareDecreaseAmountPastes(*conn_);
	SqlActions::PrepareChangePasswordPaste(*conn_);
	SqlActions::PrepareDeletePaste(*conn_);
	SqlActions::PrepareChangeTitlePaste(*conn_);
	SqlActions::PrepareGetLastUserPastes(*conn_);
}

pqxx::connection& Postgres::DbConnection::get() const noexcept { 
	return *conn_;
}

Postgres::PostgresConn Postgres::get_connection() { // NOLINT
    PostgresPool& pool = PostgresPool::get_instance(5, Config::Conn);
    return pool.get_connection();
}

PasteInfo Postgres::GetPasteInfo(const std::string& public_key) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	PasteInfo res = SqlActions::ExecuteGetInfoPaste(txn, public_key);
	txn.commit();
	return res;
}

void Postgres::ChangePassword(const std::string& public_key, const std::string& new_password) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	SqlActions::ExecuteChangePasswordPaste(txn, new_password, public_key);
	txn.commit();
}

void Postgres::ChangeTitle(const std::string& public_key, const std::string& new_name) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	SqlActions::ExecuteChangeTitlePaste(txn, new_name, public_key);
	txn.commit();
}

Keys Postgres::CreateNewPaste(uint64_t login) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	Keys res = SqlActions::NewPaste(txn, login);
	txn.commit();
	return res;
}

void Postgres::DelPaste(const std::string& public_key, uint64_t login) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	SqlActions::ExecuteDeletePaste(txn, public_key, login);
	txn.commit();
}

LastPastesInfo Postgres::GetLastUserPastes(uint64_t login, uint16_t limit) {
	PostgresConn conn = Postgres::get_connection();
	pqxx::work txn(conn->get());
	LastPastesInfo res = SqlActions::ExecuteGetLastUserPastes(txn, login, limit);
	txn.commit();
	return res;
}
