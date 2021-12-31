#pragma
#include <bitset>
#include <iostream>

class Cron
{
	public:
	Cron()
	{}
	void Init()
	{
		weeks.reset();
		days.reset();
	}
	std::bitset<60> seconds;
	std::bitset<60> minutes;
	std::bitset<24> hours;
	std::bitset<31> days;
	std::bitset<12> month;
	std::bitset<7> weeks;
	void bitprintf(int line)
	{
		std::cout << "======line=" << line << std::endl;
		std::cout << "second=" << seconds.to_string()  << std::endl;
		std::cout << "minutes=" << minutes.to_string() << std::endl;
		std::cout << "hours=" << hours.to_string() << std::endl;
		std::cout << "days=" << days.to_string() << std::endl;
		std::cout << "month=" << month.to_string() << std::endl;
		std::cout << "weeks=" << weeks.to_string() << std::endl;
		std::cout << "year=" << year.to_string() << std::endl;
	}
	//2019-2099年
	std::bitset<80> year;
};
