#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void clrscr();
void gotoxy(int x, int y);
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);

#define delay(n) Sleep(n)							// n/1000�ʸ�ŭ �ð� ����
#define randomize() srand((unsigned)time(NULL))		// ���� �߻��� �ʱ�ȭ
#define random(n) (rand() % (n))					//0~n������ ���� �߻�


// ȭ���� ��� �����.
void clrscr()
{
	system("cls");
}

// Ŀ���� x,y��ǥ�� �̵���Ų��.
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}

// Ŀ���� x ��ǥ�� �����Ѵ�.
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.X;
}

// Ŀ���� y��ǥ�� �����Ѵ�.
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

// Ŀ���� ����ų� �ٽ� ǥ���Ѵ�.
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize=1;
		CurInfo.bVisible=FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize=100;
		CurInfo.bVisible=TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize=20;
		CurInfo.bVisible=TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

using namespace std;



const int MaxCard = 52;

const int CardGap = 4;

const int Speed = 1000;

const int PromptSpeed = 2000;

int nd = 25 , nc = 1 , sd = 25 , sc = 1;



struct SCard

{

	char Name[5];

	SCard() { Name[0] = 0; }

	SCard(const char *pName) {

		strcpy(Name, pName);

	}

	int GetNumber() const {

		if (isdigit(Name[0])) return Name[0] - '0';

		if (Name[0] == 'T') return 10;

		if (Name[0] == 'J') return 11;
		
		if (Name[0] == 'Q') return 12;

		return 13;

	};

	int GetKind() const {

		if (strcmp(Name + 1, "��") == 0) return 0;

		else if (strcmp(Name + 1, "��") == 0) return 1;

		else if (strcmp(Name + 1, "��") == 0) return 2;

		else return 3;

	}

	friend ostream &operator <<(ostream &c, const SCard &C) {

		return c << C.Name;

	}

	bool operator ==(const SCard &Other) const {

		return (strcmp(Name, Other.Name) == 0);

	}

	bool operator <(const SCard &Other) const {

		if (GetNumber() < Other.GetNumber()) return true;

		if (GetNumber() > Other.GetNumber()) return false;

		if (GetKind() < Other.GetKind()) return true;

		return false;

	}

};

SCard HwaToo[MaxCard] = {

	"1��","2��","3��","4��","5��","6��","7��","8��","9��","T��","J��","Q��","K��",
	
	"1��","2��","3��","4��","5��","6��","7��","8��","9��","T��","J��","Q��","K��",
	
	"1��","2��","3��","4��","5��","6��","7��","8��","9��","T��","J��","Q��","K��",
	
	"1��","2��","3��","4��","5��","6��","7��","8��","9��","T��","J��","Q��","K��"

};

// ī���� ������ �����ϴ� Ŭ����
class CCardSet
{
protected:
	SCard Card[MaxCard];
	int Num;
	const int sx,sy;
	CCardSet(int asx,int asy) : sx(asx), sy(asy) { Num=0; }

public:
	int GetNum() { return Num; }
	SCard GetCard(int idx) { return Card[idx]; }
	void Reset() { 
		for (int i=0;i<MaxCard;i++) Card[i].Name[0]=0;
		Num=0; 
	}
	void InsertCard(SCard C);
	SCard RemoveCard(int idx);
	int FindSameCard(SCard C,int *pSame);
	int FindFirstCard(const char *pName);
};

void CCardSet::InsertCard(SCard C) {
	int i;

	if (C.Name[0] == 0) return;
	for (i=0;i<Num;i++) {
		if (C < Card[i]) break;
	}
	memmove(&Card[i+1],&Card[i],sizeof(SCard)*(Num-i));
	Card[i]=C;
	Num++;
}

SCard CCardSet::RemoveCard(int idx) {
	assert(idx < Num);
	SCard C=Card[idx];
	memmove(&Card[idx],&Card[idx+1],sizeof(SCard)*(Num-idx-1));
	Num--;
	return C;
}

int CCardSet::FindSameCard(SCard C,int *pSame) {
	int i,num;
	int *p=pSame;

	for (i=0,num=0;i<Num;i++) {
		if (Card[i].GetNumber() == C.GetNumber()) {
			num++;
			*p++=i;
		}
	}
	*p=-1;
	return num;
}

