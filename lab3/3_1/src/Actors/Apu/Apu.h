#pragma once

#include "../PersonWithAccount/PersonWithAccount.h"
#include "../../types.h"
#include "../Bank/Bank.h"
#include "../Contact/ContactList.h"

class Apu : public PersonWithAccount
{
public:
	Apu(Name name, Money cash, Bank& bank, ContactList& contact);

	Apu(const Apu&) = delete;
	Apu& operator=(const Apu&) = delete;

	void Step() override;

private:
	void DepositCash();
	void PayElectricity();
	ContactList& m_contacts;
	const int toBerns = 200;
};

