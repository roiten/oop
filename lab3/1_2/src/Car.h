#pragma once
#include <string>
#include <vector>

enum class Engine { off, on };

class Car
{
public:
	Car() = default;

	void TurnOnEngine();
	void TurnOffEngine();

	void SetGear(int gear);
	int GetGear() const;

	void SetSpeed(int speed);
	int GetSpeed() const;

	bool IsTurnedOn() const;
	void Info() const;
	std::string GetDirection() const;

private:
	Engine m_engine = Engine::off;
	int m_speed = 0;
	int m_gear = 0;

	bool IsSpeedInGearRange(int gear, int speed) const;
};