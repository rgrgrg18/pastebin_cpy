#pragma once

#include "ConnectionPool.hpp"
#include "sql_actions.hpp"
#include "storage_interface.hpp"

class postgres : public Storage {
private:
	class db_connection {
	private:
		std::shared_ptr<pqxx::connection> conn_;

		void prepare_queries();
	public:
		explicit db_connection(const std::string& conn_str)
			: conn_(std::make_shared<pqxx::connection>(conn_str)) {
			prepare_queries();
		}

		db_connection(const db_connection& other) = delete;
		db_connection& operator=(const db_connection& other) = delete;

		db_connection(const db_connection&& other) 
			: conn_(std::move(other.conn_)) {
			conn_ = nullptr; 	
		}

		db_connection& operator=(const db_connection&& other) noexcept {
			conn_ = std::move(other.conn_);
			return *this;
		}

		pqxx::connection& get() const noexcept;
	};

	using postgres_pool = ConnectionPool<db_connection>;
	using postgres_conn = Connection<db_connection>;

	static postgres_conn getConnection();

public:
    paste_info get_paste_info(const std::string& public_key) final;

    void change_password(const std::string& public_key, const std::string& new_password) final;

	void change_title(const std::string& public_key, const std::string& new_name) final;

    keys create_new_paste(uint64_t login) final;

    void del_paste(const std::string& public_key, uint64_t login) final;

    last_pastes_info get_last_user_pastes(uint64_t login, uint16_t limit) final;
};