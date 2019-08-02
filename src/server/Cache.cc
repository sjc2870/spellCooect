/*************************************************************************
 > File Name: Cache.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Mon 29 Jul 2019 11:36:41 PM PDT
 ************************************************************************/

#include "Cache.h"
#include "TcpConnection.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
#include<iostream>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

namespace wd{

Cache* Cache::getInstance(string path){
	if(m_pInstance == nullptr){
		m_pInstance = new Cache(path);
	}
	return m_pInstance;
}

void Cache::readCache(){
//以尾插法插入到双向链表中
	ifstream ifs(m_fileName);
	string key;
	string value;
	while(ifs.good()){
		ifs >> key;
		ifs >> value;
		CacheNode *c = new CacheNode(key,value);
		m_cache.insert(std::make_pair(key,c));

		//下面以尾插法插入到链表之中
		if(!m_head&&!m_tail){
			m_head = c;
			m_tail = c;
		}else{
			m_tail->m_next = c;
			c->m_pre = m_tail;
			m_tail = c;
		}
	}
}


string Cache::get(string key){
	auto it = m_cache.find(key);
	if(it!=m_cache.end()){
		//如果缓存命中,把它从原来位置删除，置为头节点
		auto pnode = it->second;
		remove(pnode);
		setHead(pnode);
		return it->second->m_value;
	}
	//如果缓存未命中，返回一个空串
	string s;
	return s;
}

void Cache::set(string key,string value){
	CacheNode* c = new CacheNode(key,value);
	if(m_cache.size()==m_size){
		//如果缓存已满,先剔除链表尾的元素
		auto it2 = m_cache.find(m_tail->m_key);
		remove(m_tail);
		m_cache.erase(it2);
	}
	setHead(c);
	m_cache[key] = c;
}
void Cache::remove(CacheNode* c){
	if(c->m_pre!=nullptr){
		c->m_pre->m_next = c->m_next;
	}else{
		m_head = c->m_next;
	}
	if(c->m_next!=nullptr){
		c->m_next->m_pre = c->m_pre;
	}else{
		m_tail = c->m_pre;
	}
}
void Cache::setHead(CacheNode* c){
	if(!m_head){
		m_head = c;
		m_tail = c;
	}else{
		m_head->m_pre = c;
		c->m_next = m_head;
		m_head = c;
	}
}

void Cache::writeToFile(){
	//写文件时要加锁
	MutexLockGuard autolock(m_mutex);
	ofstream ofs(m_fileName);
	int fd = open(m_fileName.c_str(),O_RDWR|O_TRUNC);
	for(auto &ele:m_cache){
		ofs << ele.first << " " << ele.second->m_value << endl;
	}
}

void Cache::test(){
	cout << "map.size = " << m_cache.size() << endl;
	for(auto &ele:m_cache){
		cout << ele.first << " " << ele.second->m_value << endl;
	}
}
}//end of namespace wd
