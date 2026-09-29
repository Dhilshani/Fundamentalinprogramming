#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

// ============================================================
// FURNITURE VILLEGE FURNITURE ORDERING SYSTEM
// CSE4002 - Fundamentals in Programming
// ============================================================

// ---------------------------
// Constants
// ---------------------------

const string FURNITURE_FILE = "furniture.txt";
const string ORDER_FILE = "orders.txt";
const string FURNITURE_BACKUP = "furniture_backup.txt";
const string ORDER_BACKUP = "orders_backup.txt";

const string VALID_USERNAME = "admin";
const string VALID_PASSWORD = "admin123";

const int MAX_FURNITURE = 500;
const int MAX_ORDERS = 1000;

// ---------------------------
// Structure / Record: Furniture
// ---------------------------

struct Furniture
{
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

// ---------------------------
// Structure / Record: Order
// ---------------------------

struct Order
{
    int orderId;
    int furnitureId;
    string furnitureName;
    int quantity;
    double totalPrice;
};
// ---------------------------
// Function Prototypes
// ---------------------------

void displayHeader(const string& title);
void pauseScreen();

string trim(const string& text);
string toLowerCase(string text);

bool isInteger(const string& input);
bool isPositiveInteger(const string& input);
bool isValidPrice(const string& input);

int getInteger(const string& prompt);
int getPositiveInteger(const string& prompt);
double getPositiveDouble(const string& prompt);
string getNonEmptyString(const string& prompt);

bool login();

void loadFurniture(vector<Furniture>& furniture);
void saveFurniture(const vector<Furniture>& furniture);
void backupFurniture(const vector<Furniture>& furniture);

void loadOrders(vector<Order>& orders);
void saveOrders(const vector<Order>& orders);
void backupOrders(const vector<Order>& orders);

int findFurnitureById(
    const vector<Furniture>& furniture,
    int furnitureId
);

void addFurniture(vector<Furniture>& furniture);

void listAvailableFurniture(
    const vector<Furniture>& furniture
);

void searchFurniture(
    const vector<Furniture>& furniture
);

void placeOrder(
    vector<Furniture>& furniture,
    vector<Order>& orders
);

void displayFurniture(
    const Furniture& furniture
);

void displayHelp();

void displayMainMenu();

void runSystem(
    vector<Furniture>& furniture,
    vector<Order>& orders
);

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    vector<Furniture> furniture;
    vector<Order> orders;

    displayHeader("FURNITURE VILLEGE FURNITURE ORDERING SYSTEM");

    loadFurniture(furniture);
    loadOrders(orders);

    if (!login())
    {
        cout << "\nToo many unsuccessful login attempts.\n";
        cout << "The system will now close.\n";
        return 0;
    }

    runSystem(furniture, orders);

    cout << "\nThank you for using the FURNITURE VILLEGE system.\n";

    return 0;
}

// ============================================================
// DISPLAY HEADER
// ============================================================

void displayHeader(const string& title)
{
    cout << "\n============================================================\n";
    cout << title << "\n";
    cout << "============================================================\n";
}

// ============================================================
// PAUSE SCREEN
// ============================================================

void pauseScreen()
{
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
// TRIM WHITESPACE
// ============================================================

string trim(const string& text)
{
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos)
    {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

// ============================================================
// CONVERT STRING TO LOWERCASE
// ============================================================

string toLowerCase(string text)
{
    transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(tolower(character));
        }
    );

    return text;
}

// ============================================================
// INTEGER VALIDATION
// ============================================================

bool isInteger(const string& input)
{
    string value = trim(input);

    if (value.empty())
    {
        return false;
    }

    size_t start = 0;

    if (value[0] == '-' || value[0] == '+')
    {
        start = 1;
    }

    if (start == value.length())
    {
        return false;
    }

    for (size_t i = start; i < value.length(); ++i)
    {
        if (!isdigit(static_cast<unsigned char>(value[i])))
        {
            return false;
        }
    }

    return true;
}

// ============================================================
// POSITIVE INTEGER VALIDATION
// ============================================================

bool isPositiveInteger(const string& input)
{
    if (!isInteger(input))
    {
        return false;
    }

    try
    {
        int value = stoi(trim(input));
        return value > 0;
    }
    catch (...)
    {
        return false;
    }
}

