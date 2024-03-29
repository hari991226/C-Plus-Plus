
#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

void creatingFiles()
{
	std::fstream wallets("wallets.dat", std::ios::binary | std::ios::app);
	wallets.close();
	std::fstream transactions("transactions.dat", std::ios::binary | std::ios::app);
	transactions.close();
	std::fstream orders("orders.dat", std::ios::binary | std::ios::app);
	orders.close();
}

class TransactionContainer
{
private:
	struct Transaction
	{
		long long time = std::time(0);
		unsigned senderId;
		unsigned receiverId;
		double fmiCoins;
	};

	unsigned capacity;
	unsigned size;
	Transaction* transactions;

	//when adding new transaction
	void resize()
	{
		Transaction* temp = transactions;
		capacity *= 2;
		transactions = new Transaction[capacity];
		for (int i = 0; i < size; ++i)
		{
			transactions[i] = temp[i];
		}
		delete[] temp;
	}

	//when reading from the file
	void setNewTransactionCapacity()
	{
		std::ifstream infile("transactions.dat", std::ios::binary | std::ios::app);
		infile.seekg(0, std::ios::end);
		size = infile.tellg() / sizeof(Transaction);
		capacity = size + 1;
		delete[] transactions;
		transactions = new Transaction[capacity];
		infile.close();
	}


public:
	TransactionContainer()
	{
		size = 0;
		capacity = 1;
		transactions = new Transaction[capacity];
	}

	~TransactionContainer()
	{
		delete[] transactions;
	}

	TransactionContainer(const Transaction& otherTransaction) = delete;

	Transaction& operator=(const Transaction& otherTransaction) = delete;

	void createTransaction(const unsigned newSenderId, const unsigned newReceiverId, const double newFmiCoins)
	{
		Transaction d;
		d.fmiCoins = newFmiCoins;
		d.receiverId = newReceiverId;
		d.senderId = newSenderId;
		addTransactionToConteiner(d);
	}


	void addTransactionToConteiner(const Transaction& tempTransaction)
	{
		if (size == capacity)
		{
			resize();
		}
		transactions[size++] = tempTransaction;
	}

	void writeTransactionToFile()const
	{
		std::ofstream outfile("transactions.dat", std::ios::binary | std::ios::trunc);
		if (!outfile) { exit(1); }
		outfile.write(reinterpret_cast<char*>(transactions), sizeof(Transaction) * size);
		outfile.close();
	}

