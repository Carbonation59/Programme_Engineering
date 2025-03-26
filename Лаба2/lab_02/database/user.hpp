#pragma once

#include <string>
#include <vector>
#include <optional>

#include "Poco/JSON/Object.h"


namespace database
{
    class User{
        private:
            long _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _login;
            std::string _password;

        public:

            long get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_email() const;
            const std::string &get_login() const;
            const std::string &get_password() const;

            long& id();
            std::string &first_name();
            std::string &last_name();
            std::string &email();
            std::string &login();
            std::string &password();

            static std::optional<User> read_by_id(long id, const std::vector<User>* users);
            static std::optional<User> read_by_login(std::string login, const std::vector<User>* users);
            static std::optional<long> auth(std::string &login, std::string &password, const std::vector<User>* users);
            static std::vector<User> search(std::string first_name,std::string last_name, const std::vector<User>* users);
            void save_to_mysql(std::vector<User>* users);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}