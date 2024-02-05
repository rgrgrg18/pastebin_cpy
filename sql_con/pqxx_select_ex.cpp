#include <iostream>
#include <iomanip>
#include <sstream>
#include <pqxx/pqxx>

void add_user (pqxx::work& txn, const std::string& login);
void add_paste (pqxx::work& txn, const std::string& login, const std::string& amazon_link);
std::string get_amazon_link (pqxx::work& txn, const std::string& hash);

int main () {
	pqxx::connection conn{"postgresql://postgres:sql_database_senior_ye@127.0.0.1:5432/postgres"};
	pqxx::work txn(conn);

	//add_paste(txn, "kirill", "amazon.s3.test");
	//add_user(txn, "kirill");
	
	std::string res = get_amazon_link(txn, "test");
	std::cout << res << std::endl;

	/*if (!res.empty()) {
		std::cout << res.column_name(0) << " " << res.column_name(1) << std::endl;
		std::cout << res[0][0].c_str() << " " << res[0][1].c_str() << std::endl;
		std::cout << res[1][0].c_str() << " " << res[1][1].c_str() << std::endl;
	}*/
}

std::string get_amazon_link (pqxx::work& txn, const std::string& hash) {
	std::string get_link = "SELECT amazon_link FROM pastes WHERE hash = '";
	get_link += hash;
	get_link.push_back('\'');

	pqxx::result res(txn.exec(get_link.c_str()));

	return res[0][0].c_str();
}

void add_user (pqxx::work& txn, const std::string& login) {
	std::string insert_users = "INSERT INTO users (id, login) VALUES (nextval('seq_person'), '";
	insert_users += login;

	insert_users.push_back('\'');
	insert_users.push_back(')');

	txn.exec(insert_users.c_str());

	txn.commit();
}

void add_paste (pqxx::work& txn, const std::string& login, const std::string& amazon_link) {

	std::string insert_paste = "INSERT INTO pastes (hash, amazon_link, user_id) VALUES ('";

	std::string hash_link = "test";
	hash_link += "', '";

	insert_paste += hash_link;
	insert_paste += amazon_link;

	std::string user_id = "', (SELECT id FROM users WHERE login = '";
	user_id += login;
	user_id.push_back('\'');
	user_id.push_back(')');
	user_id.push_back(')');

	insert_paste += user_id;

	txn.exec(insert_paste.c_str());

	txn.commit();
}
