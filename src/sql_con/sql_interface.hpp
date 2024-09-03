#ifndef __SQL_INTERFACE__
#define __SQL_INTERFACE__

#include "sql_actions.h"
#include "../tools/ConnectionPool.hpp"

class postgres {
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
    static paste_info get_paste_info(const std::string& public_key);

    static void change_password(const std::string& public_key, const std::string& new_password);

    static void change_title(const std::string& public_key, const std::string& new_name);

    static keys create_new_paste(int64_t login);

    static void del_paste(const std::string& public_key, int64_t login);

    static last_pastes_info get_last_user_pastes(int64_t login, int64_t limit);
};

#endif