// ============================================================
// PRICE VALIDATION
// ============================================================

bool isValidPrice(const string& input)
{
    string value = trim(input);

    if (value.empty())
    {
        return false;
    }

    try
    {
        size_t position = 0;
        double price = stod(value, &position);

        if (position != value.length())
        {
            return false;
        }

        return price > 0;
    }
    catch (...)
    {
        return false;
    }
}

// ============================================================
// GET INTEGER
// ============================================================

int getInteger(const string& prompt)
{
    while (true)
    {
        cout << prompt;

        string input;
        getline(cin, input);

        if (isInteger(input))
        {
            try
            {
                return stoi(trim(input));
            }
            catch (...)
            {
                cout << "Invalid number. Please enter a valid integer.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a whole number.\n";
        }
    }
}

// ============================================================
// GET POSITIVE INTEGER
// ============================================================

int getPositiveInteger(const string& prompt)
{
    while (true)
    {
        cout << prompt;

        string input;
        getline(cin, input);

        if (isPositiveInteger(input))
        {
            try
            {
                return stoi(trim(input));
            }
            catch (...)
            {
                cout << "The number entered is too large. Please try again.\n";
            }
        }
        else
        {
            cout << "Please enter a positive whole number greater than zero.\n";
        }
    }
}

// ============================================================
// GET POSITIVE DOUBLE
// ============================================================

double getPositiveDouble(const string& prompt)
{
    while (true)
    {
        cout << prompt;

        string input;
        getline(cin, input);

        if (isValidPrice(input))
        {
            try
            {
                return stod(trim(input));
            }
            catch (...)
            {
                cout << "Invalid price. Please enter a valid amount.\n";
            }
        }
        else
        {
            cout << "Please enter a valid positive price.\n";
        }
    }
}

// ============================================================
// GET NON-EMPTY STRING
// ============================================================

string getNonEmptyString(const string& prompt)
{
    while (true)
    {
        cout << prompt;

        string input;
        getline(cin, input);

        input = trim(input);

        if (!input.empty())
        {
            return input;
        }

        cout << "This field cannot be empty. Please enter a value.\n";
    }
}

// ============================================================
// LOGIN
// ============================================================

bool login()
{
    const int MAX_ATTEMPTS = 3;

    displayHeader("SYSTEM LOGIN");

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt)
    {
        string username;
        string password;

        cout << "Username: ";
        getline(cin, username);

        cout << "Password: ";
        getline(cin, password);

        username = trim(username);
        password = trim(password);

        if (username == VALID_USERNAME &&
            password == VALID_PASSWORD)
        {
            cout << "\nLogin successful.\n";
            cout << "Welcome to FURNITURE VILLEGE.\n";

            return true;
        }

        cout << "\nInvalid username or password.\n";

        if (attempt < MAX_ATTEMPTS)
        {
            cout << "Attempts remaining: "
                 << MAX_ATTEMPTS - attempt
                 << "\n";
        }
    }

    return false;
}

// ============================================================
// LOAD FURNITURE
// ============================================================

void loadFurniture(vector<Furniture>& furniture)
{
    ifstream inputFile(FURNITURE_FILE);

    if (!inputFile)
    {
        return;
    }

    string line;

    while (getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream stream(line);

        Furniture item;
        string idText;
        string priceText;
        string quantityText;

        getline(stream, idText, '|');
        getline(stream, item.name, '|');
        getline(stream, item.category, '|');
        getline(stream, priceText, '|');
        getline(stream, quantityText, '|');

        try
        {
            item.id = stoi(idText);
            item.price = stod(priceText);
            item.quantity = stoi(quantityText);

            if (furniture.size() < MAX_FURNITURE)
            {
                furniture.push_back(item);
            }
        }
        catch (...)
        {
            // Ignore corrupted records rather than terminating
            // the entire application.
        }
    }

    inputFile.close();
}

// ============================================================
// SAVE FURNITURE
// ============================================================

void saveFurniture(const vector<Furniture>& furniture)
{
    ofstream outputFile(FURNITURE_FILE);

    if (!outputFile)
    {
        cout << "Error: furniture data could not be saved.\n";
        return;
    }

    outputFile << fixed << setprecision(2);

    for (const Furniture& item : furniture)
    {
        outputFile
            << item.id << "|"
            << item.name << "|"
            << item.category << "|"
            << item.price << "|"
            << item.quantity << "\n";
    }

    outputFile.close();
}

