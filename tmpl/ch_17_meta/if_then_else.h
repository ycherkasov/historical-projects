#pragma once

// ������ ������������ ��������� IfThenElse,
// ������������ ��� � ����������� �� �������

// ��������� ������ - �� ����� �����������
template <bool B, typename Tr, typename Fls>
struct IfThenElse;

// ��������� ������������� ��� True
template <typename Tr, typename Fls>
struct IfThenElse<true, Tr, Fls>{
	typedef Tr Result;
};

// ��������� ������������� ��� False
template <typename Tr, typename Fls>
struct IfThenElse<false, Tr, Fls>{
	typedef Fls Result;
};

