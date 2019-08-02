#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#define ERROR_HANDLE(normal,error,funcname) {if(normal==error) {perror(funcname); return -1;}}
using std::map;
using std::unordered_map;
using std::vector;
using std::string;
using std::set;
namespace wd{
class Config{
public:
	Config (const string&,const string&);
	int createEnglishFrequencyFile();  //创建词频文件和索引文件
	int createChineseConfigFile();		//创建中文词频文件和索引文件
	int configFileHandle(); //大小写转换
	void test();
	int getFrequency(const string&);
private:
	bool isPunct(string word);
private:
	string m_configFileName;//英文语料库的相对路径
	string m_chineseFileName;//中文语料库目录的相对路径
	map<string,int> m_frequencyFile;//英文语料库的词频文件
	unordered_map<string,set<int>> m_indexFile;//英文语料的索引文件
};
}
