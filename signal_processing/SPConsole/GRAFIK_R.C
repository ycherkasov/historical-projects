#include <stdio.h>
#include <math.h>

#define n 1000
 #define deltat 0.001 // znachenie deltat
 //#define deltat 9.0497737556561085972850678733032e-5 // 1/11050 znachenie deltat dlya zvukovogo faila
 #define Pi 3.14159265 // chislo "Pi"
 #define m 128 // kolichestvo vesov
#define NULL 0

 //***************************

 
 //***************************
 // massX1[n]   - massiv znacheniy dlya funkcii vvedennyh/schitannyh/zapisyvaemyh dannyh
 // massX2[n]   - massiv znacheniy dlya RxxL/schitannye dannye*exp
 // massY1[n]   - massiv znacheniy funkcii, znacheniya kotoroy Random
 // massY2[n]   - massiv znacheniy dlya RxyL/svertka
 // massShum[n] - massiv znacheniy dlya shuma
 // massRef[n]  - massiv dlya vychisleniya Cf (deystvitel'naya chast')
 // massImf[n]  - massiv dlya vychisleniya Cf (mnimaya chast')
 // massCf[n]   - massiv znacheniy dlya spektra
 // massNorm[]  - massivy posle normirovki
 double massX1[n], massX2[n], massY1[n], massY2[n];
 double massShum[n];
 double massTemp[n];
 double massRef[n], massImf[n], massCf[n];
 double massNorm[n];
 double massbk[m], massbk1[m], massbk2[m], massBbk[2*m+1], massBbkGraph[n];
 double massFilt[n+m];


 int A0, A1, A2, A0y, A1y, A2y; // znacheniya A0, A1, A2
 int f0, f1, f2, f0y, f1y, f2y; // znacheniya f0, f1, f2
 double t; // znacheniye t
 double xt, xty; // znacheniye x(t)
 double deltaf; //*****
 int k, l; // peremennye dlya ciklov
 int temp;
 //int x1 = 20; 
 //int y1 = 240; // dopolnitel'niye peremenniye koordinat
 double Maxxx;

 //**************************
 char *name_of_file; // imya faila
 short flag = 0; // flazhok (0 - fail ne chitalsya, 1 - fail chitalsya)


 //**************************
 //preobrazovaniye Furie
 int the_furie(int kol, double mass[n])
 {
	double sumRef; //*****
	double sumImf; //*****

	// obnulyaem massivy
	// -----------------
	for(k = 0; k<n; k++)
	{
		massRef[k]  = 0;
		massImf[k]  = 0;
		massCf[k]   = 0;
		massNorm[k] = 0;
	}// ----------------

	for(l = 0; l<kol; l++)
	{
		sumRef = 0;
		sumImf = 0;
		for(k = 0; k<kol; k++)
		{
			sumRef = sumRef + (mass[k] * cos(2*Pi*l*k*deltat));
			sumImf = sumImf + (mass[k] * sin(2*Pi*l*k*deltat));		
		}
		massRef[l] = sumRef/n;
		massImf[l] = sumImf/n;
		massCf[l]  = sqrt(massRef[l]*massRef[l] + massImf[l]*massImf[l]);
		//printf("%d.%f\n", l, massCf[l]);
	}

	return 0;
 }


 //**************************
 // vychislitel'naya funkciya
 int the_calculation(void)
 {
	double SZx  = 0; // sredneye znacheniye po x
	double SZy  = 0; // sredneye znacheniye po y
	double SK   = 0; // sredniy kvadrat
	double SKZ  = 0; // sredne-kvadratichnoye znacheniye
	double D    = 0; // dispersiya
	double SKO  = 0; // sredne-kvadratichnoye otkloneniye
	double RxxL = 0; // avto-korrelyacionnaya funkciya
	double RxyL = 0; // vzaimno-korrelyacionnaya funkciya

	// obnulyaem massivy
	// -----------------
	for(k = 0; k<n; k++)
	{
		massX1[k]   = 0;
		massX2[k]   = 0;
		massY1[k]   = 0;
		massY2[k]   = 0;
		massNorm[k] = 0;
	}// ----------------

	for(k = 0; k<n; k++)
	{
		t  = k*deltat; // vychisleniye t = k * deltat

		//---------2 formula-------------------------
		xt   = (A0*sin(2*Pi*f0*t))+(A1*sin(2*Pi*f1*t))+(A2*sin(2*Pi*f2*t)); // vychisleniye x(t) po formule

		// dopolnitel'naya funkciya, znacheniya v kotoroy formiruyutsya Random
		srand ((unsigned)time(NULL));
		temp = rand()%100;
		A0y  = temp;
		//A0y  = 10;
		temp = rand()%100;
		A1y  = temp;
		//A1y  = 100;
		temp = rand()%100;
		A2y  = temp;
		//A2y  = 20;
		temp = rand()%250;
		f0y  = temp;
		//f0y  = 15;
		temp = rand()%250;
		f1y  = temp;
		//f1y  = 60;
		temp = rand()%250;
		f2y  = temp;
		//f2y  = 250;

		xty  = (A0y*sin(2*Pi*f0y*t))+(A1y*sin(2*Pi*f1y*t))+(A2y*sin(2*Pi*f2y*t));

		//-------------------------------------------
		// massX1 - massiv znacheniy funkcii po vvedennym znacheniyam
		// massY1 - massiv znacheniy funkcii, gde A i f vybirayutsya Random
		// formirovaniye znacheniy v massive
		massX1[k] = xt;

		massY1[k] = xty;
		//printf("x(%.3f) = %.7f\n", k*deltat, massX1[k]);
		//-------------------------------------------
	}

	//----------------------------------------------
	// vychisleniye harakteristik
	for(k = 0; k<n; k++)
	{
		SZx = SZx + massX1[k]; // narashivayetsya summa dlya SZx = x1+x2+x3+...
		SZy = SZy + massY1[k]; // narashivayetsya summa dlya SZy = y1+y2+y3+...(eto ta zhe funkciya, chto i vyshe, tol'ko s drugimi nachal'nymi znacheniyami A i f)
		SK  = SK  + massX1[k] * massX1[k]; // narashivayetsya summa dlya SK = x1^2+x2^2+x3^2+...
	}

	SZx   = (1/(double)n) * SZx; // poluchennaya summa SZx delitsya na n
	SZy   = (1/(double)n) * SZy; // poluchennaya summa SZy delitsya na n
	SK    = (1/(double)n) * SK; // poluchennaya summa SK delitsya na n
	SKZ   = sqrt(SK); // beretsya kvadratniy koren' iz SK (srednego kvadrata)

	for(k = 0; k<n; k++)
	{
		D  = D + (massX1[k]-SZx) * (massX1[k]-SZx); // narashivayetsya summa dlya D = (x1-SZx)^2+(x2-SZx)^2+(x3-SZx)^2+...
	}
	D     = (1/(double)n) * D; // poluchennaya summa D delitsya na n
	SKO   = sqrt(D); // beretsya kvadratniy koren' iz D (dispersii)

	///////////////////////////////////////////////////
	for(l = 0; l<n; l++)
	{
		RxxL = 0;
		RxyL = 0;
		for(k = 0; k<n; k++)
		{
			if((k+l)<n)
			{
				RxxL = RxxL + (massX1[k]-SZx) * (massX1[k+l]-SZx);
				RxyL = RxyL + (massX1[k]-SZx) * (massY1[k+l]-SZy);
			}
			else continue;
		}
		RxxL = (1/(double)n) * RxxL;
		massX2[l] = RxxL;
		RxyL = (1/(double)n) * RxyL;
		massY2[l] = RxyL;
		//printf("x(%.3f) = %.7f\n", k*deltat, massX2[k]);
		//printf("x(%.3f) = %.7f\n", k*deltat, massY2[k]);
	}
	///////////////////////////////////////////////////

	printf("=================================\n");
	// vyvod harakteristik na ekran
	printf("SZx  = %f\n", SZx); // Sredneye Znacheniye
	printf("SK   = %f\n", SK); // Sredniy Kvadrat
	printf("SKZ  = %f\n", SKZ); // Sredne-Kvadratichnoye Znacheniye
	printf("D    = %f\n", D); // Dispersiya
	printf("SKO  = %f\n", SKO); // Sredne-Kvadratichnoye Otkloneniye
	printf("=================================\n");
	//getch();
	//----------------------------------------------

	return 0;
 }


 //**************************
 //umnozhenie grafika na exponentu
 int the_exp_multiplication(double massIn[n], double massOut[n]) // kuda - otkuda
 {
	int alfa = 20;

	// obnulyaem massivy
	// -----------------
	for(k = 0; k<n; k++)
	{
		massIn[k]   = 0;
		massNorm[k] = 0;
	}// ----------------

	for(k = 0; k<n; k++)
	{
		massIn[k] = massOut[k] * exp(-1*alfa*k*deltat);
		//printf("x(%d) = %.7f\n", k, massIn[k]);
	}
	//getch();

	return 0;
 }


 //**************************
 //svertka
 int the_svertka(int kol1, int kol2, double massIn[], double massOut[], double massSvr[]) // kolichestvo - kuda - otkuda - s chem svorachivaem
 {
	int Svr = 0;

	// obnulyaem massivy
	// ----------------
	memset(massNorm, 0, n);
	memset(massIn, 0, k);

	for(l = 0; l<kol1+kol2; l++)
	{
		Svr = 0;
		for(k = 0; k<kol2; k++)
		{
			if((l-k)>=0)
			{
				Svr = Svr + (massSvr[k] * massOut[(l-k)]);
			}
			else continue;
		}
		massIn[l] = Svr;
		//printf("x(%d) = %.0f\n", l, massIn[l]);
	}
	//getch();

	return 0;
 }


 //**************************
 //funkciya plus shum
 int the_noise_plus(double massIn[n], double massOut[n]) //  kuda - otkuda
 {
	// obnulyaem massivy
	// -----------------
	for(k = 0; k<n; k++)
	{
		massIn[k]   = 0;
		massNorm[k] = 0;
	}// ----------------

	for(k = 0; k<n; k++)
	{
		massIn[k] = massOut[k] + (massShum[k]*1000);
	}
	
	return 0;
 }


 //**************************
 //procedurkaa, ubirayushaya smesheniya
 void the_no_offset()
 {
	double sr = 0;

	// vyschityvaem sredneye znacheniye funkcii ->
	for(k = 0; k<n; k++)
	{
		sr = sr + massNorm[k];
	}
	sr = sr/n;

	// -> i vychitayem ego iz kazhdogo znacheniya funkcii
	for(k = 0; k<n; k++)
	{
		massNorm[k] = massNorm[k] - sr;
	}

	// vychislyaem raznicu mezhdu smesheniyem i srednim znacheniem,
	// potom delim ego na 2, chtoby sdvinut' grafik funkcii po osi y:
	// znacheniye peremennoy temp bylo prisvoeno v procedurke 'the_offset()'
	// potom budet ispol'zovano v procedurke risovaniya grafika 'the_picture_read()'
	temp = ((sr-0.5) - temp)/2;
	printf("\nsr=%f", sr);
 }


 //**************************
 //procedurka, ubirayushaya spayki
 void the_no_hits()
 {
	for(k = 0; k<n; k++)
	{
		if((massNorm[k]<-100)||(massNorm[k]>100))
		{
			massNorm[k] = (massNorm[k-1] + massNorm[k+1])/2;
		}
	}
 }


 //**************************
 //funkciya plus smesheniye
 void the_offset()
 {
	int mov = -1000; // znachenie '-' peremennoy 'mov' oznachaet, chto smesheniye osushestvlyaetsya naverh

	// zapominaem znacheniye smesheniya, chtoby potom ispol'zovat' ego v procedure 'the_no_offset()'
	temp = mov;
	printf("\nmov=%d", mov);

	// osushestvlyaetsya smesheniye na 'mov' poziciy narisovannoy funkcii
	for(k = 0; k<n; k++)
	{
		massNorm[k] = massNorm[k] + mov; 
	}
 }


 //**************************
 //funkciya, plus spayki
 void the_hits()
 {
	// k narisovannoy funkcii v tochkah dobavlyayutsya vspleski
	massNorm[100] = -200;
	massNorm[300] = 150;
	massNorm[600] = -250;
 }


 //**************************
 //raschet vesovoy funkcii bk dlya fil'tra
 int the_filter_bk(int B, double massIn[m])
 {
	double d[4] = {0.35577019, 0.24369830, 0.07211497, 0.00630165};
	double fact, sum, sumg;

	memset(massIn, 0, m);

	fact       = 2 * (double)B * deltat;
	massIn[0]  = fact;
	fact       = fact * Pi;

	for(k = 1; k<m; k++)
	{

		double tmp1 = sin(fact * k);
		double tmp2 = (Pi * k);
		massIn[k] = tmp1/tmp2;
	}

	// trapeciya
	massIn[m-1] = massIn[m-1]/2; // [m] imeetsya vvidu poslednee m-toe znacheniye

	// primenyaem okno Pottera
	sumg = massIn[0];
	for(k = 1; k<m; k++)
	{
		sum  = d[0];
		fact = Pi * (k/m);
		for(l = 1; l<=3; l++)
		{
			sum = sum + 2 * d[l] * cos(fact*l);
		}
		massIn[k] = massIn[k] * sum; // neposredstvennoe sglazhivaniye
		sumg      = sumg + 2 * massIn[k];
	}

	for(k = 0; k<m; k++)
	{
		massIn[k] = massIn[k]/sumg;
	}
	
	return 0;
 }


 //**************************
 //procedurka formirovaniya massiva Bbk iz bk
 void bk_in_Bbk(double massOut[m])
 {
	// obnuleniye massiva
	memset(massBbk, 0, 2*m+1);

	// pervaya chast' massiva - perev`rnutiy massiv bk
	for(k = 0; k<=m; k++)
	{
		massBbk[k] = massOut[m-k];
	}
	// vtoraya chast' massiva - massiv bk
	l = 1;
	for(k = m+1; k<=(2*m); k++)
	{
		massBbk[k] = massOut[l];
		l++;
	}
 }


 //**************************
 //nerekursivniy fil'tr nizkih chastot
 int the_filter_N_CH(double massIn[m], double massOut[m]) // kuda - otkuda
 {
	memset(massIn, 0, m);

	for(k = 0; k<=m; k++)
	{
		massIn[k] = massOut[k];
	}

   return 0;
 }


 //**************************
 //nerekursivniy fil'tr vysokih chastot
 int the_filter_V_CH(double massIn[m], double massOut[m]) // kuda - otkuda
 {
	memset(massIn, 0, m);
	
	massIn[0] = 1 - massOut[0];
	for(k = 1; k<=m; k++)
	{
		massIn[k] = (-1) * massOut[k];
	}

   return 0;
 }
 
 
 //**************************
 //nerekursivniy polosovoy fil'tr 
 int the_filter_P(double massIn[m], double massOut1[m], double massOut2[m]) // kuda - otkuda
 {
	memset(massIn, 0, m);

	for(k = 0; k<=m; k++)
	{
		massIn[k] = massOut2[k] - massOut1[k];
	}

   return 0;
 }
 
 
 //**************************
 //nerekursivniy rezhektorniy fil'tr 
 int the_filter_R(double massIn[m], double massOut1[m], double massOut2[m]) // kuda - otkuda
 {
	memset(massIn, 0, m);

	massIn[0] = 1 + massOut1[0] - massOut2[0];
	for(k = 1; k<=m; k++)
	{
		massIn[k] = massOut1[k] - massOut2[k];
	}

   return 0;
 }


 //**************************
 //dopolnitel'naya procedurka, dobavlyayushyaya massiv Bbk do n elementov
 void mass_for_graph(double massIn[], double massOut[]) //  kuda - otkuda
 {
	// obnuleniye massiva
	memset(massIn, 0, n);

	// pervye elementy - elementy massiva Out
	for(k = 0; k<=(2*m); k++)
	{
		massIn[k] = massOut[k];
	}
	// ostal'nye elementy - nuli
	for(k = (2*m+1); k<n; k++)
	{
		massIn[k] = 0;
	}
 }


 //**************************
 //generator sluchaynyh chisel ot 0 do 1
 void the_generation()
 {
	double zn;
	int p;
	char chislo[4];
	char pch[1];
	int i;

	// obnulyaem massivy
	// -----------------
	for(k = 0; k<n; k++)
	{
		massShum[k] = 0;
		massNorm[k] = 0;
	}
	memset(chislo, 0, 4);
	// -----------------

	srand ((unsigned)time(NULL));
	p = rand()%9+1; // iznachal'no 'p' vybiraetsya Random [1,9]

	for(k = 0; k<n; k++)
	{
		// esli vdrug 'p' ravno 0 to krutim cykl, poka ne budut znacheniya iz promezhutka [1,9]
		while (p == 0)
		{
			p = rand()%9+1;
		}

		//printf("sin(%d)  ", p);
		zn = sin(p); // vychislyaem sin ot vhodyasego chisla 'p'
		if (zn<0)
		{
			zn = zn*-1; // znacheniye delaem polozhitel'nym
		}
		massShum[k] = zn; // zapominaem eto znacheniye v massive

		p = zn * 10000; // znachenie umnozhaem na 10000, chtoby rassmotret' 4 chisla chto byli posle zapyatoy
		itoa(p, chislo, 10); // preobrazuem chislo v stroku
		//printf("%.7f ", massShum[k]);
		i = rand()%4; // vybiraem 'i' Random
		//printf("i%d %c\n", i+1, chislo[i]);
		pch[0] = chislo[i]; // v stroku 'pch' (sostoyashuyu iz 1-go elementa) zapisyvaem simvol, stoyashiy na i-toy pozicii
		//printf("%c ", pch[0]);
		p = atoi(pch); // v 'p' zapisyvaem element stroki 'pch', odnovremenno perevodya ego v chislo
		//printf("%d\n", p);
	}
	//getch();
 }


 //**************************
 //normirovochnaya funkciya
 int the_normal(double mass[n], double massN[n], int Scale)
 {
	// iznachal'no maksimal'nym i odnovremenno minimal'nym elementom predpolagaem perviy element v strukture
	double XMax = mass[0];
	double XMin = mass[0];

	for(k = 0; k<n; k++)
	{
		// esli tekushiy element men'she minimal'nogo,
		// togda ego znacheniye zanosim v XMin
		if(mass[k]<XMin)
		{
			XMin = mass[k];
		}
		// esli tekushiy element bol'she maksimal'nogo,
		// togda ego znacheniye zanosim v XMax
		if(mass[k]>XMax)
		{
			XMax = mass[k];
		}
	}
	//printf("XMin = %.7f, XMax = %.7f\n", XMin, XMax);
	//getch();

	for(k = 0; k<n; k++)
	{
		massN[k] = ((mass[k] - XMin)/(XMax - XMin))*Scale;
		//printf("x(%d) = %.7f\n", k, massN[k]);
	}
	//getch();

	return 0;
 }


 //**************************
 //procedurka poiska maksimal'nogo znacheniya 'y'
 //(chtoby grafik vyvodilsa simmetrichno otnositel'no osi x)
 int find_max(double mass[n])
 {
	Maxxx = mass[0];

	for(k = 0; k<n; k++)
	{
		// esli tekushiy element bol'she maksimal'nogo,
		// togda ego znacheniye zanosim v Maxxx
		if(mass[k]*-1>Maxxx)
		{
			Maxxx = mass[k]*-1;
		}
	}
	//printf("Maxx = %f", Maxxx);
	//getch();

	return 0;
 }


 //**************************
 //procedurka dlya grafika (sama sinusoida)
 void the_grafik(int beg, double massN[n], int mv, int kol) // kol - kolichestvo tochek
 {
	//find_max(massN);
	//moveto(0+x1, massN[0]+y1+Maxxx/2+mv); // sdelat' tekushey tochkoy tochku so znacheniyami x i y nulevogo elementa v massive

	//for(k = 1; k<kol; k++)
	//{
	//	lineto(k+x1, massN[k+beg]+y1+Maxxx/2+mv); // provesti liniyu v tochku so znacheniyami x i y elementa struktury
	//}
 }


 //**************************
 //procedura risovaniya koordinatnyh osey i setki
 void the_koordinats()
 {
	srand ((unsigned)time(NULL));
	temp = rand()%15; // cvet fona random
	//setbkcolor(temp);
	//setbkcolor(WHITE);

	// koordinatnaya setka:
	//-------------------
	//setcolor(10);
	temp = 0;
	for(k = 0; k<70; k++)
	{
		temp = temp + 20;
		//line(x1, y1-temp, 640, y1-temp); // po osi x vverh
		//line(x1, y1+temp, 640, y1+temp); // po osi x vniz
		//line(x1+temp, 0, x1+temp, 480); // po osi y
	}
	//-------------------

	// koordinatnaya os':
	//-------------------
	//setcolor(4);
	//line(x1, y1, 640, y1); // os' x
	//line(x1, 0, x1, 480); // os' y
	//-------------------
 }


 //**************************
 //risovaniye grafika po vvedennym dannym
 void the_picture_enter()
 {
	//the_graph_on(); // podklyucheniye graficheskogo rezhima

	// na grafik nanosyatsya tochki,
	// sootvetstvuyushiye znacheniyam,
	// hranyashimsya v massive massNormX1 - eto po osi y, a po osi x prosto budut 1,2,3...

	//I---------------------------------------------------------
	the_koordinats();
	printf("  Poligarmonic process\n");

	// 1. grafik dlya iznachal'noy funkcii
	//setcolor(MAGENTA); // cvet grafika budet MAGENTA
	//line(x1, y1-150, 640, y1-150); // os' x
	printf("   --->Magenta-x(t)");
	the_normal(massX1, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -150, n); // n - kolichestvo tochek

	// 2. grafik dlya RxxL funkcii
	//setcolor(YELLOW); // cvet grafika budet YELLOW
	//line(x1, y1-50, 640, y1-50); // os' x
	printf("   --->Yellow-RxxL");
	the_normal(massX2, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -50, n); // n - kolichestvo tochek

	// 3. grafik dlya RxyL funkcii
	//setcolor(WHITE); // cvet grafika budet WHITE
	//line(x1, y1+50, 640, y1+50); // os' x
	printf("   --->White-RxyL");
	the_normal(massY2, massNorm, -100); // normirovka
	the_grafik(0, massNorm, +50, n); // n - kolichestvo tochek

	// 4. grafik preobrazovaniya Furie
	the_furie(n, massX1); // preobrazovanie Furie
	//setcolor(BLUE); // cvet grafika budet BLUE
	//line(x1, y1+170, 640, y1+170); // os' x
	printf("   --->Blue-Cf");
	the_normal(massCf, massNorm, -100); // normirovka
	the_grafik(0, massNorm, +170, n); // n - kolichestvo tochek
	//---------------------------------------------------------I

	getch();
	//cleardevice(); // ochistit' ekran

	//II---------------------------------------------------------
	the_koordinats();

	// 1. grafik shuma
	the_generation(); // generaciya shuma
	//line(x1, y1, 640, y1); // os' x
	//line(x1, 0, x1, 480); // os' y
	//setcolor(BLUE); // cvet grafika budet BLUE
	printf("   --->noise");
	the_normal(massShum, massNorm, -50); // normirovka
	the_grafik(0, massNorm, 0, n); // n - kolichestvo tochek
	//---------------------------------------------------------II

	getch();
	//closegraph();
 }


 //**************************
 //risovaniye grafika po dannym iz faila
 void the_picture_read()
 {
	//the_graph_on(); // podklyucheniye graficheskogo rezhima

	//I---------------------------------------------------------
	the_koordinats();
	//printf("  %s", name_of_file);

	// 1. grafik dlya funkcii, znacheniya kotoroy prochitany iz faila
	//setcolor(BLUE); // cvet grafika budet BLUE
	the_normal(massX1, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*6), n); // n - kolichestvo tochek

	// 2. grafik funkcii, umnozhenniy na exponentu
	//the_exp_multiplication(massX2, massX1); // kuda - otkuda
	//the_normal(massX2, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, -(30*2), n); // n - kolichestvo tochek

	// 3. grafik vspleskov
	//for(k = 0; k<n; k++)
	//{
	//	massTemp[k] = 0;
	//}
	//massTemp[100] = 40;
	//massTemp[200] = 70;
	//massTemp[350] = 20;
	//the_normal(massTemp, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*2, n); // n - kolichestvo tochek

	// 4. grafik svertki
	//the_svertka(n, n, massY2, massX2, massTemp);
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek

	// 5. grafik preobrazovaniya Furie
	the_furie(n, massX1); // preobrazovanie Furie
	the_normal(massCf, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*2), n); // n - kolichestvo tochek
	//---------------------------------------------------------I
	
	//getch();
	//cleardevice(); // ochistit' ekran
	//II---------------------------------------------------------
	//the_koordinats();

	//// 1. grafik svertki
	////setcolor(BLUE); // cvet grafika budet BLUE
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, -(30*6), n); // n - kolichestvo tochek

	//// 2. grafik funkcii(svertki), plus shum
	//the_generation(); // generaciya shuma
	//the_noise_plus(massY1, massY2);
	//the_normal(massY1, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, -(30*2), n); // n - kolichestvo tochek

	//// 3. grafik funkcii(svertki), plus smesheniye
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_offset();
	////the_grafik(0, massNorm, 30*2, n); // n - kolichestvo tochek
	//the_no_offset();
	//// znacheniye peremennoy temp bylo prisvoeno v procedurke 'the_no_offset()'
	//the_grafik(0, massNorm, 30*2+temp, n); // n - kolichestvo tochek

	//// 4. grafik funkcii(svertki), plus spayki
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_hits();
	////the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek
	//the_no_hits();
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek
	////---------------------------------------------------------II

	//getch();
	//cleardevice(); // ochistit' ekran

	//III---------------------------------------------------------
	//the_koordinats();

	//// 1.grafik iznachal'noy funkcii
	////setcolor(BLUE);
	//the_normal(massX1, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, -(30*6), n); // n - kolichestvo tochek

	////2. grafik preobrazovaniya Furie iznachal'noy funkcii
	//the_furie(n, massX1); // preobrazovanie Furie
	//the_normal(massCf, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, -(30*2), n); // n - kolichestvo tochek

	//// 3. grafik vesovoy funkcii
	//the_filter_bk(100, massbk1);
	//the_filter_N_CH(massbk, massbk1);
	//bk_in_Bbk(massbk);
	//mass_for_graph(massBbkGraph, massBbk);
	//the_normal(massBbkGraph, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*2, 2*m+1);

	//// 4. grafik preobrazovaniya Furie ot vesovoy funkcii
	//the_furie(n, massBbkGraph);
	//the_normal(massCf, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*2, n); // n - kolichestvo tochek

	//// 5. grafik funkcii posle filtracii
	//// TODO : m, m ?
	//the_svertka(m, m, massY2, massX1, massBbk); // kuda - otkuda - s chem svorachivaem
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek

	//// 6. grafik preobrazovaniya Furie otfil'trovannoy funkcii
	//the_furie(n, massY2); // preobrazovanie Furie
	//the_normal(massCf, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek
	//---------------------------------------------------------III
	
	//getch();
	//closegraph();
 }


 //**************************
 //risovaniye grafika posle primeneniya fil'tra

 void the_picture_after_filter()
 {
	//the_graph_on(); // podklyucheniye graficheskogo rezhima

	//I---------------------------------------------------------
	the_koordinats();


	// 1. grafik vesovoy funkcii
	//setcolor(BLUE);
	bk_in_Bbk(massbk);
	mass_for_graph(massBbkGraph, massBbk);
	the_normal(massBbkGraph, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*6), 2*m+1); // 2*m+1 - kolichestvo tochek

	// 2. grafik preobrazovaniya Furie ot vesovoy funkcii
	the_furie(2*m+1, massBbk);
	the_normal(massCf, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*2), 2*m+1); // 2*m+1 - kolichestvo tochek

	// 5. grafik funkcii posle filtracii
	//the_svertka(n, 2*m+1, massFilt, massX1, massBbk);
	//the_svertka_filter(massFilt, massX1, massBbk);
	//the_normal(massFilt, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek
	//the_svertka(m, massY2, massX1, massBbk); // kuda - otkuda - s chem svorachivaem
	//the_normal(massY2, massNorm, -100); // normirovka
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek

	// 6. grafik preobrazovaniya Furie otfil'trovannoy funkcii
	//the_furie(n, massFilt); // preobrazovanie Furie
	//the_normal(massCf, massNorm, -100); // normirovka
	//setcolor(9);
	//the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek

	//---------------------------------------------------------I

	getch();
	//cleardevice(); // ochistit' ekran

	//II---------------------------------------------------------
	the_koordinats();

	// 1.grafik iznachal'noy funkcii
	//setcolor(BLUE);
	the_normal(massX1, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*6), n); // n - kolichestvo tochek

	//2. grafik preobrazovaniya Furie iznachal'noy funkcii
	the_furie(n, massX1); // preobrazovanie Furie
	the_normal(massCf, massNorm, -100); // normirovka
	the_grafik(0, massNorm, -(30*2), n); // n - kolichestvo tochek

	// 3. grafik funkcii posle filtracii
	the_svertka(n, m, massFilt, massX1, massBbk);
	the_normal(massFilt, massNorm, -100); // normirovka
	the_grafik(m, massNorm, 30*2, n); // n - kolichestvo tochek

	// 4. grafik preobrazovaniya Furie otfil'trovannoy funkcii
	the_furie(n, massFilt); // preobrazovanie Furie
	the_normal(massCf, massNorm, -100); // normirovka
	the_grafik(0, massNorm, 30*6, n); // n - kolichestvo tochek
	//---------------------------------------------------------II

	getch();
	//closegraph();
 }


 //**************************
 //procedurka schityvaniya iz faila
 void the_read()
 {
	short int mass[n];

	FILE * out; // ukazatel' na FILE

	// massX1 - massiv znacheniy funkcii po schitannym znacheniyam

	// obnulyaem massiv
	memset(mass, 0, n);

	if((out = fopen(name_of_file, "rb")) == NULL)
	{
		printf("File could not be opened!");
		getch();
		exit(1);
	}
	else
	{
		// chityvaem iz faila 'n' elementov, razmerom 'short int' (t.e. 2 baita)
		if(fread(mass, sizeof(short int), n, out) != n)
		{
			if(feof(out))
			{
				printf("End of file!");
				getch();
			}
			else
			{
				printf("Error of reading file!");
				getch();
			}
		}
		// prisvaivaem elementam massiva 'massX1' znacheniya elementov massiva 'mass', perevodya ih v 'double'
		for(k = 0; k<n; k++)
		{
			massX1[k] = (double)mass[k];
			//printf("%f\n", massX1[k]);
		}
		//getch();

		fclose(out);
	}
 }


 //**************************
 //dialog chteniya dannyh iz faila
 void read_from_file()
 {
	char cho;

	for(;;)
	{
		//clrscr(); // libo mozhno napisat' "system("cls");", chto budet to zhe samoe
		printf("\n******* read from file *******\n\n");
		printf("--------------choose process:\n\n");
		printf("   ---> Garmonic     - g\n");
		printf("   ---> PolyGarmonic - p\n");
		printf("   ---> Sound        - s\n");
		printf("   ---> Image        - i\n");
		printf("   ---> Quit         - q\n");
		cho = getchar();

		switch(cho)
		{
			case 'g':
			{
				// garmonicheskiy process
				name_of_file = "..\\files\\gp55_1ms.dat";
				flag = 1; // menyaem znachenie flazhka
				return;
			}
			case
			'p':
			{
				// poligarmonicheskiy process
				name_of_file = "..\\files\\pgp5_55_250_1ms.dat";
				flag = 1; // menyaem znachenie flazhka
				return;
			}
			case
			's':
			{
				// zvuk
				//name_of_file = "rama.dat";
				//name_of_file = "no_003.dat";
				name_of_file = "..\\files\\ramaOrig.DAT";
				flag = 1; // menyaem znachenie flazhka
				return;
			}
			case
			'i':
			{
				// izobrazhenie
				name_of_file = "Hollywood.img";
				flag = 1; // menyaem znachenie flazhka
				return;
			}
			case 'q':
			{
			  return;
			}
		}
	}
 }


 //**************************
 //dialog vybora fil'tra
 void choice_filters()
 {
	char cho;
	int B1, B2;

	for(;;)
	{
		//clrscr(); // libo mozhno napisat' "system("cls");", chto budet to zhe samoe
		printf("\n******* choice_filters *******\n\n");
		printf("--------------choose filter:\n\n");
		printf("     low frequencies  - l\n");
		printf("     high frequencies - h\n");
		printf("     polosovoy        - p\n");
		printf("     rezhektorniy     - r\n");
		printf("     quit             - q\n");
		cho = getchar();

		switch(cho)
		{
			case 'l':
			{
				// fil'tr nizkih chastot
				printf("          -----> B:  ");
				scanf("%i", &B1);
				printf("\n");
				the_filter_bk(B1, massbk1); // schitaem vesovuyu funkciyu
				the_filter_N_CH(massbk, massbk1);
				the_picture_after_filter(); // grafik posle fil'tracii
			} break;
			case 'h':
			{
				// fil'tr vysokih chastot
				printf("          -----> B:  ");
				scanf("%i", &B1);
				printf("\n");
				the_filter_bk(B1, massbk1); // schitaem vesovuyu funkciyu
				the_filter_V_CH(massbk, massbk1);
				the_picture_after_filter(); // grafik posle fil'tracii
			} break;
			case 'p':
			{
				// polosovoy fil'tr
				printf("          -----> B1:  ");
				scanf("%i", &B1);
				printf("\n");
				printf("          -----> B2:  ");
				scanf("%i", &B2);
				printf("\n");
				the_filter_bk(B1, massbk1); // schitaem vesovuyu funkciyu 1
				the_filter_bk(B2, massbk2); // schitaem vesovuyu funkciyu 2
				the_filter_P(massbk, massbk1, massbk2);
				the_picture_after_filter(); // grafik posle fil'tracii
			} break;
			case 'r':
			{
				// rezhektorniy fil'tr
				printf("          -----> B1:  ");
				scanf("%i", &B1);
				printf("\n");
				printf("          -----> B2:  ");
				scanf("%i", &B2);
				printf("\n");
				the_filter_bk(B1, massbk1); // schitaem vesovuyu funkciyu 1
				the_filter_bk(B2, massbk2); // schitaem vesovuyu funkciyu 2
				the_filter_R(massbk, massbk1, massbk2);
				the_picture_after_filter(); // grafik posle fil'tracii
			} break;
			case 'q':
			{
			  return;
			}
		}
	}
 }


 //**************************
 //dialog vvedeniya dannyh
 void enter_data()
 {
	// poocheredniy vvod peremennyh
	printf("\n******* enter data *******\n\n");
	printf("----------------values A:\n\n");
	printf("          -----> A0:  ");
	scanf("%i", &A0);
	printf("\n");
	printf("          -----> A1:  ");
	scanf("%i", &A1);
	printf("\n");
	printf("          -----> A2:  ");
	scanf("%i", &A2);
	printf("\n");
	printf("----------------values f:\n\n");
	printf("          -----> f0:  ");
	scanf("%i", &f0);
	printf("\n");
	printf("          -----> f1:  ");
	scanf("%i", &f1);
	printf("\n");
	printf("          -----> f2:  ");
	scanf("%i", &f2);
 }


 //**************************
 //zapis' v fail
 void write_to_file()
 {
	short int list;

	FILE * cfPtr; // ukazatel' na FILE

	if((cfPtr = fopen("pgp_1ms.dat", "wb")) == NULL)
	{
		printf("File could not be opened!\n");
		getch();
		exit(1);
	}
	else
	{
		for(k = 0; k<n; k++)
		{
			// perevod v 'short int' s okrugleniyem
			if (massX1[k]>0)    list = (short int)(massX1[k]+0.5);
			else
			if (massX1[k]<0)    list = (short int)(massX1[k]-0.5);
			else
			if (massX1[k] == 0) list = (short int)(massX1[k]);
			//printf("%d.%.4f-->%d\n", k, massX1[k], list);
			// zapisyvaem v fail peremennuyu 'list', vsego ih budet 'n', razmerom 'short int' - 2 baita
			fwrite(&list, sizeof(short int), 1, cfPtr);
		}
		//getch();
		fclose(cfPtr);
	}
 }


 //**************************
 //osnovnaya programma
 int main(void)
 {
	char ch;

	for(;;)
	{
		//clrscr(); // libo mozhno napisat' "system("cls");", chto budet to zhe samoe
		printf("********************************************************\n");
		printf("**FUNCTION***FUNCTION***FUNCTION***FUNCTION***FUNCTION**\n");
		printf("********************************************************\n\n");
		printf("->enter data     - d\n");
		printf("->read from file - r\n");
		printf("->exit           - e\n\n");
		ch = getchar();

		switch(ch)
		{
			case 'd':
			{
				enter_data(); // dialog s pol'zovatelem - vvedeniya dannyh
				the_calculation(); // raschetnaya procedurka
				getch();
				the_picture_enter(); // grafik po vvedennym dannym
				//write_to_file(); // zapis' v fail
				printf("\n");
			} break;
			case 'r':
			{
				read_from_file(); // dialog s pol'zovatelem - chteniya iz faila

				// esli fail deystvitel'no byl otkryt - prochitat' dannye i narisovat' grafik,
				if (flag != 0) // esli flazhok ne 0, fail byl otkryt
				{
					the_read(); // chtenie faila
					the_picture_read(); // grafik po prochitannym dannym
					choice_filters(); // vybor fil'tra
					flag = 0; // obnulyaem flazhok
				}
				else continue; // inache viyti
				printf("\n");
			} break;
			case 'e':
			{
				printf("\n******* exit *******\n\n");
				getch();
				exit(1);
			}
		}
	}
 }