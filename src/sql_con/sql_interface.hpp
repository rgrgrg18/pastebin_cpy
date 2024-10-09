#pragma once

#include "ConnectionPool.hpp"
#include "sql_actions.hpp"
#include "storage_interface.hpp"

class Postgres : public Storage {
public:
    PasteInfo GetPasteInfo(const std::string& public_key) final;

    void ChangePassword(const std::string& public_key, const std::string& new_password) final;

	void ChangeTitle(const std::string& public_key, const std::string& new_name) final;

    Keys CreateNewPaste(uint64_t login) final;

    void DelPaste(const std::string& public_key, uint64_t login) final;

    LastPastesInfo GetLastUserPastes(uint64_t login, uint16_t limit) final;

private:
	class DbConnection {
	public:
	    explicit DbConnection(const std::string& conn_str);

		// Non-Copyable
		DbConnection(const DbConnection& other) = delete;
		DbConnection& operator=(const DbConnection& other) = delete;
		
		// Movable
		DbConnection(const DbConnection&& other);
		DbConnection& operator=(const DbConnection&& other) noexcept;

		pqxx::connection& get() const noexcept; 

	private:
		void PrepareQueries();

		std::shared_ptr<pqxx::connection> conn_;
	};

	using PostgresPool = ConnectionPool<DbConnection>;
	using PostgresConn = Connection<DbConnection>;

	static PostgresConn get_connection();
};