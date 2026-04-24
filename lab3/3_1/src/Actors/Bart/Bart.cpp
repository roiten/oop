#include "Bart.h"

Bart::Bart(Name name, Money cash, ContactList& contact)
	: Person(name, cash)
	, m_contacts(contact)
{
	m_contacts.AddPerson(*this);
}

void Bart::Step()
{
	PayForBubbleGums();
}

void Bart::PayForBubbleGums()
{
	auto& apu = m_contacts.GetAccountPerson(Name::Apu);
	TransferTo(apu, toApu);
	std::cout << "Барт потратил деньги у Апу " << toApu << std::endl;
}