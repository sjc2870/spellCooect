#pragma once
#include "TcpConnection.h"
namespace wd{
class Task
{
public:
	Task(const string & msg,
		 const wd::TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}

	//运行在线程池的某一个子线程中
	void process()
	{
		//decode
		//compute
		//encode
		string response = _conn->getReturnWordFromCache(_msg);//要返回给客户端的消息
		string result = _conn->toJson(_msg,response);
		_conn->sendInLoop(result);
	}
private:
	string _msg;
	wd::TcpConnectionPtr _conn;
};

}//end of namespace wd
