#include "cron.h"
#include <time.h>
#include <string.h>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <iostream>

class CronHelp
{
public:
	std::vector<int64_t> CronToDateTime(std::string cron)
	{
		std::vector<int64_t> lits;
		Cron c;
		std::vector<std::string> arr = split(cron, ' ');
		Seconds(c, arr[0]);
		Minutes(c, arr[1]);
		Hours(c, arr[2]);
		Month(c, arr[4]);
		if (arr.size() < 7)
		{
			Year(c, "");
		}
		else
		{
			Year(c, arr[6]);
		}

		time_t timep;
		struct tm *now;
		time(&timep); /*获得time_t结构的时间，UTC时间*/
		now = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
		int addtime = 1;
		while (true)
		{
			std::cout << "sec=" << now->tm_sec;
			std::cout << "min=" << now->tm_min;
			std::cout << "hours=" << now->tm_hour;
			std::cout << "mon=" << now->tm_mon;
			std::cout << "year=" << now->tm_year;
			if (c.seconds[now->tm_sec] == 1 && c.minutes[now->tm_min] == 1 && c.hours[now->tm_hour] == 1 && c.month[now->tm_mon - 1] == 1 && c.year[now->tm_year - 2019] == 1)
			{
				if (arr[3] != "?")
				{
					Days(c, arr[3], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = ((now->tm_wday) + 6) % 7;
					if (c.days[now->tm_mday - 1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						time_t nowtime = mktime(now);
						lits.push_back(nowtime);
					}
				}
				else
				{
					Weeks(c, arr[5], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = ((now->tm_wday) + 6) % 7;
					if (c.days[now->tm_mday - 1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						time_t nowtime = mktime(now);
						lits.push_back(nowtime);
					}
				}
			}
			if (lits.size() >= 5)
			{
				break;
			}
			c.Init();
			if (!contains(arr[1], '-') && !contains(arr[1], ',') && !contains(arr[1], '*') && !contains(arr[1], '/'))
			{
				if (now->tm_min == to_cron_int(arr[1]))
				{
					addtime = 3600;
				}
			}
			else if (arr[0] == "0" && now->tm_sec == 0)
			{
				addtime = 60;
			}
			now->tm_sec = now->tm_sec + addtime;
		}
		return lits;
	}

	std::vector<uint64_t> CronToDateTime(std::string cron, uint64_t oldnow)
	{
		//time_t timep;
		struct tm *now;
		//time(&timep); /*获得time_t结构的时间，UTC时间*/
		now = gmtime((time_t*)&oldnow); /*转换为struct tm结构的UTC时间*/

		std::vector<uint64_t> lits;
		Cron c;
		std::vector<std::string> arr = split(cron, ' ');
		Seconds(c, arr[0]);
		Minutes(c, arr[1]);
		Hours(c, arr[2]);
		Month(c, arr[4]);
		if (arr.size() < 7)
			Year(c, "");
		else
			Year(c, arr[6]);
		int addtime = 1;
		while (true)
		{
			if (c.seconds[now->tm_sec] == 1 && c.minutes[now->tm_min] == 1 && c.hours[now->tm_hour] == 1 && c.month[now->tm_mon - 1] == 1 && c.year[now->tm_year - 2019] == 1)
			{
				if (arr[3] != "?")
				{
					Days(c, arr[3], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = ((now->tm_wday) + 6) % 7;
					if (c.days[now->tm_mday - 1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						time_t nowtime = mktime(now);
						lits.push_back(nowtime);
					}
				}
				else
				{
					Weeks(c, arr[5], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = (((int)now->tm_wday) + 6) % 7;
					if (c.days[now->tm_mday - 1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						time_t nowtime = mktime(now);
						lits.push_back(nowtime);
					}
				}
			}
			if (lits.size() >= 5)
			{
				break;
			}
			c.Init();
			if (!contains(arr[1], '-') && !contains(arr[1], ',') && !contains(arr[1], '*') && !contains(arr[1], '/'))
			{
				if (now->tm_min == to_cron_int(arr[1]))
				{
					addtime = 3600;
				}
			}
			else if (arr[0] == "0" && now->tm_sec == 0)
			{
				addtime = 60;
			}
			now->tm_sec = now->tm_sec + addtime;
		}
		return lits;
	}

	bool GetNextDateTime(std::string cron, uint64_t nowtime, uint64_t& nexttime)
	{
		time_t timep;
		if (nowtime <= 0)
			time(&timep); /*获得time_t结构的时间，UTC时间*/
		else
			timep = nowtime;

		struct tm *now;
		now = localtime(&timep); /*转换为struct tm结构的UTC时间*/
		std::vector<std::string> arr = split(cron, ' ');
		if (IsOrNoOne(cron))
		{
			std::string date = arr[6] + "-" + arr[4] + "-" + arr[3] + " " + arr[2] + ":" + arr[1] + ":" + arr[0];
			uint64_t nowtime = stringtotime(date);
			if (nowtime  >= timep)
			{
				nexttime = nowtime;
				return true;
			}
			else
			{
				return false;
			}
		}
		Cron c;
		Seconds(c, arr[0]);
		Minutes(c, arr[1]);
		Hours(c, arr[2]);
		Month(c, arr[4]);
		c.bitprintf(__LINE__);
		if (arr.size() < 7)
		{
			Year(c, "");
		}
		else
		{
			Year(c, arr[6]);
		}
		int addtime = 1;
		nexttime += timep;
		while (true)
		{
			//now = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
			now = localtime(&timep); /*转换为struct tm结构的UTC时间*/
			//std::cout << "second=" << now->tm_sec;
			//std::cout << ",min=" << now->tm_min;
			//std::cout << ",hour=" << now->tm_hour;
			//std::cout << ",wday=" << now->tm_wday;
			//std::cout << ",day=" << now->tm_mday;
			//std::cout << ",mon=" << now->tm_mon;
			//std::cout << ",year=" << now->tm_year;
			//std::cout << ",timer=" << timep << std::endl;
			if (c.seconds[now->tm_sec] == 1 && c.minutes[now->tm_min] == 1 && c.hours[now->tm_hour] == 1 && c.month[now->tm_mon] == 1 && c.year[now->tm_year + 1900 - 2021] == 1)
			{
				//std::cout << "===================================timer=" << timep << std::endl;
				if (arr[3] != "?")
				{
					Days(c, arr[3], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = (((int)now->tm_wday) + 6) % 7;
					//std::cout << "dayofweek=" << DayOfWeek << std::endl;
					//c.bitprintf(__LINE__);
					if (c.days[now->tm_mday-1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						//std::cout << "111111timer=" << timep << std::endl;
						//nexttime = newmktime(now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
						//nexttime = mktime(now);
						nexttime = timep;
						return true;
					}
				}
				else
				{
					Weeks(c, arr[5], daysInMonth(now->tm_year, now->tm_mon));
					int DayOfWeek = (((int)now->tm_wday) + 6) % 7;
					//std::cout << "dayofweek=" << DayOfWeek << std::endl;
					//c.bitprintf(__LINE__);
					if (c.days[now->tm_mday-1] == 1 && c.weeks[DayOfWeek] == 1)
					{
						//std::cout << "222222timer=" << timep << std::endl;
						//nexttime = newmktime(now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
						//nexttime = mktime(now);
						nexttime = timep;
						return true;
					}
				}
			}
			c.Init();
			if (!contains(arr[1], '-') && !contains(arr[1], ',') && !contains(arr[1], '*') && !contains(arr[1], '/'))
			{
				if (now->tm_min == to_cron_int(arr[1]))
				{
					addtime = 3600;
				}
			}
			else if (arr[0] == "0" && now->tm_sec == 0)
			{
				addtime = 60;
			}
			timep = timep+addtime; 
		}
		return false;
	}

	void Seconds(Cron& c, std::string str)
	{
		if (str == "*")
		{
			c.seconds.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.seconds.set(i);
			}
		}
		else if (contains(str, '/'))
		{
			std::vector<std::string> range = split(str, '/');
			int begin = to_cron_int(range[0]);
			int interval = to_cron_int(range[1]);
			while (true)
			{
				c.seconds.set(begin);
				if ((begin + interval) >= 60)
					break;
				begin += interval;
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.seconds.set(index);
			}
		}
		else
		{
			int index = to_cron_int(str);
			c.seconds.set(index);
		}
	}
	void Minutes(Cron& c, std::string str)
	{
		if (str == "*")
		{
			c.minutes.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.minutes.set(i);
			}
		}
		else if (contains(str, '/'))
		{
			std::vector<std::string> range = split(str, '/');
			int begin = to_cron_int(range[0]);
			int interval = to_cron_int(range[1]);
			while (true)
			{
				c.minutes.set(begin);
				if ((begin + interval) >= 60)
					break;
				begin += interval;
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.minutes.set(index);
			}
		}
		else
		{
			int index = to_cron_int(str);
			c.minutes.set(index);
		}
	}
	void Hours(Cron& c, std::string str)
	{
		if (str == "*")
		{
			c.hours.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.hours.set(i);
			}
		}
		else if (contains(str, '/'))
		{
			std::vector<std::string> range = split(str, '/');
			int begin = to_cron_int(range[0]);
			int interval = to_cron_int(range[1]);
			while (true)
			{
				c.hours.set(begin);
				if ((begin + interval) >= 24)
					break;
				begin += interval;
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.hours.set(index);
			}
		}
		else
		{
			int index = to_cron_int(str);
			c.hours.set(index);
		}
	}
	void Month(Cron& c, std::string str)
	{
		if (str == "*")
		{
			c.month.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.month.set(i-1);
			}
		}
		else if (contains(str, '/'))
		{
			std::vector<std::string> range = split(str, '/');
			int begin = to_cron_int(range[0]);
			int interval = to_cron_int(range[1]);
			while (true)
			{
				c.month.set(begin - 1);
				if ((begin + interval) >= 12)
					break;
				begin += interval;
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.month.set(index-1);
			}
		}
		else
		{
			int index = to_cron_int(str);
			c.month.set(index - 1);
		}
	}
	void Year(Cron& c, std::string str)
	{
		if (str == "" || str == "*")
		{
			c.year.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin - 2021; i <= end - 2021; i++)
			{
				c.year.set(i);
			}
		}
		else
		{
			int index = to_cron_int(str);
			c.year.set(index-2021);
		}
	}
	void Days(Cron& c, std::string str, int len)
	{
		c.weeks.set();
		if (str == "*" || str == "?")
		{
			for (int i = 0; i < len; i++)
			{
				c.days.set(i);
			}
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.days.set(i-1);
			}
		}
		else if (contains(str, '/'))
		{
			std::vector<std::string> range = split(str, '/');
			int begin = to_cron_int(range[0]);
			int interval = to_cron_int(range[1]);
			while (true)
			{
				c.days.set(begin - 1);
				if ((begin + interval) >= len)
					break;
				begin += interval;
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.days.set(index - 1);
			}
		}
		else if (contains(str, 'L'))
		{
			//int i = str.Replace("L", "") == "" ? 0 : int.Parse(str.Replace("L", ""));
			//c.Days[len - 1 - i] = 1;
		}
		else if (contains(str, 'W'))
		{
			c.days.set(len - 1);
		}
		else
		{
			int index = to_cron_int(str);
			c.days.set(index - 1);
		}
	}
	void Weeks(Cron& c, std::string str, int len)
	{
		if (str == "*" || str == "?")
		{
			c.weeks.set();
		}
		else if (contains(str, '-'))
		{
			std::vector<std::string> range = split(str, '-');
			int begin = to_cron_int(range[0]);
			int end = to_cron_int(range[1]);
			for (int i = begin; i <= end; i++)
			{
				c.weeks.set(i - 1);
			}
		}
		else if (contains(str, ','))
		{
			std::vector<std::string> range = split(str, ',');
			for (int i = 0; i < range.size(); i++)
			{
				int index = to_cron_int(range[i]);
				c.weeks.set(index - 1);
			}
		}
		else if (contains(str, 'L'))
		{
			//int i = str.Replace("L", "") == "" ? 0 : int.Parse(str.Replace("L", ""));
			//if (i == 0)
			//{
			//	c.Weeks[6] = 1;
			//}
			//else
			//{
			//	c.Weeks[i - 1] = 1;
			//	c.Days[GetLastWeek(i, now) - 1] = 1;
			//	return;
			//}
		}
		else if (contains(str, '#'))
		{
			std::vector<std::string> range = split(str, '#');
			int i = to_cron_int(range[0]);
			int j = to_cron_int(range[1]);

			c.weeks.set(i-1);
			c.days.set(GetWeek(i - 1, j));
			return;
		}
		else
		{
			int index = to_cron_int(str);
			c.weeks.set(index-1);
		}
		//week中初始化day，则说明day没要求
		for (int i = 0; i < len; i++)
		{
			c.days.set(i);
		}
	}
	bool IsOrNoOne(std::string cron)
	{
		if (contains(cron, '-') || contains(cron, ',') || contains(cron, '/') || contains(cron, '*'))
			return false;
		return true;
	}
	int GetLastWeek(int i, uint64_t now)
	{
		struct tm *p;
		p = gmtime((time_t*)&now); /*转换为struct tm结构的UTC时间*/
		return 0;

		//DateTime d = now.AddDays(1 - now.Day).Date.AddMonths(1).AddSeconds(-1);
		//int DayOfWeek = ((((int)d.DayOfWeek) + 6) % 7) + 1;
		//int a = DayOfWeek >= i ? DayOfWeek - i : 7 + DayOfWeek - i;
		//return DateTime.DaysInMonth(now.Year, now.Month) - a;
	}
	int GetWeek(int i, int j)
	{
		time_t timep;
		struct tm *p;
		time(&timep); /*获得time_t结构的时间，UTC时间*/
		p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/

		int day = 0;
		int DayOfWeek = ((p->tm_wday + 6) % 7) + 1;
		if (i >= DayOfWeek)
		{
			day = (7 - DayOfWeek + 1) + 7 * (j - 2) + i;
		}
		else
		{
			day = (7 - DayOfWeek + 1) + 7 * (j - 1) + i;
		}
		return day;
	}

private:
	inline bool contains(const std::string& text, char const ch)
	{
		return std::string::npos != text.find_first_of(ch);
	}
	static std::vector<std::string> split(const std::string& text, char const delimiter)
	{
		std::vector<std::string> tokens;
     	std::string token;
     	std::istringstream tokenStream(text.data());
     	while (std::getline(tokenStream, token, delimiter))
     	{
     	   tokens.push_back(token);
     	}
     	return tokens;
	}
	inline int to_cron_int(const std::string& text)
  	{
		return std::stoi(text.c_str());
  	}
	int daysInMonth(int oyear, int omonth)
	{
		int year = oyear+1;
		int month = omonth+1900;
		int result;
		if (2==month)
		{
			if(year%4==0&&year%100!=0||year%400==0)
				result=29;
			else
				result=28;
		}
		else if (month==4||month==6||month==9||month==11)
	   	{
	   		result=30;
	   	}
	    else
		{
	   		result=31;
	   	}
	   	return result;
	}
	uint64_t stringtotime(std::string strtimer)
	{
		tm tm_;  
		time_t t_;  
		char buf[128]= {0};  
		  
		strcpy(buf, strtimer.c_str());  
		strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间  
		tm_.tm_isdst = -1;  
		t_  = mktime(&tm_); //将tm时间转换为秒时间  
		return t_;
	}
};
