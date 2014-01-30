#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
	char 	*fName;				// DB file name
	File 	*heapFile;			// DB file instance
	Page 	*readPage; 			// Buffer page to store while reading from catalog
	Page 	*writePage;			// Buffer page to store while writing to console
	int	currRead;			// To point page currently reading from db
	ComparisonEngine comp;  		// Instance of comparision Engine
	int  IsReadBuffEmpty(); 		// To check if readBuff is Empty
	void AddReadPagetoFile();		// Add current read buff page to dbFile
	void PrintNumRecordsinPage(Page &pg);	// Print num of record in Page; Used for debugging

public:
	DBFile (); 
	~DBFile();
	int Create (char *fpath, fType file_type, void *startup);
	int Open (char *fpath);
	int Close ();

	void Load (Schema &myschema, char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

};
#endif
