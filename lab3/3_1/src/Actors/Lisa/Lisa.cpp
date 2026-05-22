#include "Lisa.h"

Lisa::Lisa(Name name, Money cash, ContactList& contact)
	: Person(name, cash)
	, m_contacts(contact)
{
	m_contacts.AddPerson(*this);
}

void Lisa::Step()
{
	PayForBooks();
}

void Lisa::PayForBooks()
{
	auto& apu = m_contacts.GetAccountPerson(Name::Apu);
	TransferTo(apu, toApu);
	std::cout << "Лиза купила книги у Апу " << toApu << std::endl;
}
