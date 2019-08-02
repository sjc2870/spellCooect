/*************************************************************************
 > File Name: Task2.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Fri 02 Aug 2019 12:25:18 AM PDT
 ************************************************************************/
#include "Task2.h"
#include <json/json.h>
#include<iostream>
#include <map>
#include <set>
using std::set;
using std::multimap;
using std::cout;
using std::endl;
namespace wd{
	void Task2::process(){
		//decode
		//compute
		//encode
		string response = getReturnWordFromCache(_msg);//要返回给客户端的消息
		if(response.size()==0){
			//如果没有在缓存中找到
			response = getReturnWord(_msg);
			_pCache->set(_msg,response);
		}
		string result = toJson(_msg,response);
		_conn->sendInLoop(result);
	}

	string Task2::getReturnWordFromCache(string msg){
		return _pCache->get(msg);
	}

	string Task2::toJson(string key,string value){
		Json::Value root;
		Json::FastWriter fwriter;
		root[key] = value;
		return fwriter.write(root);
	}

string Task2::getReturnWord(string msg){
	int dis;					
	multimap<int,int> mm;  		//存放dis-idx对,idx是该单词在vector<pair<string,int>>中的下标
	vector<int> lessDisWords;	//存放最小距离相等的一些单词的下标值

	int nbytes;
	for(int i = 0;i<msg.size();++i){
		nbytes = nBytes(msg[i]);
		string word;
		word = msg.substr(i,nbytes);
		i += (nbytes-1);
		//_config.test();
		auto &idxs = _pConfig->getIdxs(word,nbytes);
		if(idxs.size()==0){
			cout << "语料库中没有这个词，将返回"<<msg << endl;
			return msg;
		}
		for(auto idx:idxs){
			word = _pConfig->getWordAndFre(idx,nbytes).first;
			dis = editDistance(msg,word);
			mm.insert(std::make_pair(dis,idx));
		}
	}
	cout << endl;
	int lessDis = mm.begin()->first;
	cout << "lessDis = " << lessDis << endl;
	auto it = mm.begin();
	while(it!=mm.end() && lessDis == it->first){
		lessDisWords.push_back(it->second);
		++it;
	}
	cout << "lessDisWords.size = " << lessDisWords.size() << endl;
	if(lessDisWords.size()==1){
		int idx = lessDisWords[0];
		return _pConfig->getWordAndFre(idx,nbytes).first;
	}
	map<int,string> sameDisWords;  //最小编辑距离相同但频率不同的单词，存放单词--频率
	for(auto& idx:lessDisWords){
		int fre = _pConfig->getWordAndFre(idx,nbytes).second;
		string word  = _pConfig->getWordAndFre(idx,nbytes).first;
		sameDisWords.insert(std::make_pair(fre,word));
	}
	cout << sameDisWords.size() << endl;
#if 1
	cout << "最小编辑距离相同的单词：频率--单词" << endl;
	for(auto &it:sameDisWords){
		cout << it.first << "--" << it.second << " ";
	}
	cout << endl;
#endif
	auto it2 = sameDisWords.rbegin();
	return it2->second;//返回最小编辑距离相等的单词中频率最大的
}

int Task2::editDistance(string src,string target){
	int row = length(src)+1;
	int column = length(target)+1;
	int process[row][column];
	//初始化process
	process[0][0] = 0;
	for(int i = 1;i<row;++i){
		process[i][0] = i;
	}

	for(int i = 1;i<column;++i){
		process[0][i] = i;
	}
	//初始化完毕
	//下面开始计算矩阵的内容
	string sublhs,subrhs;
	for(int i = 1,word_i = 0;i<row;++i,++word_i){
		int nbytes = nBytes(src[word_i]);
		sublhs = src.substr(word_i,nbytes);
		word_i +=(nbytes-1);
		for(int j = 1,word_j = 0;j<column;++j,++word_j){
			nbytes = nBytes(target[word_j]);
			subrhs = target.substr(word_j,nbytes);
			word_j +=(nbytes-1);
			int min1 = process[i-1][j]+1;
			int min2 = process[i][j-1]+1;
			int flag = sublhs==subrhs?0:1;
			int min3 = process[i-1][j-1] + flag;
			int min4 = min1<min2?min1:min2;
			int min  = min4<min3?min4:min3;
			process[i][j] = min;
		}
	}
#if 0
	for(int i = 0;i<row;++i){
		for(int j = 0;j<column;++j){
			cout << process[i][j] << " ";
		}
		cout << endl;
	}
#endif
	return process[row-1][column-1];
}

size_t Task2::nBytes(const char c){
	if(c & (1 << 7)){
		int n = 1;
		for(int i = 0;i<6;++i){
			if(c & (1 << (6-i))){
				++n;
			}else{
				break;
			}
		}
		return n;
	}
	return 1;
}

size_t Task2::length(const string& word){
	int len = 0;
	for(int i = 0;i<word.size();++i){
		int nbytes = nBytes(word[i]);
		i += (nbytes-1);
		++len;
	}
	return len;
}
}//end of namespace wd