int CCardSet::FindFirstCard(const char *pName) {
	int i;

	for (i=0;i<Num;i++) {
		if (strstr(Card[i].Name,pName) != NULL) {
			return i;
		}
	}
	return -1;
}

// ��� ���ʿ� ī�带 �׾� ���� ��ũ
class CDeck : public CCardSet
{
public:
	CDeck(int asx,int asy) : CCardSet(asx,asy) { ; }
	void Shuffle() {
		int i,n;
		for (i=0;i<MaxCard;i++) {
			do {
				n=random(MaxCard);
			} while (Card[n].Name[0] != '\0');
			Card[n]=HwaToo[i];
			Num++;
		}
	}
	SCard Pop() { return RemoveCard(Num-1); }
	bool IsEmpty() { return Num==0; }
	bool IsNotLast() { return Num > 1; }
	void Draw(bool bFlip) {
		gotoxy(18,12);
		cout << " " << (bFlip ? Card[Num-1].Name:"   ");
		gotoxy(18,1);
		cout << "??? " /*<< (bFlip ? Card[Num-1].Name:"   ")*/;
		gotoxy(18,21);
		cout << "??? " /*<< (bFlip ? Card[Num-1].Name:"   ")*/;
		gotoxy(30,1);
		cout << "����ī��:" << nd ; 
		gotoxy(30,2);
		cout << "������ī��:" << nc;	
		gotoxy(30,20);
		cout << "������ī��:" << sc;	
		gotoxy(30,21);
		cout << "����ī��:" << sd;	
		
	}
};


// ������ �ϴ� �÷��̾�
class CPlayer : public CCardSet
{
public:
	CPlayer(int asx,int asy) : CCardSet(asx,asy) { ; }
	void Draw(bool MyTurn) {
		int i,x;
		for (i=0,x=sx;i<Num;i++,x+=CardGap) {
			gotoxy(x+13,sy);
			cout << Card[i];
			if (MyTurn) {
				gotoxy(5,sy+2);
				cout << '[' << "����� �����Դϴ�" << ']';
			}
		}
//		gotoxy(5,8);
///		cout << Card[i];
	}
};

// ������ ����Ǵ� ���
class CBlanket: public CPlayer
{
public:
	CBlanket(int asx,int asy) : CPlayer(asx,asy) { ; }
	/*void Draw() {
		CPlayer::Draw(false);
	}*/
};

// �Լ� ����
void Initialize();
void DrawScreen();
void OutPrompt(const char *Mes,int Wait=0);
int InputInt(const char *Mes, int start, int end);

// ���� ����
CDeck Deck(5,21);
CPlayer South(5,20), North(5,2);
CBlanket Blanket(5,12);
bool SouthTurn;

