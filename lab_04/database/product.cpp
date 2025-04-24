#include "product.hpp"
#include "database.hpp"

#include <sstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace database
{

    long Product::get_id() const
    {
        return _id;
    }

    const std::string &Product::get_name() const
    {
        return _name;
    }

    const std::string &Product::get_colour() const
    {
        return _colour;
    }

    long Product::get_weight() const
    {
        return _weight;
    }

    long Product::get_price() const
    {
        return _price;
    }

    long &Product::id()
    {
        return _id;
    }

    std::string &Product::name()
    {
        return _name;
    }

    std::string &Product::colour()
    {
        return _colour;
    }

    long &Product::weight()
    {
        return _weight;
    }

    long &Product::price()
    {
        return _price;
    }


    std::optional<Product> Product::read_by_id(long id)
    {
        std::optional<Product> result;
        std::map<std::string,long> params_long;
        std::map<std::string,std::string> params_string;
        params_long["id"] = id;
        std::vector<std::string> results = database::Database::get().get_from_mongo("products",params_long,params_string);

        if(!results.empty())
            result = fromJSON(results[0]);
        
        return result;
    }

    std::vector<Product> Product::search(std::string name)
    {
        std::vector<Product> result;
        std::map<std::string,long> params_long;
        std::map<std::string,std::string> params_string;
        params_string["name"] = name;

        std::vector<std::string> results = database::Database::get().get_from_mongo("products",params_long,params_string);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    std::vector<Product> Product::read_all()
    {
        std::vector<Product> result;
        std::map<std::string,long> params_long;
        std::map<std::string,std::string> params_string;

        std::vector<std::string> results = database::Database::get().get_from_mongo("products",params_long, params_string);

        for(std::string& s : results) 
            result.push_back(fromJSON(s));
        

        return result;
    }

    void Product::add()
    {
        this->id() = database::Database::get().count_from_mongo("products");
        database::Database::get().send_to_mongo("products",toJSON());
    }

    void Product::update()
    {
        std::map<std::string,long> params;
        params["id"] = _id;       
        database::Database::get().update_mongo("products",params,toJSON());
    }

    Poco::JSON::Object::Ptr Product::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("colour", _colour);
        root->set("weight", _weight);
        root->set("price", _price);

        return root;
    }

    Product Product::fromJSON(const std::string &str)
    {
        int start = str.find("_id");
        int end = str.find(",",start);

        std::string s1 = str.substr(0,start-1);
        std::string s2 = str.substr(end+1);

        // std::cout << s1 << s2 << std::endl;
        // std::cout << "from json:" << str << std::endl;
        Product product;

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(s1+s2);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        product.id() = object->getValue<long>("id");
        product.name() = object->getValue<std::string>("name");
        product.colour() = object->getValue<std::string>("colour");
        product.weight() = object->getValue<long>("weight");
        product.price() = object->getValue<long>("price");

        return product;
    }

}