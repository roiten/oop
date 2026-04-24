#pragma once

#include "../Person/Person.h"
#include "../PersonWithAccount/PersonWithAccount.h"
#include <functional>
#include <unordered_map>

struct ContactMaps
{
	std::unordered_map<Name, std::reference_wrapper<Person>> person;
	std::unordered_map<Name, std::reference_wrapper<PersonWithAccount>> personWithAccount;
};

class ContactList
{
public:
	ContactList() = default;
	ContactList(const ContactList&) = delete;
	ContactList& operator=(const ContactList&) = delete;

	void AddPerson(Person& person);
	void AddPersonWithAccount(PersonWithAccount& person);
	Person& GetPerson(Name name) const;
	PersonWithAccount& GetAccountPerson(Name name) const;
	const ContactMaps& GetAllContacts() const;

private:
	void AssertIsContactExists(Name name) const;
	void AssertIsContactNonExists(Name name) const;

	ContactMaps m_registry;
};

