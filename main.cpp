#include<iostream>
#include<fstream>
#include<string>
#include"SkupProjekata.h"

using namespace std;

int main(void)
{
	SkupProjekata s(L"C:\\Users\\Bojan\\Desktop\\Vodovod\\ObradaIzvjestaja");

	s.ExportAllProjectsInExcelCSVFormat();
	s.ExportAllProjectsCombinedInExcelCSVFormat();


	return 0;
}