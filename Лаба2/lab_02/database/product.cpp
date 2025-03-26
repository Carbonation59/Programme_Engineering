#include "product.hpp"

#include <sstream>

#include "Poco/JSON/Object.h"

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


    std::optional<Product> Product::read_by_id(long id, const std::vector<Product>* products)
    {
        long n = (*products).size();
        for(long i = 0 ; i < n ; i++){
            if((*products)[i].get_id() == id){
                return (*products)[i];
            }
        }
        return {};
    }

    std::vector<Product> Product::search(std::string name, const std::vector<Product>* products)
    {
        long n = (*products).size();
        std::vector<Product> res;
        for(long i = 0 ; i < n ; i++){
            if((*products)[i].get_name() == name){
                res.push_back((*products)[i]);
            }
        }
        return res;
    }

    void Product::add(std::vector<Product>* products)
    {
        (*products).push_back(*this);
    }

    void Product::update(std::vector<Product>* products)
    {
        long n = (*products).size();
        for(long i = 0 ; i < n ; i++){
            if((*products)[i].get_id() == _id){
                (*products)[i].id() = _id;
                (*products)[i].name() = _name;
                (*products)[i].colour() = _colour;
                (*products)[i].weight() = _weight;
                (*products)[i].price() = _price;
            }
        }
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

}