 ///
 /// @file    TestAcceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:42:14
 ///
 
#include "Threadpool.h"
#include "TcpServer.h"
#include "Config.h"
#include "Task2.h"
#include "Cache.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using wd::Task2;
using wd::Config;
using wd::Cache;
wd::Threadpool * gThreadpool = nullptr;
Config* Config::m_instance = nullptr;
Cache* Cache::m_pInstance = nullptr;
Config* pConfig = wd::Config::getInstance();
Cache* pCache = Cache::getInstance("./cache_file");

 
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
	Task2 task(msg, conn,pConfig,pCache);

	gThreadpool->addTask(std::bind(&Task2::process, task));
}

void onClose(const wd::TcpConnectionPtr & conn)
{	
	pCache->writeToFile();
	cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

int main(void)
{
	wd::Threadpool threadpool(4, 10); 
	threadpool.start();
	
	gThreadpool = &threadpool;

	pConfig->readFile();
	pConfig->test();
	wd::TcpServer server("192.168.4.130", 8888);

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();

	return 0;
}