// ============================================================
// BACKUP FURNITURE
// ============================================================

void backupFurniture(const vector<Furniture>& furniture)
{
    ofstream backupFile(FURNITURE_BACKUP);

    if (!backupFile)
    {
        cout << "Warning: furniture backup could not be created.\n";
        return;
    }

    backupFile << fixed << setprecision(2);

    for (const Furniture& item : furniture)
    {
        backupFile
            << item.id << "|"
            << item.name << "|"
            << item.category << "|"
            << item.price << "|"
            << item.quantity << "\n";
    }

    backupFile.close();
}

// ============================================================
// LOAD ORDERS
// ============================================================

void loadOrders(vector<Order>& orders)
{
    ifstream inputFile(ORDER_FILE);

    if (!inputFile)
    {
        return;
    }

    string line;

    while (getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream stream(line);

        Order order;
        string orderIdText;
        string furnitureIdText;
        string quantityText;
        string totalPriceText;

        getline(stream, orderIdText, '|');
        getline(stream, furnitureIdText, '|');
        getline(stream, order.furnitureName, '|');
        getline(stream, quantityText, '|');
        getline(stream, totalPriceText, '|');

        try
        {
            order.orderId = stoi(orderIdText);
            order.furnitureId = stoi(furnitureIdText);
            order.quantity = stoi(quantityText);
            order.totalPrice = stod(totalPriceText);

            if (orders.size() < MAX_ORDERS)
            {
                orders.push_back(order);
            }
        }
        catch (...)
        {
            // Ignore invalid stored order records.
        }
    }

    inputFile.close();
}

// ============================================================
// SAVE ORDERS
// ============================================================

void saveOrders(const vector<Order>& orders)
{
    ofstream outputFile(ORDER_FILE);

    if (!outputFile)
    {
        cout << "Error: order data could not be saved.\n";
        return;
    }

    outputFile << fixed << setprecision(2);

    for (const Order& order : orders)
    {
        outputFile
            << order.orderId << "|"
            << order.furnitureId << "|"
            << order.furnitureName << "|"
            << order.quantity << "|"
            << order.totalPrice << "\n";
    }

    outputFile.close();
}

// ============================================================
// BACKUP ORDERS
// ============================================================

void backupOrders(const vector<Order>& orders)
{
    ofstream backupFile(ORDER_BACKUP);

    if (!backupFile)
    {
        cout << "Warning: order backup could not be created.\n";
        return;
    }

    backupFile << fixed << setprecision(2);

    for (const Order& order : orders)
    {
        backupFile
            << order.orderId << "|"
            << order.furnitureId << "|"
            << order.furnitureName << "|"
            << order.quantity << "|"
            << order.totalPrice << "\n";
    }

    backupFile.close();
}

// ============================================================
// FIND FURNITURE BY ID
// ============================================================

