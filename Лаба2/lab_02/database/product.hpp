#pragma once

#include <string>
#include <vector>
#include <optional>

#include "Poco/JSON/Object.h"

namespace database
{
    class Product{
        private:
            long _id;
            std::string _name;
            std::string _colour;
            long _weight;
            long _price;

        public:

            long get_id() const;
            const std::string &get_name() const;
            const std::string &get_colour() const;
            long get_weight() const;
            long get_price() const;

            long& id();
            std::string &name();
            std::string &colour();
            long &weight();
            long &price();

            static std::optional<Product> read_by_id(long id, const std::vector<Product>* products);
            static std::vector<Product> search(std::string name, const std::vector<Product>* products);
            void add(std::vector<Product>* products);
            void update(std::vector<Product>* products);

            Poco::JSON::Object::Ptr toJSON() const;
    };
}