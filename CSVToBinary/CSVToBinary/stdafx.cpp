#include "stdafx.h"

std::wstring trim(std::wstring s)
{
	if (s.length() == 0) return s;
	std::size_t begin = s.find_first_not_of(L" \a\b\f\n\r\t\v");
	if (begin == std::wstring::npos) return s;
	std::size_t end = s.find_last_not_of(L" \a\b\f\n\r\t\v");

	return std::wstring(s, begin, end - begin + 1);
}
