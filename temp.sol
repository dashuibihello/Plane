pragma solidity ^0.4.19;

contract TradingSystem {
    struct Goods {
        string owner;
        string name;
        string information;
        uint price;
        uint count;
    }
    
    struct User {
        string name;
        string password;
        uint goodsCount;
        mapping(string => uint) goodsPos;
        mapping(string => bool) goodsFlag;
    }

    User[] users;
    uint userCount = 0;
    
    mapping(string => bool) usersFlag;
    mapping(string => uint) userPos;
    mapping(address => string) addressToUser;
    mapping(string => Goods[]) userToGoods;
    //mapping(string => string) userToAddress;

    function regist(string name, string password) public {
        require(usersFlag[name] == false, "The name has been used!");    
        require(usersFlag[addressToUser[msg.sender]] == false, "The address has registed!");   
        users.push(User(name, password, 0));
        userPos[name] = userCount;
        userCount++;
        usersFlag[name] = true;
        addressToUser[msg.sender] = name;
    }

    function getName(uint x) public view returns (string){
        return users[x].name;
    }

    function getNameByAddress() public view returns (string){
        return addressToUser[msg.sender];
    }

    function setPassword(string newPassword) public {
        users[userPos[addressToUser[msg.sender]]].password = newPassword;
    }
    
    function getPassword() public view returns (string){ 
        return users[userPos[addressToUser[msg.sender]]].password;
    }

    //Goods operation
    function addGoods(string name, string information, uint price, uint count) public {
        users[userPos[addressToUser[msg.sender]]].goodsPos[name] = users[userPos[addressToUser[msg.sender]]].goodsCount;
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == false, "The goods exists!");
        users[userPos[addressToUser[msg.sender]]].goodsFlag[name] = true;
        users[userPos[addressToUser[msg.sender]]].goodsCount++;
        userToGoods[addressToUser[msg.sender]].push(Goods(addressToUser[msg.sender], name, information, price, count));
    }

    function deleteGoods(string name) public {           
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]] = userToGoods[addressToUser[msg.sender]][userToGoods[addressToUser[msg.sender]].length - 1];
        users[userPos[addressToUser[msg.sender]]].goodsPos[userToGoods[addressToUser[msg.sender]][(userToGoods[addressToUser[msg.sender]].length - 1)].name] = users[userPos[addressToUser[msg.sender]]].goodsPos[name];
        delete users[userPos[addressToUser[msg.sender]]].goodsPos[name];
        users[userPos[addressToUser[msg.sender]]].goodsFlag[name] = false;
        userToGoods[addressToUser[msg.sender]].length = userToGoods[addressToUser[msg.sender]].length - 1;
        users[userPos[addressToUser[msg.sender]]].goodsCount--;
    }

    function getUserGoodsCount() public view returns (uint) {
        return users[userPos[addressToUser[msg.sender]]].goodsCount;
    }

    function setGoodsName(string name, string newName) public {
        userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].name = newName;
    }

    function getGoodsName(uint x) public view returns (string) {
        return userToGoods[addressToUser[msg.sender]][x].name;
    }

    function getGoodsNameByname(string name) public view returns (string) {
        return userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].name;
    }

    function getGoodsOwner(uint x) public view returns (string) {
        return userToGoods[addressToUser[msg.sender]][x].owner;
    }

    function setGoodsInformation(string name, string newInformation) public {
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].information = newInformation;
    }

    function getGoodsInformation(string name) public view returns (string) {    
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        return userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].information;
    }

    function setGoodsPrice(string name, uint newPrice) public {
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].price = newPrice;
    }

    function getGoodsPrice(string name) public view returns (uint) {
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        return userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].price;
    }

    function setGoodsCount(string name, uint newCount) public {
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].count = newCount;
    }

    function getGoodsCount(string name) public view returns (uint) {
        require(users[userPos[addressToUser[msg.sender]]].goodsFlag[name] == true, "The goods doesn't exist!");
        return userToGoods[addressToUser[msg.sender]][users[userPos[addressToUser[msg.sender]]].goodsPos[name]].count;
    }
}
