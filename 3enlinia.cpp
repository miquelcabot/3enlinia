
		/* Joc de tres en retxa	per Miquel A. Cabot */

#include <dos.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

union REGS pregs;

int puntuacio[9]={6,1,8,7,5,3,2,9,4}; //matriu per calcular els punts
int seleccio[9]={0,0,0,0,0,0,0,0,0};  //per assenyalar una fitxa seleccionada
int posicio_jugadors[9]={0,0,0,0,0,0,0,0,0};

int cuadrant[9][2]={   //coordx,coordy
     180,100,320,100,460,100,180,240,320,240,460,240,180,380,320,380,460,380};

int posar,llevar;

struct nodo{
	int pos[9];
	int guanya;
	int llevar,posar;
	struct nodo *variant[9];
	struct nodo *ant;
	};

//****************** detectar si hi ha un guanyador *************************
//***************************************************************************
int hi_ha_guanyador(int matriu[9])
{
int i;
int punts1=0,punts2=0;
int n_fitxes1=0,n_fitxes2=0;

for (i=0;i<9;i++)
	{
	if (matriu[i]==1)	//calcula els punts del jugador
		{
		n_fitxes1++;
		punts1+=puntuacio[i];

		}
	if (matriu[i]==2)	//calcula els punts de l'ordenador
		{
		n_fitxes2++;
		punts2+=puntuacio[i];
		}
	}

if (punts1==15 && n_fitxes1==3)
	return 1;		//guanya el jugador
if (punts2==15 && n_fitxes2==3)
	return 2;		//guanya l'ordenador
return 0;		//no hi ha guanyador
}


//****************** Funci¢ pensar jugada de l'ordenador ********************
//***************************************************************************
class pensar_jugada
	{
	struct nodo *jugada;
	  public:
	pensar_jugada();
	};

pensar_jugada::pensar_jugada()
{
int i,j,k;

if((jugada=(struct nodo *)malloc(sizeof(struct nodo)))==NULL)
	{
	printf("NO HI HA MEMORIA");
	getch();
	exit(1);
	}
for(i=0;i<9;i++)
	jugada->pos[i]=posicio_jugadors[i];
jugada->ant=NULL;

int buit[3];
int fitxa_ord[3];
int n=0,m=0;

for (i=0;i<9;i++)
	{
	if (posicio_jugadors[i]==0)
		{
		buit[n]=i;
		n++;
		}
	else if (posicio_jugadors[i]==2)
		{
		fitxa_ord[m]=i;
		m++;
		}
	}

struct nodo *jugada_2[9];

for (i=0;i<9;i++)
	{
	jugada_2[i]=(struct nodo *)malloc(sizeof(struct nodo));
	jugada_2[i]->ant=jugada;
	for (j=0;j<9;j++)
		jugada_2[i]->pos[j]=jugada->pos[j];
	}

i=0;
for (n=0;n<3;n++)
	for (m=0;m<3;m++)
		{
		jugada_2[i]->pos[fitxa_ord[n]]=0;
		jugada_2[i]->pos[buit[m]]=2;
		jugada_2[i]->llevar=fitxa_ord[n];
		jugada_2[i]->posar=buit[m];
		i++;
		}

for(i=0;i<9;i++)
	jugada_2[i]->guanya=hi_ha_guanyador(jugada_2[i]->pos);

for (i=0;i<9;i++)
	jugada->variant[i]=jugada_2[i];

int ok=0;
for (i=0;i<9;i++)
	if (jugada_2[i]->guanya==2)
		{
		posar=jugada_2[i]->posar;
		llevar=jugada_2[i]->llevar;
		ok=1;
		}
if (ok) return;
if (!ok)
	for (k=0;k<9;k++)
		{
		int fitxa_jug[3];
		n=0,m=0;

		for (i=0;i<9;i++)
			{
			if (jugada_2[k]->pos[i]==0)
				{
				buit[n]=i;
				n++;
				}
			else if (jugada_2[k]->pos[i]==1)
				{
				fitxa_jug[m]=i;
				m++;
				}
			}

		struct nodo *jugada_3[9];

		for (i=0;i<9;i++)
			{
			jugada_3[i]=(struct nodo *)malloc(sizeof(struct nodo));
			jugada_3[i]->ant=jugada_2[k];
			for (j=0;j<9;j++)
				jugada_3[i]->pos[j]=jugada_2[k]->pos[j];
			}

		i=0;
		for (n=0;n<3;n++)
			for (m=0;m<3;m++)
				{
				jugada_3[i]->pos[fitxa_jug[n]]=0;
				jugada_3[i]->pos[buit[m]]=1;
				i++;
				}

		for(i=0;i<9;i++)
			jugada_3[i]->guanya=hi_ha_guanyador(jugada_3[i]->pos);

		for (i=0;i<9;i++)
			jugada_2[k]->variant[i]=jugada_3[i];

		int va_be=0;
		for (i=0;i<9;i++)
			if (jugada_3[i]->guanya!=1)
				va_be++;
		if (va_be==9)
			{
			posar=jugada_2[k]->posar;
			llevar=jugada_2[k]->llevar;
            return;
			}
		else 	{
			posar=-1;
			llevar=-1;
			}
		}
}

