#include "PersonWithAccount.h"
#include "../Bank/Bank.h"
#include <iostream>

PersonWithAccount::PersonWithAccount(Name name, Money cash, Bank& bank)
	: Person(name, cash)
	, m_bank(bank)
	, m_id(std::nullopt)
{
}

AccountId PersonWithAccount::GetDeposit() const
{
	AssertIsAccountIdExist();
	return m_bank.GetAccountBalance(m_id.value());
}

AccountId PersonWithAccount::GetAccountId() const
{
	AssertIsAccountIdExist();
	return m_id.value();
}

void PersonWithAccount::OpenAccount()
{
	m_id = m_bank.OpenAccount();
}

void PersonWithAccount::CloseAccount()
{
	if (!m_id.has_value())
	{
		return;
	}
	auto cash = m_bank.CloseAccount(this->GetAccountId());
	ReceiveCash(cash);
	m_id.reset();
}

void PersonWithAccount::Deposit(Money cash)
{
	SpendCash(cash);
	try
	{
		m_bank.DepositMoney(this->GetAccountId(), cash);
	}
	catch (const BankOperationError& error)
	{
		ReceiveCash(cash);
	}
}

void PersonWithAccount::Withdraw(Money cash)
{
	m_bank.WithdrawMoney(this->GetAccountId(), cash);
	try
	{
		ReceiveCash(cash);
	}
	catch (const BankOperationError& error)
	{
		m_bank.DepositMoney(this->GetAccountId(), cash);
	}
}

void PersonWithAccount::SendMoney(AccountId dstId, Money amount)
{
	m_bank.SendMoney(this->GetAccountId(), dstId, amount);
}

void PersonWithAccount::AssertIsAccountIdExist() const
{
	if (!m_id.has_value())
	{
		throw std::runtime_error("Account ID is not set");
	}
}