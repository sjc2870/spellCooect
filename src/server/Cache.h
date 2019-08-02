#pragma once

#include "MutexLock.h"
#include <map>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::map;
#define CACHE_NUM 10000
namespace wd{
class TcpConnection;
struct CacheNode{
CacheNode(string key,string value)
:m_key(key)
,m_value(value)
,m_pre(nullptr)
,m_next(nullptr)	{	}

string m_key;
string m_value;
CacheNode *m_pre,*m_next;
};

class Cache{
public:
	//对外的接口
	static Cache* getInstance(string);
	static void destory();
	string get(string);
	void writeToFile();
	void set(string,string);
private:
	Cache(string filename)
	:m_fileName(filename)
	,m_size(CACHE_NUM)
	,m_head(nullptr)
	,m_tail(nullptr)
	,m_mutex(){
		readCache();
	}
	
	~Cache(){
		
	}
	void readCache();
	void setHead(CacheNode*);
	void remove(CacheNode*);
	void test();
private:
	static Cache* m_pInstance;
	string m_fileName;
	int m_size;
	CacheNode *m_head,*m_tail;
	map<string,CacheNode*> m_cache;
	MutexLock m_mutex;
};
}//end of namespace wd
