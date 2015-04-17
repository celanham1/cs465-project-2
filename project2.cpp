/*
Christopher Lanham
CS465
Programming Assignment 2
12/3/13
*/
#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>

#include "project2.h"
using namespace std;


string user;	//Global variable to store the name of the logged in user
int first = 0;

int main(int argc, char *argv[]){
	int exit = 0;
	string filename;
	string input;
	vector<string> tokens;

	clearFiles();

	if(argc > 1) filename = argv[1];

	ifstream file;
	file.open(filename.c_str(), std::ios::out | std::ios::app);
	if(file.is_open()){
		while(getline(file,input)){
			cmdHandler(input);
		}
		file.close();
	}
	
	//Write the vectors to .txt files
	printAccounts();
	printGroups();
	printFiles();

	return 0;
}

void cmdHandler(string in){
	int exit = 0;

	vector<string> tokens = tokenize(in);
	
	if(first == 0 && tokens.front().compare("useradd") != 0)  {
		print("First command must be to add root user");
	}
	else if(tokens.front().compare("useradd") == 0){
		if(tokens.size() != 3) {
			print("Error: Wrong number of inputs");
		}
		else{
			if(first == 0 && tokens.at(1).compare("root") != 0) {
				print("Error: Must add root user first");
			}
			else if(first == 0 && tokens.at(1).compare("root") == 0) {
				first = 1;
				useradd(tokens.at(1),tokens.at(2));
			}
			else{
				if(user.compare("root") == 0) useradd(tokens.at(1),tokens.at(2));
				else{
					print("Error: Must be logged in as root to add users");
				}
			}
		}
	} 
	else if(tokens.front().compare("login") == 0){
		if(tokens.size() != 3) {
			print("Error: Wrong number of inputs");
		}
		else{
			if(user.empty()) login(tokens.at(1),tokens.at(2));
			else {
				print("Error: Only one user can be logged in at the same time"); 
			}
		}
	} 
	else if(tokens.front().compare("logout") == 0){
		if(tokens.size() != 1) {
			print("Error: Wrong number of inputs");
		}
		else {
			if (user.empty()) print("Error: User must be logged in");
			else {
				print(user + " logged out"); 
				user.clear();
			}
		}
	} 
	else if(tokens.front().compare("groupadd") == 0){
		if(tokens.size() != 2) {
			print("Error: Wrong number of inputs");
		}
		else {
			if(user.compare("root") == 0) groupadd(tokens.at(1));
			else {
				print("Error: "+user+"dosn't have permission to create groups"); 
			}
		}
	} 
	else if(tokens.front().compare("usergrp") == 0){
		if(tokens.size() != 3) {
			print("Error: rong number of inputs");
		}
		else if (user.empty()) print("Error: User must be logged in");
		else {
			if(user.compare("root") == 0) usergrp(tokens.at(1),tokens.at(2));
			else {
				print("Error: "+user+"dosn't have permission to assign groups"); 
			}
		}
		
	} 
	else if(tokens.front().compare("mkfile") == 0){
		if(tokens.size() != 2) {
			print("ErrorWrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else mkfile(tokens.at(1));
	} 
	else if(tokens.front().compare("chmod") == 0){
		if(tokens.size() != 5) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else{
			chmod(tokens.at(1),tokens.at(2),tokens.at(3),tokens.at(4));
		}
	} 
	else if(tokens.front().compare("chown") == 0){
		if(tokens.size() != 3) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else{ 
			if(user.compare("root") == 0) chown(tokens.at(1),tokens.at(2));
			else {
				print("Error: "+user+"dosn't have permission to change owners");
			}
		}
	} 
	else if(tokens.front().compare("chgrp") == 0){
		if(tokens.size() != 3) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else chgrp(tokens.at(1),tokens.at(2));
	} 
	else if(tokens.front().compare("read") == 0){
		if(tokens.size() != 2) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else read(tokens.at(1));
	} 
	else if(tokens.front().compare("write") == 0){
		if(tokens.size() < 3) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else{
			string filename = tokens.at(1);
			string text;
			//Concatonate the rest of the tokens into a string
			for(int i=2; i<tokens.size(); i++){
				text += tokens.at(i) + " ";
			}
			write(filename,text);
		}
	} 
	else if(tokens.front().compare("execute") == 0){
		if(tokens.size() != 2) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else execute(tokens.at(1));
	} 
	else if(tokens.front().compare("ls") == 0){
		if(tokens.size() != 2) {
			print("Error: Wrong number of inputs");
		}
		else if (user.empty()) {
			print("Error: User must be logged in");
		}
		else ls(tokens.at(1));
	} 
	else if(tokens.front().compare("end") == 0){
		if(tokens.size() != 1) {
			print("Error: Wrong number of inputs");
		}
		else return;
	} 
	else {
		print("Error: No commands match " + tokens.front()); 
	}
	return;
}

void print(string str){
	ofstream audit;
	audit.open("audits.txt", std::ios::out | std::ios::app);
	cout << str << endl;
	audit << str << endl;
	audit.close();
}

int checkName(string in){
	int len = in.length();
	if(len > 30) return 1;
	else if(in.find('/') <= len || in.find(':') <= len) return 1;
	else if(checkWhitespace(in) == 1) return 1;
	else return 0;
}

int checkPswd(string in){
	if(in.length() > 30) return 1;
	else if(checkWhitespace(in) == 1) return 1;
	else return 0;
}

int checkWhitespace(string in){
	for(int i=0; i < in.length(); i++){
		if(isspace(in[i]) != 0) return 1;
	}
	return 0;
}

void clearFiles(){
	ofstream accounts;
	ofstream groups;
	ofstream files;
	ofstream audits;
	
	accounts.open("accounts.txt", std::ios::out | std::ios::trunc);
	groups.open("groups.txt", std::ios::out | std::ios::trunc);
	files.open("files.txt", std::ios::out | std::ios::trunc);
	audits.open("audits.txt", std::ios::out | std::ios::trunc);

	accounts.close();
	groups.close();
	files.close();
	audits.close();
}

void useradd(string username, string password){
	account a;
	if(findUser(username) == 0){
		if(checkName(username) != 0) print("Username is invalid");
		else if(checkPswd(password) != 0) print("Password is invalid");
		else{
			a.name = username;
			a.password = password;
			accounts.push_back(a);
			print("username "+ username + " created");
		}
	}
	else {
		print(username + " already created"); 
	}
}

void printAccounts(){
	ofstream outFile;
	account a;
	outFile.open("accounts.txt", std::ios::out | std::ios::app);
	for(int i=0; i<accounts.size(); i++){
		a = accounts.at(i);
		outFile << a.name << " " << a.password << endl;
	}
	outFile.close();
}

int findUser(string username){
	account a;
	for(int i=0; i<accounts.size(); i++){
		a = accounts.at(i);
		if(a.name.compare(username)==0) return 1;
	}

	return 0;
}

vector<string> tokenize(string line){
	string input = line;
	string buffer;
	stringstream ss(input);
	vector<string> tokens;
	while(ss >> buffer){
		tokens.push_back(buffer);
	}
	return tokens;
}

void login(string username, string password){
	string line;
	vector<string> tokens;
	account a;
	for(int i=0; i<accounts.size(); i++){
		a = accounts.at(i);
		if(a.name.compare(username) == 0){
			if(a.password.compare(password) != 0) {
				print("password incorrect"); 
			}
			else{
				user = a.name;
				print("logged in as " + username); 
			}
			return;
		}
	}
	print("username not found"); 
}

void groupadd(string groupname){
	group g;
	if(findGroup(groupname) == -1) {
		if(checkName(groupname) != 0) print("name is invalid");
		else{
			g.name = groupname;
			groups.push_back(g);
			print("group "+ groupname + " created");
		}
	}
	else {
		print(groupname + " already created"); 
	}
}

void printGroups(){
	ofstream outFile;
	group g;
	string members;
	outFile.open("groups.txt", std::ios::out | std::ios::app);
	for(int i=0; i<groups.size(); i++){
		g = groups.at(i);
		for(int j=0; j<g.members.size(); j++){
			members += " "+ g.members.at(j);
		}
		outFile << g.name << members << endl;
		members.clear();
	}
	outFile.close();
}

int findGroup(string str){
	group g;
	for(int i=0; i<groups.size(); i++){
		g = groups.at(i);
		if(g.name.compare(str)==0) return i;
	}
	return -1;
}

void usergrp(string username, string groupname){
	int pos = findGroup(groupname);
	group g;
	if(findUser(username) == 0) {
		print(username + " not found"); 
	}
	else if(pos == -1) {
		print(groupname + " not found"); 
	}
	else{
		g = groups.at(pos);
		if(checkDuplicate(g.members,username) == 1) {
			print(username + " already in " + groupname); 
		}
		else {
			g.members.push_back(username);
			groups.at(pos) = g;
			print(username + " added to group " + groupname); 
		}
	}
}

int checkDuplicate(vector<string> tokens, string str){
	for(int i=0; i < tokens.size(); i++){
		if(tokens.at(i).compare(str) == 0) return 1;
	}
	return 0;
}

int checkFilename(string s){
	if(s.compare("accounts.txt") == 0) return 1;
	else if(s.compare("groups.txt") == 0) return 1;
	else if(s.compare("files.txt") == 0) return 1;
	else if(s.compare("audits.txt") == 0) return 1;
	else if(checkName(s) != 0) return 1;
	else return 0;
}

void mkfile(string filename){
	ofstream newFile;
	file file;
	if(checkFilename(filename) == 1){
		print("Can't use " + filename); 
	}
	else if(findFile(filename) == -1) {
		file.name = filename;
		file.owner = user;
		file.group = "nil";
		file.owner_permission = "rw-";
		file.group_permission = "---";
		file.other_permission = "---";
		files.push_back(file);
		newFile.open(filename.c_str());
		newFile.close();
		print(user+" created file "+filename+" with default permissions");
	}
	else {
		print(filename + " already exists"); 
	}
}

int findFile(string name){
	file f;
	for(int i=0; i<files.size(); i++){
		f = files.at(i);
		if(f.name.compare(name)==0) return i;
	}
	return -1;
}

void printFiles(){
	ofstream outFile;
	file f;
	outFile.open("files.txt", std::ios::out | std::ios::app);
	for(int i=0; i<files.size(); i++){
		f = files.at(i);
		string line = f.name + " " + f.owner + " " + f.group + " " + f.owner_permission + " " + f.group_permission + " " + f.other_permission;
		outFile << line << endl;
	}
	outFile.close();
}

void chmod(string filename, string owner_p, string group_p, string other_p){
	int pos = findFile(filename);
	file f;
	if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else{
		f = files.at(pos);
		if(f.owner.compare(user) == 0 || user.compare("root") == 0){
			f.owner_permission = owner_p;
			f.group_permission = group_p;
			f.other_permission = other_p;
			print(user+" set "+filename+" permissions to "+owner_p+" "+group_p+" "+other_p);
			files.at(pos) = f;
		}
		else {
			print("Error: "+user+"doesn't have permission to modify permisisons"); 
		}
	}
}

void chown(string filename, string username){
	int pos = findFile(filename);
	file f;
	if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else if(findUser(username) == 0) {
		print(username + " doesn't exist"); 
	}
	else{
		f = files.at(pos);
		if(user.compare("root") == 0){
			f.name = filename;
			f.owner = username;
			files.at(pos) = f;
			print(user + "set " + filename + " owner to "+username);
		}
		else {
			print("Error: "+user+"doesn't have permission to change owners"); 
		}
	}
}

void chgrp(string filename, string groupname){
	int fpos = findFile(filename);
	int gpos = findGroup(groupname);
	file f;
	if(fpos == -1) {
		print(filename + " doesn't exist"); 
	}
	else if(gpos == -1) {
		print(groupname +" doesn't exist"); 
	}
	else{
		f = files.at(fpos);
		if(user.compare("root") == 0 || f.owner.compare(user) == 0){
			if(checkUsergrp(groupname,gpos) == 1 || user.compare("root") == 0 || groupname.compare("nil") == 0){
				print(f.group + " changed to " + groupname); 
				f.group = groupname;
				files.at(fpos) = f;
			}
			else {
				print("Error: "+user+" doesn't have permission to modify group of " + filename); 
			}
		}
		else {
			print("Error: "+user+"doesn't have permission to change group"); 
		}
	}
}

int checkUsergrp(string groupname, int pos){
	group g;
	g = groups.at(pos);
	for(int i=0; i< g.members.size(); i++){
		if(g.members.at(i).compare(user) == 0) return 1;
	}
	return 0;
}

bool fileExists(string name){
	ifstream ifs;
	ifs.open(name.c_str());
	return ifs.good();
}

void read(string filename){
	ifstream ifs;
	file f;
	int pos = findFile(filename);
	string line;
	if(checkFilename(filename) == 1){
		print("Can't use " + filename); 
	}
	else if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else{
		f = files.at(pos);
		if(checkPermission(f,0,'r') == 1){
			print("Reading from " + filename); 
			ifs.open(filename.c_str());
			while(getline(ifs,line)){
				print(line);
			}
			ifs.close();
		}
		else {
			print("Error: "+user+"doesn't have permission to read "+filename); 
		}
	}
	
}

int checkPermission(file f, int pos, char p){
	int gpos = findGroup(f.group);
	int sameGroup;

	if(f.group.compare("nil") == 0) sameGroup = 0;
	else sameGroup = checkUsergrp(f.group, gpos);
	
	if(user.compare(f.owner) == 0 && f.owner_permission.at(pos) == p) return 1;
	else if(user.compare(f.owner) != 0 && sameGroup == 1 && f.group_permission.at(pos) == p) return 1;
	else if(user.compare(f.owner) != 0 && sameGroup == 0 && f.other_permission.at(pos) == p) return 1;
	
	return 0;
}

void write(string filename, string text){
	ofstream ofs;
	file f;
	int pos = findFile(filename);
	if(checkFilename(filename) == 1){
		print("Can't use " + filename); 
	}
	else if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else{
		f = files.at(pos);
		if(checkPermission(f,1,'w') == 1){
			print(user + " wrote to file " + filename + ": " + text); 
			ofs.open(filename.c_str(), std::ios::out | std::ios::app);
			ofs << text << endl;
			ofs.close();
		}
		else {
			print("Error: "+user+" doesn't have permission to write to "+filename); 
		}
	}
}

void execute(string filename){
	file f;
	int pos = findFile(filename);
	if(checkFilename(filename) == 1){
		print("Can't use " + filename); 
	}
	else if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else{
		f = files.at(pos);
		if(checkPermission(f,2,'x') == 1){
			print(filename + " executed by " + user); 
		}
		else {
			print("Error: "+user+" doesn't have permission to execute "+filename); 
		}
	}
	
}

void ls(string filename){
	int pos = findFile(filename);
	if(checkFilename(filename) == 1){
		print("Can't use " + filename); 
	}
	else if(pos == -1) {
		print(filename + " doesn't exist"); 
	}
	else{
		file f = files.at(pos);
		string line = f.name + " " + f.owner + " " + f.group + " " + f.owner_permission + " " + f.group_permission + " " + f.other_permission;
		print(line);
	}
}

