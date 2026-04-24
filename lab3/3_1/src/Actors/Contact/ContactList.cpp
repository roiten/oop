#include "ContactList.h"

void ContactList::AddPerson(Person& person)
{
	const auto& name = person.GetName();
	AssertIsContactNonExists(name);
	m_registry.person.emplace(name, std::ref(person));
}

void ContactList::AddPersonWithAccount(PersonWithAccount& person)
{
	const auto& name = person.GetName();
	AssertIsContactNonExists(name);
	m_registry.personWithAccount.emplace(name, std::ref(person));
}

Person& ContactList::GetPerson(Name name) const
{
	AssertIsContactExists(name);
	return m_registry.person.at(name).get();
}

const ContactMaps& ContactList::GetAllContacts() const
{
	return m_registry;
}

PersonWithAccount& ContactList::GetAccountPerson(Name name) const
{
	AssertIsContactExists(name);
	return m_registry.personWithAccount.at(name).get();
}

void ContactList::AssertIsContactExists(Name name) const
{
	if (!m_registry.person.count(name) && !m_registry.personWithAccount.count(name))
	{
		throw std::runtime_error("Контакт не найден!");
	}
}

void ContactList::AssertIsContactNonExists(Name name) const
{
	if (m_registry.person.count(name) || m_registry.personWithAccount.count(name))
	{
		throw std::runtime_error("Контакт уже существует");
	}
}