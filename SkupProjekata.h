#include<vector>
#include<string>
#include"Projekt.h"

using namespace std;

class SkupProjekata
{
private:
	vector<Projekt> Projekti;
	wstring directoryPath;

	set<wstring> setNedostatci;
	set<wstring> setProfili;

public:
	SkupProjekata();
	SkupProjekata(wstring path);

	int UcitajProjekteIzDirektorija(wstring path);

	bool ExportAllProjectsInExcelCSVFormat(wstring path);
	bool ExportAllProjectsInTxtFormat(wstring path);
	bool ExportAllProjectsCombinedInExcelCSVFormat(wstring path);

	int NumberOfNedostatakaGivenProfil(wstring profil);
	int NumberOfNedostatakaGivenCode(wstring nedostatakCode);
	int NumberOfNedostatakaGivenCodeandProfil(wstring nedostatakCode, wstring profil);

	int SkupProjekata::TotalNumberOfNedostatakaSummingProfiles(void);
	int SkupProjekata::TotalNumberOfNedostatakaSummingCodes(void);

	bool ExportAllProjectsInExcelCSVFormat(void);
	bool ExportAllProjectsInTxtFormat(void);
	bool ExportAllProjectsCombinedInExcelCSVFormat(void);
};
