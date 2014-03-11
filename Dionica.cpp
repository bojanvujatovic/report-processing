#include<string>
#include<vector>
#include<fstream>
#include"Dionica.h"

vector<wstring> ExtractStrings(wstring line);
wstring ExtractNedostatakCode(wstring line);
wstring ExtractNedostatakCode2(wstring line);
wstring ExtractNedostatakCode3(wstring line);
bool RegisterCode(wstring nedostatakCode);
int FindIndexOfString(vector<wstring> strings, wstring str);

Dionica::Dionica()
{
	naziv = mjesto = ulica = datum = profilCijevi = L"";
}
Dionica::Dionica(wstring path)
{
	naziv = mjesto = ulica = datum = profilCijevi = L"";
	isLoaded = false;
	UcitajIzCSVDatoteke(path);
}

bool Dionica::UcitajIzCSVDatoteke(wstring path)
{
	// Openinf file
	wifstream csvFile(path);
	if(!csvFile.is_open()) {isLoaded = false; return false;}

	// Imbue file stream
	std::locale loc("");
	csvFile.imbue(loc);

	// Other variables
	wstring line;
	vector<wstring> strings;
	int index;

	//**************************************************//
	//***** First line containing naziv and datum ******//
	//**************************************************//
	getline<wchar_t>(csvFile, line);
	strings = ExtractStrings(line);

	// Naziv
	index = FindIndexOfString(strings, L"Broj inspekcije");
	if(index == -1) {isLoaded = false; return false;}
	naziv = strings[index + 2];

	// Datum
	index = FindIndexOfString(strings, L"Datum inspekcije");
	if(index == -1) {isLoaded = false; return false;}
	datum = strings[index + 1];

	//*******************************************************************//
	//***** Second line containing mjesto, ulica and profilCijevi *******//
	//*******************************************************************//
	getline<wchar_t>(csvFile, line);
	strings = ExtractStrings(line);

	// Mjesto
	index = FindIndexOfString(strings, L"Mjesto");
	if(index == -1) {isLoaded = false; return false;}
	mjesto = strings[index + 1];

	// Ulica
	index = FindIndexOfString(strings, L"Ulica");
	if(index == -1) {isLoaded = false; return false;}
	ulica = strings[index + 1];

	// Profil Cijevi
	index = FindIndexOfString(strings, L"Širina");
	if(index == -1) {isLoaded = false; return false;}
	profilCijevi = strings[index + 1];

	//*************************************************//
	//***** Rest of lines containing Nedostatci *******//
	//*************************************************//
	
	// Skipping one tile line 
	getline<wchar_t>(csvFile, line);

	while(!csvFile.eof())
	{
		getline<wchar_t>(csvFile, line);
		if(line.length() < 2) continue;
		wstring nedostatakCode = ExtractNedostatakCode(line);

		if(RegisterCode(nedostatakCode))
		{
			map<wstring, int>::iterator it = Nedostatci.find(nedostatakCode);
			if(it == Nedostatci.end())
				Nedostatci[nedostatakCode] = 1;
			else
				Nedostatci[nedostatakCode] += 1;
		}
	}
	
	csvFile.close();
	isLoaded = true;
	return true;
}

int Dionica::ZbrojNedostataka(void)
{
	int sum = 0;

	for(std::map<wstring, int>::const_iterator it = Nedostatci.begin(); it != Nedostatci.end(); it++)
		sum += (*it).second;

	return sum;
}

int Dionica::NumberOfNedostatakaGivenCode(wstring nedostatakCode)
{
	if(Nedostatci.find(nedostatakCode) == Nedostatci.end())
		return 0;

	return Nedostatci[nedostatakCode];
}

vector<wstring> ExtractStrings(wstring line)
{
	// Variables
	int N = line.length(), idx = -1;
	bool isFirstQuote = true;
	vector<wstring> strings;

	for(int i = 0; i < N; i++)
	{
		if (line[i] == L'"' && !isFirstQuote)   // End of the string
		{
			isFirstQuote = true;
		}
		else if(line[i] == L'"' && isFirstQuote)   // Beginning of the string
		{
			isFirstQuote = false;
			strings.push_back(L"");
			idx++;
		}
		else if(!isFirstQuote)   // Middle of the string
			strings[idx].push_back(line[i]); 
	}

	return strings;
}

int FindIndexOfString(vector<wstring> strings, wstring str)
{
	int N = strings.size();

	for(int i = 0; i < N; i++)
		if(str == strings[i])
			return i;

	return -1;
}

wostream& operator<<(wostream& s, const Dionica& d)
{
	// Imbue stream
	std::locale loc("");
	s.imbue(loc);

	// Streaming data
	s << endl << L"Naziv: " << d.naziv << endl;
	s << L"Mjesto: " << d.mjesto << endl;
	s << L"Ulica: " << d.ulica << endl;
	s << L"Datum: " << d.datum << endl;
	s << L"Profil: " << d.profilCijevi << endl;
	s << L"Nedostatci: "<< endl;

	for(std::map<wstring, int>::const_iterator it = d.Nedostatci.begin(); it != d.Nedostatci.end(); it++)
		s << it->first << L"\t => " << it->second << endl;

	s << endl;
	return s;
}

// Uzima samo drugi dio koda
wstring ExtractNedostatakCode(wstring line)
{
	vector<wstring> strings = ExtractStrings(line);
	return strings[4];
}

// Uzima kod u prvoj kucici
wstring ExtractNedostatakCode2(wstring line)
{
	vector<wstring> strings = ExtractStrings(line);
	return strings[4];
}

// Uzima citavi kod u prvoj kucici
wstring ExtractNedostatakCode3(wstring line)
{
	vector<wstring> strings = ExtractStrings(line);
	return strings[4];
}


bool RegisterCode(wstring nedostatakCode)
{
	return true;
}