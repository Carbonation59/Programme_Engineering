#include "user.hpp"

#include <sstream>
#include <exception>

#include "Poco/JSON/Object.h"

namespace database
{

    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    std::optional<long> User::auth(std::string &login, std::string &password, const std::vector<User>* users)
    {
        long n = (*users).size();
        for(long i = 0 ; i < n ; i++){
            if((*users)[i].get_login() == login && (*users)[i].get_password() == password){
                return (*users)[i].get_id();
            }
        }
        return {};
    }

    std::optional<User> User::read_by_id(long id, const std::vector<User>* users)
    {
        long n = (*users).size();
        for(long i = 0 ; i < n ; i++){
            if((*users)[i].get_id() == id){
                return (*users)[i];
            }
        }
        return {};
    }

    std::optional<User> User::read_by_login(std::string login, const std::vector<User>* users)
    {
        long n = (*users).size();
        for(long i = 0 ; i < n ; i++){
            if((*users)[i].get_login() == login){
                return (*users)[i];
            }
        }
        return {};
    }

    std::vector<User> User::search(std::string first_name, std::string last_name, const std::vector<User>* users)
    {
        long n = (*users).size();
        std::vector<User> v;
        for(long i = 0 ; i < n ; i++){
            if((*users)[i].get_first_name() == first_name && (*users)[i].get_last_name() == last_name){
                v.push_back((*users)[i]);
            }
        }
        return v;
    }

    void User::save_to_mysql(std::vector<User>* users)
    {
        (*users).push_back(*this);
    }

    const std::string &User::get_login() const
    {
        return _login;
    }

    const std::string &User::get_password() const
    {
        return _password;
    }

    std::string &User::login()
    {
        return _login;
    }

    std::string &User::password()
    {
        return _password;
    }

    long User::get_id() const
    {
        return _id;
    }

    const std::string &User::get_first_name() const
    {
        return _first_name;
    }

    const std::string &User::get_last_name() const
    {
        return _last_name;
    }

    const std::string &User::get_email() const
    {
        return _email;
    }

    long &User::id()
    {
        return _id;
    }

    std::string &User::first_name()
    {
        return _first_name;
    }

    std::string &User::last_name()
    {
        return _last_name;
    }

    std::string &User::email()
    {
        return _email;
    }
}