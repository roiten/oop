#pragma once
#include "../../types.h"
#include "../Bank/Bank.h"
#include "../Person/Person.h"
#include <optional>

class PersonWithAccount : public Person
{
public:
	explicit PersonWithAccount(Name name, Money cash, Bank& bank);
	PersonWithAccount(const PersonWithAccount&) = delete;
	PersonWithAccount& operator=(const PersonWithAccount&) = delete;

	[[nodiscard]] AccountId GetDeposit() const;
	[[nodiscard]] AccountId GetAccountId() const;

	void OpenAccount();
	void CloseAccount();
	void Deposit(Money cash);
	void Withdraw(Money cash);
	void SendMoney(AccountId dstId, Money amount);

private:
	void AssertIsAccountIdExist() const;
	Bank& m_bank;
	std::optional<AccountId> m_id;
};
