#include "product.hpp"
#include "database.hpp"
#include "../config/config.hpp"

#include <sstream>
#include <exception>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <cppkafka/cppkafka.h>

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

    #include <mutex>
    void Product::send_to_queue()
    {
        this->id() = database::Database::get().count_from_mongo("products");

       /** static cppkafka::Configuration config = [&](){
            cppkafka::Configuration config={
            {"metadata.broker.list", Config::get().get_queue_host()}};
            std::vector<cppkafka::ConfigurationOption> options;
            options.push_back({"acks","all"});
            config.set(options);
            std::cout << "kafka configured" << std::endl;
            return config;
        }();*/
        
        static cppkafka::Configuration config ={
            {"metadata.broker.list", Config::get().get_queue_host()},
            {"acks","all"}};
        static cppkafka::Producer producer(config);
        static std::mutex mtx;
        static int message_key{0};
        using Hdr = cppkafka::MessageBuilder::HeaderType;
        
        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream ss;
        Poco::JSON::Stringifier::stringify(toJSON(), ss);
        std::string message = ss.str();
        bool not_sent = true;

        cppkafka::MessageBuilder builder(Config::get().get_queue_topic());
        std::string mk=std::to_string(++message_key);
        builder.key(mk); // set some key
        builder.header(Hdr{"producer_type","author writer"}); // set some custom header
        builder.payload(message); // set message

        while (not_sent)
        {
            try
            {
                producer.produce(builder);
                not_sent = false;
            }
            catch (...)
            {
                std::cout << "Wrong send to kafka\n";
            }
        }
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

    Product Product::fromJSON_writer(const std::string &str)
    {
        Product product;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        product.id() = object->getValue<long>("id");
        product.name() = object->getValue<std::string>("name");
        product.colour() = object->getValue<std::string>("colour");
        product.weight() = object->getValue<long>("weight");
        product.price() = object->getValue<long>("price");

        return product;
    }

}