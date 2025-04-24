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

            backets = container "Backet Manager"{
                technology "C++, POCO"
            }

            user_db = container "User Database"{
                technology "PostgreSQL"

                users = component "Users"
                // id -- id_user -- product_id
                backets = component "Backets"
            }

            product_db = container "Product Database"{
                technology "MongoDB"

                products = component "Products"
            }

        }

        
        client -> marketplace.users "Autorization" "HTTP request"
        client -> marketplace.products "Create new products" "HTTP request"
        client -> marketplace.products "Look all products" "HTTP request"
        client -> marketplace.backets "Add product in backet" "HTTP request"
        client -> marketplace.backets "Look client's backet" "HTTP request"


        marketplace.users -> marketplace.user_db.users "Check existence of user's login" "SQL request"
        marketplace.users -> marketplace.user_db.users "Check existence of user' {name, surname}" "SQL request"
        marketplace.users -> marketplace.user_db.users "Add new user" "SQL request"
        marketplace.products -> marketplace.product_db.products "Add new product" "SQL request"
        marketplace.products -> marketplace.product_db.products "Get list of products" "SQL request"
        marketplace.backets -> marketplace.user_db.backets "Add product in backet" "SQL request"
        marketplace.backets -> marketplace.user_db.backets "Get client's backet" "SQL request"
        

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

        dynamic marketplace "SampleOfWorking" "Adding product in backet"{
           autoLayout lr
           client -> marketplace.backets "Choose product and put it in backet"
           marketplace.backets -> marketplace.user_db "Add id of product and id of client in backet database"
        }
    }
}