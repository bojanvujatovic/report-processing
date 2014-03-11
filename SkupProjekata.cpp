#include<vector>
#include<string>
#include<fstream>
#include<windows.h>
#include"SkupProjekata.h"

SkupProjekata::SkupProjekata()
{
}
SkupProjekata::SkupProjekata(wstring path)
{
	UcitajProjekteIzDirektorija(path);
}

int SkupProjekata::UcitajProjekteIzDirektorija(wstring path)
{
	// Path
	directoryPath = path;
	wstring searchKeyString = path + L"\\*";
	wchar_t *searchKey = new wchar_t[searchKeyString.length() + 1]; 
	for(int i = 0; i < searchKeyString.length(); i++)
		searchKey[i] = searchKeyString[i];
	searchKey[searchKeyString.length()] = L'\0';

	// Cleaning
	Projekti.clear();

	int counter = 0;

	WIN32_FIND_DATA fi;
	HANDLE h = FindFirstFile(searchKey, &fi);

	if (h == INVALID_HANDLE_VALUE) return -1;  // Unable to find directory
    do 
	{
        if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wstring dirName(fi.cFileName);
			if(dirName != L"." && dirName != L"..")
			{
				Projekt *p = new Projekt(path + L"\\" + dirName);
				Projekti.push_back(*p);

				// Updating sets
				set<wstring> s1 = (*p).GetSetNedostatci();
				for(set<wstring>::iterator it = s1.begin(); it != s1.end(); it++)
					setNedostatci.insert(*it);

				set<wstring> s2 = (*p).GetSetProfili();
				for(set<wstring>::iterator it = s2.begin(); it != s2.end(); it++)
					setProfili.insert(*it);

				delete p;
				counter++;
			}
		}
    }while (FindNextFile(h, &fi));


    FindClose(h);
	return counter;
}

bool SkupProjekata::ExportAllProjectsInExcelCSVFormat(wstring path)
{
	for(int i = 0; i < Projekti.size(); i++)
		Projekti[i].ExportInExcelCSVFormat(path);
	return true;
}
bool SkupProjekata::ExportAllProjectsInTxtFormat(wstring path)
{
	for(int i = 0; i < Projekti.size(); i++)
		Projekti[i].ExportInTxtFormat(path);
	return true;
}
bool SkupProjekata::ExportAllProjectsCombinedInExcelCSVFormat(wstring path)
{
	wofstream out(path + L"\\ZbirnaTablica.csv");

	// Imbue stream
	std::locale loc("");
	out.imbue(loc);

	out << L";;ZBIRNA TABLICA NEDOSTATAKA" << endl;

	out << L"PREGLED NEDOSTATAKA PO PROFILIMA" << endl;
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

bool SkupProjekata::ExportAllProjectsInExcelCSVFormat()
{
	for(int i = 0; i < Projekti.size(); i++)
		Projekti[i].ExportInExcelCSVFormat(directoryPath);
	return true;
}
bool SkupProjekata::ExportAllProjectsInTxtFormat()
{
	for(int i = 0; i < Projekti.size(); i++)
		Projekti[i].ExportInTxtFormat(directoryPath);
	return true;
}
bool SkupProjekata::ExportAllProjectsCombinedInExcelCSVFormat()
{
	ExportAllProjectsCombinedInExcelCSVFormat(directoryPath);
	return true;
}

int SkupProjekata::NumberOfNedostatakaGivenCode(wstring nedostatakCode)
{
	int sum = 0;

	for(int i = 0; i < Projekti.size(); i++)
	{
		sum += Projekti[i].NumberOfNedostatakaGivenCode(nedostatakCode);
	}

	return sum;
}
int SkupProjekata::NumberOfNedostatakaGivenProfil(wstring profil)
{
	int sum = 0;

	for(int i = 0; i < Projekti.size(); i++)
		sum += Projekti[i].NumberOfNedostatakaGivenProfil(profil);

	return sum;
}
int SkupProjekata::NumberOfNedostatakaGivenCodeandProfil(wstring nedostatakCode, wstring profil)
{
	int sum = 0;

	for(int i = 0; i < Projekti.size(); i++)
		sum += Projekti[i].NumberOfNedostatakaGivenCodeandProfil(nedostatakCode, profil);

	return sum;
}

int SkupProjekata::TotalNumberOfNedostatakaSummingProfiles(void)
{
	int sum = 0;
	for(std::set<wstring>::iterator it = setNedostatci.begin(); it != setNedostatci.end(); it++)
	{
		sum += NumberOfNedostatakaGivenCode(*it);
	}
	return sum;
}
int SkupProjekata::TotalNumberOfNedostatakaSummingCodes(void)
{
	int sum = 0;
	for(std::set<wstring>::iterator it = setProfili.begin(); it != setProfili.end(); it++)
	{
		sum += NumberOfNedostatakaGivenProfil(*it);
	}
	return sum;
}