#pragma once

#include "../../types.h"
#include <iostream>
#include <unordered_map>

class BankOperationError : public std::runtime_error
{
public:
	explicit BankOperationError(const std::string& message)
		: std::runtime_error("Банковская ошибка: " + message)
	{
	}
};

class Bank
{
public:
	explicit Bank(Money cash);

	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	[[nodiscard]] Money GetCash() const;
	[[nodiscard]] Money GetDeposits() const;
	[[nodiscard]] Money GetAccountBalance(AccountId id) const;

	// Открытие счёта в банке.
	// Функция возвращает номер счёта, баланс пуст.
	[[nodiscard]] AccountId OpenAccount();

	// Закрытие счёта в банке.
	// Функция возвращает деньги оставшиеся на счёте.
	// Эти деньги переходят в наличный оборот.
	[[nodiscard]] Money CloseAccount(AccountId id);

	// Пополнение счёта наличными деньгами.
	// Транзакционная проверка.
	// Количество денег в наличном обороте уменьшается на величину amount.
	void DepositMoney(AccountId id, Money amount);

	// Снятие денег со счёта.
	// Транзакционная проверка.
	// Снятые деньги переходят добавляются к массе наличных денег.
	void WithdrawMoney(AccountId id, Money amount);

	// Перевод денег с исходного счёта на целевой.
	// Транзациронная проверка.
	void SendMoney(AccountId srcId, AccountId dstId, Money amount);

	// Проверка возможности перевода денег с исходного счёта на целевой.
	// Нельзя перевести отрицательное количество денег.
	// Нельзя перевести с несуществующего счёта.
	// Нельзя перевести на несуществующий счёт.
	// Нельзя перевести больше, чем есть на исходном счёте.
	[[nodiscard]] bool TrySendMoney(AccountId srcId, AccountId dstId, Money amount);

	// Проверка возможности снятия денег со счёта.
	// Нельзя снять со счёта отрицательное количество денег.
	// Нельзя снять деньги с отсутствующего счёта.
	// Нельзя снять больше чем есть на счёте.
	[[nodiscard]] bool TryWithdrawMoney(AccountId id, Money amount);

	// Проверка возможности пополнения счёта наличными деньгами.
	// Нельзя поместить на счёт отрицательное количество денег.
	// Нельзя поместить больше, чем имеется денег в наличном обороте.
	// Нельзя поместить деньги на отсутствующий счёт.
	[[nodiscard]] bool TryDepositMoney(AccountId account, Money amount);

private:
	void AssertIsAccountExist(AccountId id) const;
	static void AssertIsNonNegativeMoney(Money amount);
	void AssertIsAmountValid(AccountId id, Money money) const;
	void AssertIsAmountIsMoreCash(Money money) const;

	Money m_cash = 0;
	AccountId m_nextAccountId = 0;
	std::unordered_map<AccountId, Money> m_accounts;
};

