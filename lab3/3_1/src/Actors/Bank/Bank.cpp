#include "Bank.h"

Bank::Bank(Money cash)
    : m_cash(cash)
{
    AssertIsNonNegativeMoney(cash);
}

Money Bank::GetCash() const
{
    return m_cash;
}

Money Bank::GetDeposits() const
{
    Money deposits = 0;
    for (const auto& [id, deposit] : m_accounts)
    {
        deposits += deposit;
    }
    return deposits;
}

Money Bank::GetAccountBalance(AccountId id) const
{
    AssertIsAccountExist(id);
    return m_accounts.at(id);
}

AccountId Bank::OpenAccount()
{
    auto id = m_nextAccountId++;
    m_accounts[id] = 0;
    return id;
}

Money Bank::CloseAccount(AccountId id)
{
    auto balance = GetAccountBalance(id);
    m_accounts.erase(m_accounts.find(id));
    m_cash += balance;
    return balance;
}

void Bank::DepositMoney(AccountId id, Money amount)
{
    AssertIsNonNegativeMoney(amount);
    AssertIsAccountExist(id);
    AssertIsAmountIsMoreCash(amount);

    m_accounts[id] += amount;
    m_cash -= amount;
}

bool Bank::TryDepositMoney(AccountId id, Money amount)
{
    try {
        DepositMoney(id, amount);
        return true;
    } catch (const BankOperationError&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void Bank::WithdrawMoney(AccountId id, Money amount)
{
    AssertIsNonNegativeMoney(amount);
    AssertIsAccountExist(id);
    AssertIsAmountValid(id, amount);

    m_accounts[id] -= amount;
    m_cash += amount;
}

bool Bank::TryWithdrawMoney(AccountId id, Money amount)
{
    try {
        WithdrawMoney(id, amount);
        return true;
    } catch (const BankOperationError&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void Bank::SendMoney(AccountId srcId, AccountId dstId, Money amount)
{
    AssertIsNonNegativeMoney(amount);
    AssertIsAccountExist(srcId);
    AssertIsAccountExist(dstId);
    AssertIsAmountValid(srcId, amount);

    m_accounts[srcId] -= amount;
    m_accounts[dstId] += amount;
}

bool Bank::TrySendMoney(AccountId srcId, AccountId dstId, Money amount)
{
    try {
        SendMoney(srcId, dstId, amount);
        return true;
    } catch (const BankOperationError&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void Bank::AssertIsAccountExist(AccountId id) const
{
    if (m_accounts.find(id) == m_accounts.end())
    {
        throw BankOperationError("Аккаунт не существует");
    }
}

void Bank::AssertIsNonNegativeMoney(Money money)
{
    if (money < 0)
    {
        throw std::out_of_range("Отрицательное количество денег не доступно");
    }
}

void Bank::AssertIsAmountValid(AccountId id, Money money) const
{
    if (m_accounts.at(id) < money)
    {
        throw BankOperationError("Счёт не содержит столько денег");
    }
}

void Bank::AssertIsAmountIsMoreCash(Money money) const
{
    if (m_cash < money)
    {
        throw BankOperationError("Количество денег больше денежной массы");
    }
}