#include "../src/Car.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

// после создания двигатель выключен
// двигатель можно запустить
// повторный запуск уже работающего двигателя
// двигатель можно заглушить стоя на нейтрали
// нельзя заглушить двигатель во время движения
// нельзя заглушить двигатель если передача не нейтральная

TEST_CASE("Engine")
{
	Car car;

	SECTION("initial state")
	{
		REQUIRE_FALSE(car.IsTurnedOn());
	}

	SECTION("can turn on")
	{
		car.TurnOnEngine();
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("turning on an already running engine")
	{
		car.TurnOnEngine();
		REQUIRE_NOTHROW(car.TurnOnEngine());
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("can turn off when stopped on neutral")
	{
		car.TurnOnEngine();
		REQUIRE_NOTHROW(car.TurnOffEngine());
		REQUIRE_FALSE(car.IsTurnedOn());
	}

	SECTION("cannot turn off when moving")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);
		REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("cannot turn off when gear is not neutral")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
		REQUIRE(car.IsTurnedOn());
	}
}

// Манипуляции с кпп
// начальная передача нейтраль (0)
// нельзя переключить передачу при выключенном двигателе (кроме нейтралки)
// несуществующий номер передачи (6, -2) бросает invalid_argument
// можно включить первую передачу на скорости 0
// можно включить заднюю передачу на скорости 0
// нельзя включить заднюю передачу во время движения вперёд
// нельзя включить переднюю передачу во время движения назад
// нельзя переключиться на передачу если скорость не входит в её диапазон
// нельзя перепрыгнуть через передачу если скорость не подходит
// разгон через се передачи 1-2-3-4-5
// с задней на нейтраль можно, с нейтрали на первую только на скорости 0


TEST_CASE("Gearbox")
{
	Car car;

	SECTION("initial gear is neutral")
	{
		REQUIRE(car.GetGear() == 0);
	}

	SECTION("cannot set gear when engine is off")
	{
		REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);
		REQUIRE_THROWS_AS(car.SetGear(-1), std::runtime_error);
		REQUIRE_NOTHROW(car.SetGear(0));
	}

	SECTION("invalid gear number throws out_of_range")
	{
		car.TurnOnEngine();
		REQUIRE_THROWS_AS(car.SetGear(6), std::out_of_range);
		REQUIRE_THROWS_AS(car.SetGear(-2), std::out_of_range);
	}

	SECTION("can switch to first gear at speed 0")
	{
		car.TurnOnEngine();
		REQUIRE_NOTHROW(car.SetGear(1));
		REQUIRE(car.GetGear() == 1);
	}

	SECTION("can switch to reverse only at speed 0")
	{
		car.TurnOnEngine();
		REQUIRE_NOTHROW(car.SetGear(-1));
		REQUIRE(car.GetGear() == -1);
	}

	SECTION("cannot switch to reverse while moving forward")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);
		REQUIRE_THROWS_AS(car.SetGear(-1), std::runtime_error);
		REQUIRE(car.GetGear() == 1);
	}

	SECTION("cannot switch to forward while moving backward")
	{
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(15);

		REQUIRE_NOTHROW(car.SetGear(0));
		REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);
		car.SetSpeed(0);
		REQUIRE_NOTHROW(car.SetGear(1));
	}

	SECTION("speed must be in range for new gear")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(19);

		// 2я передача (20-50)
		REQUIRE_THROWS_AS(car.SetGear(2), std::runtime_error);
		REQUIRE(car.GetGear() == 1);
	}

	SECTION("cannot skip gears without suitable speed")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(25);
		REQUIRE_THROWS_AS(car.SetGear(3), std::runtime_error);
		REQUIRE_NOTHROW(car.SetGear(2));
	}

	SECTION("upshift sequence through all gears")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);
		car.SetGear(2);
		car.SetSpeed(40);
		car.SetGear(3);
		car.SetSpeed(50);
		car.SetGear(4);
		car.SetSpeed(80);
		car.SetGear(5);
		car.SetSpeed(120);
		REQUIRE(car.GetGear() == 5);
	}

	SECTION("reverse to neutral ok, neutral to forward only at speed 0")
	{
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(10);

		REQUIRE_NOTHROW(car.SetGear(0));
		REQUIRE_THROWS_AS(car.SetGear(1), std::runtime_error);

		car.SetSpeed(0);
		REQUIRE_NOTHROW(car.SetGear(1));
	}
}