int findFurnitureById(
    const vector<Furniture>& furniture,
    int furnitureId)
{
    for (size_t i = 0; i < furniture.size(); ++i)
    {
        if (furniture[i].id == furnitureId)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// ============================================================
// DISPLAY FURNITURE
// ============================================================

void displayFurniture(const Furniture& furniture)
{
    cout << left
         << setw(8) << furniture.id
         << setw(25) << furniture.name
         << setw(20) << furniture.category
         << right
         << setw(12) << fixed << setprecision(2)
         << furniture.price
         << setw(10) << furniture.quantity
         << "\n";
}

// ============================================================
// ADD FURNITURE
// ============================================================

void addFurniture(vector<Furniture>& furniture)
{
    displayHeader("ADD FURNITURE");

    if (furniture.size() >= MAX_FURNITURE)
    {
        cout << "Furniture storage limit has been reached.\n";
        pauseScreen();
        return;
    }

    int id;

    while (true)
    {
        id = getPositiveInteger("Enter furniture ID: ");

        if (findFurnitureById(furniture, id) == -1)
        {
            break;
        }

        cout << "This furniture ID already exists.\n";
        cout << "Please enter a different ID.\n";
    }

    string name = getNonEmptyString(
        "Enter furniture name: "
    );

    string category = getNonEmptyString(
        "Enter furniture category: "
    );

    double price = getPositiveDouble(
        "Enter furniture price: "
    );

    int quantity = getPositiveInteger(
        "Enter available quantity: "
    );

    Furniture newFurniture;

    newFurniture.id = id;
    newFurniture.name = name;
    newFurniture.category = category;
    newFurniture.price = price;
    newFurniture.quantity = quantity;

    furniture.push_back(newFurniture);

    saveFurniture(furniture);
    backupFurniture(furniture);

    cout << "\nFurniture has been added successfully.\n";

    cout << "\nAdded record:\n";

    cout << left
         << setw(8) << "ID"
         << setw(25) << "Name"
         << setw(20) << "Category"
         << setw(12) << "Price"
         << setw(10) << "Stock"
         << "\n";

    cout << string(75, '-') << "\n";

    displayFurniture(newFurniture);

    pauseScreen();
}

// ============================================================
// LIST AVAILABLE FURNITURE
// ============================================================

void listAvailableFurniture(
    const vector<Furniture>& furniture)
{
    displayHeader("LIST AVAILABLE FURNITURE");

    bool found = false;

    cout << left
         << setw(8) << "ID"
         << setw(25) << "Name"
         << setw(20) << "Category"
         << setw(12) << "Price"
         << setw(10) << "Stock"
         << "\n";

    cout << string(75, '-') << "\n";

    for (const Furniture& item : furniture)
    {
        if (item.quantity > 0)
        {
            displayFurniture(item);
            found = true;
        }
    }

    if (!found)
    {
        cout << "No furniture is currently available.\n";
    }

    pauseScreen();
}

// ============================================================
// SEARCH SPECIFIC FURNITURE
// ============================================================

void searchFurniture(
    const vector<Furniture>& furniture)
{
    displayHeader("SEARCH SPECIFIC FURNITURE");

    if (furniture.empty())
    {
        cout << "There are no furniture records to search.\n";
        pauseScreen();
        return;
    }

    int id = getPositiveInteger(
        "Enter furniture ID to search: "
    );

    int index = findFurnitureById(furniture, id);

    if (index == -1)
    {
        cout << "\nFurniture with ID "
             << id
             << " was not found.\n";
    }
    else
    {
        cout << "\nFurniture record found:\n\n";

        cout << left
             << setw(8) << "ID"
             << setw(25) << "Name"
             << setw(20) << "Category"
             << setw(12) << "Price"
             << setw(10) << "Stock"
             << "\n";

        cout << string(75, '-') << "\n";

        displayFurniture(furniture[index]);
    }

    pauseScreen();
}

// ============================================================
// PLACE ORDER
// ============================================================

void placeOrder(
    vector<Furniture>& furniture,
    vector<Order>& orders)
{
    displayHeader("PLACE ORDER");

    if (furniture.empty())
    {
        cout << "There are no furniture records available.\n";
        cout << "An order cannot be placed until furniture is added.\n";
        pauseScreen();
        return;
    }

    int furnitureId = getPositiveInteger(
        "Enter furniture ID to order: "
    );

    int index = findFurnitureById(furniture, furnitureId);

    if (index == -1)
    {
        cout << "\nFurniture ID not found.\n";
        cout << "Please check the ID and try again.\n";
        pauseScreen();
        return;
    }

    Furniture& selectedFurniture = furniture[index];

    if (selectedFurniture.quantity <= 0)
    {
        cout << "\nThis furniture item is currently out of stock.\n";
        pauseScreen();
        return;
    }

    cout << "\nSelected furniture:\n";
    cout << "Name: " << selectedFurniture.name << "\n";
    cout << "Category: " << selectedFurniture.category << "\n";
    cout << "Price: £"
         << fixed << setprecision(2)
         << selectedFurniture.price << "\n";
    cout << "Available quantity: "
         << selectedFurniture.quantity
         << "\n";

    int quantity = getPositiveInteger(
        "\nEnter quantity required: "
    );

    if (quantity > selectedFurniture.quantity)
    {
        cout << "\nOrder cannot be completed.\n";
        cout << "Requested quantity: "
             << quantity
             << "\n";
        cout << "Available quantity: "
             << selectedFurniture.quantity
             << "\n";
        cout << "Please enter a quantity within the available stock.\n";

        pauseScreen();
        return;
    }

    double total = selectedFurniture.price * quantity;

    int nextOrderId = 1;

    for (const Order& existingOrder : orders)
    {
        if (existingOrder.orderId >= nextOrderId)
        {
            nextOrderId = existingOrder.orderId + 1;
        }
    }

    Order newOrder;

    newOrder.orderId = nextOrderId;
    newOrder.furnitureId = selectedFurniture.id;
    newOrder.furnitureName = selectedFurniture.name;
    newOrder.quantity = quantity;
    newOrder.totalPrice = total;

    orders.push_back(newOrder);

    selectedFurniture.quantity -= quantity;

    saveFurniture(furniture);
    saveOrders(orders);

    backupFurniture(furniture);
    backupOrders(orders);

    cout << "\n============================================================\n";
    cout << "ORDER CONFIRMATION\n";
    cout << "============================================================\n";

    cout << "Order ID:       "
         << newOrder.orderId << "\n";

    cout << "Furniture:      "
         << newOrder.furnitureName << "\n";

    cout << "Quantity:       "
         << newOrder.quantity << "\n";

    cout << "Unit Price:     £"
         << fixed << setprecision(2)
         << selectedFurniture.price << "\n";

    cout << "Total Price:    £"
         << fixed << setprecision(2)
         << newOrder.totalPrice << "\n";

    cout << "Remaining Stock:"
         << selectedFurniture.quantity << "\n";

    cout << "============================================================\n";

    pauseScreen();
}

// ============================================================
// HELP
// ============================================================

void displayHelp()
{
    displayHeader("FURNITURE VILLEGE HELP");

    cout << "\n1. Login\n";
    cout << "   Enter the authorised username and password.\n";

    cout << "\n2. Add Furniture\n";
    cout << "   Enter a unique furniture ID, name, category, price\n";
    cout << "   and available quantity.\n";

    cout << "\n3. List Available Furniture\n";
    cout << "   Displays furniture records where the available\n";
    cout << "   quantity is greater than zero.\n";

    cout << "\n4. Search Specific Furniture\n";
    cout << "   Enter a furniture ID to display the matching record.\n";

    cout << "\n5. Place Order\n";
    cout << "   Enter the furniture ID and required quantity.\n";
    cout << "   The quantity cannot exceed the available stock.\n";

    cout << "\n6. Help\n";
    cout << "   Displays these instructions.\n";

    cout << "\n7. Exit\n";
    cout << "   Closes the application safely.\n";

    cout << "\nInput guidance:\n";
    cout << "- Furniture IDs must be positive whole numbers.\n";
    cout << "- Quantities must be positive whole numbers.\n";
    cout << "- Prices must be positive numeric values.\n";
    cout << "- Required text fields cannot be left empty.\n";
    cout << "- Existing furniture IDs cannot be duplicated.\n";

    pauseScreen();
}

// ============================================================
// MAIN MENU
// ============================================================

void displayMainMenu()
{
    displayHeader("FURNITURE VILLEGE MAIN MENU");

    cout << "1. Add Furniture\n";
    cout << "2. List Available Furniture\n";
    cout << "3. Search Specific Furniture\n";
    cout << "4. Place Order\n";
    cout << "5. Help\n";
    cout << "6. Exit\n";

    cout << "\nPlease select an option from 1 to 6.\n";
}

// ============================================================
// MAIN SYSTEM LOOP
// ============================================================

void runSystem(
    vector<Furniture>& furniture,
    vector<Order>& orders)
{
    bool running = true;

    while (running)
    {
        displayMainMenu();

        int choice = getInteger(
            "\nEnter your choice: "
        );

        switch (choice)
        {
            case 1:
                addFurniture(furniture);
                break;

            case 2:
                listAvailableFurniture(furniture);
                break;

            case 3:
                searchFurniture(furniture);
                break;

            case 4:
                placeOrder(furniture, orders);
                break;

            case 5:
                displayHelp();
                break;

            case 6:
                running = false;
                cout << "\nExiting the system...\n";
                break;

            default:
                cout << "\nInvalid menu choice.\n";
                cout << "Please select an option from 1 to 6.\n";
                break;
        }
    }
}
