#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
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

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

#include "handlers/user_handler.hpp"
#include "handlers/product_handler.hpp"

class HTTPRequestFactory: public HTTPRequestHandlerFactory
{
public:
    HTTPRequestFactory(const std::string& format):
        _format(format)
    {
        database::User admin;
        admin.id() = 0;
        admin.first_name() = "Admin";
        admin.last_name() = "";
        admin.email() = "";
        admin.login() = "admin";
        admin.password() = "secret";
        _users.push_back(admin);
    }

    HTTPRequestHandler* createRequestHandler(
        const HTTPServerRequest& request)
    {

        std::cout << "request:" << request.getURI()<< std::endl;
        if (hasPrefix(request.getURI(),"/user")) 
            return new UserHandler(_format, &_users);
        else if (hasPrefix(request.getURI(),"/product")) 
            return new ProductHandler(_format, &_products);
        return 0;
    }

private:
    std::string _format;
    std::vector<database::User> _users;
    std::vector<database::Product> _products;
};