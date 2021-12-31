#include "cron.h"
#include <time.h>
#include <string.h>
#include <iostream>

int test2()
{
	char c = '0';
	int i1 = c;
	int i2 = c-'0';
	std::cout << "c=" << c << std::endl;
	std::cout << "i1=" << i1 << std::endl;
	std::cout << "i2=" << i2 << std::endl;
	return 0;
}
int test1()
{
	Cron c;
	c.bitprintf(__LINE__);
	c.seconds.set(0);
	c.bitprintf(__LINE__);
	return 0;
}

int stringtotime()
{
	tm tm_;  
	time_t t_;  
	char buf[128]= {0};  
	  
	strcpy(buf, "2012-01-01 14:00:00");  
	strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间  
	tm_.tm_isdst = -1;  
	t_  = mktime(&tm_); //将tm时间转换为秒时间  
	t_ += 3600;  //秒数加3600  
	  
	tm_ = *localtime(&t_);//输出时间  
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &tm_);  
	std::cout << buf << std::endl;
	return 0;
}
int main(int argc , char** argv)
{
	//return  test2();
	time_t t;  //秒时间  
	tm* local; //本地时间   
	tm* gmt;   //格林威治时间  
	char buf[128]= {0};  

	t = time(NULL); //获取目前秒时间  
	local = localtime(&t); //转为本地时间  
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);  
	std::cout << buf << std::endl;  

	gmt = gmtime(&t);//转为格林威治时间  
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", gmt);  
	std::cout << buf << std::endl;  

	stringtotime();
	//test1();
	return 0;
}
