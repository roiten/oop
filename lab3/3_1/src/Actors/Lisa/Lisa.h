#pragma once

#include "../../types.h"
#include "../Contact/ContactList.h"

class Lisa : public Person
{
public:
	Lisa(Name name, Money cash, ContactList& contact);

	Lisa(const Lisa&) = delete;
	Lisa& operator=(const Lisa&) = delete;

	void Step() final;

private:
	void PayForBooks();

	ContactList& m_contacts;
	const Money toApu = 25;
};
