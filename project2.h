/*
Christopher Lanham
CS465
Programming Assignment 2
12/3/13
*/
#include<cstdio>
#include<string>
#include<vector>
using namespace std;

struct account{
	string name;
	string password;
};

struct group{
	string name;
	vector<string> members;
};

struct file{
	string name;
	string owner;
	string group;
	string owner_permission;
	string group_permission;
	string other_permission;
};


void cmdHandler(string);
void print(string);
int checkName(string);
int checkWhitespace(string);
int checkPswd(string);
int checkFilename(string);
void useradd(string,string);
vector<string> tokenize(string);
int findUser(string);
void login(string, string);
void clearFiles();
void groupadd(string);
int findGroup(string);
void usergrp(string, string);
int checkDuplicate(vector<string>, string);
void mkfile(string);
int findFile(string);
void printFiles();
void printAccounts();
void printGroups();
void chmod(string, string, string, string);
void chown(string, string);
void chgrp(string, string);
int checkUsergrp(string, int);
bool fileExists(string);
void read(string);
int checkPermission(file,int,char);
void write(string,string);
void execute(string);
void ls(string);

vector<account> accounts;
vector<group> groups;
vector<file> files;
