#include "../Marge/Marge.h"

Marge::Marge(Name name, Money cash, Bank& bank, ContactList& contact)
	: PersonWithAccount(name, cash, bank)
	, m_contacts(contact)
{
	m_contacts.AddPersonWithAccount(*this);
	OpenAccount();
}

void Marge::Step()
{
	PayForProducts();
}

void Marge::PayForProducts()
{
	auto& apu = m_contacts.GetAccountPerson(Name::Apu);
	SendMoney(apu.GetAccountId(), toApu);
	std::cout << "Мардж купила продукты у Апу " << toApu << std::endl;
}