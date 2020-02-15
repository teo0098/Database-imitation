#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <chrono>
#pragma package(smart_init)
#pragma resource "*.dfm"
TDB *DB;
struct list {
	int ID;
	AnsiString Performer;
	AnsiString Title;
	int Released;
	int Amount;
	AnsiString Reward;
	double Price;
	double Rating;
	AnsiString Description;
	list * prev;
	list * next;
};
list * s = NULL;
list * e = NULL;
bool import = false;
int id = 1;
struct invertedListString {
	AnsiString value;
	std::vector <int> indexes;
};
struct invertedListInt {
	int value;
	std::vector <int> indexes;
};
struct invertedListDouble {
	double value;
	std::vector <int> indexes;
};
struct chainListString {
	AnsiString value;
	int index;
};
struct chainListInt {
	int value;
	int index;
};
struct chainListDouble {
	double value;
	int index;
};
__fastcall TDB::TDB(TComponent* Owner) : TForm(Owner) {
	AnsiString headers[] = {
		"ID", "Performer", "Title", "Released", "Amount", "Reward", "Price",
		"Rating", "Description"};
	int widths[] = {60, 250, 300, 70, 80, 60, 70, 50, 500};
	for (int i = 0; i < Table->ColCount; i++) {
		Table->Cells[i][0] = headers[i];
		Table->ColWidths[i] = widths[i];
	}
}
// if there are only numbers
bool hasNumbers(TEdit * Edit) {
	for (int i = 1; i <= Edit->Text.Length(); i++) {
		if (!isdigit(Edit->Text[i])) {
			return false;
		}
	}
	return true;
}
// if ID is repeated
bool isIDRepeated(AnsiString ID, TStringGrid * Table) {
	for (int i = 1; i < Table->RowCount; i++) {
		if (ID == Table->Cells[0][i]) {
			return true;
		}
	}
	return false;
}
// if is empty
bool isEmpty(TEdit * Edit) {
	return Edit->Text.IsEmpty();
}
// if there are only letters
bool hasLetters(TEdit * Edit) {
	for (int i = 1; i <= Edit->Text.Length(); i++) {
		if (!isalpha(Edit->Text[i]) && Edit->Text[i] != ' ') {
			return false;
		}
	}
	return true;
}
// if year is valid
bool validYear(TEdit * ReleaseYear) {
	return (StrToInt(ReleaseYear->Text) > 1949 && StrToInt(ReleaseYear->Text)
		< 2020);
}
// if is valid double number
bool validDouble(TEdit * Edit) {
	for (int i = 1; i <= Edit->Text.Length(); i++) {
		if (!isdigit(Edit->Text[i]) && Edit->Text[i] != ',') {
			return false;
		}
	}
	return true;
}
// if comma in double number is valid
bool isDelimeterValid(TEdit * Edit) {
	int licznik = 0;
	if (Edit->Text[1] == ',')
		return false;
	for (int i = 2; i <= Edit->Text.Length(); i++) {
		if (Edit->Text[i] == ',') {
			licznik = licznik + 1;
			if (licznik > 1) {
				return false;
			}
		}
	}
	return true;
}
//checking if either Title or Description is valid
bool checkTitDes(TEdit * Title, int length, AnsiString text) {
	if (!isEmpty(Title) && Title->Text != "Fill in" && Title->Text != text && Title->Text != "Max " + IntToStr(length) + " letters") {
		if ((isalpha(Title->Text[1]) || isdigit(Title->Text[1]))
			&& Title->Text.Length() <= length) {
			Title->Color = clWindow;
			return true;
		}
		else {
			Title->Text = "Max " + IntToStr(length) + " letters";
			Title->Color = clRed;
			return false;
		}
	}
	else {
		Title->Text = "Fill in";
		Title->Color = clRed;
		return false;
	}
}
//checking if year is valid
bool checkYear(TEdit * ReleaseYear) {
    if (!isEmpty(ReleaseYear)) {
		if (hasNumbers(ReleaseYear) && validYear(ReleaseYear)) {
			ReleaseYear->Color = clWindow;
			return true;
		}
		else {
			ReleaseYear->Text = "Enter year <1950, 2019>";
			ReleaseYear->Color = clRed;
			return false;
		}
	}
	else {
		ReleaseYear->Text = "Fill in";
		ReleaseYear->Color = clRed;
		return false;
	}
}
//checking if amount is valid
bool checkAmount(TEdit * Amount, int limit) {
	if (!isEmpty(Amount)) {
		if (hasNumbers(Amount) && StrToInt(Amount->Text) > 0 && StrToInt(Amount->Text) <= limit) {
			Amount->Color = clWindow;
			return true;
		}
		else {
			Amount->Text = "Only numbers(0," + IntToStr(limit) + ">";
			Amount->Color = clRed;
			return false;
		}
	}
	else {
		Amount->Text = "Fill in";
		Amount->Color = clRed;
		return false;
	}
}
//checking if price is valid
bool checkPrice(TEdit * Price) {
	if (!isEmpty(Price)) {
		if (isDelimeterValid(Price) && validDouble(Price)
			&& Price->Text.ToDouble() > 0 && Price->Text.ToDouble() < 1001) {
			Price->Color = clWindow;
			return true;
		}
		else {
			Price->Text = "Enter correct price (0, 1000>";
			Price->Color = clRed;
			return false;
		}
	}
	else {
		Price->Text = "Fill in";
		Price->Color = clRed;
		return false;
	}
}
//checking if rating is valid
bool checkRating(TEdit * Rating) {
	if (!isEmpty(Rating)) {
		if (isDelimeterValid(Rating) && validDouble(Rating)
			&& Rating->Text.ToDouble() <= 10) {
			Rating->Color = clWindow;
			return true;
		}
		else {
			Rating->Text = "Enter rating <0,10>";
			Rating->Color = clRed;
			return false;
		}
	}
	else {
		Rating->Text = "Fill in";
		Rating->Color = clRed;
		return false;
	}
}
//adding data to StringGrid
void addToGrid (TStringGrid * Table, list * fresh) {
	Table->RowCount = Table->RowCount + 1;
	Table->Cells[0][Table->RowCount - 1] = fresh->ID;
	Table->Cells[1][Table->RowCount - 1] = fresh->Performer;
	Table->Cells[2][Table->RowCount - 1] = fresh->Title;
	Table->Cells[3][Table->RowCount - 1] = fresh->Released;
	Table->Cells[4][Table->RowCount - 1] = fresh->Amount;
	Table->Cells[5][Table->RowCount - 1] = fresh->Reward;
	Table->Cells[6][Table->RowCount - 1] = fresh->Price;
	Table->Cells[7][Table->RowCount - 1] = fresh->Rating;
	Table->Cells[8][Table->RowCount - 1] = fresh->Description;
}
// check if ID is already in the file
bool checkDBFile(int row, TStringGrid * Table) {
	std::string line;
	fstream dbfile;
	dbfile.open("dbfile.txt", ios::in);
	if (!dbfile.good()) return true;
	int lineNumber = 1;
	while (getline(dbfile, line)) {
		if (lineNumber == 1) {
			if (line.c_str() == Table->Cells[0][row]) {
				return false;
			}
		}
		if (lineNumber >= 9) {
			lineNumber = 0;
		}
		lineNumber = lineNumber + 1;
	}
	dbfile.close();
	return true;
}
//incrementing id per add
void incrementid() {
    std::string line;
	fstream dbfile;
	dbfile.open("dbfile.txt", ios::in);
	if (dbfile.good()) {
		int lineNumber = 1;
		while (getline(dbfile, line)) {
			if (lineNumber == 1) {
				if (line.c_str() == IntToStr(id)) {
					id = id + 1;
				}
			}
			if (lineNumber >= 9) {
				lineNumber = 0;
			}
			lineNumber = lineNumber + 1;
		}
	}
	dbfile.close();
}
// add data
// ---------------------------------------------------------------------------
void __fastcall TDB::AddClick(TObject *Sender) {
	bool validTitle = false;
	bool validReleaseYear = false;
	bool validAmount = false;
	bool validPrice = false;
	bool validRating = false;
	bool validDescription = false;
	if (checkTitDes(Title, 40, "Title")) {
		validTitle = true;
	}
	if (checkYear(ReleaseYear)) {
		validReleaseYear = true;
	}
	if (checkAmount(Amount, 999999999)) {
		validAmount = true;
	}
	if (checkPrice(Price)) {
		validPrice = true;
	}
	if (checkRating(Rating)) {
		validRating = true;
	}
	if (checkTitDes(Description, 80, "Description")) {
		validDescription = true;
	}
	// if all data are valid
	if (validTitle && validReleaseYear && validAmount &&
		validPrice && validRating && validDescription) {

		AnsiString reward = "";
		if (Rewarded->Checked) reward = "YES";
		else reward = "NO";
		list * fresh = new list;
		fresh->next = NULL;
		fresh->prev = NULL;
		incrementid();
        fresh->ID = id++;
		fresh->Performer = Performer->Text;
		fresh->Title = Title->Text;
		fresh->Released = ReleaseYear->Text.ToInt();
		fresh->Amount = Amount->Text.ToInt();
		fresh->Reward = reward;
		fresh->Price = Price->Text.ToDouble();
		fresh->Rating = Rating->Text.ToDouble();
		fresh->Description = Description->Text;

		addToGrid(Table, fresh);
		if (s == NULL) {
			s = fresh;
			e = fresh;
		}
		else {
			e->next = fresh;
			fresh->prev = e;
			e = fresh;
		}
	}
	Error->Visible = false;
}
// find row that has to be deleted
int findRow(TEdit * IDD, TStringGrid * Table) {
	int row = 0;
	for (int i = 0; i < Table->RowCount; i++) {
		if (IDD->Text == Table->Cells[0][i]) {
			row = i;
			break;
		}
	}
	return row;
}
//checking if deleting id is valid
bool checkIDD(TEdit * IDD, TStringGrid * Table) {
	if (!isEmpty(IDD)) {
		if (hasNumbers(IDD) && isIDRepeated(IDD->Text, Table) && StrToInt
			(IDD->Text) > 0) {
			IDD->Color = clWindow;
			return true;
		}
		else if (!isIDRepeated(IDD->Text, Table)) {
			IDD->Text = "ID doesn't exist";
			IDD->Color = clRed;
			return false;
		}
		else {
			IDD->Text = "Only positive numbers";
			IDD->Color = clRed;
			return false;
		}
	}
	else {
		IDD->Text = "Fill in";
		IDD->Color = clRed;
		return false;
	}
}
//deleting element from list
void deleteFromList(AnsiString IDD) {
	list * t = s;
	list * removed = NULL;
	list * p = NULL;
	list * n = NULL;
	int k = 0;
	if (t->next == NULL) {
		removed = s;
		s = NULL;
		e = NULL;
		delete removed;
	} else {
		while (t != NULL) {
			if (IDD == s->ID) {
				removed = s;
				s = s->next;
				s->prev = NULL;
				delete removed;
				break;
			}
			else if ((IDD == e->ID) && (t->next == NULL)) {
				removed = e;
				e = e->prev;
				e->next = NULL;
				delete removed;
				break;
			}
			else if (IDD == t->ID) {
				removed = t;
				p = t->prev;
				n = t->next;
				p->next = n;
				n->prev = p;
				delete removed;
				break;
			}
			t = t->next;
		}
	}
}
void deleteRow(TEdit * IDD, TStringGrid * Table) {
	if (checkIDD(IDD, Table)) {
		int row = findRow(IDD, Table);
		for (int i = row; i < Table->RowCount; i++) {
			for (int j = 0; j < Table->ColCount; j++) {
				Table->Cells[j][i] = Table->Cells[j][i + 1];
			}
		}
		Table->RowCount = Table->RowCount - 1;
		deleteFromList(IDD->Text);
	}
}
// delete one chosen row
void __fastcall TDB::DeleteClick(TObject *Sender) {
	deleteRow(IDD, Table);
	Error->Visible = false;
	import = false;
}
// delete all rows
void __fastcall TDB::DeleteAllClick(TObject *Sender) {
	Error->Visible = false;
	import = false;
	for (int i = 1; i < Table->RowCount; i++) {
		deleteFromList(Table->Cells[0][i]);
	}
	Table->RowCount = 1;
    id = 1;
}
// import data from the file
void __fastcall TDB::ImportClick(TObject *Sender) {
	std::string line;
	fstream dbfile;
	dbfile.open("dbfile.txt", ios::in);
	if (!dbfile.good()) {
		Error->Text = "File doesn't exist";
		Error->Visible = true;
	}
	else {
		Error->Visible = false;
		Error->Text = "These IDs are already in the database:";
		for (int i = 1; i < Table->RowCount; i++) {
			if (!checkDBFile(i, Table)) {
				Error->Visible = true;
				Error->Text = Error->Text + " " + Table->Cells[0][i];
				import = true;
			}
		}
		if (!import) {
			Error->Visible = false;
			int lineNumber = 1;
			AnsiString data = "";
			list * fresh = new list;
			fresh->next = NULL;
			fresh->prev = NULL;
			while (getline(dbfile, line)) {
				switch (lineNumber) {
					case 1: {
						data = line.c_str();
						fresh->ID = data.ToInt();
						id = data.ToInt() + 1;
					};
					break;
					case 2: fresh->Performer = line.c_str();
					break;
					case 3: fresh->Title = line.c_str();
					break;
					case 4: {
						data = line.c_str();
						fresh->Released = data.ToInt();
					};
					break;
					case 5: {
						data = line.c_str();
						fresh->Amount = data.ToInt();
					};
					break;
					case 6: fresh->Reward = line.c_str();
					break;
					case 7: {
						data = line.c_str();
						fresh->Price = StrToFloat(data, TFormatSettings::Invariant());
					};
					break;
					case 8: {
						data = line.c_str();
						fresh->Rating = StrToFloat(data, TFormatSettings::Invariant());
					};
					break;
					case 9: fresh->Description = line.c_str();
					break;
				}
				if (lineNumber >= 9) {
					lineNumber = 0;
                    if (s == NULL) {
						s = fresh;
						e = fresh;
					}
					else {
						e->next = fresh;
						fresh->prev = e;
						e = fresh;
					}
                    fresh = new list;
					fresh->next = NULL;
					fresh->prev = NULL;
				}
				lineNumber = lineNumber + 1;
			}
            list * k = s;
			while (k != NULL) {
				if (!isIDRepeated(k->ID, Table)) {
					addToGrid(Table, k);
				}
				k = k->next;
			}
            import = true;
		}
	}
	dbfile.close();
}
// write data to the file
void __fastcall TDB::ExportClick(TObject *Sender) {
	if (Table->RowCount == 1) {
		Error->Text = "There is nothing to add to the file";
		Error->Visible = true;
	}
	else {
		Error->Visible = false;
		fstream dbfile;
		dbfile.open("dbfile.txt", ios::out | ios::trunc);
		list * k = s;
		while (k != NULL) {
			dbfile << k->ID << std::endl;
			dbfile << k->Performer << std::endl;
			dbfile << k->Title << std::endl;
			dbfile << k->Released << std::endl;
			dbfile << k->Amount << std::endl;
			dbfile << k->Reward << std::endl;
			dbfile << k->Price << std::endl;
			dbfile << k->Rating << std::endl;
			dbfile << k->Description << std::endl;
			k = k->next;
		}
		Error->Visible = true;
		Error->Text = "Export successfully conducted";
		dbfile.close();
	}
}
//drawing performer
AnsiString drawPerformer() {
	AnsiString performers[] = {
		"John Simmons", "Ronald Ponder", "Glenn McIntyre", "Brandon Almond",
		"Bernardo Rivers", "Michele Green", "Edward Greer", "Nicole Coomer",
		"Chris Mendenhall", "Paul Jasinski", "Jack Campbell", "David Brown",
		"Timothy Duncan", "Michelle Mann", "Cruz Davis", "James Kenyon",
		"Maura Book", "Paul Judd", "Gladys Hatley", "Joyce Harrison",
		"Robert Brown", "Nicole Hawkins", "Olga Lester", "Gena Shepherd",
		"Scott Meyer", "Jennifer Bridges", "Carlos Roberts", "Nena Underwood",
		"Maryann Ray", "Daniel Boyd"};
	AnsiString ranPer = performers[rand() % 30];
	return ranPer;
}
//drawing title or description
AnsiString drawTitDes() {
	char letters[] = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' '};
	AnsiString d = "";
	bool space = false;
	for (int i = 0; i < rand() % 35 + 5; i++) {
		if (i == 0) {
        	d = d + letters[rand() % 26];
		} else {
			d = d + letters[rand() % 27];
        }
	}
	return d;
}
//drawing price
AnsiString drawPrice() {
	double d = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / 900));
	int i;
	AnsiString price;
	if (rand() % 2 == 0) {
		i = d * 10;
		price = static_cast<double>(i) / 10;
	}
	else {
		i = d * 100;
		price = static_cast<double>(i) / 100;
	}
	return price;
}
//drawing rating
double drawRating(AnsiString price) {
	AnsiString numeral = price[1];
	double d = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)) + numeral.ToInt();
	int i = d * 10;
    return (static_cast<double>(i) / 10);
}
//general drawing records
void __fastcall TDB::DrawClick(TObject *Sender) {
	srand(time(NULL));
	bool validAmount = false;
	if (checkAmount(Drawing, 500000)) {
        validAmount = true;
	}
	if (validAmount) {
		AnsiString reward = "";
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < StrToInt(Drawing->Text); i++) {
            if (rand() % 2 == 0) reward = "NO";
			else reward = "YES";
			list * fresh = new list;
			fresh->prev = NULL;
			fresh->next = NULL;
			fresh->ID = id++;
            fresh->Performer = drawPerformer();
			fresh->Title = drawTitDes();
			fresh->Released = rand() % 70 + 1950;
			fresh->Amount = rand() % 999999999 + 1;
			fresh->Reward = reward;
            fresh->Price = drawPrice().ToDouble();
			fresh->Rating = drawRating(fresh->Price);
			fresh->Description = drawTitDes();

			addToGrid(Table, fresh);
			if (s == NULL) {
				s = fresh;
				e = fresh;
			}
			else {
				e->next = fresh;
				fresh->prev = e;
				e = fresh;
			}
		}
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		AnsiString time = elapsed.count();
		ShowMessage("Time taken: " + time + "s");
	}
}
void addToNewList(list * k, list ** s2, list ** e2) {
	list * fresh = new list;
    fresh->next = NULL;
	fresh->prev = NULL;
	fresh->ID = k->ID;
	fresh->Performer = k->Performer;
	fresh->Title = k->Title;
	fresh->Released = k->Released;
	fresh->Amount = k->Amount;
	fresh->Reward = k->Reward;
	fresh->Price = k->Price;
	fresh->Rating = k->Rating;
	fresh->Description = k->Description;
	if ((*s2) == NULL) {
		(*s2) = fresh;
		(*e2) = fresh;
	} else {
		(*e2)->next = fresh;
		fresh->prev = (*e2);
		(*e2) = fresh;
	}
}
//searching for values
void __fastcall TDB::SearchClick(TObject *Sender)
{
	list * s2 = NULL;
	list * e2 = NULL;
	list * k = s;
	bool searched = false;
	switch (Searchings->ItemIndex) {
		case 0: {
			auto start = std::chrono::high_resolution_clock::now();
			while (k != NULL) {
				if (k->Performer == Performer->Text) {
					searched = true;
					addToNewList(k, &s2, &e2);
				}
				k = k->next;
			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;
			AnsiString time = elapsed.count();
			ShowMessage("Time taken: " + time + "s");
		};
		break;
		case 1: {
			while (k != NULL) {
				if (k->Title == Title->Text) {
					searched = true;
					addToNewList(k, &s2, &e2);
				}
				k = k->next;
			}
		};
		break;
		case 2: {
			if (checkYear(ReleaseYear)) {
				auto start = std::chrono::high_resolution_clock::now();
				while (k != NULL) {
					if (k->Released == ReleaseYear->Text) {
						searched = true;
						addToNewList(k, &s2, &e2);
					}
					k = k->next;
				}
                auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		};
		break;
		case 3: {
			if (checkAmount(Amount, 999999999)) {
				while (k != NULL) {
					if (k->Amount == Amount->Text) {
						searched = true;
						addToNewList(k, &s2, &e2);
					}
					k = k->next;
				}
			}
		};
		break;
		case 4: {
			AnsiString reward = "";
			if (Rewarded->Checked) {
				reward = "YES";
			} else {
				reward = "NO";
			}
			auto start = std::chrono::high_resolution_clock::now();
			while (k != NULL) {
				if (k->Reward == reward) {
					searched = true;
					addToNewList(k, &s2, &e2);
				}
				k = k->next;
			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;
			AnsiString time = elapsed.count();
			ShowMessage("Time taken: " + time + "s");
		};
		break;
		case 5: {
			if (checkPrice(Price)) {
				auto start = std::chrono::high_resolution_clock::now();
				while (k != NULL) {
					if (k->Price == Price->Text) {
						searched = true;
						addToNewList(k, &s2, &e2);
					}
					k = k->next;
				}
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		};
		break;
		case 6: {
			if (checkRating(Rating)) {
				while (k != NULL) {
					if (k->Rating == Rating->Text) {
						searched = true;
						addToNewList(k, &s2, &e2);
					}
					k = k->next;
				}
			}
		};
		break;
		case 7: {
			while (k != NULL) {
				if (k->Description == Description->Text) {
					searched = true;
					addToNewList(k, &s2, &e2);
				}
				k = k->next;
			}
		};
		break;
	}

	if (searched) {
		k = s2;
		Table->RowCount = 1;
		while (k != NULL) {
			addToGrid(Table, k);
			k = k->next;
		}
	}
}
//bringing back whole data
void __fastcall TDB::BackClick(TObject *Sender)
{
	Table->RowCount = 1;
	list * k = s;
	while (k != NULL) {
		addToGrid(Table, k);
		k = k->next;
    }
}
//deleting per given id
void __fastcall TDB::DeletePerClick(TObject *Sender)
{
    Error->Visible = false;
	import = false;
	if (hasNumbers(IDD)) {
		for (int i = 1; i < Table->RowCount; i++) {
			if (i % IDD->Text.ToInt() == 0) {
				deleteFromList(i);
			}
		}
		list * k = s;
		Table->RowCount = 1;
		while (k != NULL) {
			Table->RowCount = Table->RowCount + 1;
			Table->Cells[0][Table->RowCount - 1] = k->ID;
			Table->Cells[1][Table->RowCount - 1] = k->Performer;
			Table->Cells[2][Table->RowCount - 1] = k->Title;
			Table->Cells[3][Table->RowCount - 1] = k->Released;
			Table->Cells[4][Table->RowCount - 1] = k->Amount;
			Table->Cells[5][Table->RowCount - 1] = k->Reward;
			Table->Cells[6][Table->RowCount - 1] = k->Price;
			Table->Cells[7][Table->RowCount - 1] = k->Rating;
			Table->Cells[8][Table->RowCount - 1] = k->Description;
			k = k->next;
		}
	}
}
//additional function helping assigning data from one list to another
void assign(list * a, list * b) {
	a->ID = b->ID;
	a->Performer = b->Performer;
	a->Title = b->Title;
	a->Released = b->Released;
	a->Amount = b->Amount;
	a->Reward = b->Reward;
	a->Price = b->Price;
	a->Rating = b->Rating;
	a->Description = b->Description;
}

void sortPerformer(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (CompareStr(p->Performer, min->Performer) < 0) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortTitle(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (CompareStr(p->Title, min->Title) < 0) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortYear(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (p->Released < min->Released) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortAmount(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (p->Amount < min->Amount) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortReward(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (CompareStr(p->Reward, min->Reward) < 0) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortPrice(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (p->Price < min->Price) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortRating(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (p->Rating < min->Rating) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void sortDescription(list ** s2, TStringGrid * Table) {
	list * min;
	list * k = (*s2);
	while (k != NULL) {
		min = k;
		list * p = k->next;
		while (p != NULL) {
			if (CompareStr(p->Description, min->Description) < 0) {
				min = p;
			}
			p = p->next;
		}
		if (k != min) {
			list * t = new list;
			assign(t, k);
			assign(k, min);
			assign(min, t);
			delete t;
		}
		k = k->next;
	}
}

void assignBS(list ** s3, list ** e3, list tab[], int half) {
	list * t = new list;
	t->next = NULL;
	t->prev = NULL;
	t->ID = tab[half].ID;
	t->Performer = tab[half].Performer;
	t->Title = tab[half].Title;
	t->Released = tab[half].Released;
	t->Amount = tab[half].Amount;
	t->Reward = tab[half].Reward;
	t->Price = tab[half].Price;
	t->Rating = tab[half].Rating;
	t->Description = tab[half].Description;
	addToNewList(t, s3, e3);
	delete t;
}

void BSPerformer(list ** s3, list ** e3, int begin, int listSize, list tab[], AnsiString p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (CompareStr(tab[half].Performer, p) == 0) {
			assignBS(s3, e3, tab, half);
			if (CompareStr(tab[half + 1].Performer, p) == 0) {
				for (int i = half + 1; i < listSize; i++) {
					if (CompareStr(tab[i].Performer, p) == 0) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
			}
			if (CompareStr(tab[half - 1].Performer, p) == 0) {
				for (int i = half - 1; i > -1; i--) {
					if (CompareStr(tab[i].Performer, p) == 0) {
						assignBS(s3, e3, tab, i);
					} else {
                        break;
                    }
				}
			}
			break;
		} else if (CompareStr(p, tab[half].Performer) < 0) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSTitle(list ** s3, list ** e3, int begin, int listSize, list tab[], AnsiString p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (CompareStr(tab[half].Title, p) == 0) {
			assignBS(s3, e3, tab, half);
			if (CompareStr(tab[half + 1].Title, p) == 0) {
				for (int i = half + 1; i < listSize; i++) {
					if (CompareStr(tab[i].Title, p) == 0) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			if (CompareStr(tab[half - 1].Title, p) == 0) {
				for (int i = half - 1; i > -1; i--) {
					if (CompareStr(tab[i].Title, p) == 0) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			break;
		} else if (CompareStr(p, tab[half].Title) < 0) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSYear(list ** s3, list ** e3, int begin, int listSize, list tab[], int p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (tab[half].Released == p) {
			assignBS(s3, e3, tab, half);
			if (tab[half + 1].Released == p) {
				for (int i = half + 1; i < listSize; i++) {
					if (tab[i].Released == p) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
				break;
			}
			if (tab[half - 1].Released == p) {
				for (int i = half - 1; i > -1; i--) {
					if (tab[i].Released == p) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
				break;
			}
			break;
		} else if (p < tab[half].Released) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSAmount(list ** s3, list ** e3, int begin, int listSize, list tab[], int p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (tab[half].Amount == p) {
			assignBS(s3, e3, tab, half);
			if (tab[half + 1].Amount == p) {
				for (int i = half + 1; i < listSize; i++) {
					if (tab[i].Amount == p) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			if (tab[half - 1].Amount == p) {
				for (int i = half - 1; i > -1; i--) {
					if (tab[i].Amount == p) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			break;
		} else if (p < tab[half].Amount) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSReward(list ** s3, list ** e3, int begin, int listSize, list tab[], AnsiString p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (CompareStr(tab[half].Reward, p) == 0) {
			assignBS(s3, e3, tab, half);
			if (CompareStr(tab[half + 1].Reward, p) == 0) {
				for (int i = half + 1; i < listSize; i++) {
					if (CompareStr(tab[i].Reward, p) == 0) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
				break;
			}
			if (CompareStr(tab[half - 1].Reward, p) == 0) {
				for (int i = half - 1; i > -1; i--) {
					if (CompareStr(tab[i].Reward, p) == 0) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
				break;
			}
			break;
		} else if (CompareStr(p, tab[half].Reward) < 0) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSPrice(list ** s3, list ** e3, int begin, int listSize, list tab[], double p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (tab[half].Price == p) {
			assignBS(s3, e3, tab, half);
			if (tab[half + 1].Price == p) {
				for (int i = half + 1; i < listSize; i++) {
					if (tab[i].Price == p) {
						assignBS(s3, e3, tab, i);
					} else {
						break;
                    }
				}
				break;
			}
			if (tab[half - 1].Price == p) {
				for (int i = half - 1; i > -1; i--) {
					if (tab[i].Price == p) {
						assignBS(s3, e3, tab, i);
					} else {
                        break;
                    }
				}
				break;
			}
			break;
		} else if (p < tab[half].Price) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSRating(list ** s3, list ** e3, int begin, int listSize, list tab[], double p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (tab[half].Rating == p) {
			assignBS(s3, e3, tab, half);
			if (tab[half + 1].Rating == p) {
				for (int i = half + 1; i < listSize; i++) {
					if (tab[i].Rating == p) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			if (tab[half - 1].Rating == p) {
				for (int i = half - 1; i > -1; i--) {
					if (tab[i].Rating == p) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			break;
		} else if (p < tab[half].Rating) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void BSDescription(list ** s3, list ** e3, int begin, int listSize, list tab[], AnsiString p) {
	int left = 0;
	int right = listSize - 1;
	while (left <= right) {
		int half = left + ((right - left) / 2);
		if (CompareStr(tab[half].Description, p) == 0) {
			assignBS(s3, e3, tab, half);
			if (CompareStr(tab[half + 1].Description, p) == 0) {
				for (int i = half + 1; i < listSize; i++) {
					if (CompareStr(tab[i].Description, p) == 0) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			if (CompareStr(tab[half - 1].Description, p) == 0) {
				for (int i = half - 1; i > -1; i--) {
					if (CompareStr(tab[i].Description, p) == 0) {
						assignBS(s3, e3, tab, i);
					}
				}
			}
			break;
		} else if (CompareStr(p, tab[half].Description) < 0) {
			right = half - 1;
		} else {
			left = half + 1;
		}
	}
}

void __fastcall TDB::BSearchClick(TObject *Sender)
{
	if (s == NULL) {
		ShowMessage("Cannot find data");
	} else {
		list * s2 = s;
		list * e2 = e;
		list * k = s;
		list * s3 = NULL;
		list * e3 = NULL;
		bool searched = false;
        list * p = s2;
		int listSize = 0, begin = 1;
		switch (Searchings->ItemIndex) {
			case 0: {
				sortPerformer(&s2, Table);
				searched = true;
				while(p != NULL) {
					listSize = listSize + 1;
					p = p->next;
				}
				p = s2;
				list * tab = new list[listSize + 1];
				for (int i = 0; i < listSize + 1; i++) {
					tab[i] = *p;
					if (i > 0) {
						p = p->next;
					}
				}
				auto start = std::chrono::high_resolution_clock::now();
				BSPerformer(&s3, &e3, begin, listSize + 1, tab, Performer->Text);
                auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			};
			break;
			case 1: {
				sortTitle(&s2, Table);
				searched = true;
				while(p != NULL) {
					listSize = listSize + 1;
					p = p->next;
				}
				p = s2;
				list * tab = new list[listSize + 1];
				for (int i = 0; i < listSize + 1; i++) {
					tab[i] = *p;
					if (i > 0) {
						p = p->next;
					}
				}
				BSTitle(&s3, &e3, begin, listSize + 1, tab, Title->Text);
			};
			break;
			case 2: {
				if (checkYear(ReleaseYear)) {
					sortYear(&s2, Table);
					searched = true;
					while(p != NULL) {
						listSize = listSize + 1;
						p = p->next;
					}
					p = s2;
					list * tab = new list[listSize + 1];
					for (int i = 0; i < listSize + 1; i++) {
						tab[i] = *p;
						if (i > 0) {
							p = p->next;
						}
					}
					auto start = std::chrono::high_resolution_clock::now();
					BSYear(&s3, &e3, begin, listSize + 1, tab, ReleaseYear->Text.ToInt());
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			};
			break;
			case 3: {
				if (checkAmount(Amount, 999999999)) {
					sortAmount(&s2, Table);
					searched = true;
					while(p != NULL) {
						listSize = listSize + 1;
						p = p->next;
					}
					p = s2;
					list * tab = new list[listSize + 1];
					for (int i = 0; i < listSize + 1; i++) {
						tab[i] = *p;
						if (i > 0) {
							p = p->next;
						}
					}
					BSAmount(&s3, &e3, begin, listSize + 1, tab, StrToInt(Amount->Text));
				}
			};
			break;
			case 4: {
				AnsiString reward = "";
				if (Rewarded->Checked) {
					reward = "YES";
				} else {
					reward = "NO";
				}
				sortReward(&s2, Table);
				searched = true;
				while(p != NULL) {
					listSize = listSize + 1;
					p = p->next;
				}
				p = s2;
				list * tab = new list[listSize + 1];
				for (int i = 0; i < listSize + 1; i++) {
					tab[i] = *p;
					if (i > 0) {
						p = p->next;
					}
				}
				auto start = std::chrono::high_resolution_clock::now();
				BSReward(&s3, &e3, begin, listSize + 1, tab, reward);
                auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			};
			break;
			case 5: {
				if (checkPrice(Price)) {
					sortPrice(&s2, Table);
					searched = true;
					while(p != NULL) {
						listSize = listSize + 1;
						p = p->next;
					}
					p = s2;
					list * tab = new list[listSize + 1];
					for (int i = 0; i < listSize + 1; i++) {
						tab[i] = *p;
						if (i > 0) {
							p = p->next;
						}
					}
					auto start = std::chrono::high_resolution_clock::now();
					BSPrice(&s3, &e3, begin, listSize + 1, tab, Price->Text.ToDouble());
                    auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			};
			break;
			case 6: {
				if (checkRating(Rating)) {
					sortRating(&s2, Table);
					searched = true;
					while(p != NULL) {
						listSize = listSize + 1;
						p = p->next;
					}
					p = s2;
					list * tab = new list[listSize + 1];
					for (int i = 0; i < listSize + 1; i++) {
						tab[i] = *p;
						if (i > 0) {
							p = p->next;
						}
					}
					BSRating(&s3, &e3, begin, listSize + 1, tab, Rating->Text.ToDouble());
				}
			};
			break;
			case 7: {
				sortDescription(&s2, Table);
				searched = true;
				while(p != NULL) {
					listSize = listSize + 1;
					p = p->next;
				}
				p = s2;
				list * tab = new list[listSize + 1];
				for (int i = 0; i < listSize + 1; i++) {
					tab[i] = *p;
					if (i > 0) {
						p = p->next;
					}
				}
				BSDescription(&s3, &e3, begin, listSize + 1, tab, Description->Text);
			};
			break;
		}
		if (searched) {
			k = s3;
			Table->RowCount = 1;
			while (k != NULL) {
				if (!isIDRepeated(k->ID, Table)) {
					addToGrid(Table, k);
				}
				k = k->next;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TDB::MostRClick(TObject *Sender)
{
	list * k = s;
	list * s2 = s;
	int size = 0, count = 0, count2 = 0, licznik = 0;
	while (k != NULL) {
		size = size + 1;
		k = k->next;
	}
	list * tab = new list[size];
	int * counts = new int[size];
	AnsiString * names = new AnsiString[size];
	sortPerformer(&s2, Table);
	k = s2;
	int l = 0;
	while (k != NULL) {
		tab[l] = *k;
		l = l + 1;
		k = k->next;
	}

	k = s2;
    Table->RowCount = 1;
	for(int i = 0; i < size; i++){
		addToGrid(Table, k);
		if ((i > 0) && (tab[i].Performer != tab[i - 1].Performer)) {
			counts[count2] = count;
			names[count2] = k->prev->Performer;
			count2 = count2 + 1;
			count = 0;
		}
		count = count + 1;
		k = k->next;
	}
	int max = counts[0];
	AnsiString name = names[0];
	for (int i = 1; i < count2; i++) {
		if (max < counts[i]) {
			max = counts[i];
			name = names[i];
		}
	}
    ShowMessage("Najczesciej: " + name);
	delete [] counts;
	delete [] tab;
	delete [] names;
}

bool compareInvertedString(const invertedListString & a, const invertedListString & b) {
	return a.value < b.value;
}

bool compareInvertedInt(const invertedListInt & a, const invertedListInt & b) {
	return a.value < b.value;
}

bool compareInvertedDouble(const invertedListDouble & a, const invertedListDouble & b) {
	return a.value < b.value;
}
//---------------------------------------------------------------------------

void __fastcall TDB::InvertedClick(TObject *Sender)
{
	list * k = s;
	bool occupied = false;
	int arraySize = 0;
	switch (Searchings->ItemIndex) {
		case 0: {
			std::vector <invertedListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Performer == data[i].value) {
						occupied = true;
						break;
					} else {
						occupied = false;
					}
				}
				if (!occupied) {
					std::vector <int> performers;
					invertedListString il;
					il.value = k->Performer;
					il.indexes = performers;
					data.push_back(il);
				}
				arraySize = arraySize + 1;
				k = k->next;
			}
			std::sort(data.begin(), data.end(), compareInvertedString);
			k = s;
			list * dataArray = new list[arraySize];
			for (int i = 0; i < arraySize; i++) {
				dataArray[i] = *k;
				k = k->next;
			}
			k = s;
			for (int i = 0; i < arraySize; i++) {
				for (int j = 0; j < data.size(); j++) {
					if (dataArray[i].Performer == data[j].value) {
						data[j].indexes.push_back(i);
					}
				}
			}
			int position = -1;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (data[i].value == Performer->Text) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				for (int j = 0; j < data[position].indexes.size(); j++) {
					list * k2 = new list;
					*k2 = dataArray[data[position].indexes[j]];
					addToNewList(k2, &s2, &e2);
					addToGrid(Table, k2);
					delete k2;
				}
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
		break;
		case 1: {
			std::vector <invertedListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Title == data[i].value) {
						occupied = true;
						break;
					} else {
						occupied = false;
					}
				}
				if (!occupied) {
					std::vector <int> performers;
					invertedListString il;
					il.value = k->Title;
					il.indexes = performers;
					data.push_back(il);
				}
				arraySize = arraySize + 1;
				k = k->next;
			}
			std::sort(data.begin(), data.end(), compareInvertedString);
			k = s;
			list * dataArray = new list[arraySize];
			for (int i = 0; i < arraySize; i++) {
				dataArray[i] = *k;
				k = k->next;
			}
			k = s;
			for (int i = 0; i < arraySize; i++) {
				for (int j = 0; j < data.size(); j++) {
					if (dataArray[i].Title == data[j].value) {
						data[j].indexes.push_back(i);
					}
				}
			}
			int position = -1;
			for (int i = 0; i < data.size(); i++) {
				if (data[i].value == Title->Text) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				for (int j = 0; j < data[position].indexes.size(); j++) {
					list * k2 = new list;
					*k2 = dataArray[data[position].indexes[j]];
					addToNewList(k2, &s2, &e2);
					addToGrid(Table, k2);
					delete k2;
				}
			}
		}
		break;
		case 2: {
			if (checkYear(ReleaseYear)) {
				std::vector <invertedListInt> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Released == data[i].value) {
							occupied = true;
							break;
						} else {
							occupied = false;
						}
					}
					if (!occupied) {
						std::vector <int> performers;
						invertedListInt il;
						il.value = k->Released;
						il.indexes = performers;
						data.push_back(il);
					}
					arraySize = arraySize + 1;
					k = k->next;
				}
				std::sort(data.begin(), data.end(), compareInvertedInt);
				k = s;
				list * dataArray = new list[arraySize];
				for (int i = 0; i < arraySize; i++) {
					dataArray[i] = *k;
					k = k->next;
				}
				k = s;
				for (int i = 0; i < arraySize; i++) {
					for (int j = 0; j < data.size(); j++) {
						if (dataArray[i].Released == data[j].value) {
							data[j].indexes.push_back(i);
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (data[i].value == ReleaseYear->Text.ToInt()) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					for (int j = 0; j < data[position].indexes.size(); j++) {
						list * k2 = new list;
						*k2 = dataArray[data[position].indexes[j]];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
					}
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			}
		}
		break;
		case 3: {
			if (checkAmount(Amount, 999999999)) {
				std::vector <invertedListInt> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Amount == data[i].value) {
							occupied = true;
							break;
						} else {
							occupied = false;
						}
					}
					if (!occupied) {
						std::vector <int> performers;
						invertedListInt il;
						il.value = k->Amount;
						il.indexes = performers;
						data.push_back(il);
					}
					arraySize = arraySize + 1;
					k = k->next;
				}
				std::sort(data.begin(), data.end(), compareInvertedInt);
				k = s;
				list * dataArray = new list[arraySize];
				for (int i = 0; i < arraySize; i++) {
					dataArray[i] = *k;
					k = k->next;
				}
				k = s;
				for (int i = 0; i < arraySize; i++) {
					for (int j = 0; j < data.size(); j++) {
						if (dataArray[i].Amount == data[j].value) {
							data[j].indexes.push_back(i);
						}
					}
				}
				int position = -1;
				for (int i = 0; i < data.size(); i++) {
					if (data[i].value == Amount->Text.ToInt()) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					for (int j = 0; j < data[position].indexes.size(); j++) {
						list * k2 = new list;
						*k2 = dataArray[data[position].indexes[j]];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
					}
				}
			}
		}
		break;
		case 4: {
			AnsiString reward = "";
			if (Rewarded->Checked) {
				reward = "YES";
			} else {
				reward = "NO";
			}
            std::vector <invertedListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Reward == data[i].value) {
						occupied = true;
						break;
					} else {
						occupied = false;
					}
				}
				if (!occupied) {
					std::vector <int> performers;
					invertedListString il;
					il.value = k->Reward;
					il.indexes = performers;
					data.push_back(il);
				}
				arraySize = arraySize + 1;
				k = k->next;
			}
			std::sort(data.begin(), data.end(), compareInvertedString);
			k = s;
			list * dataArray = new list[arraySize];
			for (int i = 0; i < arraySize; i++) {
				dataArray[i] = *k;
				k = k->next;
			}
			k = s;
			for (int i = 0; i < arraySize; i++) {
				for (int j = 0; j < data.size(); j++) {
					if (dataArray[i].Reward == data[j].value) {
						data[j].indexes.push_back(i);
					}
				}
			}
			int position = -1;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (data[i].value == reward) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				for (int j = 0; j < data[position].indexes.size(); j++) {
					list * k2 = new list;
					*k2 = dataArray[data[position].indexes[j]];
					addToNewList(k2, &s2, &e2);
					addToGrid(Table, k2);
					delete k2;
				}
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
		break;
		case 5: {
			if (checkPrice(Price)) {
				std::vector <invertedListDouble> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Price == data[i].value) {
							occupied = true;
							break;
						} else {
							occupied = false;
						}
					}
					if (!occupied) {
						std::vector <int> performers;
						invertedListDouble il;
						il.value = k->Price;
						il.indexes = performers;
						data.push_back(il);
					}
					arraySize = arraySize + 1;
					k = k->next;
				}
				std::sort(data.begin(), data.end(), compareInvertedDouble);
				k = s;
				list * dataArray = new list[arraySize];
				for (int i = 0; i < arraySize; i++) {
					dataArray[i] = *k;
					k = k->next;
				}
				k = s;
				for (int i = 0; i < arraySize; i++) {
					for (int j = 0; j < data.size(); j++) {
						if (dataArray[i].Price == data[j].value) {
							data[j].indexes.push_back(i);
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (data[i].value == Price->Text.ToDouble()) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					for (int j = 0; j < data[position].indexes.size(); j++) {
						list * k2 = new list;
						*k2 = dataArray[data[position].indexes[j]];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
					}
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			}
		}
		break;
		case 6: {
			if (checkRating(Rating)) {
				std::vector <invertedListDouble> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Rating == data[i].value) {
							occupied = true;
							break;
						} else {
							occupied = false;
						}
					}
					if (!occupied) {
						std::vector <int> performers;
						invertedListDouble il;
						il.value = k->Rating;
						il.indexes = performers;
						data.push_back(il);
					}
					arraySize = arraySize + 1;
					k = k->next;
				}
				std::sort(data.begin(), data.end(), compareInvertedDouble);
				k = s;
				list * dataArray = new list[arraySize];
				for (int i = 0; i < arraySize; i++) {
					dataArray[i] = *k;
					k = k->next;
				}
				k = s;
				for (int i = 0; i < arraySize; i++) {
					for (int j = 0; j < data.size(); j++) {
						if (dataArray[i].Rating == data[j].value) {
							data[j].indexes.push_back(i);
						}
					}
				}
				int position = -1;
				for (int i = 0; i < data.size(); i++) {
					if (data[i].value == Rating->Text.ToDouble()) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					for (int j = 0; j < data[position].indexes.size(); j++) {
						list * k2 = new list;
						*k2 = dataArray[data[position].indexes[j]];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
                    	delete k2;
					}
				}
            }
		}
		break;
		case 7: {
            std::vector <invertedListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Description == data[i].value) {
						occupied = true;
						break;
					} else {
						occupied = false;
					}
				}
				if (!occupied) {
					std::vector <int> performers;
					invertedListString il;
					il.value = k->Description;
					il.indexes = performers;
					data.push_back(il);
				}
				arraySize = arraySize + 1;
				k = k->next;
			}
			std::sort(data.begin(), data.end(), compareInvertedString);
			k = s;
			list * dataArray = new list[arraySize];
			for (int i = 0; i < arraySize; i++) {
				dataArray[i] = *k;
				k = k->next;
			}
			k = s;
			for (int i = 0; i < arraySize; i++) {
				for (int j = 0; j < data.size(); j++) {
					if (dataArray[i].Description == data[j].value) {
						data[j].indexes.push_back(i);
					}
				}
			}
			int position = -1;
			for (int i = 0; i < data.size(); i++) {
				if (data[i].value == Description->Text) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				for (int j = 0; j < data[position].indexes.size(); j++) {
					list * k2 = new list;
					*k2 = dataArray[data[position].indexes[j]];
					addToNewList(k2, &s2, &e2);
					addToGrid(Table, k2);
					delete k2;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

bool compareChainString(const chainListString & a, const chainListString & b) {
	return a.value < b.value;
}

bool compareChainInt(const chainListInt & a, const chainListInt & b) {
	return a.value < b.value;
}

bool compareChainDouble(const chainListDouble & a, const chainListDouble & b) {
	return a.value < b.value;
}

void __fastcall TDB::ChainListClick(TObject *Sender)
{
	list * k = s;
	bool occupied = false;
	int arraySize = 0;
	int index = 0;
	int counter = 0;
	switch (Searchings->ItemIndex) {
		case 0: {
			std::vector <chainListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Performer == data[i].value) {
						 occupied = true;
						 break;
					} else {
						occupied = false;
						index = counter;
					}
				}
				if (!occupied) {
					chainListString l;
					l.value = k->Performer;
					l.index = index;
					data.push_back(l);
				}
				counter = counter + 1;
				k = k->next;
			}
			sort(data.begin(), data.end(), compareChainString);
			k = s;
			list * dataArray = new list [counter];
			chainListString * chainIndexes = new chainListString [counter];
			for (int i = 0; i < counter; i++) {
				dataArray[i] = *k;
				chainIndexes[i].value = "";
				chainIndexes[i].index = -1;
				k = k->next;
			}
			int index2 = 0;
			for (int i = 0; i < data.size(); i++) {
				index2 = data[i].index;
				for (int j = 0; j < counter; j++) {
					if (data[i].value == dataArray[j].Performer && j > index2) {
						chainIndexes[index2].value = dataArray[j].Performer;
						chainIndexes[index2].index = j;
						index2 = j;
					}
				}
			}
			int position = -1;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (Performer->Text == data[i].value) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				int nextIndex, jumpIndex;
				nextIndex = jumpIndex = data[position].index;
				while (nextIndex != -1) {
					if (Performer->Text == chainIndexes[nextIndex].value) {
						list * k2 = new list;
						*k2 = dataArray[jumpIndex];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
						nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
					} else {
						nextIndex = -1;
					}
				}
				list * k3 = new list;
				*k3 = dataArray[jumpIndex];
				addToNewList(k3, &s2, &e2);
				addToGrid(Table, k3);
				delete k3;
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
		break;
		case 1: {
			std::vector <chainListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Title == data[i].value) {
						 occupied = true;
						 break;
					} else {
						occupied = false;
						index = counter;
					}
				}
				if (!occupied) {
					chainListString l;
					l.value = k->Title;
					l.index = index;
					data.push_back(l);
				}
				counter = counter + 1;
				k = k->next;
			}
			sort(data.begin(), data.end(), compareChainString);
			k = s;
			list * dataArray = new list [counter];
			chainListString * chainIndexes = new chainListString [counter];
			for (int i = 0; i < counter; i++) {
				dataArray[i] = *k;
				chainIndexes[i].value = "";
				chainIndexes[i].index = -1;
				k = k->next;
			}
			int index2 = 0;
			for (int i = 0; i < data.size(); i++) {
				index2 = data[i].index;
				for (int j = 0; j < counter; j++) {
					if (data[i].value == dataArray[j].Title && j > index2) {
						chainIndexes[index2].value = dataArray[j].Title;
						chainIndexes[index2].index = j;
						index2 = j;
					}
				}
			}
			int position = -1;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (Title->Text == data[i].value) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				int nextIndex, jumpIndex;
				nextIndex = jumpIndex = data[position].index;
				while (nextIndex != -1) {
					if (Title->Text == chainIndexes[nextIndex].value) {
						list * k2 = new list;
						*k2 = dataArray[jumpIndex];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
						nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
					} else {
						nextIndex = -1;
					}
				}
				list * k3 = new list;
				*k3 = dataArray[jumpIndex];
				addToNewList(k3, &s2, &e2);
				addToGrid(Table, k3);
				delete k3;
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
		break;
		case 2: {
			if (checkYear(ReleaseYear)) {
				std::vector <chainListInt> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Released == data[i].value) {
							 occupied = true;
							 break;
						} else {
							occupied = false;
							index = counter;
						}
					}
					if (!occupied) {
						chainListInt l;
						l.value = k->Released;
						l.index = index;
						data.push_back(l);
					}
					counter = counter + 1;
					k = k->next;
				}
				sort(data.begin(), data.end(), compareChainInt);
				k = s;
				list * dataArray = new list [counter];
				chainListInt * chainIndexes = new chainListInt [counter];
				for (int i = 0; i < counter; i++) {
					dataArray[i] = *k;
					chainIndexes[i].value = -1;
					chainIndexes[i].index = -1;
					k = k->next;
				}
				int index2 = 0;
				for (int i = 0; i < data.size(); i++) {
					index2 = data[i].index;
					for (int j = 0; j < counter; j++) {
						if (data[i].value == dataArray[j].Released && j > index2) {
							chainIndexes[index2].value = dataArray[j].Released;
							chainIndexes[index2].index = j;
							index2 = j;
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (ReleaseYear->Text.ToInt() == data[i].value) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					int nextIndex, jumpIndex;
					nextIndex = jumpIndex = data[position].index;
					while (nextIndex != -1) {
						if (ReleaseYear->Text.ToInt() == chainIndexes[nextIndex].value) {
							list * k2 = new list;
							*k2 = dataArray[jumpIndex];
							addToNewList(k2, &s2, &e2);
							addToGrid(Table, k2);
							delete k2;
							nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
						} else {
							nextIndex = -1;
						}
					}
					list * k3 = new list;
					*k3 = dataArray[jumpIndex];
					addToNewList(k3, &s2, &e2);
					addToGrid(Table, k3);
					delete k3;
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			}
		}
		break;
		case 3: {
			if (checkAmount(Amount, 999999999)) {
                std::vector <chainListInt> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Amount == data[i].value) {
							 occupied = true;
							 break;
						} else {
							occupied = false;
							index = counter;
						}
					}
					if (!occupied) {
						chainListInt l;
						l.value = k->Amount;
						l.index = index;
						data.push_back(l);
					}
					counter = counter + 1;
					k = k->next;
				}
				sort(data.begin(), data.end(), compareChainInt);
				k = s;
				list * dataArray = new list [counter];
				chainListInt * chainIndexes = new chainListInt [counter];
				for (int i = 0; i < counter; i++) {
					dataArray[i] = *k;
					chainIndexes[i].value = -1;
					chainIndexes[i].index = -1;
					k = k->next;
				}
				int index2 = 0;
				for (int i = 0; i < data.size(); i++) {
					index2 = data[i].index;
					for (int j = 0; j < counter; j++) {
						if (data[i].value == dataArray[j].Amount && j > index2) {
							chainIndexes[index2].value = dataArray[j].Amount;
							chainIndexes[index2].index = j;
							index2 = j;
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (Amount->Text.ToInt() == data[i].value) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					int nextIndex, jumpIndex;
					nextIndex = jumpIndex = data[position].index;
					while (nextIndex != -1) {
						if (Amount->Text.ToInt() == chainIndexes[nextIndex].value) {
							list * k2 = new list;
							*k2 = dataArray[jumpIndex];
							addToNewList(k2, &s2, &e2);
							addToGrid(Table, k2);
							delete k2;
							nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
						} else {
							nextIndex = -1;
						}
					}
					list * k3 = new list;
					*k3 = dataArray[jumpIndex];
					addToNewList(k3, &s2, &e2);
					addToGrid(Table, k3);
					delete k3;
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
			}
		}
		break;
		case 4: {
			AnsiString reward = "";
			if (Rewarded->Checked) {
				reward = "YES";
			} else {
				reward = "NO";
			}
			std::vector <chainListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Reward == data[i].value) {
						 occupied = true;
						 break;
					} else {
						occupied = false;
						index = counter;
					}
				}
				if (!occupied) {
					chainListString l;
					l.value = k->Reward;
					l.index = index;
					data.push_back(l);
				}
				counter = counter + 1;
				k = k->next;
			}
			sort(data.begin(), data.end(), compareChainString);
			k = s;
			list * dataArray = new list [counter];
			chainListString * chainIndexes = new chainListString [counter];
			for (int i = 0; i < counter; i++) {
				dataArray[i] = *k;
				chainIndexes[i].value = "";
				chainIndexes[i].index = -1;
				k = k->next;
			}
			int index2 = 0;
			for (int i = 0; i < data.size(); i++) {
				index2 = data[i].index;
				for (int j = 0; j < counter; j++) {
					if (data[i].value == dataArray[j].Reward && j > index2) {
						chainIndexes[index2].value = dataArray[j].Reward;
						chainIndexes[index2].index = j;
						index2 = j;
					}
				}
			}
			int position = -1;
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (reward == data[i].value) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				list * firstOccurence = new list;
				int nextIndex, jumpIndex;
				nextIndex = jumpIndex = data[position].index;
				while (nextIndex != -1) {
					if (reward == chainIndexes[nextIndex].value) {
						list * k2 = new list;
						*k2 = dataArray[jumpIndex];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
						nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
					} else {
						nextIndex = -1;
					}
				}
				list * k3 = new list;
				*k3 = dataArray[jumpIndex];
				addToNewList(k3, &s2, &e2);
				addToGrid(Table, k3);
				delete k3;
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
		break;
		case 5: {
			if (checkPrice(Price)) {
				std::vector <chainListDouble> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Price == data[i].value) {
							 occupied = true;
							 break;
						} else {
							occupied = false;
							index = counter;
						}
					}
					if (!occupied) {
						chainListDouble l;
						l.value = k->Price;
						l.index = index;
						data.push_back(l);
					}
					counter = counter + 1;
					k = k->next;
				}
				sort(data.begin(), data.end(), compareChainDouble);
				k = s;
				list * dataArray = new list [counter];
				chainListDouble * chainIndexes = new chainListDouble [counter];
				for (int i = 0; i < counter; i++) {
					dataArray[i] = *k;
					chainIndexes[i].value = -1;
					chainIndexes[i].index = -1;
					k = k->next;
				}
				int index2 = 0;
				for (int i = 0; i < data.size(); i++) {
					index2 = data[i].index;
					for (int j = 0; j < counter; j++) {
						if (data[i].value == dataArray[j].Price && j > index2) {
							chainIndexes[index2].value = dataArray[j].Price;
							chainIndexes[index2].index = j;
							index2 = j;
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (Price->Text.ToDouble() == data[i].value) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					list * firstOccurence = new list;
					int nextIndex, jumpIndex;
					nextIndex = jumpIndex = data[position].index;
					while (nextIndex != -1) {
						if (Price->Text.ToDouble() == chainIndexes[nextIndex].value) {
							list * k2 = new list;
							*k2 = dataArray[jumpIndex];
							addToNewList(k2, &s2, &e2);
							addToGrid(Table, k2);
							delete k2;
							nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
						} else {
							nextIndex = -1;
						}
					}
					list * k3 = new list;
					*k3 = dataArray[jumpIndex];
					addToNewList(k3, &s2, &e2);
					addToGrid(Table, k3);
					delete k3;
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
            }
		}
		break;
		case 6: {
			if (checkRating(Rating)) {
                std::vector <chainListDouble> data;
				while (k != NULL) {
					for (int i = 0; i < data.size(); i++) {
						if (k->Rating == data[i].value) {
							 occupied = true;
							 break;
						} else {
							occupied = false;
							index = counter;
						}
					}
					if (!occupied) {
						chainListDouble l;
						l.value = k->Rating;
						l.index = index;
						data.push_back(l);
					}
					counter = counter + 1;
					k = k->next;
				}
				sort(data.begin(), data.end(), compareChainDouble);
				k = s;
				list * dataArray = new list [counter];
				chainListDouble * chainIndexes = new chainListDouble [counter];
				for (int i = 0; i < counter; i++) {
					dataArray[i] = *k;
					chainIndexes[i].value = -1;
					chainIndexes[i].index = -1;
					k = k->next;
				}
				int index2 = 0;
				for (int i = 0; i < data.size(); i++) {
					index2 = data[i].index;
					for (int j = 0; j < counter; j++) {
						if (data[i].value == dataArray[j].Rating && j > index2) {
							chainIndexes[index2].value = dataArray[j].Rating;
							chainIndexes[index2].index = j;
							index2 = j;
						}
					}
				}
				int position = -1;
				auto start = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < data.size(); i++) {
					if (Rating->Text.ToDouble() == data[i].value) {
						position = i;
						break;
					}
				}
				if (position == -1) {
					ShowMessage("Cannot find data");
				} else {
					Table->RowCount = 1;
					list * s2 = NULL;
					list * e2 = NULL;
					list * firstOccurence = new list;
					int nextIndex, jumpIndex;
					nextIndex = jumpIndex = data[position].index;
					while (nextIndex != -1) {
						if (Rating->Text.ToDouble() == chainIndexes[nextIndex].value) {
							list * k2 = new list;
							*k2 = dataArray[jumpIndex];
							addToNewList(k2, &s2, &e2);
							addToGrid(Table, k2);
							delete k2;
							nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
						} else {
							nextIndex = -1;
						}
					}
					list * k3 = new list;
					*k3 = dataArray[jumpIndex];
					addToNewList(k3, &s2, &e2);
					addToGrid(Table, k3);
					delete k3;
					auto finish = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = finish - start;
					AnsiString time = elapsed.count();
					ShowMessage("Time taken: " + time + "s");
				}
            }
		}
		break;
		case 7: {
            std::vector <chainListString> data;
			while (k != NULL) {
				for (int i = 0; i < data.size(); i++) {
					if (k->Description == data[i].value) {
						 occupied = true;
						 break;
					} else {
						occupied = false;
						index = counter;
					}
				}
				if (!occupied) {
					chainListString l;
					l.value = k->Description;
					l.index = index;
					data.push_back(l);
				}
				counter = counter + 1;
				k = k->next;
			}
			sort(data.begin(), data.end(), compareChainString);
			k = s;
			list * dataArray = new list [counter];
			chainListString * chainIndexes = new chainListString [counter];
			for (int i = 0; i < counter; i++) {
				dataArray[i] = *k;
				chainIndexes[i].value = "";
				chainIndexes[i].index = -1;
				k = k->next;
			}
			int index2 = 0;
			for (int i = 0; i < data.size(); i++) {
				index2 = data[i].index;
				for (int j = 0; j < counter; j++) {
					if (data[i].value == dataArray[j].Description && j > index2) {
						chainIndexes[index2].value = dataArray[j].Description;
						chainIndexes[index2].index = j;
                        index2 = j;
					}
				}
			}
			int position = -1;
            auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < data.size(); i++) {
				if (Description->Text == data[i].value) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				ShowMessage("Cannot find data");
			} else {
				Table->RowCount = 1;
				list * s2 = NULL;
				list * e2 = NULL;
				int nextIndex, jumpIndex;
				nextIndex = jumpIndex = data[position].index;
				while (nextIndex != -1) {
					if (Title->Text == chainIndexes[nextIndex].value) {
						list * k2 = new list;
						*k2 = dataArray[jumpIndex];
						addToNewList(k2, &s2, &e2);
						addToGrid(Table, k2);
						delete k2;
						nextIndex = jumpIndex = chainIndexes[jumpIndex].index;
					} else {
						nextIndex = -1;
					}
				}
				list * k3 = new list;
				*k3 = dataArray[jumpIndex];
				addToNewList(k3, &s2, &e2);
				addToGrid(Table, k3);
				delete k3;
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start;
				AnsiString time = elapsed.count();
				ShowMessage("Time taken: " + time + "s");
			}
		}
	}
}
//---------------------------------------------------------------------------

