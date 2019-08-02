/*************************************************************************
 > File Name: main.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Thu 25 Jul 2019 07:36:56 PM PDT
 ************************************************************************/
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
using std::cout;
using std::endl;

int main(int argc,char* argv[]){
	int fd = open("./english.txt",O_RDWR);
	if(fd == -1){
		perror("open");
	}
	char c;
	while((read(fd,&c,1))!=0){
		if(ispunct(c)){
			lseek(fd,-1,SEEK_CUR);
			c = ' ';
			write(fd,&c,1);
		}else if(isupper(c)){
			lseek(fd,-1,SEEK_CUR);
			c+=32;
			write(fd,&c,1);
		}
	}
}

