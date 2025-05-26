set -e

mongo <<EOF
db = db.getSiblingDB('arch')
db.createCollection('products')

var names = ["Laptop", "Smartphone", "Tablet", "Monitor", "Keyboard", "Mouse", "Headphones", "Printer", "Router", "Camera"]
var colors = ["Red", "Blue", "Green", "Black", "White", "Silver", "Gold", "Space Gray", "Rose Gold", "Yellow"]

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

for (var i = 0; i < 10; i++) {
    var randomName = names[Math.floor(Math.random() * names.length)];
    var randomColor = colors[Math.floor(Math.random() * colors.length)];
    var randomWeight = getRandomInt(100, 5000);
    var randomPrice = getRandomInt(50, 5000);
    
    db.products.insertOne({
        id: i,
        name: randomName,
        colour: randomColor,
        weight: randomWeight,
        price: randomPrice
    });
}

EOF