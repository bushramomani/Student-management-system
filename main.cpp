```cpp
// Student Management System - Single File (C++17)
// Features: Add, List, Search (ID/Name), Update, Delete, CSV Save/Load
// Build: g++ -std=gnu++17 -O2 -o sms main.cpp

#include <bits/stdc++.h>
using namespace std;

struct Student {
int id{};
string name;
string major;
int year{}; // e.g., 1..5
double gpa{}; // 0.0..4.0

static bool validName(const string& s) {
return !s.empty();
}
static bool validMajor(const string& s) {
return !s.empty();
}
static bool validYear(int y) {
return y >= 1 && y <= 6;
}
static bool validGPA(double g) {
return g >= 0.0 && g <= 4.0;
}
};

static inline string ltrim(string s) { s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){return !isspace(ch);})); return s; }
static inline string rtrim(string s) { s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){return !isspace(ch);}).base(), s.end()); return s; }
static inline string trim(string s) { return rtrim(ltrim(s)); }

bool icontains(const string& text, const string& sub) {
string a = text, b = sub;
transform(a.begin(), a.end(), a.begin(), ::tolower);
transform(b.begin(), b.end(), b.begin(), ::tolower);
return a.find(b) != string::npos;
}

vector<string> splitCSV(const string& line) {
vector<string> out;
string cur;
for (char c : line) {
if (c == ',') { out.push_back(cur); cur.clear(); }
else { cur.push_back(c); }
}
out.push_back(cur);
return out;
}

string joinCSV(const vector<string>& cols) {
string s;
for (size_t i = 0; i < cols.size(); ++i) {
if (i) s.push_back(',');
s += cols[i];
}
return s;
}

class StudentDB {
vector<Student> data;
string filePath = "students.csv";
int nextId = 1;

int computeNextId() const {
int mx = 0;
for (auto& s : data) mx = max(mx, s.id);
return mx + 1;
}

public:
bool load() {
data.clear();
ifstream in(filePath);
if (!in.good()) return false; // no file yet
string line;
bool first = true;
while (getline(in, line)) {
if (first) { first = false; continue; } // skip header
if (trim(line).empty()) continue;
auto cols = splitCSV(line);
if (cols.size() < 5) continue;
Student s;
try {
s.id = stoi(cols[0]);
s.name = cols[1];
s.major = cols[2];
s.year = stoi(cols[3]);
s.gpa = stod(cols[4]);
data.push_back(s);
} catch (...) {
// ignore malformed line
}
}
nextId = computeNextId();
return true;
}

bool save() const {
ofstream out(filePath, ios::trunc);
if (!out.good()) return false;
out << "id,name,major,year,gpa\n";
for (auto& s : data) {
out << joinCSV({ to_string(s.id), s.name, s.major, to_string(s.year), to_string(s.gpa) }) << "\n";
}
return true;
}

const vector<Student>& all() const { return data; }

Student* findById(int id) {
for (auto& s : data) if (s.id == id) return &s;
return nullptr;
}

vector<const Student*> searchByName(const string& q) const {
vector<const Student*> res;
for (auto& s : data) if (icontains(s.name, q)) res.push_back(&s);
return res;
}

Student& add(Student s) {
s.id = nextId++;
data.push_back(s);
return data.back();
}

bool removeById(int id) {
auto it = remove_if(data.begin(), data.end(), [&](const Student& s){ return s.id == id; });
if (it == data.end()) return false;
data.erase(it, data.end());
return true;
}
};

int readInt(const string& prompt, int lo = INT_MIN, int hi = INT_MAX) {
while (true) {
cout << prompt;
string line; getline(cin, line);
try {
int v = stoi(trim(line));
if (v < lo || v > hi) throw runtime_error("range");
return v;
} catch (...) {
cout << " ! please enter a whole number";
if (lo != INT_MIN || hi != INT_MAX) cout << " between " << lo << " and " << hi;
cout << ".\n";
}
}
}

double readDouble(const string& prompt, double lo, double hi) {
while (true) {
cout << prompt;
string line; getline(cin, line);
try {
double v = stod(trim(line));
if (v < lo || v > hi) throw runtime_error("range");
return v;
} catch (...) {
cout << fixed << setprecision(1);
cout << " ! please enter a number between " << lo << " and " << hi << ".\n";
}
}
}

string readNonEmpty(const string& prompt) {
while (true) {
cout << prompt;
string s; getline(cin, s);
s = trim(s);
if (!s.empty()) return s;
cout << " ! value cannot be empty.\n";
}
}

void printStudent(const Student& s) {
cout << "ID: " << s.id
<< " | Name: " << s.name
<< " | Major: " << s.major
<< " | Year: " << s.year
<< " | GPA: " << fixed << setprecision(2) << s.gpa << "\n";
}

void listAll(const StudentDB& db) {
const auto& v = db.all();
if (v.empty()) { cout << "(no students yet)\n"; return; }
cout << "\n==== Students (" << v.size() << ") ====\n";
for (auto& s : v) printStudent(s);
cout << "===========================\n";
}

void actionAdd(StudentDB& db) {
cout << "\nAdd Student\n";
Student s;
s.name = readNonEmpty(" Name : ");
s.major = readNonEmpty(" Major: ");
s.year = readInt(" Year (1..6): ", 1, 6);
s.gpa = readDouble(" GPA (0.0..4.0): ", 0.0, 4.0);

auto& added = db.add(s);
cout << " -> Added with ID " << added.id << "\n";
}

void actionSearchById(StudentDB& db) {
int id = readInt("Enter ID to search: ");
auto* s = db.findById(id);
if (!s) cout << " not found.\n";
else { printStudent(*s); }
}

void actionSearchByName(const StudentDB& db) {
string q = readNonEmpty("Enter name (partial OK): ");
auto hits = db.searchByName(q);
if (hits.empty()) { cout << " no matches.\n"; return; }
cout << "Matches:\n";
for (auto* s : hits) printStudent(*s);
}

void actionUpdate(StudentDB& db) {
int id = readInt("Enter ID to update: ");
auto* s = db.findById(id);
if (!s) { cout << " not found.\n"; return; }

cout << "Leave blank to keep current value.\n";
cout << " Current Name : " << s->name << "\n";
cout << " Current Major: " << s->major << "\n";
cout << " Current Year : " << s->year << "\n";
cout << " Current GPA : " << fixed << setprecision(2) << s->gpa << "\n";

auto askOpt = [](const string& prompt)->optional<string>{
cout << prompt;
string line; getline(cin, line);
if (trim(line).empty()) return nullopt;
return trim(line);
};

if (auto v = askOpt(" New Name : ")) { s->name = *v; }
if (auto v = askOpt(" New Major : ")) { s->major = *v; }
if (auto v = askOpt(" New Year : ")) {
try {
int y = stoi(*v);
if (!Student::validYear(y)) throw runtime_error("range");
s->year = y;
} catch (...) { cout << " ! invalid year, keeping old.\n"; }
}
if (auto v = askOpt(" New GPA : ")) {
try {
double g = stod(*v);
if (!Student::validGPA(g)) throw runtime_error("range");
s->gpa = g;
} catch (...) { cout << " ! invalid gpa, keeping old.\n"; }
}
cout << " -> updated.\n";
}

void actionDelete(StudentDB& db) {
int id = readInt("Enter ID to delete: ");
if (db.removeById(id)) cout << " -> deleted.\n";
else cout << " not found.\n";
}

int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);

StudentDB db;
db.load(); 

while (true) {
cout << "\n=== Student Management ===\n"
<< "1) Add student\n"
<< "2) List all\n"
<< "3) Search by ID\n"
<< "4) Search by name\n"
<< "5) Update\n"
<< "6) Delete\n"
<< "0) Exit (save)\n"
<< "Select: ";
string choice; getline(cin, choice);
if (choice == "1") actionAdd(db);
else if (choice == "2") listAll(db);
else if (choice == "3") actionSearchById(db);
else if (choice == "4") actionSearchByName(db);
else if (choice == "5") actionUpdate(db);
else if (choice == "6") actionDelete(db);
else if (choice == "0") {
if (db.save()) cout << "Saved to students.csv\n";
else cout << "Warning: could not save file.\n";
break;
} else {
cout << " ! invalid choice.\n";
}
}
return 0;
}
