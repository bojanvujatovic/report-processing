#include<vector>
#include<set>
#include<string>
#include"Dionica.h"

using namespace std;

class Projekt
{
private:
	wstring naziv;
	wstring lokacija;
	wstring datum;

	set<wstring> setNedostatci;
	set<wstring> setProfili;
	vector<Dionica> Dionice;

public:
	Projekt();
	Projekt(wstring path);

	int UcitajCSVDatotekeUzDirektorija(wstring path);
	bool ExportInExcelCSVFormat(wstring path);
	bool ExportInTxtFormat(wstring path);

	set<wstring> GetSetNedostatci(void) {return setNedostatci;}
	set<wstring> GetSetProfili(void) {return setProfili;}

	int NumberOfNedostatakaGivenProfil(wstring profil);
	int NumberOfNedostatakaGivenCode(wstring nedostatakCode);
	int NumberOfNedostatakaGivenCodeandProfil(wstring nedostatakCode, wstring profil);

	int TotalNumberOfNedostatakaSummingProfiles(void);
	int TotalNumberOfNedostatakaSummingCodes(void);

	friend wostream& operator<<(wostream& s, const Projekt& d);
};