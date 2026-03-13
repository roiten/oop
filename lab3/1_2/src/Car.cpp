#include "Car.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

bool Car::IsTurnedOn() const
{
    return m_engine == Engine::on;
}

std::string Car::GetDirection() const
{
    if (m_speed > 0) return "forward";
    if (m_speed < 0) return "backward";
    return "standing still";
}

bool Car::IsSpeedInGearRange(int gear, int speed) const
{
    switch (gear)
    {
    case -1: return speed >= 0  && speed <= 20;
    case  0: return true;
    case  1: return speed >= 0  && speed <= 30;
    case  2: return speed >= 20 && speed <= 50;
    case  3: return speed >= 30 && speed <= 60;
    case  4: return speed >= 40 && speed <= 90;
    case  5: return speed >= 50 && speed <= 150;
    default: return false;
    }
}

void Car::SetGear(int gear)
{
    if (gear < -1 || gear > 5)
        throw std::invalid_argument("Invalid gear");

    if (!IsTurnedOn())
    {
        if (gear == 0)
        {
            m_gear = 0;
            return;
        }
        throw std::runtime_error("Сannot set gear while engine is off");
    }

    if (gear == -1 && m_speed != 0)
        throw std::runtime_error("Cannot reverse while moving");

    if (gear > 0 && m_speed < 0)
        throw std::runtime_error("Cannot switch to forward gear while moving backward");

    int absSpeed = std::abs(m_speed);
    if (!IsSpeedInGearRange(gear, absSpeed))
        throw std::runtime_error("Unsuitable current speed");

    m_gear = gear;
}

int Car::GetGear() const
{
    return m_gear;
}

void Car::SetSpeed(int speed)
{
    if (speed < 0)
        throw std::invalid_argument("Speed cannot be negative");

    if (!IsTurnedOn())
        throw std::runtime_error("Cannot set speed while engine is off");

    if (m_gear == 0)
    {
        int absSpeed = std::abs(m_speed);
        if (speed > absSpeed)
            throw std::runtime_error("Cannot accelerate on neutral");

        if (m_speed < 0)
            m_speed = -speed;
        else
            m_speed = speed;
        return;
    }

    if (!IsSpeedInGearRange(m_gear, speed))
        throw std::runtime_error("Speed is out of gear range");

    if (m_gear == -1) m_speed = -speed;
    else
        m_speed = speed;
}

int Car::GetSpeed() const
{
    return std::abs(m_speed);
}

void Car::TurnOffEngine()
{
    if (m_speed != 0 || m_gear != 0)
        throw std::runtime_error("Сar must be stopped and in neutral gear");
    m_engine = Engine::off;
}

void Car::TurnOnEngine()
{
    m_engine = Engine::on;
}

void Car::Info() const
{
    std::cout << "Engine: " << (IsTurnedOn() ? "on" : "off") << std::endl;
    std::cout << "Direction: " << GetDirection() << std::endl;
    std::cout << "Speed: " << GetSpeed() << std::endl;
    std::cout << "Gear: " << GetGear() << std::endl;
}