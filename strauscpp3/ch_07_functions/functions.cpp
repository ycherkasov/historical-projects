#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64) 
#define cdecl __cdecl
#define stdcall __stdcall
#define fastcall __fastcall
#else
// TODO: manage under GCC
#define cdecl
#define stdcall
#define fastcall
#endif


// ����������� ���������� �� ������ __cdecl
// ��������� ���������� ����� ����, ������ ������. 
// ������� ����� ���������� ���������� ���������.
int cdecl cdecl_func(int a, long b)
{
	// ��������� � ���� �������� �������� ����
	//00411440  push        ebp  

	//���������� ������ ������� ������� �����
	//00411441  mov         ebp,esp 

	//��������� ��������� ����� ��� ��������� ���������� 
	//(C0h � ������������ �������� ��� ���������� ����������)
	//00411443  sub         esp,0C0h 

	// ��������� ��������� � ����
	//00411449  push        ebx  
	//0041144A  push        esi  
	//0041144B  push        edi  

	// �����-�� �������������
	//0041144C  lea         edi,[ebp-0C0h] 
	//00411452  mov         ecx,30h 
	//00411457  mov         eax,0CCCCCCCCh 
	//0041145C  rep stos    dword ptr es:[edi] 

	// ���������� ��������
	//0041145E  mov         eax,dword ptr [a] 
	//00411461  add         eax,dword ptr [b] 
	return a + static_cast<int>(b);

	// ������� �������� �� ����� 
	// � �������� ������� 
	//00411464  pop         edi  
	//00411465  pop         esi  
	//00411466  pop         ebx  

	// ���������� ��������� ����� �� �����,
	// ������� ������� ���� � �������
	//00411467  mov         esp,ebp
	//00411469  pop         ebp  
	
	// ���� ����� ������ ����� ������
	//0041146A  ret   
}


// ���������� �������� � WINAPI __stdcall
// ��������� ���������� ����� ����, ������ ������. 
// ������� ����� ���������� ���������� �������.
int stdcall stdcall_func(int a, long* b)
{
	// ��������� � ���� �������� �������� ����
	//  push        ebp  

	//���������� ������ ������� ������� �����
	//  mov         ebp,esp 
	
	//��������� ��������� ����� ��� ��������� ���������� 
	//(C0h � ������������ �������� ��� ���������� ����������)
	//  sub         esp,0C0h 

	// ��������� ��������� � ����
	//  push        ebx  
	//  push        esi  
	//  push        edi  

	// �����-�� �������������
	//  lea         edi,[ebp-0C0h] 
	//  mov         ecx,30h 
	//  mov         eax,0CCCCCCCCh 
	//  rep stos    dword ptr es:[edi] 

	// ����������
	//	mov         eax,dword ptr [b] 
	//	mov         ecx,dword ptr [a] 
	//	mov         dword ptr [eax],ecx 
	*b = static_cast<long>(a);

	return 0;
	//  xor         eax,eax 

	// ������� �������� �� ����� 
	// � �������� ������� 
	//	pop         edi  
	//	pop         esi  
	//	pop         ebx  
	
	// ���������� ��������� ����� �� �����,
	// ������� ������� ���� � �������
	//	mov         esp,ebp 
	//	pop         ebp  

	// ��� ������ ����������� 8 ���� ��� ��� ������� ��� ����
	//	ret         8  
}


// "�������" ���������� �� ������ - 
// �������� ���������� ����� �������� ������ �����,
// ����� ������� � eax, edx, ecx �, ���� ���������� ������ ���, � �����.
// ��������� ����� �� �������� �������� ���������� �������.
int fastcall fastcall_func(char a, short b)
{
	return a%b;
}


// ���� ���� ������� ���������� �������
// ����� ���������� ���������� ����� ����������
// 0 - ������������ ������ ����������
// The Microsoft Visual Studio C/C++ compiler resolves this conflict 
// by silently converting the calling convention to __cdecl
int cdecl var_param_func(int a, ...)
{
	va_list argptr;
	va_start(argptr, a);
	int i = a;
	int sum = 0;

	while( i != 0 )
	{
		sum += i;
		i = va_arg( argptr, int);
	}
	va_end( argptr );
	return sum;
}


