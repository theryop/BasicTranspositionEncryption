#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <limits>

using namespace std;

int main(int argc, char *argv[])
{
	bool process = true; // true for encrypt, false for decrypt
	int counter = 0;
	int counter2 = 0;
	int blocksize = 8;
	char a;
	string keyfile = "key.txt";
	vector<char> keylist;
	if (argc > 1)
	{
		counter = 0;
		string comptemp = string(argv[counter]);
		size_t found = comptemp.find("-k");
		size_t found2 = comptemp.find("-b");
		while (counter < argc)
		{
			comptemp = string(argv[counter]);
			found = comptemp.find("-k");
			found2 = comptemp.find("-b");
			if ((comptemp)== "-d")
			{
				process = false;
			}
			if ((comptemp) == "-e")
			{
				process = true;
			}
			if (found != string::npos)
			{
				keyfile = comptemp.substr(2,comptemp.size() - 2);
				cout << "keyfile is now " << keyfile << endl;
			}	
			if(found2 != string::npos)
			{
				string bstemp = comptemp.substr(2, comptemp.size() - 1);
				cout << "block size is now " << bstemp << endl;
				blocksize = atoi(bstemp.c_str());
			}
			counter++;
		}
	}
	ifstream key (keyfile.c_str());
	if (!key.is_open())
	{
		throw invalid_argument("key file not found");
		exit(1);		
	}
	//int ki = getchar();
	//while(ki != EOF)
	//{
	//	keylist.push_back(ki);
	//	ki = getchar();
	//}
	while (true)
	{
		if(!(key >> a))
		{
			break;
		}
		char temp;
		key.get(temp);
		keylist.push_back(temp);
		cout << "keylist now has " << keylist.back() << endl;
	}
	
	int keysize = keylist.size() / 2;
	
	
	
	counter = 0;
	while ((unsigned)counter < keylist.size()) // checking to see if any bytes are larger than n or if there are any duplicates
	{
		char ccheck1 = keylist.at(counter);
		char ccheck2 = keylist.at(counter + 1);
		cout << "ccheck1 is " << keylist.at(counter) << " and ccheck2 is " << keylist.at(counter + 1) << endl; 
		if ((((+ccheck1 < 58) && (+ccheck1 > 47)) || ((+ccheck1 < 103) && (+ccheck1 > 96))) && (((+ccheck2 < 58) && (+ccheck2 > 47)) || ((+ccheck2 < 103) && (+ccheck2 > 96))))
		{
			int dec1, dec2, findec;
			int num1 = +ccheck1;
			int num2 = +ccheck2;
			if (+num1 > 58)
			{
				switch (num1)
				{
					case 97: dec1 = 160; break;
					case 98: dec1 = 176; break;
					case 99: dec1 = 192; break;
					case 100: dec1 = 208; break;
					case 101: dec1 = 224; break;
					case 102: dec1 = 240; break;
				}
			}
			else
			{
				dec1 = ccheck1 - '0';
			}
			if (num2 > 58)
			{
				switch (num2)
				{
					case 97: dec2 = 10; break;
					case 98: dec2 = 11; break;
					case 99: dec2 = 12; break;
					case 100: dec2 = 13; break;
					case 101: dec2 = 14; break;
					case 102: dec2 = 15; break;
				}
			}
			else
			{
				dec2 = ccheck2 - '0';
			}
			findec = dec1 + dec2;
			if (findec >= keysize)
			{
				cerr << "Error: byte " << counter << " is larger than the number of bytes." << endl;
				exit(1);
			}
			counter2 = counter + 2;
			while ((unsigned)counter2 < keylist.size())
			{
				char mccheck1 = keylist.at(counter2);
				char mccheck2 = keylist.at(counter2 + 1);
				if (mccheck1 == ccheck1 && mccheck2 == ccheck2)
				{
					cerr << "duplicate byte at " << counter << " and " << counter2 << ":" << mccheck1 << mccheck2 << endl;
				}
				counter2 +=2;
			}
		}
		else //keybyte is not in hex format 
		{
			cerr << "byte " << counter << " (" << ccheck1 << ") " << " is not in hexidecimal format" << endl;
			exit(1);
		}
		
	counter+=2;
	}
	
	vector<char> useri(keysize * 2);
	
	
	int ui = getchar();
	counter = 0;
	while (ui != EOF) // reads inputstream and fills input string with chosen input
	{
		useri.push_back(ui);
		ui = getchar();
	}
	

	
	if (useri.size() % blocksize != 0)
	{
		cerr << "Input is not split into acceptible blocks of size " << blocksize << endl;
	}
	
	vector<char> usero (useri.size());
	
	if (process) //transposition encryption
	{
		counter = 0;
		counter2 = 0; //counter goes thru entire input, counter2 assigns by block
		while ((unsigned)counter < useri.size()/ blocksize)
		{
			while (counter2 < blocksize)
			{
				char check = useri.at(counter2 + (counter * (blocksize)));
				char keyi1;
				char keyi2;
				if (counter2 == 0)
				{
					keyi1 = keylist.at(0);
					keyi2 = keylist.at(1);
				}
				else
				{
					keyi1 = keylist.at(((counter2 + 1) * 2) - 2);
					keyi2 = keylist.at(((counter2 + 1) * 2) - 1);
				}
				int sum1 = 0;
				int sum2 = 0;
				int fsum = 0;
				int num3 = +keyi1;
				int num4 = +keyi2;
				if (+num3 > 58)
				{
					switch (num3)
					{
						case 97: sum1 = 160; break;
						case 98: sum1 = 176; break;
						case 99: sum1 = 192; break;
						case 100: sum1 = 208; break;
						case 101: sum1 = 224; break;
						case 102: sum1 = 240; break;
					}
				}
				else
				{
					sum1 = keyi1 - '0';
				}
				if (num4 > 58)
				{
					switch (num4)
					{	
						case 97: sum2 = 10; break;
						case 98: sum2 = 11; break;
						case 99: sum2 = 12; break;
						case 100: sum2 = 13; break;
						case 101: sum2 = 14; break;
						case 102: sum2 = 15; break;
					}
				}
				else
				{
					sum2 = keyi2 - '0';
				}
				fsum = sum1+ sum2 + ((blocksize) * counter);
				usero.at(fsum) = check;	
				counter2++;
			}
			counter ++;
			counter2 = 0;
		}
			
	}
	else // decryption
	{
		counter = 0;
		counter2 = 0;
		while ((unsigned)counter < useri.size()/ blocksize)
		{
			while (counter2 < blocksize)
			{
				char check = useri.at(counter2 + (counter * (blocksize)));
				int countind = counter2+ (counter * (blocksize));
				char check2; // index to search for
				char check3;
				int keyindex = 0;
				
				while (countind >= blocksize)
				{
					countind = countind - blocksize;
				}
				int remain = countind % 16;
				countind = countind - remain;
				countind = countind / 16;
				if (countind > 9)
				{
					switch (countind)
					{
						case 10: check2 = 'a'; break;
						case 11: check2 = 'b'; break;
						case 12: check2 = 'c'; break;
						case 13: check2 = 'd'; break;
						case 14: check2 = 'e'; break;
						case 15: check2 = 'f'; break;
					}
				}
				else
				{
					char conv = countind + '0';
					check2 = conv;
				}
				if (remain > 9)
				{
					switch (remain)
					{
						case 10: check3 = 'a'; break;
						case 11: check3 = 'b'; break;
						case 12: check3 = 'c'; break;
						case 13: check3 = 'd'; break;
						case 14: check3 = 'e'; break;
						case 15: check3 = 'f'; break;
					}
				}
				else
				{
					char conv2 = remain + '0';
					check3 = conv2;
				}
				int counter3 = 0; 
				while ( (unsigned)counter3 < keylist.size()) // searching loop for index
				{
					if (keylist.at(counter3) == check2 && keylist.at(counter3 + 1) == check3)
					{
						keyindex = (counter3/2) ;
					}
					
					counter3 += 2;
				}
				usero.at(keyindex + (counter * (blocksize))) = check;
				counter2++;
			}
		counter++;
		counter2 = 0;
		}
	}
		
	
	counter = 0;
	while (unsigned(counter) < usero.size()) //returns output string in characters
	{
		putchar(usero.at(counter));
		counter++;
	}
}
