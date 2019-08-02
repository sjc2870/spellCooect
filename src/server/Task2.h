#pragma once
#include "TcpConnection.h"
#include "Config.h"
#include "Cache.h"
namespace wd{
class Task2
{
public:
	Task2(const string & msg,const wd::TcpConnectionPtr & conn,Config* pconfig,Cache* pcache)
	: _msg(msg)
	, _conn(conn)
	,_pConfig(pconfig)
	,_pCache(pcache)
	{}

	//运行在线程池的某一个子线程中
	void process();
private:
	string getReturnWordFromCache(string );
	string toJson(string,string);
	string getReturnWord(string);
	int editDistance(string,string);
	size_t nBytes(const char);
	size_t length(const string&);
private:
	string _msg;
	wd::TcpConnectionPtr _conn;
	Config* _pConfig;
	Cache* _pCache;
};

}//end of namespace wd