// ������ �������� � ���������� �� ����
// ��� �������� ������������� ����� ����������
double f_var(double n, ...)    //--��������� � ���������� ������ ����������
{
	double *p = &n;        //--������������ �� ������ ������ ����������
	double sum = 0, count = 0;    
	while (*p)            //--���� �������� �� ����� ����
	{ 
		sum+=(*p);            //--��������� ��������
		p++;                  //--������������� �� ��������� ��������
		count++;              //--�������  ���������� ����������
	}
	return ((sum)?sum/count:0);    //--��������� �������
}


// �������� ����������� ����� ����������
double f_var_pointers(int n, ...)
{  
	int *p = &n;        //--������������ ��������� �� ���������� ����������
	p++;                //--�������� ������ ����������-����������

	union Pointer 
	{
		double **pp;
		double *kp; 
	};    //--��������� ����������

	Pointer A = {0};
	A.kp = (double *)p;           //-����������� �����������
	double sum = 0, count = n;
	for ( ; n-- ; A.pp++ )			//--�������� ������� ���������!
		sum += (**A.pp);         //--������� ������!
	return ((sum)?sum/count:0);
}

// ------------------------------ 
// ��� �������� ���������� �� _�������������_ ������ 
// ������ ������ ������������� ������������ ��������
// �� ��������� ��������������� ��������� ��� ������
// �������� ���� �.�. �������������!
void update(float& f)
{
	f += 1.0f;
}

void show_unconst_ref()
{
	float f = 1.0f;
	double d = 2.0;
	//update(2.0f); // ������ - ����������� ��������, rvalue
	update(f);
	//update(d); // ������ - ������� ��������������
}


// ------------------------------ 
// ��������� �� �������
void error_(const std::string& s){ std::cerr << s << std::endl; }

void show_func_pointer()
{
	// ������� ��������� �� �������
	void (*p_func)(const std::string&) = 0;

	p_func = error_;	// ���������� ������� ��������� - ���������
	p_func = &error_;	// ��� ���� ���������

	p_func("error");	// ���������� �����
	(*p_func)("error");	// ���� ����������

	// ����������� ������ ���� 'err_func_t' ��� ���������� ������
	typedef void (*err_func_t)(const std::string&);

	err_func_t p = p_func;
	p("more errors");
}

// ------------------------------ 
// ������ ������������
// ������������ ��� ������ ������ ������� ������������
// � ������ ��������, �������� 'show'
#define CONCAT_FUNC_NAME(pref,name) pref##_##name()
void show_macro()
{
	CONCAT_FUNC_NAME(show, unconst_ref);	// show_unconst_ref()
	CONCAT_FUNC_NAME(show, func_pointer);	// show_func_pointer()
	//CONCAT_FUNC_NAME(show, macro); // - � ��� ����������� �������� � ������������� �����
}

// ----------------------------------
int main()
{
	int a = 109;
	long b = 1090;
	char aa = 56;
	short bb  = 10;

	int c = cdecl_func(a, b);
	//mov         eax,dword ptr [b] 
	//push        eax						// �������� � ���� �������� b
	//mov         ecx,dword ptr [a] 
	//push        ecx						// �������� � ���� �������� a
	//call        cdecl_func (41102Dh)		// ����� �������
	//add         esp,8						// �������� ���� (8 = sizeof(int)*2)
	//mov         dword ptr [c],eax			// ������������ ��������

	int d = stdcall_func(a, &b);
	//lea         eax,[b]					
	//push        eax						// �������� � ���� �������� �� ������ b
	//mov         ecx,dword ptr [a] 
	//push        ecx						// �������� � ���� �������� �
	//call        stdcall_func (4110B9h)	// ����� �������
	//mov         dword ptr [d],eax			// ����������� ���������� ��� ������� �����!


	int e = fastcall_func(aa, bb);
	//mov         dx,word ptr [bb]		// ������ ��� �������� ����� � ��������
	//mov         cl,byte ptr [aa] 
	//call        fastcall_func (411019h) 
	//mov         dword ptr [e],eax 

	int sm = var_param_func(1,2,3,4,-1);

	double da = 1.0, db = 2.0, dc = 3.0;
	double avg = f_var_pointers(3, &da, &db, &dc);

	show_macro();

	return 0;
}
