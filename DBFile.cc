//
// Authors : Jaideep Bethu 	   : 9756-9416
// 	     Sai Krishna Vadrevu   : 1033-8830 	
		

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <string.h>
#include<string>

DBFile::DBFile () {
	heapFile 	= new File;
	readPage 	= new Page;
	writePage	= new Page;
	fName 		= NULL;
	currRead	= 0;
}

DBFile::~DBFile() {
	delete(heapFile);
	delete(readPage);
	delete(writePage);
}

// Return the file length (in number of pages)
int DBFile::Create (char *f_path, fType f_type, void *startup) {
	
	fName  = strdup(f_path);
	heapFile->Open(0,f_path);

/* To-do: For Project-2
	FILE *metaFile=fopen("heapfile.meta","w");
	fprintf(metaFile,"%s",f_path);
	fclose(metaFile);
*/

	return 1;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
	Record temp;
	FILE *tableFile = fopen (loadpath, "r");
	while (temp.SuckNextRecord(&f_schema,tableFile) == 1) {
			this->Add(temp);
	}
}

int DBFile::Open (char *f_path) {
	heapFile->Open(1,f_path);
		return 1;

/* To-do: For Project-2
	if(fName==NULL){
		FILE *metaFile = fopen("heapfile.meta","r");
		char temp[50];
		if(fscanf(metaFile,"%s",temp)!=EOF){
			fName=strdup(temp);
		}
		else
			return 0;
	}

	if(strcmp(fName,f_path)==0) {

		heapFile->Open(1,f_path);

		return 1;
	}
	else {
		return 0;
	}
*/

}

void DBFile::MoveFirst () {
	writePage->EmptyItOut();
	currRead = 0;
}

int DBFile::Close () {
	this->AddReadPagetoFile();

	if(heapFile->Close() >= 0 )
		return 1;
	else
		return 0;
}

void DBFile::Add (Record &rec) {
	int res= readPage->Append(&rec);
	if(!res) {
		heapFile->AddPage(readPage,heapFile->GetLength());
		readPage->EmptyItOut();
		readPage->Append(&rec);
	}
	
	// for Debug
	// this->PrintNumRecordsinPage(*readPage); 
}

// For Debugging
void DBFile::PrintNumRecordsinPage(Page &pg) {
	printf("%d : %d \n",pg.GetNumRecords(),heapFile->GetLength());
}

void DBFile::AddReadPagetoFile() {
	if(readPage->GetNumRecords()!=0) {
		heapFile->AddPage(readPage,heapFile->GetLength());
		readPage->EmptyItOut();
	}
}

// returns 0 if no next record exists
int DBFile::GetNext (Record &fetchme) {

	if(writePage->GetFirst(&fetchme)==0){

		if(currRead<=heapFile->GetLength()-2) {
			heapFile->GetPage(writePage,currRead);
			currRead+=2;
		}
		else {
			if(this->IsReadBuffEmpty()) {
				this->AddReadPagetoFile();
				heapFile->GetPage(writePage,currRead);
				currRead+=2;
				return 1;
			}
			else
				return 0;
		}

		writePage->GetFirst(&fetchme);
		}
		else
			return 1;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

	Record temp;

	while(this->GetNext(temp)){
		if (comp.Compare(&temp, &literal, &cnf)) {
			fetchme.Consume(&temp);
			return 1;
		}
	}
			return 0;
}

// returns 0 if no records present in read buffer
int DBFile::IsReadBuffEmpty(){
	return readPage->GetNumRecords();
}