//**************************** Clase mouse***********************************
//***************************************************************************
class mouse
	{
      public:
	int x;
	int y;
	int der;
	int izq;
	int iniciar();
	void mostrar();
	void ocultar();
	void estat();
	} rata;

int mouse::iniciar()
{
pregs.x.ax=0x0000;
int86(0x33,&pregs,&pregs);
if (pregs.x.ax==0x0000) return 0;
else return 1;
}

void mouse::mostrar()
{
pregs.x.ax=0x0001;
int86(0x33,&pregs,&pregs);
}

void mouse::ocultar()
{
pregs.x.ax=0x0002;
int86(0x33,&pregs,&pregs);
}

void mouse::estat()
{
pregs.x.ax=0x0003;
int86(0x33,&pregs,&pregs);
x=pregs.x.cx;
y=pregs.x.dx;
if (pregs.x.bx==1) izq=1;
else izq=0;
if (pregs.x.bx==2) der=1;
else der=0;
}

//************************ clase tauler *************************************
//***************************************************************************
class tauler{
      public:
	void dib_tauler();
	void dib_cercle(int pos);
	void dib_creu(int pos);
	} taula;

void tauler::dib_tauler()
{
cleardevice();

for (int i=0;i<3;i++)     //dibuixa les 4 l¡nies
	{
	line(250+i,30,250+i,450);
	line(250-i,30,250-i,450);
	line(390+i,30,390+i,450);
	line(390-i,30,390-i,450);
	line(110,170+i,530,170+i);
	line(110,170-i,530,170-i);
	line(110,310+i,530,310+i);
	line(110,310-i,530,310-i);
	}

for (i=0;i<9;i++)	//dibuixa les fitxes
	{
	if (posicio_jugadors[i]==1) dib_cercle(i);
	if (posicio_jugadors[i]==2) dib_creu(i);
	}
}

void tauler::dib_cercle(int pos)
{
rata.ocultar();
if (seleccio[pos]==1) setcolor(12);
for (int i=0;i<4;i++)
	circle(cuadrant[pos][0],cuadrant[pos][1],40+i);
setcolor(15);
rata.mostrar();
}

void tauler::dib_creu(int pos)
{
rata.ocultar();
if (seleccio[pos]==1) setcolor(12);
for (int i=-2;i<2;i++)
	{
	line(cuadrant[pos][0]-40+i,cuadrant[pos][1]-40,
		cuadrant[pos][0]+40+i,cuadrant[pos][1]+40);
	line(cuadrant[pos][0]+40+i,cuadrant[pos][1]-40,
		cuadrant[pos][0]-40+i,cuadrant[pos][1]+40);
	}
setcolor(15);
rata.mostrar();
}

//**************************** funci¢ errada ********************************
//***************************************************************************
void errada()
{
setcolor(0);
outtextxy(10,10,"ÛÛÛÛÛÛÛÛÛÛÛ");
setcolor(15);
settextstyle(1,0,2);
outtextxy(10,10,"No val!");
sound(900);
delay(400);
nosound();
delay(500);
setcolor(0);
outtextxy(10,10,"ÛÛÛÛÛÛÛÛÛÛÛ");
setcolor(15);
}

