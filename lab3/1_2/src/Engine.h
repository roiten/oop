#pragma once

class Engine
{
public:
	void TurnOn();
	void TurnOff();
	bool IsTurnedOn() const;

private:
	bool m_isWorking = false;
};