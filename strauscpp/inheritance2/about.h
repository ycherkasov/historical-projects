/* ����� 12 �����������
����� 9 �������
������ 1 ����� 6
����������� ������. �������� �������. 
������������ ����������.
����������� �������. ����������� ������. 
�������������� �������� �������.
*/

/*�������
*/

/** �������:
1. ������ �� ����������� � ���������� �������������( ���. 363 )
2. ������� �� ��������� ������ (������ 2-29)
*/

// ����������� ������ ������ ����������� � ��������� �������������� � ���������� COM.

// ������� ����������� ������:
// �������� ���� ������: �������, ������������ ������ ��� ��������� �� ������� ����� 
// ������ ����� �������� � � ��������-������������, ��� �� �������������.
// http://old.softerra.ru/review/program/15958/page1.html
// http://forums.realcoding.net/index.php?showtopic=16693

/*
���������������, � ������ ������, �������� ������������ �������� ���� ���������� ������� ��� ���������. 
��������, ������-������ ����� ��� ����������: ��������� ����� ������ � ����� ������. 
���������� ������� ����� ������� ������, �� ������ ���������� ������ ���� ������, �������� �����, 
����������� ������, �������� �������� ��������� � �������� �����. ����� ����������� ������ 
�� ���� ��������, ������ ������� ������������, ����� ���������� ��������� � ����. 
����� ������ � ��������� ����������, ��������������� ��������.

������� 1: ��� �������� �� ������ public ������, ������ ������� ������ ���� ������������.
������� 2: ����� ����������� ���������� ������ ������� ���������� ��������������� ������ �������.
������� 3: ���������� ���������, ���, �� ����� ���������� ������, ��������� ������� ����� ���� ��������.
*/

	/*
	In order to control these types of conversions between classes, 
	we have four specific casting operators: 
	dynamic_cast, reinterpret_cast, static_cast and const_cast. 
	Their format is to follow the new type enclosed between angle-brackets (<>) and immediately after, 
	the expression to be converted between parentheses.

	dynamic_cast <new_type> (expression)
	reinterpret_cast <new_type> (expression)
	static_cast <new_type> (expression)
	const_cast <new_type> (expression)

	The traditional type-casting equivalents to these expressions would be:

	(new_type) expression
	new_type (expression)

	static_cast can perform conversions between pointers to related classes, 
	not only upcasts (from pointer-to-derived to pointer-to-base), 
	but also downcasts (from pointer-to-base to pointer-to-derived). 
	No checks are performed during runtime to guarantee that the object being converted 
	is in fact a full object of the destination type. 
	Therefore, it is up to the programmer to ensure that the conversion is safe. 
	On the other side, it does not incur the overhead of the type-safety checks of dynamic_cast.

	dynamic_cast can only be used with pointers and references to classes (or with void*). 
	Its purpose is to ensure that the result of the type conversion points to a valid 
	complete object of the destination pointer type.
	This naturally includes pointer upcast (converting from pointer-to-derived to pointer-to-base), 
	in the same way as allowed as an implicit conversion.
	But dynamic_cast can also downcast (convert from pointer-to-base to pointer-to-derived) 
	polymorphic classes (those with virtual members) if -and only if- 
	the pointed object is a valid complete object of the target type.


	reinterpret_cast converts any pointer type to any other pointer type, even of unrelated classes. 
	The operation result is a simple binary copy of the value from one pointer to the other. 
	All pointer conversions are allowed: neither the content pointed nor the pointer type itself is checked.

	This type of casting manipulates the constness of the object pointed by a pointer,
	*/


/*
������:
1. �� ����������� ���� �������� ���������� � ����.
2. �� ����������� ������� �� �������� �� ��������� ������.
3. ��������������� ����������� ����������� �������������� ��� ���� �� 
    (���� ������� ����� ��� ����������� ������������� ������ � �������������� �����).
4. ����������� ������������ �������� ��� 
   - ����������� ����������
   - ������������ ���������
   - ���������� �� ���������
5. � ����������� ������� ����� ����� ����������� ����������.
6. ����������� ����� ������ �� ��������� � ������������
7. ������������ �������� - ����������� ������ �� ������� (�.�. ��������� � ������������ �� ���������)
8. ����������� �� ����������� - �� ��������� �� ��������� ��� ����������� ����
9. ����� �� ����������� ��������� ������������ � ������������� �������
10.����������� ���������� � �������, �������� ������������, � �� �����������
11.����������� ���� ����� ��������� �� ����� � ������ ��������
12.���� ��� ������ �� ������������� (� ���� ������ ������� ���� ������ ���������� �����������)
13.� ����������� ������ ������������� ������ ���������������� ����� ������� �������, �� ����� � ���� ������������
14.������ �������� ������ ����� �������� ����.
15.����������� �������� ��� ���������� ������������� ��� �������� ���������� �������� ������ (������� �������)
16.��� ����� �� ����� ������������ �������������� (�����������)
17.�������� ������� ����� ���� ������������� ������. ������� �������� ����� �� ����������.
18.� ������� ������ ���������� ������� ������ _�����_ ����� ������ �� ��������� �������������� ������������
19.� ����������� ������� ����������� ������� ����������� ������ ���� � ���� ���� �������������
20.����� �������� � ���������� ����� ����������� ����������. �� ����� ��������� ���������� � �� �������������.
   �.�. �����-��������� ����������� �� ��������. (�� ����� ������ ���� ����������� �� ���������)
21.� ����������� ������ ����� ����������� �� ����� ����������� ������ � �������� �� ��� ����������� �������.
22.����� "����������������" ������� ������� (�������� ������) ����� �������� ������ 
   �� ������� ����� � ��������������.
23.����� ��������� using::f, ����� �������������� ������������ ����� ������ ������������ � ������
24.�������� �� ��������� � ������ ���������� ����������� ������� �����������!
25.������������ �������� ����������� ������� � ������ ������� ����� ����������, ���� ���:
  - ��������� ��� ������
  - ��� ���������� ���� � �����
26. ��������� ����� ���� ������������.
27. ������ VTBL ������������ ABI.
28. � ������������ ��� ����������� ���������� ������ ���� ����������� ������.
29.� ������ ��������� ������������ ����� �� �������� ���������� �� ������-��������� � �������� ������.
30.��� ���������� �������� ������������� ����������. ����������� ������������, ������ ����� ������� ��� �������.
   (������ � ���������� ������, �������������)
*/