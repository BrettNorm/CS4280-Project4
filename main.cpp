/*
	main function for driving project. Three different invocation methods as stated in README
*/

#include "scanner.h"

int main(int argc, char *argv[]) {
	
	ifstream inFile;
	string file;
	
	if(argc == 1){
		try {
			string input;
			ofstream inputFile;
			file = "userInputFile.sp2022";
			inputFile.open(file.c_str());
			while(getline(cin, input)) {
				inputFile << input << "\n";
			}

			inputFile.close();

		} catch(const ifstream::failure& e){
			cout << "File does not exist\n";
			return 1;
		}

	} else if(argc == 2){
		file = argv[1];
		file.append(".sp2022");

	} else if(argc > 2){ 
		cout << "ERROR: Too many arguments. 2 or less, please.\n";
		cout << "USAGE: 	   './scanner'	(hit enter, then keyboard input to userInputFile),\n";
		cout << "	or './scanner [fileName]'	(.sp2022 extension added automatically),\n";
		cout << "  	or './scanner < [fileName.ext]'\n";
		return 1;
	}
	cout << "\nOpening file stream...\n\n";
	inFile.open(file.c_str());
	testScanner(inFile);

	
	inFile.close();
	cout << "\nFile stream closed. Bye!\n\n";
		
	return 0;
}
