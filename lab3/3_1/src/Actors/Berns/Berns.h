#pragma once

#include "../../types.h"
#include "../PersonWithAccount/PersonWithAccount.h"
#include "../Contact/ContactList.h"
#include "../Bank/Bank.h"

class Berns : public PersonWithAccount
{
public:
	Berns(Name name, Money cash, Bank& bank, ContactList& contact);

	Berns(const Berns&) = delete;
	Berns& operator=(const Berns&) = delete;

	void Step() final;

private:
	void PaySalary();

	ContactList& m_contacts;
	const int toEmployer = 400;
};
