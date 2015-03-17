namespace cpp TimeServer

struct TimeInfo
{
	1: string timestamp /* YYYY-mm-dd HH:ii:ss */
}

service TimeService
{
	TimeInfo get_Time()
}
