/*************************************************************************
 > File Name: Config.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Thu 25 Jul 2019 08:28:12 PM PDT
 ************************************************************************/

#include "Config.h"
#include "cppjieba/Jieba.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;
namespace wd{

Config::Config(const string& name,const string &chineseName)
:m_configFileName(name)
,m_chineseFileName(chineseName){
	int ret;
#if 0
	ret = configFileHandle();
	if(ret==-1){
		perror("configFileHandle");
	}
#endif
#if 0
	ret = createEnglishFrequencyFile();
	if(ret==-1){
		perror("createFrequencyFile");
	}
#endif
	m_frequencyFile.clear();
	m_indexFile.clear();
	ret = createChineseConfigFile();
	if(ret==-1){
		perror("createChineseConfigFile");
	}

//	test();
}

void Config::test(){
	for(auto &it:m_frequencyFile){
		cout << it.first << ":" << it.second << endl;
	}

	for(auto &it:m_indexFile){
		cout << it.first << ":";
		for(auto &idx:it.second){
			cout << idx << " ";
		}
		cout << endl;
	}
}
int Config::configFileHandle(){
	int fd = open(m_configFileName.c_str(),O_RDWR);
	ERROR_HANDLE(fd,-1,"open");
	char c;
	while((read(fd,&c,1))!=0){
		if(ispunct(c)){
			lseek(fd,-1,SEEK_CUR);
			c = ' ';
			write(fd,&c,1);
		}else if(isupper(c)){
			lseek(fd,-1,SEEK_CUR);
			c += 32;
			write(fd,&c,1);	
		}
	}
	return 0;
}

int Config::createEnglishFrequencyFile(){
	string word;
	ifstream ifs(m_configFileName);
	while(ifs.good()){
		ifs >> word; 
		auto it = m_frequencyFile.find(word);
		if(it == m_frequencyFile.end()){
			m_frequencyFile[word] = 1;
		}else{
			++m_frequencyFile[word];
		}
		word.clear();
}
	//写入文件并创建索引文件的内容到m_indexfile中
	ofstream ofs("../../config/frquency_file.txt");
	int i = 0;//词频文件中单词的下标
	for(auto &it:m_frequencyFile){
		ofs << it.first << " ";
		ofs << it.second << endl;
		//写入文件完毕，下面创建索引文件
		for(auto &c1:it.first){
			//对每个字母创建索引，并将i插入set中
			if(isalpha(c1)){
				string s ;
				s+=c1;
				auto ret = m_indexFile.find(s);
				if(ret==m_indexFile.end()){
					//如果索引还不存在,创建一个索引
					set<int> se;
					se.insert(i);
					m_indexFile.insert(std::make_pair(s,se));
				}else{
					//如果索引存在，那么往set中插入这个下标值
					ret->second.insert(i);
				}
			}
		}
		++i;
	}//词频文件写入完毕，索引文件创建也完毕，下面写入索引文件
	ofs = ofstream("../../config/index_file.txt");
	for(auto &it:m_indexFile){
		ofs << it.first << " ";
		for(auto &j:it.second){
			ofs << j << " ";
		}
		ofs << endl;
	}
	return 0;
}

int Config::createChineseConfigFile(){
	//流程：读取文件的每行内容，对行进行分词，分词后的字符串进行频率统计

	const char* const DICT_PATH = "./dict/jieba.dict.utf8";
	const char* const HMM_PATH = "./dict/hmm_model.utf8";
	const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
	const char* const IDF_PATH = "./dict/idf.utf8";
	const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

	cppjieba::Jieba jieba(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH);
	vector<string> words;
	string word;
	string result;

  	cout << limonp::Join(words.begin(), words.end(), "/") << endl;

	DIR* dir = opendir(m_chineseFileName.c_str());
	if(dir==NULL){
		return -1;
	}
	struct dirent* d;
	string s;
	while((d = readdir(dir))!=NULL){
		string filename = d->d_name;
		if(filename!="."&&filename!=".."){
			string realtivePath = m_chineseFileName+filename;
			cout << realtivePath << endl;
			ifstream ifs(realtivePath);
			char line[1024] = {0};
			while(ifs.good()){
				ifs.getline(line,sizeof(line));
				s = line;
				jieba.Cut(s, words, true);
				result = limonp::Join(words.begin(),words.end()," ");
				istringstream iss(result);
				while(iss.good()){
					iss >> word;  //分词后的词
					//下面创建词频文件
					if(!isPunct(word)){//剔除标点符号
						auto it = m_frequencyFile.find(word);
						if(it!=m_frequencyFile.end()){
							//如果已经有这条记录，词频+1
							++it->second;
						}else{
							//如果没有这条记录，创建并初始化词频为1
							m_frequencyFile.insert(std::make_pair(word,1));
						}
					}
				}
			}
		}
	}//创建词频文件完毕,下面写入词频文件，并创建索引
	ofstream ofs("../../config/chinese_fre_file");
	int idx = 0;
	for(auto &ele:m_frequencyFile){
		ofs << ele.first << " " << ele.second << endl;
		string words = ele.first;
		for(int i = 0;i<words.size();i+=3){
			string word = words.substr(i,3);
			auto it = m_indexFile.find(word);
			if(it!=m_indexFile.end()){
				it->second.insert(idx);
			}else{
				set<int> se = {idx};
				m_indexFile.insert(std::make_pair(word,se));
			}
		}
		++idx;
	}
	ofs = ofstream("../../config/chinese_index_file");
	for(auto&ele:m_indexFile){
		ofs << ele.first << " ";
		for(auto &i:ele.second){
			ofs << i << " ";
		}
		ofs << endl;
	}
	return 0;
}

bool Config::isPunct(string word){
	if(word.size()%3!=0){
		//不是3个字节的倍数的，肯定不是中文汉字
		return true;
	}
	vector<string> punct = {"：","；","！","》","《","？","。","，","【","】","）","（"};
	auto it = ::find(punct.begin(),punct.end(),word);
	if(it==punct.end()){
		return false;
	}else{
		return true;
	}
}
int Config::getFrequency(const string& word){
	return m_frequencyFile[word];
}
}//end of namespace wd
