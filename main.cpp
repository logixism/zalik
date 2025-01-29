#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

double balance = 0.0;
std::vector<std::string> ownedComputers = {};

struct Manager {
    std::string surname;
    std::string name;
    std::string patronymic;
    std::string position;
    std::string phone;
};

struct Computer {
    std::string name;
    std::vector<std::string> components;
};

struct Component {
    std::string name;
    std::string manufacturer;
    std::string features;
    double price;
};

bool isComputerOwned(const std::string &computerName) {
    for (const auto &ownedComputer : ownedComputers) {
        if (ownedComputer == computerName) {
            return true;
        }
    }
    return false;
}

void saveDataToFile() {
    FILE *file = fopen("data.txt", "w");
    if (file) {
        fprintf(file, "%.2f\n", balance);
        fprintf(file, "\n");
        for (const auto &computer : ownedComputers) {
            fprintf(file, "%s\n", computer.c_str());
        }
        fclose(file);
        std::cout << "Data saved to file successfully." << std::endl;
    } else {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
}

void addBalance(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "Balance topped up. New balance: " << balance << std::endl;
    } else {
        std::cout << "Invalid amount. Please enter a positive value."
                  << std::endl;
    }
}

void printBalance() { std::cout << "Balance: " << balance << std::endl; }

void buyComputer(const std::string &computerName, Shop &shop) {
    auto it = shop.computers.find(computerName);
    if (it != shop.computers.end()) {
        double totalPrice = shop.calculateComputerPrice(it->second.name);

        if (balance >= totalPrice) {
            balance -= totalPrice;
            std::cout << "Computer purchased: " << computerName << std::endl;
            std::cout << "Remaining balance: " << balance << std::endl;
            ownedComputers.push_back(computerName);

            sleep(2);
        } else {
            std::cout << "Insufficient balance. Please top up your balance."
                      << std::endl;
            sleep(2);
        }
    } else {
        std::cout << "Computer not found." << std::endl;
        sleep(2);
    }
}

struct Shop {
    std::vector<Manager> managers;
    std::map<std::string, Computer> computers;
    std::map<std::string, Component> components;

    void addManager(const Manager &manager) { managers.push_back(manager); }
    void addComputer(const Computer &computer) {
        computers[computer.name] = computer;
    }
    void addComponent(const Component &component) {
        components[component.name] = component;
    }
    void printPriceList() {
        std::cout << "Price list:" << std::endl;
        for (const auto &component : components) {
            std::cout << component.second.name << " - "
                      << component.second.price << std::endl;
        }
    }

    double calculateComputerPrice(const std::string &computerName) {
        double totalPrice = 0.0;
        const auto &computer = computers[computerName];
        for (const auto &componentName : computer.components) {
            totalPrice += components[componentName].price;
        }
        return totalPrice;
    }

    void printComputers() {
        std::cout << "Computers:" << std::endl;
        for (const auto &computer : computers) {
            double totalPrice = calculateComputerPrice(computer.second.name);
            std::cout << (isComputerOwned(computer.first) ? "(Owned) " : "")
                      << computer.first << " - " << totalPrice << std::endl;
        }
    }

    void printManagers() {
        std::cout << "Managers:" << std::endl;
        for (const auto &manager : managers) {
            std::cout << manager.surname << " " << manager.name << " "
                      << manager.patronymic << " - " << manager.position
                      << ", phone: " << manager.phone << std::endl;
        }
    }
};

int main() {
    Shop shop;
    shop.addManager(
        {"Ivanov", "Ivan", "Ivanovich", "director", "123-456-7890"});
    shop.addManager(
        {"Petrov", "Petr", "Petrovich", "salesman", "987-654-3210"});

    shop.addComputer({"Acer", {"Intel Core i7-9700K", "8 GB RAM", "1 TB SSD"}});
    shop.addComputer(
        {"Asus", {"Intel Core i9-13900K", "32 GB RAM", "2 TB SSD"}});

    shop.addComponent(
        {"Intel Core i9-13900K", "Intel", "Fast processor", 1000.0});
    shop.addComponent(
        {"Intel Core i7-9700K", "Intel", "Fast processor", 500.0});

    shop.addComponent({"32 GB RAM", "Corsair", "Fast memory", 200.0});
    shop.addComponent({"8 GB RAM", "Corsair", "Fast memory", 100.0});

    shop.addComponent({"1 TB SSD", "Samsung", "Fast disk", 500.0});
    shop.addComponent({"2 TB SSD", "WD", "Fast disk", 200.0});

    std::ifstream file("data.txt");
    if (file) {
        file >> balance;
        std::string computerName;
        ownedComputers.clear();
        std::getline(file, computerName);
        while (std::getline(file, computerName)) {
            if (!computerName.empty()) {
                ownedComputers.push_back(computerName);
            }
        }
        file.close();
        std::cout << "Data loaded from file successfully." << std::endl;
        sleep(2);
    } else {
        std::cerr << "Failed to open file for reading." << std::endl;
        sleep(2);
    }

    while (true) {
        system("clear");

        shop.printPriceList();
        std::cout << std::endl;
        shop.printManagers();
        std::cout << std::endl;
        shop.printComputers();
        std::cout << std::endl;
        printBalance();

        int action;
        std::cout << "Please select an action (1 - top up balance, 2 - buy "
                     "computer, 3 - exit): ";
        std::cin >> action;

        switch (action) {
        case 1: {
            double amount;
            std::cout << "Enter amount to top up balance: ";
            std::cin >> amount;
            addBalance(amount);
            break;
        }
        case 2: {
            std::string computerName;
            std::cout << "Enter computer name to buy: ";
            std::cin >> computerName;
            buyComputer(computerName, shop);
            break;
        }
        case 3: {
            saveDataToFile();
            return 0;
        }
        default:
            std::cout << "Invalid action selected." << std::endl;
            break;
        }
    }

    return 0;
}