// Тесты по авто:
// состояние после создания (0, нейтраль, стоит)
// нельзя задать скорость при выключенном двигателе
// отрицательная скорость бросает invalid_argument
// можно разгоняться в диапазоне первой передачи (0–30)
// нельзя превысить ограничение передачи
// нельзя опуститься ниже минимума текущей передачи
// на нейтрали можно только катиться
// торможение до нуля на нейтрали переводит в состояние "стоит"
// на задней передаче скорость возвращается положительной
// на задней передаче нельзя превысить 20
// на нейтрали при движении назад можно только тормозить
// направление сохраняется при переходе на нейтраль во время движения

TEST_CASE("Car")
{
	Car car;

	SECTION("initial state")
	{
		REQUIRE(car.GetSpeed() == 0);
		REQUIRE(car.GetGear() == 0);
		REQUIRE(car.GetDirection() == "standing still");
	}

	SECTION("cannot set speed when engine is off")
	{
		REQUIRE_THROWS_AS(car.SetSpeed(10), std::runtime_error);
	}

	SECTION("negative speed throws invalid_argument")
	{
		car.TurnOnEngine();
		REQUIRE_THROWS_AS(car.SetSpeed(-1), std::invalid_argument);
	}

	SECTION("accelerate within first gear range")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		REQUIRE_NOTHROW(car.SetSpeed(0));
		REQUIRE_NOTHROW(car.SetSpeed(15));
		REQUIRE(car.GetSpeed() == 15);
		REQUIRE_NOTHROW(car.SetSpeed(30));
		REQUIRE(car.GetSpeed() == 30);
	}

	SECTION("cannot exceed gear maximum speed")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		REQUIRE_THROWS_AS(car.SetSpeed(31), std::runtime_error);
		REQUIRE(car.GetSpeed() == 0);
	}

	SECTION("cannot go below gear minimum on active gear")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);
		car.SetGear(2);
		REQUIRE_THROWS_AS(car.SetSpeed(19), std::runtime_error);
		REQUIRE(car.GetSpeed() == 20);
	}

	SECTION("can only decelerate on neutral")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(25);
		car.SetGear(0);

		REQUIRE_NOTHROW(car.SetSpeed(15));
		REQUIRE(car.GetSpeed() == 15);
		REQUIRE_THROWS_AS(car.SetSpeed(20), std::runtime_error);
		REQUIRE(car.GetSpeed() == 15);
	}

	SECTION("decelerate to zero on neutral gives standing still")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);
		car.SetGear(0);
		REQUIRE_NOTHROW(car.SetSpeed(0));
		REQUIRE(car.GetSpeed() == 0);
		REQUIRE(car.GetDirection() == "standing still");
	}

	SECTION("reverse gear: speed shown as positive, direction is backward")
	{
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(15);
		REQUIRE(car.GetSpeed() == 15);
		REQUIRE(car.GetDirection() == "backward");
	}

	SECTION("cannot accelerate on reverse beyond 20")
	{
		car.TurnOnEngine();
		car.SetGear(-1);
		REQUIRE_THROWS_AS(car.SetSpeed(21), std::runtime_error);
	}

	SECTION("decelerate on neutral while moving backward")
	{
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(15);
		car.SetGear(0);

		REQUIRE_NOTHROW(car.SetSpeed(10));
		REQUIRE(car.GetSpeed() == 10);
		REQUIRE(car.GetDirection() == "backward");
		REQUIRE_THROWS_AS(car.SetSpeed(11), std::runtime_error);
	}

	SECTION("direction persists on neutral while still moving")
	{
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);
		car.SetGear(0);
		REQUIRE(car.GetDirection() == "forward");
		REQUIRE(car.GetSpeed() == 20);
	}
}