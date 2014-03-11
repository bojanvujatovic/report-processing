#include<iostream>
#include<string>
#include<map>

using namespace std;

class Dionica
{
private:
	wstring naziv;
	wstring mjesto;
	wstring ulica;
	wstring datum;
	wstring profilCijevi;
	bool isLoaded;

	map<wstring, int> Nedostatci;

public:
	Dionica();
	Dionica(wstring path);

	wstring GetNaziv(void)  {return naziv;}
	wstring GetDatum(void)  {return datum;}
	wstring GetMjesto(void)  {return mjesto;}
	wstring GetUlica(void)  {return ulica;}
	wstring GetProfil(void)  {return profilCijevi;}
	map<wstring, int> GetNedostatci(void)  {return Nedostatci;}

	bool UcitajIzCSVDatoteke(wstring path);
	bool IsLoaded(void) {return isLoaded;}
	
	int NumberOfNedostatakaGivenCode(wstring nedostatakCode);
	int ZbrojNedostataka(void);

	friend wostream& operator<<(wostream& s, const Dionica& d);
};