/*************************************************************************
 > File Name: main.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Sun 28 Jul 2019 06:18:09 PM PDT
 ************************************************************************/

#include "Config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace wd;
int main(int argc,char* argv[]){
	Config("../../config/english.txt","../../config/chinese/");
}
