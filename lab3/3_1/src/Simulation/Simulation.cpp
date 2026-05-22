#include "Simulation.h"

#include "../Actors/Apu/Apu.h"
#include "../Actors/Bart/Bart.h"
#include "../Actors/Berns/Berns.h"
#include "../Actors/Homer/Homer.h"
#include "../Actors/Lisa/Lisa.h"
#include "../Actors/Marge/Marge.h"
#include "../utils.h"
#include <iomanip>

Simulation::Simulation(const Money money, const int days)
	: m_simulationMoney(money)
	, m_days(days)
	, m_bank(money)
{
	AssertIsNumberValid(days);
	AssertIsNumberValid(money);
	m_days = days;

	m_actors.personsWithAccount.emplace_back(std::make_unique<Homer>(Name::Homer, 0, m_bank, m_contacts));
	m_actors.personsWithAccount.emplace_back(std::make_unique<Marge>(Name::Marge, 0, m_bank, m_contacts));
	m_actors.personsWithAccount.emplace_back(std::make_unique<Apu>(Name::Apu, 0, m_bank, m_contacts));
	m_actors.personsWithAccount.emplace_back(std::make_unique<Berns>(Name::Berns, 0, m_bank, m_contacts));

	m_actors.persons.emplace_back(std::make_unique<Lisa>(Name::Lisa, 0, m_contacts));
	m_actors.persons.emplace_back(std::make_unique<Bart>(Name::Bart, 0, m_contacts));

	DivisionMoney(money);
}

void Simulation::RunSimulation()
{
	for (size_t day = 1; day <= m_days; ++day)
	{
		std::cout << std::endl
				  << "День " << day << std::endl;
		std::cout << "______" << std::endl;

		for (auto& actor : m_actors.personsWithAccount)
		{
			try
			{
				actor->Step();
			}
			catch (const std::exception& error)
			{
				std::cout << NameToString(actor->GetName()) << " пропустил ход: " << error.what() << std::endl;
			}
		}
		for (auto& actor : m_actors.persons)
		{
			try
			{
				actor->Step();
			}
			catch (const std::exception& error)
			{
				std::cout << "Персонаж пропустил этот ход: " << error.what() << std::endl;
			}
		}

		ShowBalances();
	}
	PrintResults();
}

void Simulation::PrintResults() const
{
	std::cout << "\nИтоги:" << std::endl;

	Money sum = 0;
	for (auto& actor : m_actors.personsWithAccount)
	{
		sum += actor->GetMoney();
	}
	for (auto& actor : m_actors.persons)
	{
		sum += actor->GetMoney();
	}

	// Сумма наличных денег у персонажей совпадает с суммой наличных денег,
	// зарегистрированных в банке.

	std::cout << "ВАЛЮТА " << std::endl;
	std::cout << "  Валюты у персонажей: " << sum << std::endl;
	std::cout << "  Валюты лежит в банке:   " << m_bank.GetCash() << std::endl;

	// Общая сумма всех наличных и безналичных денег на счетах равна сумме, которая
	// изначально была положена в банк в виде наличных.

	std::cout << "  Сумма валюты у персонажей равна изначально эмиссированной сумме? ";

	if (sum + m_bank.GetDeposits() == m_simulationMoney)
	{
		std::cout << "[да]" << std::endl;
	}
	else
	{
		std::cout << "[нет]" << std::endl;
	}
}

const std::string& ToString(Name name);

void Simulation::ShowBalances() const
{
	std::cout << "\nБалансы персонажей:\n";
	for (const auto& actor : m_actors.personsWithAccount)
	{
		std::cout << ToString(actor->GetName())
				  << "\n  В банке: " << m_bank.GetAccountBalance(actor->GetAccountId())
				  << "\n  Наличкой: " << actor->GetMoney() << "\n";
	}
	for (const auto& actor : m_actors.persons)
	{
		std::cout << ToString(actor->GetName()) << ": " << "Наличкой: " << actor->GetMoney() << "\n";
	}
}

void Simulation::DivisionMoney(Money money)
{
	size_t totalActors = m_actors.persons.size() + m_actors.personsWithAccount.size();
	if (totalActors == 0)
		return;

	Money cashForPeople = money / static_cast<Money>(totalActors);
	Money distributed = 0;

	for (const auto& actor : m_actors.personsWithAccount)
	{
		actor->ReceiveCash(cashForPeople);
		distributed += cashForPeople;
		actor->Deposit(actor->GetMoney());
	}
	for (const auto& actor : m_actors.persons)
	{
		actor->ReceiveCash(cashForPeople);
		distributed += cashForPeople;
	}

	Money remainder = money - distributed;
	if (remainder > 0)
	{
		if (!m_actors.personsWithAccount.empty())
			m_actors.personsWithAccount.front()->ReceiveCash(remainder);
		else if (!m_actors.persons.empty())
			m_actors.persons.front()->ReceiveCash(remainder);
	}
}

void Simulation::AssertIsNumberValid(Number number)
{
	if (number < 0)
	{
		throw std::invalid_argument(
			"Число должно быть натуральным и больше чем 100");
	}
}
const std::string& ToString(Name name)
{
	static const std::string homer = "Homer";
	static const std::string marge = "Marge";
	static const std::string lisa = "Lisa";
	static const std::string bart = "Bart";
	static const std::string apu = "Apu";
	static const std::string berns = "Berns";
	static const std::string nelson = "Nelson";
	static const std::string chester = "Chester";
	static const std::string waylon = "Waylon";
	static const std::string unknown = "Unknown";

	switch (name)
	{
	case Name::Homer:
		return homer;
	case Name::Marge:
		return marge;
	case Name::Lisa:
		return lisa;
	case Name::Bart:
		return bart;
	case Name::Apu:
		return apu;
	case Name::Berns:
		return berns;
	default:
		return unknown;
	}
}