#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::ifstream;

namespace wd{

class Config{
public:
	Config(string);
	~Config();
	void readFile();
	set<int>& getIdxs(string,int );
	std::pair<string,int> getWordAndFre(int i,int flag);
	void test();
private:
	void readfreFile(const string&,int flag); //0代表英文，1代表中文
	void readidxFile(const string&,int flag);
private:
	string m_configFileName;
	string m_englishfreFileName;
	string m_englishidxFileName;
	string m_chinesefreFileName;
	string m_chineseidxFileName;
	vector<std::pair<string,int>> m_freFile;
	unordered_map<string,set<int>> m_idxFile;
	vector<std::pair<string,int>> m_chinesefreFile;
	unordered_map<string,set<int>> m_chineseidxFile;

};
}//end of namespace wd
