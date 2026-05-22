#pragma once

#include "../PersonWithAccount/PersonWithAccount.h"
#include "../../types.h"
#include "../Bank/Bank.h"
#include "../Contact/ContactList.h"

class Homer : public PersonWithAccount
{
public:
	Homer(Name name, Money cash, Bank& bank, ContactList& contact);

	Homer(const Homer&) = delete;
	Homer& operator=(const Homer&) = delete;

	void Step() final;

private:
	void GiveMoneyToMarge();
	void PayElectricity();
	void GiveMoneyToKids();

private:
	ContactList& m_contacts;

	const Money toMarge = 100;
	const Money toBerns = 200;
	const Money toBart = 50;
	const Money toLisa = 50;
};
