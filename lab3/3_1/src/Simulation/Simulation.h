#include "../Actors/Bank/Bank.h"
#include "../Actors/Contact/ContactList.h"
#include "../Actors/Person/Person.h"
#include <memory>
#include <vector>

struct SimulationActors
{
	std::vector<std::unique_ptr<Person>> persons;
	std::vector<std::unique_ptr<PersonWithAccount>> personsWithAccount;
};
using Number = int;

class Simulation
{
public:
	Simulation(Money money, int days);
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;

	void RunSimulation();

private:
	void PrintResults() const;
	void ShowBalances() const;
	void DivisionMoney(Money money);
	static void AssertIsNumberValid(Number number);

	Money m_simulationMoney = 0;
	Number m_days = 0;
	Bank m_bank;
	SimulationActors m_actors;
	ContactList m_contacts;
};