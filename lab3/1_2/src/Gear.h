#pragma once

class Gear
{
	public:
	Gear();
	void Set(int gear);
	int Get() const;
	// bool IsSpeedInGearRange(int gear, int speed) const;

	private:
	int m_gear = 0;
	//m_speed
};