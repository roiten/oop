#pragma once
#include "../../types.h"

#include <string>

class Person
{
public:
	Person(Name name, Money cash);
	Person(Person&&) = default;
	Person& operator=(Person&&) = default;
	Person(const Person&) = delete;
	Person& operator=(const Person&) = delete;
	virtual ~Person() = default;

	[[nodiscard]] const Name GetName() const;
	[[nodiscard]] Money GetMoney() const;

	virtual void Step() = 0;
	void SpendCash(Money cash);
	void ReceiveCash(Money cash);
	void TransferTo(Person& recipient, Money cash);

protected:
	bool RandomChance(int success, int total);
	void AssertIsEnoughMoney(Money money) const;
	void AssertIsCashOverflow(Money cash) const;
	void AssertIsMoneyPositive() const;
	void AssertIsTransferMoneyValid(Money cash) const;
	void AssertIsWalletSizeEnough(Money cash) const;

private:
	const Name m_name;
	Money m_cash;
};

