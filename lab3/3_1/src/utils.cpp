#include "utils.h"

std::string_view NameToString(Name name)
{
	switch (name)
	{
	case Name::Homer:
		return "Homer";
	case Name::Marge:
		return "Marge";
	case Name::Lisa:
		return "Lisa";
	case Name::Bart:
		return "Bart";
	case Name::Apu:
		return "Apu";
	case Name::Berns:
		return "Berns";
	default:
		return "Unknown";
	}
}
