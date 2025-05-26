workspace {
    name "Artemizer's Marketplace"
    !identifiers hierarchical

    model {
        client = Person "User"

        marketplace = softwareSystem "Artemizer's Marketplace" {

            users = container "User Manager"{
                technology "C++, POCO"
            }

            products = container "Product Manager"{
                technology "C++, POCO"
            }

            user_db = container "User Database"{
                technology "PostgreSQL"

                users = component "Users"
                // id -- user_id -- product_id
                baskets = component "Baskets"
            }
            
            cache_db = container "Cache for User Database"{
                technology "Redis"

                users = component "Users"
            }

            product_db = container "Product Database"{
                technology "MongoDB"

                products = component "Products"
            }

            queue_db = container "Queue broker for Product Database"{
                technology "Kafka"
            }

            writer = container "Writer of data from Kafka to MongoDB"{
                technology "C++, POCO"
            }

        }

        
        client -> marketplace.users "Autorization" "HTTP request"
        client -> marketplace.products "Create new products" "HTTP request"
        client -> marketplace.products "Look all products" "HTTP request"
        client -> marketplace.users "Add product in basket" "HTTP request"
        client -> marketplace.users "Look client's basket" "HTTP request"


        marketplace.users -> marketplace.user_db.users "Check existence of user with specified login" "SQL request"
        marketplace.users -> marketplace.user_db.users "Check existence of user with specified {name, surname}" "SQL request"
        marketplace.users -> marketplace.user_db.users "Check existence of user with specified id" "SQL request"
        marketplace.users -> marketplace.user_db.users "Add new user" "SQL request"
        marketplace.users -> marketplace.cache_db.users "Get user from cache if exists there"
        marketplace.products -> marketplace.queue_db "Add new product" "KSQL"
        marketplace.writer -> marketplace.queue_db "Get new product from queue" "KSQL"
        marketplace.writer -> marketplace.product_db.products "Add new product to database" "JavaScript"
        marketplace.products -> marketplace.product_db.products "Get list of products" "JavaScript"
        marketplace.products -> marketplace.product_db.products "Find product with with selected name" "JavaScript"
        marketplace.users -> marketplace.user_db.baskets "Add product in client's basket" "SQL request"
        marketplace.users -> marketplace.user_db.baskets "Get client's basket" "SQL request"
        

    }

    views {
        themes default

        systemContext marketplace "context" {
            include *
            autoLayout lr
        }

        container marketplace "c2" {
            include *
            autoLayout
        }

        component marketplace.user_db "c3" {
            include *
            autoLayout
        }

        dynamic marketplace "SampleOfWorking" "Adding product in basket"{
           autoLayout lr
           client -> marketplace.users "Choose product and put it in basket"
           marketplace.users -> marketplace.user_db "Add id of product and id of client in basket database"
        }
    }
}