	void readTransactionsFromFile()
	{
		std::ifstream infile("transactions.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		int length = infile.tellg();
		setNewTransactionCapacity();
		infile.seekg(0, std::ios::beg);
		infile.read(reinterpret_cast<char*>(transactions), length);
		infile.close();
	}

	bool checkingIfThereSavedTransactions()const
	{
		std::ifstream infile("transactions.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		int temp = infile.tellg();
		infile.seekg(0, std::ios::beg);
		infile.close();
		return temp;
	}

	void print()const
	{
		for (int i = 0; i < size; ++i)
		{
			std::cout << transactions[i].senderId << std::endl <<
				transactions[i].receiverId << std::endl << transactions[i].fmiCoins
				<< std::endl << transactions[i].time << std::endl;
		}
	}

	double findingNumberOfCoins(const unsigned walletId)const
	{
		double tempFmiCoins = 0;
		for (unsigned i = 0; i < size; ++i)
		{
			if (transactions[i].receiverId == walletId)
			{
				tempFmiCoins += transactions[i].fmiCoins;
			}
			else if (transactions[i].senderId == walletId)
			{
				tempFmiCoins -= transactions[i].fmiCoins;
			}
		}
		return tempFmiCoins;
	}

};

class WalletContainer
{
private:
	struct Wallet
	{
		char owner[256];
		unsigned id;
		double fiatMoney;
	};

	Wallet* wallets;
	unsigned capacity;
	unsigned size;
	int walletCounter;

	//resize for resizing when adding to the conteiner
	void resize()
	{
		Wallet* tmp = wallets;
		capacity *= 2;
		wallets = new Wallet[capacity];
		for (int i = 0; i < size; ++i)
		{
			wallets[i] = tmp[i];
		}
		delete[] tmp;
	}

	//resizing when reading from file
	void setNewCapacity()
	{
		std::ifstream infile("wallets.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		size = infile.tellg() / sizeof(Wallet);
		capacity = size + 1;
		delete[] wallets;
		wallets = new Wallet[capacity];
		infile.close();
	}

public:
	WalletContainer()
	{
		size = 0;
		capacity = 1;
		wallets = new Wallet[capacity];
		walletCounter = 0;
	}

	~WalletContainer()
	{
		delete[] wallets;
	}

	WalletContainer(const Wallet& otherWallet) = delete;

	Wallet& operator=(const Wallet& otherWallet) = delete;

	void addWallet(const double newFiatMoney, const char newName[256])
	{
		Wallet k;
		strcpy_s(k.owner, 256, newName);
		k.id = 1 + walletCounter;
		k.fiatMoney = newFiatMoney;
		addWalletToConteiner(k);
		walletCounter++;
	}

	unsigned getWalletId()const
	{
		return wallets[size - 1].id;
	}

	void addWalletToConteiner(const Wallet& newElement)
	{
		if (size == capacity)
		{
			resize();
		}
		wallets[size++] = newElement;
	}

	void print()const
	{
		for (int i = 0; i < size; ++i)
		{
			std::cout << wallets[i].owner << std::endl << wallets[i].id << std::endl
				<< wallets[i].fiatMoney << std::endl;
		}
	}

	void writingToFile()const
	{
		std::ofstream outfile("wallets.dat", std::ios::binary | std::ios::trunc);
		if (!outfile) { exit(1); }
		outfile.write(reinterpret_cast<char*>(wallets), sizeof(Wallet) * size);
		outfile.close();
	}

	void readingFromFile()
	{
		std::ifstream infile("wallets.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		int length = infile.tellg();
		setNewCapacity();
		infile.seekg(0, std::ios::beg);
		infile.read(reinterpret_cast<char *>(wallets), length);
		infile.close();
		if (size == 0)
		{
			walletCounter = 0;
		}
		else
		{
			walletCounter = wallets[size - 1].id;
		}
	}

	//change fiat money when SELLING transaction was made
	void changingFiatMoneyWhenSell(double tempCoins, const unsigned tempId)
	{
		wallets[tempId - 1].fiatMoney += 375.0 * tempCoins;
	}

	//change fiat money when BUYING transaction was made
	void changingFiatMoneyWhenBuy(double tempCoins, const unsigned tempId)
	{
		wallets[tempId - 1].fiatMoney -= 375.0 * tempCoins;
	}

	int getWalletCounter()const
	{
		return walletCounter;
	}


	bool checkingForIncorrectId(const unsigned ID)const
	{
		if (ID > 0 && ID <= wallets[size - 1].id)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	//getting fiat money via ID
	double getFiatMoney(const unsigned tempId)
	{
		return wallets[tempId - 1].fiatMoney;
	}

	void printInfoForCertainWallet(const unsigned tempId)const
	{
		std::cout << wallets[tempId - 1].owner << " " << wallets[tempId - 1].fiatMoney << std::endl;
	}

};

class OrderContainer
{
private:
	struct Order
	{
		enum Type { SELL, BUY } type;
		unsigned walletId;
		double fmiCoins;

		//setting the type of the order
		void settingTheType(const char enteredType[5])
		{
			if (!strcmp(enteredType, "SELL")) {
				type = Order::SELL;
			}
			else if (!strcmp(enteredType, "BUY")) {
				type = Order::BUY;
			}
			else {
				std::cout << "You've entered invalid type.";
				return;
			}
		}

		bool operator==(const Order& op)
		{
			if (walletId == op.walletId)
			{
				if (fmiCoins == op.fmiCoins)
				{
					if (type == op.type)
					{
						return true;
					}
					else return false;
				}
				else return false;
			}
			else return false;

		}
		Order& operator=(const Order& other)
		{
			if (this != &other)
			{
				walletId = other.walletId;
				fmiCoins = other.fmiCoins;
				type = other.type;
			}
			return *this;
		}
	};

	Order* orders;
	unsigned capacity;
	unsigned size;

	//resizing when adding to the conteiner
	void resize()
	{
		Order* temp = orders;
		capacity *= 2;
		orders = new Order[capacity];
		for (unsigned i = 0; i < size; ++i)
		{
			orders[i] = temp[i];
		}
		delete[] temp;
	}

	//set capacity when reading from file
	void setNewCapacity()
	{
		std::ifstream infile("orders.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		size = infile.tellg() / sizeof(Order);
		capacity = size + 1;
		delete[] orders;
		orders = new Order[capacity];
		infile.seekg(0, std::ios::beg);
		infile.close();
	}

public:
	OrderContainer()
	{
		size = 0;
		capacity = 1;
		orders = new Order[capacity];
	}

	~OrderContainer()
	{
		delete[] orders;
	}

	void printorder() const
	{
		for (int i = 0; i < size; i++)
		{
			std::cout << orders[i].walletId << " " << orders[i].fmiCoins << " " << orders[i].type << std::endl;
		}
	}

	void addOrderToConteiner(const Order& newOrder)
	{
		if (size == capacity)
		{
			resize();
		}
		orders[size++] = newOrder;
	}

	void removeOrderFromConteiner(const Order& removableOrder)
	{
		for (unsigned i = 0; i < size; i++)
		{
			if (orders[i] == removableOrder)
			{
				for (int j = i; j < size - 1; j++)
				{
					orders[i] = orders[j + 1];
				}
				size--;
				break;
			}
		}
	}

	void writeOrdersToFile()const
	{
		std::ofstream outfile("orders.dat", std::ios::binary | std::ios::trunc);
		if (!outfile) { exit(1); }
		outfile.write(reinterpret_cast<char*>(orders), size * sizeof(Order));
		outfile.close();
	}

	void readOrdersFromFile()
	{
		std::ifstream infile("orders.dat", std::ios::binary | std::ios::app);
		if (!infile) { exit(1); }
		infile.seekg(0, std::ios::end);
		unsigned length = static_cast<unsigned>(infile.tellg());
		setNewCapacity();
		infile.seekg(0, std::ios::beg);
		infile.read(reinterpret_cast<char*>(orders), length);
		infile.close();
	}



	//checking if there are already approved orders with this id
	double checkingSellingOrders(const unsigned enteredId, char typeOfOrder[5])const
	{
		double tempFmiCoins = 0;
		for (unsigned i = 0; i < size; ++i)
		{
			if (orders[i].walletId == enteredId && !strcmp(typeOfOrder, "SELL")) //no need for checking for inavalid char, because the function is executed only when the order type is sell
			{
				tempFmiCoins += orders[i].fmiCoins;
			}
		}
		return tempFmiCoins;
	}

	double checkingBuyingOrders(const unsigned enteredId, char typeOfOrder[5])const
	{
		double tempFiatMoney = 0;
		for (unsigned i = 0; i < size; ++i)
		{
			if (orders[i].walletId == enteredId && !strcmp(typeOfOrder, "BUY"))
			{
				tempFiatMoney += orders[i].fmiCoins;
			}
		}
		return tempFiatMoney * 375.0;
	}

	void MakingOrder(WalletContainer& a, TransactionContainer& b)
	{
		char type[5];
		unsigned orderId;
		double orderFmiCoins;

		std::cout << "Please enter your wallet's id: ";
		std::cin >> orderId;

		if (!a.checkingForIncorrectId(orderId))
		{
			std::cout << "Wallet with this id does not exist.";
			return;
		}

		std::cout << "Please enter the amount of coins you want to sell/buy: ";
		std::cin >> orderFmiCoins;

		if (orderFmiCoins <= 0)
		{
			std::cout << "You have entered invalid number of coins!";
			return;
		}


		std::cout << "Please enter the order type: ";
		std::cin >> type;

		if (!strcmp(type, "SELL"))
		{
			if (b.findingNumberOfCoins(orderId) < orderFmiCoins)
			{
				std::cout << "This wallet doesn't have so many fmi coins.";
				return;
			}
			else if (b.findingNumberOfCoins(orderId) - checkingSellingOrders(orderId, type) < orderFmiCoins)
			{
				std::cout << "You have already approved selling transactions with saved coins for them! ";
				return;
			}
			else
			{
				std::cout << "You made an order successfully!" << std::endl;
				Order newOrder;
				newOrder.settingTheType(type);
				newOrder.fmiCoins = orderFmiCoins;
				newOrder.walletId = orderId;
				addOrderToConteiner(newOrder);
				checkingForSuitableBuyingOrders(newOrder, b, a);
			}
		}
		else if (!strcmp(type, "BUY"))
		{
			if ((a.getFiatMoney(orderId) / 375.0) < orderFmiCoins)
			{
				std::cout << "Not enough money in the wallet! ";
				return;
			}
			else if ((a.getFiatMoney(orderId) - checkingBuyingOrders(orderId, type)) < (orderFmiCoins * 375.0))
			{
				std::cout << "You have already approved buying transactions with saved money for them! ";
				return;
			}
			else
			{
				std::cout << "You made an order successfully!" << std::endl;
				Order  newOrder;
				newOrder.settingTheType(type);
				newOrder.fmiCoins = orderFmiCoins;
				newOrder.walletId = orderId;
				addOrderToConteiner(newOrder);
				checkingForSuitableSellingOrders(newOrder, b, a);
			}
		}
		else
		{
			std::cout << "You have entered an invalid type of order! " << std::endl;
			return;
		}
	}

	//this function takes the the !!!SELL!!! order we have created and start checking for others which are suitable for making transaction
	void checkingForSuitableBuyingOrders(Order currentOrder, TransactionContainer& tempT, WalletContainer& tempW)
	{
		for (unsigned i = 0; i < size; ++i)
		{
			if (currentOrder.walletId != orders[i].walletId && currentOrder.type != orders[i].type)
			{
				if (currentOrder.fmiCoins > orders[i].fmiCoins)
				{
					currentOrder.fmiCoins -= orders[i].fmiCoins;
					tempT.createTransaction(currentOrder.walletId, orders[i].walletId, orders[i].fmiCoins);
					tempW.changingFiatMoneyWhenSell(orders[i].fmiCoins, currentOrder.walletId);
					tempW.changingFiatMoneyWhenBuy(orders[i].fmiCoins, orders[i].walletId);
					removeOrderFromConteiner(orders[i]);
				}
				else if (currentOrder.fmiCoins < orders[i].fmiCoins)
				{
					orders[i].fmiCoins -= currentOrder.fmiCoins;
					tempT.createTransaction(currentOrder.walletId, orders[i].walletId, currentOrder.fmiCoins);
					tempW.changingFiatMoneyWhenSell(currentOrder.fmiCoins, currentOrder.walletId);
					tempW.changingFiatMoneyWhenBuy(currentOrder.fmiCoins, orders[i].walletId);
					removeOrderFromConteiner(currentOrder);
				}
				else
				{
					tempT.createTransaction(currentOrder.walletId, orders[i].walletId, orders[i].fmiCoins);
					tempW.changingFiatMoneyWhenSell(orders[i].fmiCoins, currentOrder.walletId);
					tempW.changingFiatMoneyWhenBuy(orders[i].fmiCoins, orders[i].walletId);
					removeOrderFromConteiner(orders[i]);
					removeOrderFromConteiner(currentOrder);
				}
			}
		}
	}

	//this function takes the the !!!BUY!!! order we have created and start checking for others which are suitable for making transaction
	void checkingForSuitableSellingOrders(Order currentOrder, TransactionContainer& tempT, WalletContainer& tempW)
	{
		for (unsigned i = 0; i < size; ++i)
		{
			if (currentOrder.walletId != orders[i].walletId && currentOrder.type != orders[i].type)
			{
				if (currentOrder.fmiCoins > orders[i].fmiCoins)
				{
					currentOrder.fmiCoins -= orders[i].fmiCoins;
					tempT.createTransaction(orders[i].walletId, currentOrder.walletId, orders[i].fmiCoins);
					tempW.changingFiatMoneyWhenSell(orders[i].fmiCoins, orders[i].walletId);
					tempW.changingFiatMoneyWhenBuy(orders[i].fmiCoins, currentOrder.walletId);
					removeOrderFromConteiner(orders[i]);
				}
				else if (currentOrder.fmiCoins < orders[i].fmiCoins)
				{
					orders[i].fmiCoins -= currentOrder.fmiCoins;
					tempT.createTransaction(orders[i].walletId, currentOrder.walletId, currentOrder.fmiCoins);
					tempW.changingFiatMoneyWhenSell(currentOrder.fmiCoins, orders[i].walletId);
					tempW.changingFiatMoneyWhenBuy(currentOrder.fmiCoins, currentOrder.walletId);
					removeOrderFromConteiner(currentOrder);
				}
				else
				{
					tempT.createTransaction(orders[i].walletId, currentOrder.walletId, orders[i].fmiCoins);
					tempW.changingFiatMoneyWhenSell(orders[i].fmiCoins, orders[i].walletId);
					tempW.changingFiatMoneyWhenBuy(orders[i].fmiCoins, currentOrder.walletId);
					removeOrderFromConteiner(orders[i]);
					removeOrderFromConteiner(currentOrder);
				}
			}
		}
	}
};

class ManageEverything
{
private:
	WalletContainer a;
	TransactionContainer b;
	OrderContainer c;
public:
	void MakingWallet()
	{
		std::cout << "Please enter the owner's name: ";
		char name[256];
		std::cin >> name;
		std::cout << "Please enter the initial investment: ";
		double fiatMoney;
		std::cin >> fiatMoney;
		if (fiatMoney < 0) { std::cout << "Invalid money" << std::endl; return; }
		a.addWallet(fiatMoney, name);
		b.createTransaction(4294967295, a.getWalletId(), fiatMoney / 375.0);
		std::cout << "Successfully created a wallet, your id is: " << a.getWalletId() << std::endl;	
		std::cout << "You can choose a new command now:" << std::endl;
		
	}
	void readAllContainersFromFile()
	{
		a.readingFromFile();
		b.readTransactionsFromFile();
		c.readOrdersFromFile();
	}
	void MakingOrder()
	{
		c.MakingOrder(a, b);
	}
	void transefer()
	{
		unsigned senderId;
		unsigned receiverId;
		double coinsForCharity;
		std::cout << "You've entered our charity transaction programme. " << std::endl
			<< "Please enter the sender's id: ";
		std::cin >> senderId;
		if (!a.checkingForIncorrectId(senderId))
		{
			std::cout << "Wallet with this id does not exist." << std::endl;
			return;
		}

		std::cout << "Please enter the receiver's id: ";
		std::cin >> receiverId;
		if (!a.checkingForIncorrectId(receiverId))
		{
			std::cout << "Wallet with this id does not exist." << std::endl;
			return;
		}

		std::cout << "Please enter the amount of the donation. ";
		std::cin >> coinsForCharity;

		if (b.findingNumberOfCoins(senderId) < coinsForCharity)
		{
			std::cout << "The sender doesnt have that amount of money!" << std::endl;
			return;
		}

		b.createTransaction(senderId, receiverId, coinsForCharity);
	}

	void writeAllContainersToFile()
	{
		a.writingToFile();
		b.writeTransactionToFile();
		c.writeOrdersToFile();
	}

	void WalletInfo()const
	{
		unsigned tempWalletId;
		std::cout << "Please enter the id of the wallet that you want to check: ";
		std::cin >> tempWalletId;

		if (!a.checkingForIncorrectId(tempWalletId))
		{
			std::cout << "Wallet with this id does not exist.";
			return;
		}

		a.printInfoForCertainWallet(tempWalletId);
		std::cout << b.findingNumberOfCoins(tempWalletId);
	}

	//premahni
	void print()const
	{
		b.print();
		std::cout << "------------------------------------------\n\n";
		c.printorder();
	}
};

int main()
{
	ManageEverything a;
	a.readAllContainersFromFile();

	std::cout << "Choose from the existing commands:" << std::endl
		<< "1. add-wallet" << std::endl
		<< "2. make-order" << std::endl
		<< "3. transfer" << std::endl
		<< "4. wallet-info" << std::endl
		<< "5. attract-investros" << std::endl
		<< "6. quit" << std::endl;
		
	char command[30];
	std::cin >> command;
	while (strcmp(command, "quit"))
	{
		if (!strcmp(command, "add-wallet"))
		{
			a.MakingWallet();
			std::cin >> command;
		}
		else if (!strcmp(command, "make-order"))
		{
			a.MakingOrder();
			std::cin >> command;
		}
		else if (!strcmp(command, "transfer"))
		{
			a.transefer();
			std::cin >> command;
		}
		else if (!strcmp(command, "wallet-info"))
		{
			a.WalletInfo();
			std::cin >> command;
		}
		else if (!strcmp(command, "attract-investors"))
		{

		}
		else
		{
			std::cout << "Incorrect command, please eneter a new one!: " << std::endl;
			std::cin >> command;
		}
	}
	std::cout << "You have exited the programme, all the data will be saved to files." << std::endl;
	a.writeAllContainersToFile();
	a.print();
}

