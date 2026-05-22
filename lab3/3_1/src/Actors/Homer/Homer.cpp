#include "Homer.h"

Homer::Homer(Name name, Money cash, Bank& bank, ContactList& contact)
	: PersonWithAccount(name, cash, bank)
	, m_contacts(contact)
{
	m_contacts.AddPersonWithAccount(*this);
	OpenAccount();
}

void Homer::Step()
{
	GiveMoneyToMarge();
	PayElectricity();
	GiveMoneyToKids();
}

void Homer::GiveMoneyToMarge()
{
	auto& marge = m_contacts.GetAccountPerson(Name::Marge);
	SendMoney(marge.GetAccountId(), toMarge);
	std::cout << "Гомер передал деньги Мардж " << toMarge << std::endl;
}

void Homer::PayElectricity()
{
	auto& berns = m_contacts.GetAccountPerson(Name::Berns);
	SendMoney(berns.GetAccountId(), toBerns);
	std::cout << "Гомер платит за электричество (Бёрнсу) " << toBerns << std::endl;
}

void Homer::GiveMoneyToKids()
{
	Withdraw(toLisa + toBart);

	auto& lisa = m_contacts.GetPerson(Name::Lisa);
	TransferTo(lisa, toLisa);
	std::cout << "Гомер раздаёт деньги Лизе " << toLisa << std::endl;

	auto& bart = m_contacts.GetPerson(Name::Bart);
	TransferTo(bart, toBart);
	std::cout << "Гомер раздаёт деньги Барту " << toLisa << std::endl;
}