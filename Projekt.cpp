#include<string>
#include<vector>
#include<set>
#include<fstream>
#include<windows.h>
#include"Projekt.h"

wstring ConcatStringsFromSet(set<wstring>, wstring);

Projekt::Projekt()
{
	naziv = lokacija = datum = L"";
}
Projekt::Projekt(wstring path)
{
	naziv = lokacija = datum = L"";
	UcitajCSVDatotekeUzDirektorija(path);
}

int Projekt::UcitajCSVDatotekeUzDirektorija(wstring path)
{
	// Cleaning
	setNedostatci.clear();
	setProfili.clear();
	Dionice.clear();

	// Variables
	Dionica *d;
	int counter = 0;
	bool isLoadedCSV;
	wchar_t *searchKey;
	wstring searchKeyString;
	WIN32_FIND_DATA fd;
	set<wstring> setDatum;
	set<wstring> setMjesto;
	set<wstring> setUlica;


	//*******************************************//
	//***** Getting naziv (directory name) ******//
	//*******************************************//
	searchKeyString = path + L"*";
	searchKey = new wchar_t[searchKeyString.length() + 1]; 
	for(int i = 0; i < searchKeyString.length(); i++)
		searchKey[i] = searchKeyString[i];
	searchKey[searchKeyString.length()] = L'\0';
	
    HANDLE h = FindFirstFile(searchKey, &fd);
	naziv = fd.cFileName;
	delete [] searchKey;


	//**************************************//
	//***** Getting list of CSV files ******//
	//**************************************//
	searchKeyString = path + L"\\*.csv";
	searchKey = new wchar_t[searchKeyString.length()  + 1]; 
	for(int i = 0; i < searchKeyString.length(); i++)
		searchKey[i] = searchKeyString[i];
	searchKey[searchKeyString.length()] = L'\0';
	
    h = FindFirstFile(searchKey, &fd);
	delete [] searchKey;

	//******************************//
	//***** Loading CSV files ******//
	//******************************//
    if(h == INVALID_HANDLE_VALUE)  return 0;    // If none found
    do
    {
		wstring CSVFilePath = path + L"\\" + fd.cFileName;
		d = new Dionica(CSVFilePath);
		if((*d).IsLoaded())
		{
			// Counter, adding to vector
			counter++;
			Dionice.push_back(*d);

			// Updating sets
			setDatum.insert((*d).GetDatum());
			setMjesto.insert((*d).GetMjesto());
			setUlica.insert((*d).GetUlica());

			setProfili.insert((*d).GetProfil());
			map<wstring, int> m = (*d).GetNedostatci();
			for(std::map<wstring, int>::const_iterator it = m.begin(); it != m.end(); it++)
				setNedostatci.insert((*it).first);
		}

		delete d;
    }while(FindNextFile(h, &fd) != FALSE);

	// Getting projects data
	datum = ConcatStringsFromSet(setDatum, L" ");
	lokacija =  ConcatStringsFromSet(setMjesto, L", ");
	lokacija += L"ULICE: " + ConcatStringsFromSet(setUlica, L", ");

    return counter;
}
bool Projekt::ExportInExcelCSVFormat(wstring path)
{
	wofstream out(path + L"\\" + naziv + L".csv");

	// Imbue stream
	std::locale loc("");
	out.imbue(loc);

	out << L";;STATISTIÈKI PREGLED NEDOSTATAKA" << endl; 

	out << L"NAZIV OBJEKTA:;" << naziv << endl; 
	out << L"LOKACIJA:;" << lokacija << endl; 
	out << L"DATUM:;" << datum << endl << endl;

	out << L"I. PREGLED NEDOSTATAKA PO DIONICAMA" << endl;
	out << L";;KOD NEDOSTATKA I KOLIÈINA" << endl;
	out << L"NAZIV DIONICE;PROFIL CIJEVI;";

	for(set<wstring>::iterator it = setNedostatci.begin(); it != setNedostatci.end(); it++)
		out << *it << L";";
	out << L"UKUPNO;" << endl;

	for(int i = 0; i < Dionice.size(); i++)
	{
		out << Dionice[i].GetNaziv() << L";" << Dionice[i].GetProfil() << L";";
		for(set<wstring>::iterator it = setNedostatci.begin(); it != setNedostatci.end(); it++)
		{
			int num = Dionice[i].NumberOfNedostatakaGivenCode(*it);
			if(num != 0)
				out << num << L";";
			else
				out << L"-;";
		}
		out << Dionice[i].ZbrojNedostataka() << endl;
	}

	out  << endl; 

	out << L"II. PREGLED NEDOSTATAKA PO PROFILIMA" << endl;
	out << L";KOD NEDOSTATKA I KOLIÈINA" << endl;
	out << L"PROFIL CIJEVI;";

	for(set<wstring>::iterator it = setNedostatci.begin(); it != setNedostatci.end(); it++)
		out << *it << L";";
	out << L"UKUPNO:" << endl;
	for(set<wstring>::iterator it = setProfili.begin(); it != setProfili.end(); it++)
	{
		out << *it << L";";
		for(set<wstring>::iterator iit = setNedostatci.begin(); iit != setNedostatci.end(); iit++)
		{
			int num = NumberOfNedostatakaGivenCodeandProfil(*iit, *it);
			if(num != 0)
				out << num << L";";
			else
				out << L"-;";
		}
		out << NumberOfNedostatakaGivenProfil(*it) << endl;
	}
	out << L"UKUPNO:;";
	for(set<wstring>::iterator iit = setNedostatci.begin(); iit != setNedostatci.end(); iit++)
	{
		out << NumberOfNedostatakaGivenCode(*iit) << L";";
	}
	out << TotalNumberOfNedostatakaSummingProfiles() << endl;

	return true;
}
bool Projekt::ExportInTxtFormat(wstring path)
{
	wofstream out(path + L"\\" + naziv + L".txt");

	out << *this;

	out.close();

	return true;
}