// ���α׷��� �������ϴ� main �Լ�
int main()
{
	int i,ch;
	int arSame[4],SameNum;
	char Mes[256];
	CPlayer *Turn;
	int UserIdx,UserSel,DeckSel,z;
	SCard UserCard, DeckCard;
	bool UserTriple, DeckTriple;
	int nSnatch;
	int NewScore;
	

	randomize();
	Initialize();
	for (SouthTurn=true;!Deck.IsEmpty();SouthTurn=!SouthTurn) {
		
		DrawScreen();
		if (SouthTurn) {
			Turn=&South;
			South.InsertCard(DeckCard);
	
			
		} else {
			Turn=&North;
			North.InsertCard(DeckCard);
	
		}

		sprintf(Mes,"��ư�� ��������(1:����,0:����) ",Turn->GetNum());
		ch=InputInt(Mes,0,Turn->GetNum());
		
		if(!SouthTurn) nd--,nc++; //�߰� �κ�
		else sd--,sc++;
		
		if (ch == 0) {
			if (InputInt("���� �����̴ϱ�?(0:��,1:�ƴϿ�)",0,1)==0) 
				return 0;
			else 
				continue;
		}
		
/*
		// �÷��̾ ī�带 ���� ����.
		UserTriple=DeckTriple=false;
		UserIdx=ch-1;
		UserCard=Turn->GetCard(UserIdx);
		//SameNum=Blanket.FindSameCard(UserCard,arSame);
		switch (SameNum) {
		case 0:
			UserSel=-1;
			//Blanket.InsertCard(Turn->RemoveCard(UserIdx));
			DrawScreen();
			break;
		case 1:
			UserSel=arSame[0];
			break;
		case 2:
			if (Blanket.GetCard(arSame[0]) == Blanket.GetCard(arSame[1])) {
				UserSel=arSame[0];
			} else {
				Blanket.DrawSelNum(arSame);
				sprintf(Mes,"� ī�带 �����Ͻðڽ��ϱ�?(1~%d)",SameNum);
				UserSel=arSame[InputInt(Mes,1,SameNum)-1];
			}
			break;
		case 3:
			UserSel=arSame[1];
			UserTriple=true;
			break;
		}
		if (UserSel != -1) {
			//Blanket.DrawTempCard(UserSel,UserCard);
		}
		delay(Speed);
*/
		// ��ũ���� ������ �����´�.
		Deck.Draw(true);
		delay(Speed);
		DeckCard=Deck.Pop();		
		//SameNum=Blanket.FindSameCard(DeckCard,arSame);
		switch (SameNum) {
		case 0:
			DeckSel=-1;
			
			break;
		case 1:
			DeckSel=arSame[0];
			if (DeckSel == UserSel) {
				if (Deck.IsNotLast()) {
					//Blanket.InsertCard(DeckCard);
					//Blanket.InsertCard(Turn->RemoveCard(UserIdx));
					OutPrompt("�����߽��ϴ�.",PromptSpeed);
					continue;
				} else {
					DeckSel=-1;
				}
			}
			break;
		case 2:
			if (UserSel == arSame[0]) {
				DeckSel=arSame[1];
			} else if (UserSel == arSame[1]) {
				DeckSel=arSame[0];
			} else {
				/*if (Blanket.GetCard(arSame[0]) == Blanket.GetCard(arSame[1])) {
					DeckSel=arSame[0];
				} else {
					Blanket.DrawSelNum(arSame);
					sprintf(Mes,"� ī�带 �����Ͻðڽ��ϱ�?(1~%d)",SameNum);
					DeckSel=arSame[InputInt(Mes,1,SameNum)-1];
				}*/
			}
			break;
		case 3:
			DeckSel=arSame[1];
			DeckTriple=true;
			break;
		}
		
			DrawScreen();
		if (SouthTurn) {
			Turn=&South;
			Blanket.InsertCard(Turn->RemoveCard(UserIdx));					
			South.InsertCard(DeckCard);
			
	
					
					
		} else {
			Turn=&North;
			Blanket.InsertCard(Turn->RemoveCard(UserIdx));		
			North.InsertCard(DeckCard);
			
		}
		
		DeckCard = "";  //�߰� �κ� 
		

		if (DeckSel != -1) {
		//	Blanket.DrawTempCard(DeckSel,DeckCard);
		}
		
		Deck.Draw(false);
		delay(Speed);
	}
	DrawScreen();
	OutPrompt("������ �������ϴ�.",0);
}

void Initialize()
{
	int i;

	Deck.Shuffle();
	for (i=0;i<1;i++) {
		South.InsertCard(Deck.Pop());
		North.InsertCard(Deck.Pop());
	//	if (i < 8) Blanket.InsertCard(Deck.Pop());
	}
	
}

void DrawScreen()
{
	clrscr();
	South.Draw(SouthTurn);
	North.Draw(!SouthTurn);
	//Blanket.Draw();
	Deck.Draw(false);
	
}

void OutPrompt(const char *Mes,int Wait/*=0*/)
{
	gotoxy(5,23);
	for (int i=5;i<79;i++) { cout << ' '; }
	gotoxy(5,23);
	cout << Mes;
	delay(Wait);
}

int InputInt(const char *Mes, int start, int end)
{
	int ch;

	OutPrompt(Mes);
	for (;;) {
		ch=tolower(getch());
		if (ch == 0xE0 || ch == 0) {
			ch=getch();
			continue;
		}
		if (!(isdigit(ch) || ch=='a')) continue;
		if (ch=='a') ch=10; else ch=ch-'0';
		if (ch >= start && ch <= end) {
			return ch;
		}
		OutPrompt("��ȿ�� ��ȣ�Դϴ�. ������ ������ �°� �ٽ� �Է��� �ּ���.");
	}
}


