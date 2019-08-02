/*************************************************************************
 > File Name: main.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Fri 26 Jul 2019 07:26:23 PM PDT
 ************************************************************************/

#include<iostream>
#include <string>
#include <vector>
#include <map>
using std::map;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::multimap;

int editDistance(string src,string target){
	int row = src.size()+1;
	int column = target.size()+1;
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
	for(int i = 1;i<row;++i){
		for(int j = 1;j<column;++j){
			int min1 = process[i-1][j]+1;
			int min2 = process[i][j-1]+1;
			int flag = src[i-1]==target[j-1]?0:1;
			int min3 = process[i-1][j-1] + flag;
			int min4 = min1<min2?min1:min2;
			int min  = min4<min3?min4:min3;
			process[i][j] = min;
		}
	}
	for(int i = 0;i<row;++i){
		for(int j = 0;j<column;++j){
			cout << process[i][j] << " ";
		}
		cout << endl;
	}
	return process[row-1][column-1];
}
string getReturnWord(string msg,vector<string> &svc){
	multimap<int,string> v;
	int dis;
	for(auto &word:svc){
		dis = editDistance(msg,word);
		v.insert(std::make_pair(dis,word));
	}
	
	vector<string> lessDisWords;//存放最小距离相等的单词
	//处理最小距离相等的单词
	int lessDis = v.begin()->first;
	auto it2 = v.begin();
	while(it2!=v.end() && lessDis== it2->first){
		lessDisWords.push_back(it2->second);
		++it2;
	}
	//处理
	//
	return lessDisWords[0];
}
int main(int argc,char* argv[]){
	editDistance((string)"aphoe",(string)"iphone");
}

