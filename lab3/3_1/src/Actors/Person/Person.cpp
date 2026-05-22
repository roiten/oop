#include "Person.h"
#include <limits>
#include <random>
#include <stdexcept>

Person::Person(Name name, Money cash)
	: m_name(name)
	, m_cash(0)
{
	AssertIsCashOverflow(cash);
	m_cash = cash;
	AssertIsMoneyPositive();
}

const Name Person::GetName() const
{
	return m_name;
}

Money Person::GetMoney() const
{
	return m_cash;
}

void Person::SpendCash(Money cash)
{
	AssertIsMoneyPositive();
	AssertIsTransferMoneyValid(cash);
	m_cash = m_cash - cash;
}

void Person::ReceiveCash(Money cash)
{
	AssertIsWalletSizeEnough(cash);
	m_cash = m_cash + cash;
}

void Person::TransferTo(Person& recipient, Money cash)
{
	SpendCash(cash);
	recipient.ReceiveCash(cash);
}

bool Person::RandomChance(int success, int total)
{
	static thread_local std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(1, total);
	return dist(gen) <= success;
}

void Person::AssertIsEnoughMoney(Money money) const
{
	if (GetMoney() < money)
	{
		throw std::runtime_error("денег для транзакции недостаточно");
	}
}

void Person::AssertIsMoneyPositive() const
{
	if (m_cash < 0)
	{
		throw std::invalid_argument("номинал должен быть положительным!");
	}
}

void Person::AssertIsTransferMoneyValid(Money cash) const
{
	if (cash > m_cash)
	{
		throw std::runtime_error("You can only transfer your amount of money");
	}
}

void Person::AssertIsWalletSizeEnough(Money cash) const
{
	if (m_cash > std::numeric_limits<Money>::max() - cash)
	{
		throw std::runtime_error("There is no place for money");
	}
}

void Person::AssertIsCashOverflow(Money cash) const
{
	if (cash >= std::numeric_limits<Money>::max())
	{
		throw std::runtime_error("Overflow money");
	}
}