//***************** funcio de moviment del jugador **************************
//***************************************************************************
void mov_jug()
{
int ok;
int i,j;
int n_fitxes=0;

settextstyle(1,0,2);
outtextxy(10,10,"Mous t£");

for (int n=0;n<9;n++)
	if (posicio_jugadors[n]==1) n_fitxes++;

if (n_fitxes<3)
	do	{            	//colocar la fitxa a un lloc buit
		do 	{
			rata.estat();
			}while (!rata.izq);
		for (i=0;i<9;i++)
			if (rata.x>(cuadrant[i][0]-50)&& rata.x<(cuadrant[i][0]+50) && rata.y>(cuadrant[i][1]-50) && rata.y<(cuadrant[i][1]+50))
				break;
		if (i>=0 && i<=8 && posicio_jugadors[i]==0) ok=1;
		else 	{
			ok=0;
			errada();
			}
		}while (!ok);

else if (n_fitxes==3)
	{
	do	{		//seleccionar la fitxa a moure
		do	{
			rata.estat();
			}while(!rata.izq);
		for (j=0;j<9;j++)
			if (rata.x>(cuadrant[j][0]-50) && rata.x<(cuadrant[j][0]+50) && rata.y>(cuadrant[j][1]-50) && rata.y<(cuadrant[j][1]+50))
				break;
		if (j>=0 && j<=8 && posicio_jugadors[j]==1)
			{
			ok=1;
			seleccio[j]=1;
			taula.dib_tauler();
			}
		else 	{
			ok=0;
			errada();
			}
		}while (!ok);

	delay(200);

	do 	{               //seleccionar la posici¢ on es mou la fitxa
		do	{
			rata.estat();
			}while(!rata.izq);
		for (i=0;i<9;i++)
			if (rata.x>(cuadrant[i][0]-50) && rata.x<(cuadrant[i][0]+50) && rata.y>(cuadrant[i][1]-50) && rata.y<(cuadrant[i][1]+50))
				break;
		if (i>=0 && i<=8 && posicio_jugadors[i]==0)
			{
			ok=1;
			seleccio[j]=0;
			}
		else 	{
			ok=0;
			errada();
			}
		}while (!ok);
	posicio_jugadors[j]=0;
	}

posicio_jugadors[i]=1;
}

//****************** funcio de moviment de l'ordenador **********************
//***************************************************************************
void mov_ord()
{
int i,j;
int n_fitxes=0;
int punts_jug=0,punts_ord=0;

settextstyle(1,0,2);
outtextxy(10,10,"Pensant...");
rata.ocultar();

for (int n=0;n<9;n++)
	if (posicio_jugadors[n]==2) n_fitxes++;

if (n_fitxes==0)
	{ 			//si al mitg est… buit, posar-hi la fitxa
	if (posicio_jugadors[4]!=1) i=4;
	else do	{		//si no, aleatoriament
		randomize();
		i=random(9);
		}while (posicio_jugadors[i]!=0);
	delay(500);
	}

else if (n_fitxes==1)
	{          		//tapar el possible 3 en retxa del jugador
	for (i=0;i<9;i++)
		if (posicio_jugadors[i]==1) punts_jug+=puntuacio[i];
	for (i=0;i<9;i++)
		if (puntuacio[i]==(15-punts_jug)) break;
	if (posicio_jugadors[i]!=0 || i<0 || i>8)   //si no, aleatoriament
		do	{
			randomize();
			i=random(9);
			}while (posicio_jugadors[i]!=0);
	delay(500);
	}

else if (n_fitxes==2)
	{
	for (i=0;i<9;i++)	//cercar si es pot fer 3 en retxa
		if (posicio_jugadors[i]==2) punts_ord+=puntuacio[i];
	for (i=0;i<9;i++)
		if (puntuacio[i]==(15-punts_ord)) break;
	if (posicio_jugadors[i]!=0 || i<0 || i>8)
		{		//si no, tapar el del jugador
		j=0;
		int fitxa[3];
		for (i=0;i<9;i++)
			if (posicio_jugadors[i]==1)
				{
				fitxa[j]=i;
				j++;
				}
		for (i=0;i<9;i++)
			if (puntuacio[i]==((15-puntuacio[fitxa[0]])-puntuacio[fitxa[1]])) break;
		if (i<0 || i>8 || posicio_jugadors[i]!=0)
			for (i=0;i<9;i++)
				if (puntuacio[i]==((15-puntuacio[fitxa[1]])-puntuacio[fitxa[2]])) break;
		if (i<0 || i>8 || posicio_jugadors[i]!=0)
			for (i=0;i<9;i++)
				if (puntuacio[i]==((15-puntuacio[fitxa[0]])-puntuacio[fitxa[2]])) break;

		if (i<0 || i>8 || posicio_jugadors[i]!=0)
			do	{	//si no, aleatoriament
				randomize();
				i=random(9);
				}while (posicio_jugadors[i]!=0);
		}
	}

else if(n_fitxes==3)
	{
	pensar_jugada();
	if (llevar==-1)
		{
		do	{
			randomize();
			j=random(9);
			}while (posicio_jugadors[j]!=2);
		seleccio[j]=1;
		taula.dib_tauler();
		delay(500);
		do	{
			randomize();
			i=random(9);
			}while (posicio_jugadors[i]!=0);
		seleccio[j]=0;
		posicio_jugadors[j]=0;
		}
	else  	{
		seleccio[llevar]=1;
		taula.dib_tauler();
		delay(500);
		seleccio[llevar]=0;
		posicio_jugadors[llevar]=0;
		i=posar;
		}
	}

posicio_jugadors[i]=2;
rata.mostrar();
}