int Projekt::NumberOfNedostatakaGivenCode(wstring nedostatakCode)
{
	int sum = 0;

	for(int i = 0; i < Dionice.size(); i++)
	{
		sum += Dionice[i].NumberOfNedostatakaGivenCode(nedostatakCode);
	}

	return sum;
}
int Projekt::NumberOfNedostatakaGivenProfil(wstring profil)
{
	int sum = 0;

	for(int i = 0; i < Dionice.size(); i++)
		if(Dionice[i].GetProfil() == profil)
		{
			sum += Dionice[i].ZbrojNedostataka();
		}

	return sum;
}
int Projekt::NumberOfNedostatakaGivenCodeandProfil(wstring nedostatakCode, wstring profil)
{
	int sum = 0;

	for(int i = 0; i < Dionice.size(); i++)
		if(Dionice[i].GetProfil() == profil)
		{
			sum += Dionice[i].NumberOfNedostatakaGivenCode(nedostatakCode);
		}

	return sum;
}

int Projekt::TotalNumberOfNedostatakaSummingProfiles(void)
{
	int sum = 0;
	for(std::set<wstring>::iterator it = setNedostatci.begin(); it != setNedostatci.end(); it++)
	{
		sum += NumberOfNedostatakaGivenCode(*it);
	}
	return sum;
}
int Projekt::TotalNumberOfNedostatakaSummingCodes(void)
{
	int sum = 0;
	for(std::set<wstring>::iterator it = setProfili.begin(); it != setProfili.end(); it++)
	{
		sum += NumberOfNedostatakaGivenProfil(*it);
	}
	return sum;
}

wostream& operator<<(wostream& s, const Projekt& p)
{
	// Imbue stream
	std::locale loc("");
	s.imbue(loc);

	s << L"Naziv projekta: " << p.naziv << endl;
	s << L"Lokacije projekta: " << p.lokacija << endl;
	s << L"Datumi projekta: " << p.datum << endl;

	int N = p.Dionice.size();

	for(int i = 0; i < N; i++)
		s << p.Dionice[i];

	return s;
}

wstring ConcatStringsFromSet(set<wstring> s, wstring separator)
{
	wstring ret = L"";

	for(set<wstring>::iterator myIterator = s.begin(); myIterator != s.end(); myIterator++)
		ret += *myIterator + separator;

	return ret;
}