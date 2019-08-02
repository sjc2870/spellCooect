#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <json/json.h>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void do_service(int sockfd);
string jsonToString(char*,char*);
int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.4.130"); //localhost
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(8888);
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

	char buf[1024] = {0};
	int length;
	read(peerfd,&length,sizeof(length));
	read(peerfd, buf, length);
	printf("%s\n", buf);

    do_service(peerfd);
    return 0;
}



void do_service(int sockfd)
{
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    while(1)
    {
		cout << "请输入数据：" << endl;
        fgets(sendbuf, sizeof sendbuf, stdin);
        write(sockfd, sendbuf, strlen(sendbuf));
		//sleep(5);

        //read
		int len = 0;
		read(sockfd,&len,sizeof(len));
        int nread = read(sockfd, recvbuf,len);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }

        printf("receive msg : %s\n", recvbuf);
		string ret;
		ret = jsonToString(sendbuf,recvbuf);
		cout << "解析后的数据为：" << ret << endl;
        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}

string jsonToString(char* key,char* src){
	Json::Reader reader;
	Json::Value root;
	key[strlen(key)-1] = '\0';  //去掉\n
	cout << "key = " << key
		 << ",src = " << src << endl;
	string key1 = key;
	string src1 = src;
	if(!reader.parse(src1,root)){
		perror("parse");
		return NULL;
	}
	string ret = root[key1].asString();
	cout << "ret = " << ret << endl;
	return ret;
}
