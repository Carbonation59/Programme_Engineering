#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/product.hpp"
#include "../../helper.hpp"

class ProductHandler : public HTTPRequestHandler
{

public:
    ProductHandler(const std::string &format) : _format(format)
    {
    }


    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            if (hasPrefix(request.getURI(),"/product") && form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = stol(form.get("id"));

                std::optional<database::Product> result = database::Product::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "product not found");
                    root->set("instance", "/product");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            else if (request.getURI() == "/products" && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                auto results = database::Product::read_all();
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }
            else if (hasPrefix(request.getURI(),"/product/search") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) && form.has("name"))
            {

                std::string name = form.get("name");
                auto results = database::Product::search(name);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }
            else if (request.getURI() == "/product" && request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
            {
                if (form.has("name") && form.has("colour") && form.has("weight") && form.has("price"))
                {

                    database::Product product;
                    product.name() = form.get("name");
                    product.colour() = form.get("colour");
                    product.weight() = stol(form.get("weight"));
                    product.price() = stol(form.get("price"));

                    bool check_result = true;
                    std::string message;
                    std::string reason;

                    if (product.get_weight() <= 0)
                    {
                        check_result = false;
                        message += "Weight of product must be positive";
                        message += "<br>";
                    }

                    if (product.get_price() <= 0)
                    {
                        check_result = false;
                        message += "Price of product must be positive";
                        message += "<br>";
                    }

                    if (check_result){
                        product.add();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << product.get_id();
                        return;
                    } else {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
            else if (request.getURI() == "/product" && request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT)
            {
                if (form.has("name") && form.has("colour") && form.has("weight") && form.has("price") && form.has("id"))
                {
                    
                    database::Product product;
                    product.id() = stol(form.get("id"));
                    product.name() = form.get("name");
                    product.colour() = form.get("colour");
                    product.weight() = stol(form.get("weight"));
                    product.price() = stol(form.get("price"));

                    bool check_result = true;
                    std::string message;
                    std::string reason;

                    if (product.get_weight() <= 0)
                    {
                        check_result = false;
                        message += "Weight of product must be positive";
                        message += "<br>";
                    }

                    if (product.get_price() <= 0)
                    {
                        check_result = false;
                        message += "Price of product must be positive";
                        message += "<br>";
                    }

                    std::optional<database::Product> result = database::Product::read_by_id(product.get_id());

                    if (!result) {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                        root->set("type", "/errors/not_found");
                        root->set("title", "Internal exception");
                        root->set("status", "404");
                        root->set("detail", "product not found");
                        root->set("instance", "/product");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(root, ostr);
                        return;
                    }

                    if (check_result){
                        product.update();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << product.get_id();
                        return;
                    } else {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
        }
        catch (std::exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl;
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request not found");
        root->set("instance", "/product");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};