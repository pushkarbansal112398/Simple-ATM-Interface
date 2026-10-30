#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;
class Account
{
private:
    string accNo;
    string pin;
    double balance;

public:
    Account(string a, string p, double b)
    {
        accNo = a;
        pin = p;
        balance = b;
    }
    string getAccNo()
    {
        return accNo;
    }
    bool authenticate(string p)
    {
        return pin == p;
    }
    double getBalance()
    {
        return balance;
    }
    void deposit(double amount)
    {
        balance += amount;
    }
    bool withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            return true;
        }
        return false;
    }
    void save(ofstream &file)
    {
        file << accNo << " " << pin << " " << fixed << setprecision(2) << balance << endl;
    }
};
class ATM
{
private:
    vector<Account> accounts;
    void loadData()
    {
        ifstream file("accounts.txt");
        if (!file)
        {
            ofstream create("accounts.txt");
            create << "1001 1234 5000.00\n";
            create << "1002 5678 10000.00\n";
            create.close();
            file.open("accounts.txt");
        }
        string acc, pin;
        double bal;
        while (file >> acc >> pin >> bal)
        {
            accounts.emplace_back(acc, pin, bal);
        }
        file.close();
    }
    void saveData()
    {
        ofstream file("accounts.txt");
        for (auto &acc : accounts)
        {
            acc.save(file);
        }
        file.close();
    }
    int findAccount(string accNo)
    {
        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i].getAccNo() == accNo)
            {
                return i;
            }
        }
        return -1;
    }
    void createAccount()
    {
        string accNo, pin;
        double amount;
        cout << "Enter new account number: ";
        cin >> accNo;
        if (findAccount(accNo) != -1)
        {
            cout << "Account already exists!" << endl;
            return;
        }
        cout << "Set a 4-digit PIN: ";
        cin >> pin;
        if (pin.length() != 4 || pin.find_first_not_of("0123456789") != string::npos)
        {
            cout << "Invalid PIN format! Use exactly 4 digits." << endl;
            return;
        }
        cout << "Enter initiial deposit amount: Rs. ";
        cin >> amount;
        if (amount < 0)
        {
            cout << "Invalid deposit amount!" << endl;
            return;
        }
        accounts.emplace_back(accNo, pin, amount);
        saveData();
        cout << "Account created successfully!" << endl;
    }
    void userMenu(int index)
    {
        int choice;
        double amount;
        do
        {
            cout << "\n======= ATM Menu =======\n";
            cout << "1. Check Balance\n";
            cout << "2. Withdraw Money\n";
            cout << "3. Deposit Money\n";
            cout << "4. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Current Balance: Rs. "
                     << fixed << setprecision(2)
                     << accounts[index].getBalance() << endl;
                break;

            case 2:
                cout << "Enter withdrawal amount: Rs. ";
                cin >> amount;

                if (accounts[index].withdraw(amount))
                {
                    saveData();
                    cout << "Please collect your cash.\n";
                    cout << "Remaining Balance: Rs. "
                         << accounts[index].getBalance() << endl;
                }
                else
                {
                    cout << "Invalid amount or insufficient balance!\n";
                }
                break;

            case 3:
                cout << "Enter deposit amount: Rs. ";
                cin >> amount;

                if (amount > 0)
                {
                    accounts[index].deposit(amount);
                    saveData();
                    cout << "Amount deposited successfully!\n";
                    cout << "Updated Balance: Rs. "
                         << accounts[index].getBalance() << endl;
                }
                else
                {
                    cout << "Invalid deposit amount!\n";
                }
                break;

            case 4:
                cout << "Logged out successfully.\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 4);
    }

public:
    ATM()
    {
        loadData();
    }
    void start()
    {
        int choice;
        string accNo, pin;
        do
        {
            cout << "\n======= Welcome to ATM =======\n";
            cout << "1. Login\n";
            cout << "2. Create New Account\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                cout << "Enter account number: ";
                cin >> accNo;
                int index = findAccount(accNo);
                if (index == -1)
                {
                    cout << "Account not found!" << endl;
                    break;
                }
                cout << "Enter PIN: ";
                cin >> pin;
                if (accounts[index].authenticate(pin))
                {
                    cout << "Login successful!" << endl;
                    userMenu(index);
                }
                else
                {
                    cout << "Incorrect PIN!" << endl;
                }
            }
            break;

            case 2:
                createAccount();
                break;

            case 3:
                cout << "Thank you for using the ATM. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice! Try again." << endl;
            }
        } while (choice != 3);
    }
};
int main()
{
    ATM atm;
    atm.start();
    return 0;
}