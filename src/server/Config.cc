/*************************************************************************
 > File Name: Config.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Mon 29 Jul 2019 01:32:08 AM PDT
 ************************************************************************/

#include "Config.h"
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::ifstream;
using std::ofstream;
using std::istringstream;
namespace wd{
	Config* Config::getInstance(){
		if(m_instance==nullptr){
			m_instance = new Config("../../config/config");
		}
		return m_instance;
	}
	Config::Config(string configFileName)
	:m_configFileName(configFileName){
		ifstream ifs(m_configFileName);
		ifs >> m_englishfreFileName;
		ifs >> m_englishidxFileName;
		ifs >> m_chinesefreFileName;
		ifs >> m_chineseidxFileName;
	}

	void Config::readFile(){
		cout << m_englishfreFileName << endl;
		cout << m_englishidxFileName << endl;
		readfreFile(m_englishfreFileName,0);
		readfreFile(m_chinesefreFileName,1);
		readidxFile(m_englishidxFileName,0);
		readidxFile(m_chineseidxFileName,1);
		
	}
	void Config::readfreFile(const string& name,int flag){
		char line[100860] = {0};
		string word;
		int fre;
		ifstream ifs(name);
		while(ifs.good()){
			ifs.getline(line,sizeof(line));
			istringstream iss(line);
			iss >> word;
			iss >> fre;
			if(flag==0){
				m_freFile.push_back(std::make_pair(word,fre));
			}else{
				m_chinesefreFile.push_back(std::make_pair(word,fre));
			}
		}
	}

	void Config::readidxFile(const string& name,int flag){
		ifstream ifs(name);
		char line[100860] = {0};
		string word;
		int idx;
		while(ifs.good()){
			bzero(line,sizeof(line));
			ifs.getline(line,sizeof(line));
			istringstream iss(line);
			iss >> word;
			set<int> se;
			while(iss.good()){
				iss >> idx;
				se.insert(idx);
			}
			if(flag==0){
				m_idxFile.insert(std::make_pair(word,se));
			}else{
				m_chineseidxFile.insert(std::make_pair(word,se));
			}
		}
	}
	void Config::test(){
		cout << "中文频率文件的大小为" << m_chinesefreFile.size() << endl;
		cout << "中文索引文件的大小为" << m_chineseidxFile.size() << "  "<< m_chineseidxFile.empty() << endl;
		auto it = m_chinesefreFile.rbegin();
		cout << "中文频率文件最后一条记录的单词--频率是:" << it->first <<"--" << it->second << endl;
#if 0
		auto it2 = m_chineseidxFile["一"];
		for(auto &i:it2){
			cout << i << " ";
		}
		cout << endl;

#endif
	}
	set<int>& Config::getIdxs(string word,int flag){
//		cout << "flag = " << flag << endl;
		if(flag==1){
			return m_idxFile[word];
		}
#if 0
		for(auto &i:it2){
			cout << i << " ";
		}
		cout << endl;
#endif
		return m_chineseidxFile[word];
	}

	std::pair<string,int> Config::getWordAndFre(int i,int flag){
		if(flag==1){
			return m_freFile[i];
		}
		return m_chinesefreFile[i];
	}
}
