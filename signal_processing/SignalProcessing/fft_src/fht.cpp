#include "stdafx.h"
#include <cmath>
#include "general.h"

real SineTable[80];

int Log2(ulong Num) {
	int x=-1;
	if (Num==0) return 0;
	while (Num) {x++;Num/=2;}
	return x;
}

#define INIT_TRIG(LENGTH)        	\
	ulong x=Log2(LENGTH);		\
	real Sin0=SineTable[x];		\
	real Cos0=SineTable[x+1];	\
	Cos0=-2.0*Cos0*Cos0;		\
	real Sin=Sin0,Cos=1.0+Cos0;	

#define NEXT_TRIG_POW {				\
	real temp=Cos;                		\
	Cos = Cos*Cos0 - Sin*Sin0 + Cos;	\
	Sin = Sin*Cos0 + temp*Sin0 + Sin;   	\
}


#define FHT_F2Butterfly(N1,N2,C,S) {				\
	real D1,D2;                            			\
	ulong i1=N1, i2=N2;                       		\
	D1=Left[i1];D2=Left[i2];                		\
	{							\
		real temp;                             		\
		Left[i1] =D1+(temp=Right[i1]);D1=D1-temp;	\
		Left[i2] =D2+(temp=Right[i2]);D2=D2-temp;	\
	}                                       		\
	Right[i1]=D1*(C)+D2*(S);				\
	Right[i2]=D1*(S)-D2*(C);                		\
}

void FHT_F(real *Data,ulong Len) {

	if (Len==4) {
		real d0=Data[0]; real d1=Data[1];
		real d2=Data[2]; real d3=Data[3];
		{
			real d02=d0+d2; real d13=d1+d3;
			Data[0]=d02+d13; Data[1]=d02-d13;
		}
		{
			real d02=d0-d2; real d13=d1-d3;
			Data[2]=d02+d13; Data[3]=d02-d13;
		}
		return;
	}

	Len/=2;
	ulong Len2,Len4;
	real *Left = &Data[0], *Right = &Data[Len];

	{
		real t1,t2;
		t1=Left[0];t2=Right[0];
		Left[0]=t1+t2;Right[0]=t1-t2;

		t1=Left[Len/2];t2=Right[Len/2];
		Left[Len/2]=t1+t2;Right[Len/2]=t1-t2;
	}


	INIT_TRIG(Len);

	Len2=Len/2;
	Len4=Len/4;
	for (x=1;x<Len4;x++) {
		FHT_F2Butterfly(x,Len-x,Cos,Sin);
		FHT_F2Butterfly(Len2-x,Len2+x,Sin,Cos);
		NEXT_TRIG_POW;
	}

	FHT_F2Butterfly(Len4,Len-Len4,CONST_SQRT_2,CONST_SQRT_2);

	FHT_F(Left, Len);
	FHT_F(Right,Len);
}


#define FHT_T2Butterfly(N1,N2,C,S) {	\
	real Rx,Ri;			\
	ulong i1=N1,i2=N2;		\
	Rx=Right[i1];Ri=Right[i2];     	\
	{				\
		real cas1,Lx;		\
		cas1=Rx*(C)+Ri*(S);	\
		Lx=Left[i1];           	\
		Left[i1]  = Lx+cas1;    \
		Right[i1] = Lx-cas1;    \
	}                               \
	{				\
		real cas2,Li;         \
		cas2=Rx*(S)-Ri*(C);     \
		Li=Left[i2];            \
		Left[i2]  = Li+cas2;    \
		Right[i2] = Li-cas2;    \
	}                              	\
}

#define FHT_T1Butterfly(N1,N2,C,S) {       	\
	ulong i1=N1,i2=N2;                        \
	real cas1=Right[i1]*(C)+Right[i2]*(S); 	\
	real temp=Left[i1];                    	\
	Left[i1] = temp + cas1;                 \
	Right[i2]= temp - cas1;                 \
}


void FHT_T(real *Data,ulong Len) {
	if (Len==8) {
		real d45,d67,sd0123,dd0123;
		{
			real ss0123,ds0123,ss4567,ds4567;
			{
				real s01,s23,d01,d23;
				d01 = Data[0] - Data[1];
				s01 = Data[0] + Data[1];
				d23 = Data[2] - Data[3];
				s23 = Data[2] + Data[3];
				ds0123 = (s01 - s23);
				ss0123 = (s01 + s23);
				dd0123 = (d01 - d23);
				sd0123 = (d01 + d23);
			}
			{
				real s45,s67;
				s45 = Data[4] + Data[5];
				s67 = Data[6] + Data[7];
				d45 = Data[4] - Data[5];
				d67 = Data[6] - Data[7];
				ds4567 = (s45 - s67);
				ss4567 = (s45 + s67);
			}				
			Data[4] = ss0123 - ss4567;
			Data[0] = ss0123 + ss4567;
			Data[6] = ds0123 - ds4567;
			Data[2] = ds0123 + ds4567;
		}
		d45 *= CONST_SQRT2;
		d67 *= CONST_SQRT2;
		Data[5] = sd0123 - d45;
		Data[1] = sd0123 + d45;
		Data[7] = dd0123 - d67;
		Data[3] = dd0123 + d67;
		return;
	}
	
	ulong Len2,Len4;
	real *Left,*Right;
	Len/=2;                         

	Right=&Data[Len];Left=&Data[0];  
	FHT_T(&Left[0], Len);             
	FHT_T(&Right[0],Len);            

	INIT_TRIG(Len);                   
                                       
	Len2=Len/2;                      
	Len4=Len/4;			
	for (x=1;x<Len4;x++) {           
		FHT_T2Butterfly(x,Len-x,Cos,Sin);
		FHT_T2Butterfly(Len2-x,Len2+x,Sin,Cos);
		NEXT_TRIG_POW;            
	}

	FHT_T2Butterfly(Len4,Len-Len4,CONST_SQRT_2,CONST_SQRT_2);
	FHT_T1Butterfly(0,0,1.0,0.0);	
	FHT_T1Butterfly(Len2,Len2,0.0,1.0);
}



inline ulong rev_next(ulong r, ulong n) { 
	do {
		n = n >> 1;
		r = r^n;
	} while ( (r&n) == 0);
	return r;
}


// FFTReOrder для действительных векторов
void FHTReOrder(real *Data, ulong Len) {
	real temp;
	if (Len <= 2) return;
	ulong r=0;
	for ( ulong x=1; x<Len; x++) {
		r = rev_next(r, Len);
		if (r>x) { temp=Data[x]; Data[x]=Data[r]; Data[r]=temp; } 
	}
}


void CreateSineTable(ulong Len) {
	int x=0; ulong P=1;
	while (P<=Len*4)  {
		SineTable[x]=sin(CONST_PI/P);
		P*=2;
		x++;
	}
}
