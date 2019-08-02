 ///
 /// @file    TestAcceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:42:14
 ///
 
#include "Threadpool.h"
#include "TcpServer.h"
#include "Config.h"
#include "Task.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
wd::Threadpool * gThreadpool = nullptr;

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
 
//回调函数体现了扩展性
void onConnection(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
	conn->send("welcome to server.");
}

void onMessage(const wd::TcpConnectionPtr & conn)
{
	cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << ">> receive msg from client: " << msg << endl;
	Task task(msg, conn);

	gThreadpool->addTask(std::bind(&Task::process, task));
}

void onClose(const wd::TcpConnectionPtr & conn)
{	
	conn->writeToFile();
	cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

int main(void)
{
	wd::Threadpool threadpool(4, 10); 
	threadpool.start();
	
	gThreadpool = &threadpool;

	wd::Config c("../../config/config");
	c.readFile();
//	c.test();
	wd::TcpServer server("192.168.4.130", 8888,c);

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();

	return 0;
}
