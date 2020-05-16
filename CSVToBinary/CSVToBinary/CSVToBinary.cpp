#include "stdafx.h"

std::wregex reg1(L"(\\+|-)?[[:digit:]]+");
std::wregex reg2(L"((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");

std::vector<std::wstring> split(std::wstring str, wchar_t delimiter) {
	std::vector<std::wstring> internal;
	std::wstringstream ss(str);
	std::wstring temp;

	while (std::getline(ss, temp, delimiter)) {
		internal.push_back(temp);
	}

	return internal;
}

bool CheckFloatInt(std::wstring str) {
	if (str.find('.') != std::wstring::npos) {
		return true;
	}
	return false;
}

bool CheckFloat(std::wstring bstr, float* result) {
	std::wstring str = trim(bstr);
	if (!std::regex_match(str, reg1) && !std::regex_match(str, reg2)) return false;
	try {
		*result = std::stof(str);
		return true;
	}
	catch (...) {

	}
	return false;
}

bool CheckInt(std::wstring bstr, int* result) {
	std::wstring str = trim(bstr);
	if (!std::regex_match(str, reg1) && !std::regex_match(str, reg2)) return false;
	try {
		*result = std::stoi(str);
		return true;
	}
	catch (...) {

	}
	return false;
}

size_t count_line(std::wistream& is)
{
	// skip when bad
	if (is.bad()) return 0;
	// save state
	std::wistream::iostate state_backup = is.rdstate();
	// clear state
	is.clear();
	std::wistream::streampos pos_backup = is.tellg();

	is.seekg(0);
	size_t line_cnt;
	size_t lf_cnt = std::count(std::istreambuf_iterator<wchar_t>(is), std::istreambuf_iterator<wchar_t>(), '\n');
	line_cnt = lf_cnt;
	// if the file is not end with '\n' , then line_cnt should plus 1  
	is.unget();
	if (is.get() != '\n') { ++line_cnt; }

	// recover state
	is.clear(); // previous reading may set eofbit
	is.seekg(pos_backup);
	is.setstate(state_backup);

	return line_cnt;
}

int main()
{
	/**
	bofstream t1("test.txt");
	t1.writeInt32(1);
	t1.writeSingle(1.4f);
	t1.writeWideString(L"가이아");
	t1.writeInt32(1);
	t1.flush();
	t1.close();
	bifstream t2("test.txt");
	int test1 = t2.readInt32();
	float test2 = t2.readSingle();
	std::wstring test3 = t2.readWideString();
	int test4 = t2.readInt32();
	t2.close();

	/**/
	mains:
	std::cout << "파일명 입력 (without .csv) : ";
	std::string fileName;
	std::cin >> fileName;
	int maxLen = 0;
	std::cout << "읽을 열 개수 (음수 일 경우 무제한) : ";
	std::cin >> maxLen;
	std::wifstream is;
	is.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));

	is.open("./" + fileName + ".csv");

	bofstream os;
	os.open("./" + fileName + ".asyub");
	
	std::wstring buffer;

	int ITemp = 0;
	float FTemp = 0;

	int length = count_line(is);
	os.writeInt32(length);
	os.writeInt32(maxLen);

	while (std::getline(is, buffer)) {
		int Len = 0;
		if (buffer[buffer.length() - 1] == ',')
			buffer += L"0";
		std::vector<std::wstring> result = split(buffer, ',');
		for (std::wstring s : result) {
			if (maxLen >= 0 && ++Len > maxLen) break;
			bool IsFloat = CheckFloatInt(s);
			if (IsFloat && CheckFloat(s, &FTemp)) {
				os.writeSingle(FTemp);
			} else if (!IsFloat && CheckInt(s, &ITemp)) {
				os.writeInt32(ITemp);
			} else {
				os.writeWideString(s);
			}
		}
	}

	os.flush();
	os.close();
	is.close();

	system("pause");

	goto mains;
}