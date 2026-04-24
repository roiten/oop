#pragma once

#include "../../types.h"
#include "../Person/Person.h"
#include "../Contact/ContactList.h"

class Bart : public Person
{
public:
	Bart(Name name, Money cash, ContactList& contact);

	Bart(const Bart&) = delete;
	Bart& operator=(const Bart&) = delete;

	void Step() final;

private:
	void PayForBubbleGums();

	ContactList& m_contacts;
	const Money toApu = 25;
};

