#include "namespaces.h"
#include <vector>
// ����� ����������� ������������� �������� ��������� namespace
// ����������� � �������������, ����������, �����������


// ����������� ������������ ����
namespace my
{
    int a;
}

// ������������ ������������ ���� ����� ��������� ������������ ����� ���
// ����� ����������� ����� ����������
namespace my
{
    int aa;
}

// ������������� ������������ ����
// ���������� ��� ���������� ���������� ���� � ������� ������� �� ������ ������ ����������
namespace
{
    int anonimous;
}

// ��� ����������� ������������ ����
namespace your1
{
    int a;
}

namespace your2
{
    int b;
}

// ����������� ������������ ����
namespace your
{
    using namespace your1;
    using namespace your2;
}

// ������� ������������ ����
// ������ ������� ��� ����� ������ ������������ ����
namespace aaa = my;


// ����� (� ����� ����� �������������)
// ��������� ������������� ������ ������������ ����
// � ������, ������ ���, � ������� ��� ����������.
namespace parser{
    int a;
    using namespace your;
}

// ����� ��������� ������� ������ ����������� ����
// �� ������ � ����� ������ �� � ��� ������ (� ������ h-�����)
namespace parser{
    int b;
}

// ����� ��������� ��������� C-���������� � ������������ ����
/*
namespace CLib
{
int printf();
}
*/

// ������ 1 ������� 28
// ���� ���� ������������ ���� �� ������������, �� �� ������ ������� ���������������
// �.�.
namespace A{
    int aaa;
}

namespace B{
    int aaa;
}

using namespace A;
using namespace B;

void show_namespaces()
{
    my::a = 1;
    your1::a = 2;

    // namespace your ���������� your1 � your2
    your::a = 3;

    // ������������ ���������� using namespace �����
    // � �������� ������� ���������, ��� �������������� ����������
    {
        using namespace your;
        a = 4;
    }

    {
        // ����� ��������� ��������� ����� ������������
        // _using-���������_
        using namespace my;

        // � ����� ������ ���� �������
        // _using-����������_ -
        // ����� ��������� �� ��������� � using-����������
        using your1::a;
        a = 5;	// your1::a
    }

    aaa::a = 6;

    // �������� ���������� �������� ������ ������������ ����
    // ��� ���������� using operator+ ����� ��������� ��� �������������
    // ����� operator+()

    // Using ambuguosity with the class name
    {
        using namespace std;
        // local names hides using-directive opened names
        int vector = 10; // hides std::vector
    }

    {
        using std::vector;
        // local names overloads with using-declaration opened names
        // int vector = 10; compiler error (hahaha)
    }
}