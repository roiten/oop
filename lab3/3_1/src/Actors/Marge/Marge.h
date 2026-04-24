#pragma once

#include "../PersonWithAccount/PersonWithAccount.h"
#include "../../types.h"
#include "../Bank/Bank.h"
#include "../Contact/ContactList.h"

class Marge : public PersonWithAccount
{
public:
	Marge(Name name, Money cash, Bank& bank, ContactList& contact);

	Marge(const Marge&) = delete;
	Marge& operator=(const Marge&) = delete;

	void Step() final;

private:
	void PayForProducts();

	ContactList& m_contacts;
	const Money toApu = 100;
};
