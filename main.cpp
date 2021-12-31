#include <map>
#include "cron.cpp"
#include <stdio.h>

std::map<std::string, std::string> array;

void init()
{
	array["0 0 2 1 * ? *"] = "表示在每月的1日的凌晨2点调整任务"; 
	array["0 0 10,14,16 * * ?"]= "每天上午10点，下午2点，4点";
	array["0 0/30 9-17 * * ?"]= "朝九晚五工作时间内每半小时";
	array["0 0 12 * * ?"] = "每天中午12点触发"; 
	array["0 15 10 ? * * "] = "每天上午10:15触发"; 
	array["0 15 10 * * ?"] = "每天上午10:15触发";
	array["0 15 10 * * ? *"] = "每天上午10:15触发";
	array["0 15 10 * * ? 2023"] = "2023年的每天上午10:15触发";
	array["0 * 14 * * ?"] = "在每天下午2点到下午2:59期间的每1分钟触发";
	array["0 0/5 14 * * ?"] = "在每天下午2点到下午2:55期间的每5分钟触发";
	array["0 0/5 14,18 * * ?"] = "在每天下午2点到2:55期间和下午6点到6:55期间的每5分钟触发";
	array["0 0-5 14 * * ?"] = "在每天下午2点到下午2:05期间的每1分钟触发";
	array["0 15 10 15 * ?"] = "每月15日上午10:15触发";
	//array["0 15 10 ? * 6#3"] = "每月的第三个星期五上午10:15触发";
	for (auto iter = array.begin(); iter != array.end(); iter++)
	{
		std::cout << iter->first << std::endl;
		std::cout << iter->second << std::endl;
	}
}

void Test_GetNextDateTime()
{
	for (auto iter = array.begin(); iter != array.end(); iter++)
	{
		std::cout << "cron=" << iter->first << std::endl;
		std::cout << iter->second << std::endl;
		uint64_t timer = 0;
		CronHelp cronhelp;
		if (!cronhelp.GetNextDateTime(iter->first, 0, timer))
		{
			std::cout << "GetNextDateTime error" << std::endl;
			return;
		}
		std::cout << "GetNextDateTime success timer=" << timer << std::endl;
		time_t time = timer;
		struct tm *p;
		p = localtime(&time); /*转换为struct tm结构的本地时间*/
		printf("%d-%d-%d %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	}
}

int main(int argc, char** argv)
{
	//
	init();
	if (argc != 2)
	{
		Test_GetNextDateTime();
		return 0;
	}
	//std::string cron="0 15 10 * * ?";
	std::string cron=argv[1];
	
	std::cout << "cron=" << cron << std::endl;
	uint64_t timer = 0;
	CronHelp cronhelp;
	if (!cronhelp.GetNextDateTime(cron,0, timer))
	{
		std::cout << "GetNextDateTime error" << std::endl;
		return -1;
	}
	std::cout << "GetNextDateTime success timer=" << timer << std::endl;
	//cronhelp.CronToDateTime(cron, timer);
	//cronhelp.CronToDateTime(cron);
	return 0;
}
