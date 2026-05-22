#include "Berns.h"

Berns::Berns(Name name, Money cash, Bank& bank, ContactList& contact)
	: PersonWithAccount(name, cash, bank)
	, m_contacts(contact)
{
	m_contacts.AddPersonWithAccount(*this);
	OpenAccount();
}

void Berns::Step()
{
	PaySalary();
}

void Berns::PaySalary()
{
	auto& homer = m_contacts.GetAccountPerson(Name::Homer);
	SendMoney(homer.GetAccountId(), toEmployer);
	std::cout << "Бёрнс выдал зарплату Гомеру " << toEmployer << std::endl;
}
