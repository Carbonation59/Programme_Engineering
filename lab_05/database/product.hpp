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

            static std::optional<Product> read_by_id(long id);
            static std::vector<Product> search(std::string name);
            static std::vector<Product> read_all();
            void add();
            void update();

            static Product fromJSON(const std::string & str);
            Poco::JSON::Object::Ptr toJSON() const;
    };
}