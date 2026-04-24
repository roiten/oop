#include "Apu.h"

Apu::Apu(Name name, Money cash, Bank& bank, ContactList& contact)
	: PersonWithAccount(name, cash, bank)
	, m_contacts(contact)
{
	m_contacts.AddPersonWithAccount(*this);
	OpenAccount();
}

void Apu::Step()
{
	if (GetMoney() > 0)
	{
		DepositCash();
	}
	PayElectricity();
}

void Apu::PayElectricity()
{
	auto& berns = m_contacts.GetAccountPerson(Name::Berns);
	SendMoney(berns.GetAccountId(), toBerns);
	std::cout << "Апу платит за электричество (Бёрнсу) " << toBerns << std::endl;
}

void Apu::DepositCash()
{
	Deposit(GetMoney());
	std::cout << "Апу вносит деньги в банк " << GetMoney() << std::endl;
}