//*********************** funci¢ final **************************************
//***************************************************************************
void final(int guanyador)
{
taula.dib_tauler();
gotoxy(1,1);
if (guanyador==0) printf("No ha guanyat ning£\n");
if (guanyador==1)
	{
	printf("Enhorabona, has guanyat\n");
	for(int j=0;j<4;j++)
		for (int i=10000;i>200;i-=200)
			{
			sound(i);
			delay(15);
			}
	nosound();
	}
if (guanyador==2)
	{
	printf("Has perdut, pardillo!!\n");
	for(int j=0;j<20;j++)
		for (int i=200;i<1000;i+=100)
			{
			sound(i);
			delay(10);
		}
	nosound();
	}

printf("Pitja una tecla per seguir...");
getch();
}

//****************************** MAIN  **************************************
//***************************************************************************
void main()
{
if(!rata.iniciar())		//inicialitzar rata
	{
	printf("Rata no disponible");
	getch();
	exit(1);
	}

int gdriver = DETECT, gmode=VGAHI, errorcode;	//iniciar gr…fics
initgraph(&gdriver, &gmode, "c:\\tc\\bgi");
errorcode = graphresult();
if (errorcode != grOk)
	{
	printf("Graphics error: %s\n", grapherrormsg(errorcode));
	printf("Press any key to halt:");
	getch();
	exit(1);
	}

settextstyle(1,0,5);
setcolor(5);
outtextxy(100,100,"Joc de tres en retxa");
settextstyle(1,0,3);
setcolor(15);
outtextxy(200,300,"Per Miquel A. Cabot");
getch();

int pos;     //posici¢ del cuadrant(0,1,2,..)
int jugador;
int seguir;

do	{
	for (int i=0;i<9;i++)
		posicio_jugadors[i]=0;
	do      {
		taula.dib_tauler();
		rata.mostrar();
		if (jugador==1)
			{
			mov_jug();
			delay(200);
			}

		if (jugador==2)
			{
			mov_ord();
			delay(200);
			}

		if (jugador==1) jugador=2;
		else jugador=1;
		rata.estat();
		}while(!hi_ha_guanyador(posicio_jugadors));

	if (hi_ha_guanyador(posicio_jugadors)==1) final(1);
	if (hi_ha_guanyador(posicio_jugadors)==2) final(2);
	rata.ocultar();
	cleardevice();
	settextstyle(1,0,3);
	setcolor(15);
	outtextxy(200,300,"Vols seguir (s/n)?");
	seguir=getch();
	}while(seguir=='s' || seguir=='S');

closegraph();
exit (1);
}