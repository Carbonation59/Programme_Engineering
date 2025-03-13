workspace {
    name "Artemizer's Marketplace"
    !identifiers hierarchical

    model {
        client = Person "User"

        marketplace = softwareSystem "Artemizer's Marketplace" {

            service = container "Client Service"{
                technology "TypeScript + React"
            }

            server = container "Request Manager Server"{
                technology "C++"
            }

            db = container "Database"{
                technology "PostgreSQL"

                users = component "Users"
                products = component "Products"
                // id -- id_user -- product_id
                backets = component "Backets"
            }

        }

        
        client -> marketplace.service "Autorization" "Browser"
        client -> marketplace.service "Create new products" "Browser"
        client -> marketplace.service "Look all products" "Browsert"
        client -> marketplace.service "Add product in backet" "Browser"
        client -> marketplace.service "Look client's backet" "Browser"

        marketplace.service -> marketplace.server "Resend request to server" "Nginx"

        marketplace.server -> marketplace.db.users "Check existence of user's login" "SQL request"
        marketplace.server -> marketplace.db.users "Check existence of user' {name, surname}" "SQL request"
        marketplace.server -> marketplace.db.users "Add new user" "SQL request"
        marketplace.server -> marketplace.db.products "Add new product" "SQL request"
        marketplace.server -> marketplace.db.products "Get list of products" "SQL request"
        marketplace.server -> marketplace.db.backets "Add product in backet" "SQL request"
        marketplace.server -> marketplace.db.backets "Get client's backet" "SQL request"
        

    }

    views {
        themes default

        systemContext marketplace "context" {
            include *
            autoLayout lr
        }

        container marketplace "c2" {
            include *
            autoLayout lr
        }

        component marketplace.db "c3" {
            include *
            autoLayout
        }

        dynamic marketplace "SampleOfWorking" "Adding product in backet"{
           autoLayout lr
           client -> marketplace.service "Choose product and put it in backet"
           marketplace.service -> marketplace.server "Send request on server"
           marketplace.server -> marketplace.db "Add id of product and id of client in backet database"
        }
    }
}
