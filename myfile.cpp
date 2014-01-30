
#include<fcntl.h>
#include<iostream>

using namespace std;

int main() {

	int fd = open("lineitem.bin",2,S_IRUSR);

	cout<<fd;
	return 0;
}



