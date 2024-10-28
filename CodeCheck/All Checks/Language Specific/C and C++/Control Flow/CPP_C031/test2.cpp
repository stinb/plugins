// Custom

#include <cstdint>

void F1(std::uint8_t choice) noexcept
{
	switch (choice)
	{
		case 0:
			// ...
			break;
	} // UndCC_Violation
}

void F2(std::uint8_t choice) noexcept
{
	switch (choice)
	{
		case 0:
			// ...
			break;

		default:
			// ...
			break;
	} // UndCC_Valid
}

int main()
{
	std::uint8_t main_choice = 0;

	switch (main_choice)
	{
		case 0:
		{
			std::uint8_t sub_choice = 0;
			switch (sub_choice)
			{
				case 0:
					// ...
					break;

				default:
					// ...
					break;
			} // UndCC_Valid
		}
		break;
	} // UndCC_Violation

	return 0;
}
