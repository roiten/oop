#include <catch2/catch_test_macros.hpp>
#include "../src/Car.h"
#include <stdexcept>

TEST_CASE("initial state")
{
    Car car;
    REQUIRE_FALSE(car.IsTurnedOn());
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetGear() == 0);
    REQUIRE(car.GetDirection() == "standing still");
}

TEST_CASE("Engine can turn on")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("Engine can turn off when stopped on neutral")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_NOTHROW(car.TurnOffEngine());
    REQUIRE_FALSE(car.IsTurnedOn());
}

TEST_CASE("Engine cannot turn off when moving")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("Engine cannot turn off when gear is not neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("Engine can be turned on already running engine")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_NOTHROW(car.TurnOnEngine());
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("initial gear is neutral")
{
    Car car;
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("Gear cannot set gear (except neutral) when engine is off")
{
    Car car;
    REQUIRE_THROWS_AS(car.SetGear(1),  std::runtime_error);
    REQUIRE_THROWS_AS(car.SetGear(-1), std::runtime_error);
    REQUIRE_NOTHROW(car.SetGear(0));
}

TEST_CASE("invalid gear number throws invalid_argument")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_THROWS_AS(car.SetGear(6),  std::invalid_argument);
    REQUIRE_THROWS_AS(car.SetGear(-2), std::invalid_argument);
}

TEST_CASE("Gear can switch to first at speed 0")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_NOTHROW(car.SetGear(1));
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("Gear can switch to reverse only at speed 0")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_NOTHROW(car.SetGear(-1));
    REQUIRE(car.GetGear() == -1);
}

TEST_CASE("Gear cannot switch to reverse while moving forward")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    REQUIRE_THROWS_AS(car.SetGear(-1), std::runtime_error);
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("Gear cannot switch to forward while moving backward")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(15);

    REQUIRE_NOTHROW(car.SetGear(0));
    REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);
	car.SetSpeed(0);
    REQUIRE_NOTHROW(car.SetGear(1));
}

TEST_CASE("speed must be in range for new gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(19);

    // 2 передача (20-50), скорость 19
    REQUIRE_THROWS_AS(car.SetGear(2), std::runtime_error);
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("upshift sequence")
{
    Car car;
    car.TurnOnEngine();

    car.SetGear(1); car.SetSpeed(20);
    car.SetGear(2); car.SetSpeed(40);
    car.SetGear(3); car.SetSpeed(50);
    car.SetGear(4); car.SetSpeed(80);
    car.SetGear(5); car.SetSpeed(120);

    REQUIRE(car.GetGear() == 5);
    REQUIRE(car.GetSpeed() == 120);
    REQUIRE(car.GetDirection() == "forward");
}

TEST_CASE("cannot skip gears without suitable speed")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(25);

    REQUIRE_THROWS_AS(car.SetGear(3), std::runtime_error);
	REQUIRE_NOTHROW(car.SetGear(2));
}

TEST_CASE("reverse to neutral and neutral to forward only at 0")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);

    REQUIRE_NOTHROW(car.SetGear(0));
    REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);

    car.SetSpeed(0);
    REQUIRE_NOTHROW(car.SetGear(1));
}

TEST_CASE("initial speed is 0")
{
    Car car;
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("cannot set speed when engine is off")
{
    Car car;
    REQUIRE_THROWS_AS(car.SetSpeed(10), std::runtime_error);
}

TEST_CASE("negative speed throws invalid_argument")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE_THROWS_AS(car.SetSpeed(-1), std::invalid_argument);
}

TEST_CASE("accelerate within first gear range")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    REQUIRE_NOTHROW(car.SetSpeed(0));
    REQUIRE_NOTHROW(car.SetSpeed(15));
    REQUIRE(car.GetSpeed() == 15);
    REQUIRE_NOTHROW(car.SetSpeed(30));
    REQUIRE(car.GetSpeed() == 30);
}

TEST_CASE("cannot exceed gear maximum")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    REQUIRE_THROWS_AS(car.SetSpeed(31), std::runtime_error);
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("cannot go below gear minimum on active gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1); car.SetSpeed(20);
    car.SetGear(2);
    REQUIRE_THROWS_AS(car.SetSpeed(19), std::runtime_error);
    REQUIRE(car.GetSpeed() == 20);
}

TEST_CASE("can only decelerate on neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(25);
    car.SetGear(0);

    REQUIRE_NOTHROW(car.SetSpeed(15));
    REQUIRE(car.GetSpeed() == 15);

    REQUIRE_THROWS_AS(car.SetSpeed(20), std::runtime_error);
    REQUIRE(car.GetSpeed() == 15);
}

TEST_CASE("decelerate to zero on neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    car.SetGear(0);
    REQUIRE_NOTHROW(car.SetSpeed(0));
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetDirection() == "standing still");
}

TEST_CASE("reverse gear speed shown as positive")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(15);
    REQUIRE(car.GetSpeed() == 15);
    REQUIRE(car.GetDirection() == "backward");
}

TEST_CASE("cannot accelerate on reverse beyond 20")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    REQUIRE_THROWS_AS(car.SetSpeed(21), std::runtime_error);
}

TEST_CASE("decelerate on neutral while moving backward")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(15);
    car.SetGear(0);

    REQUIRE_NOTHROW(car.SetSpeed(10));
    REQUIRE(car.GetSpeed() == 10);
    REQUIRE(car.GetDirection() == "backward");

    REQUIRE_THROWS_AS(car.SetSpeed(11), std::runtime_error);
}

TEST_CASE("standing still at speed 0")
{
    Car car;
    REQUIRE(car.GetDirection() == "standing still");
}

TEST_CASE("forward on forward gears")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    REQUIRE(car.GetDirection() == "forward");
}

TEST_CASE("backward on reverse gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(15);
    REQUIRE(car.GetDirection() == "backward");
}

TEST_CASE("persists on neutral while still moving")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(20);
    car.SetGear(0);
    REQUIRE(car.GetDirection() == "forward");
    REQUIRE(car.GetSpeed() == 20);
}

TEST_CASE("Test case 1")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE(car.IsTurnedOn());
    REQUIRE(car.GetDirection() == "standing still");
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetGear() == 0);
}

TEST_CASE("Test case 2")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(15);
    REQUIRE(car.GetGear() == 1);
    REQUIRE(car.GetSpeed() == 15);
    REQUIRE(car.GetDirection() == "forward");
}

TEST_CASE("Test case 3")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(30);
    car.SetGear(0);
    car.SetSpeed(10);
    REQUIRE(car.GetGear() == 0);
    REQUIRE(car.GetSpeed() == 10);
    REQUIRE(car.GetDirection() == "forward");
}

TEST_CASE("Test case 4")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(10);
    REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    REQUIRE(car.IsTurnedOn());
}

TEST_CASE("Test case 5")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(1);
    car.SetSpeed(19);
    REQUIRE_THROWS_AS(car.SetGear(2), std::runtime_error);
    REQUIRE(car.GetGear() == 1);
}

TEST_CASE("Test case 6")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(-1);
    car.SetSpeed(10);
    REQUIRE(car.GetDirection() == "backward");

    car.SetGear(0);
    car.SetSpeed(5);
    car.SetSpeed(0);
    REQUIRE(car.GetDirection() == "standing still");

    car.SetGear(1);
    car.SetSpeed(20);
    REQUIRE(car.GetDirection() == "forward");
    REQUIRE(car.GetSpeed() == 20);
}