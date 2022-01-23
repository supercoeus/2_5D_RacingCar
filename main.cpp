//////////////////////////////////////////////
//
//	2.5D ����
//
//	�ı��� ��Ʒʷ��ķ (799052200@qq.com)
//	��� define.h
//

#include "NewDrawer.h"

void LoadIntro(wstring File)
{
	intro a;
	a.inipath = L"map/" + File + L"/set.ini";
	wchar_t tmp[30];								//������
	a.filename = File;

	GetPrivateProfileString(L"Intro", L"title", L"", tmp, 30, a.inipath.c_str());
	a.title = tmp;
	GetPrivateProfileString(L"Intro", L"intr", L"", tmp, 30, a.inipath.c_str());
	a.intr = tmp;

	IntroList.push_back(a);
}

BOOL SearchFilesByWildcard(wstring wildcardPath)	// �����ļ�
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA pNextInfo;

	hFile = FindFirstFile(wildcardPath.c_str(), &pNextInfo);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	WCHAR infPath[MAX_PATH] = { 0 };
	if (pNextInfo.cFileName[0] != '.')
	{
		LoadIntro(pNextInfo.cFileName);
	}

	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')
		{
			continue;
		}

		LoadIntro(pNextInfo.cFileName);
	}

	return TRUE;
}

MOUSEMSG m;
bool MouseTouch(int left, int top, int right, int button)
{
	for (int i1 = left; i1 < right; i1++)
	{
		for (int i2 = top; i2 < button; i2++)
		{
			if (m.x == i1 && m.y == i2)
			{
				return true;
			}
		}
	}
	return false;
}

// ѡ���ͼ�����ص�ͼ��ţ�-1 ��ʾȡ��ѡ��
int ChooseMap()
{
	// �Ƿ��Ѿ����ع���ͼ
	static bool isLoad = false;
	if (IntroList.size() == 0)
	{
		isLoad = false;
	}

	RECT r;
	cleardevice();
	wchar_t tmp[30];				//������
	wstring tw;						//�Ա���Ĵ���

	if (isLoad || SearchFilesByWildcard(L"map/*"))
	{
		isLoad = true;

		setbkmode(TRANSPARENT);
		setlinecolor(LIGHTBLUE);
		settextcolor(BLACK);

		for (int i = 0; i < (int)IntroList.size(); i++)
		{
			IMAGE t1;				//�����ʱ��ͼƬ��������

			fillrectangle(10, 60 + i * 120, 1260, 170 + i * 120);

			GetPrivateProfileString(L"File", L"Titlepic", L"", tmp, 30, IntroList[i].inipath.c_str());//��ȡ����������ͼ
			tw = tmp;
			if (tw == L"defpic")
			{
				GetPrivateProfileString(L"File", L"Racing", L"", tmp, 30, IntroList[i].inipath.c_str());
				loadimage(&t1, (L"map\\" + IntroList[i].filename + L"\\" + tmp).c_str());
				IMAGE img = zoomImage(&t1, 100, 100);
				putimage(10 + 5, 60 + i * 120 + 5, &img);
			}
			else
			{
				loadimage(&t1, (L"map\\" + IntroList[i].filename + L"\\" + tmp).c_str());
				IMAGE img = zoomImage(&t1, 100, 100);
				putimage(10 + 5, 60 + i * 120 + 5, &img);
			}



			r = { 120, 60 + i * 120 + 5, 1260, 60 + i * 120 + 25 };
			drawtext((L"����  " + IntroList[i].title).c_str(), &r, DT_WORDBREAK | DT_LEFT);
			r = { 120, 60 + i * 120 + 30, 1260, 60 + i * 120 + 80 };
			drawtext((L"����  " + IntroList[i].intr).c_str(), &r, DT_WORDBREAK | DT_LEFT | DT_WORD_ELLIPSIS);
			r = { 120, 60 + i * 120 + 85, 1260, 60 + i * 120 + 105 };
			drawtext((L"�ļ���  " + IntroList[i].filename).c_str(), &r, DT_WORDBREAK | DT_LEFT);
		}

		settextcolor(WHITE);
		r = { 0, 0, 1280, 60 };
		drawtext(L"ѡ���ͼ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		while (true)
		{
			FlushMouseMsgBuffer();
			while (true)
			{
				if (MouseHit())
				{
					m = GetMouseMsg();
					break;
				}

				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					return -1;
				}
			}

			if (m.mkLButton)
			{
				for (unsigned int i = 0; i < IntroList.size(); i++)
				{
					if (MouseTouch(10, 60 + i * 120, 1260, 170 + i * 120))
					{
						cleardevice();
						return i;
					}
				}
			}
		}
	}
	else
	{
		MessageBox(NULL, L"�Ҳ�����ͼ�ļ�", L"����", MB_OK | MB_ICONWARNING);
		exit(0);
		return 0;
	}
}

// ����һ����ά����һά�����е��±�
// ������Խ�������ŵ��߽������һ��
int PointTsm(int x, int y, int width, int height)
{
	if (x < 0)		x = 0;
	if (x >= width)	x = width;
	if (y < 0)		y = 0;
	if (y >= height)y = height;

	return width * y + x;
}

// �����������ֱ�߷���
Line GetLineOfPoints(POINT p1, POINT p2)
{
	Line l;
	if (p1.x == p2.x)
	{
		l.isVertical = true;
		l.b = p1.x;
	}
	else
	{
		l.k = (double)(p1.y - p2.y) / (p1.x - p2.x);
		l.b = (double)p1.y - l.k * p1.x;
	}

	return l;
}

// ��ȡ��ֱ�߽���
// ���� -1 ��ʾ�޽��㣨�������߲��ص���
// ���� 0 ��ʾ��һ������
// ���� 1 ��ʾ����������㣨���������ص���
int GetLinesIntersection(Line l1, Line l2, POINT* p)
{
	// ����������һ�����ߵĽ�
	Line pl[2] = { l1,l2 };
	for (int i = 0; i < 2; i++)
	{
		if (pl[i].isVertical)
		{
			if (pl[!i].isVertical)
			{
				if (pl[i].b == pl[!i].b)
				{
					*p = { (LONG)pl[i].b,0 };
					return 1;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				*p = { (LONG)pl[i].b,(LONG)(pl[!i].k * pl[i].b + pl[!i].b) };
				return 0;
			}
		}
	}

	// ����б�ߵĽ�
	double x = (l2.b - l1.b) / (l1.k - l2.k);
	double y = l1.k * x + l1.b;
	*p = { (LONG)x,(LONG)y };
	return 0;
}

// ���߼��
// ���ڿ��ܳ���̫��ֱ�Ӵ�Խ�յ��ߵ���û�м�⵽����
// ʹ�ô˺���ר�ż����Ƿ����
// p1, p2: �������λ�ȡ���������
// �����Ƿ񴩹��յ���
bool CheckPass(POINT p1, POINT p2)
{
	Line l = GetLineOfPoints(p1, p2);
	POINT p = { 0,0 };
	if (GetLinesIntersection(l, pEndLine, &p) == 0)
	{
		if (isInRect(p.x, p.y, { p1.x,p1.y,p2.x,p2.y }) &&
			isInRect(p.x, p.y, { pEndLinePoints[0].x,pEndLinePoints[0].y,pEndLinePoints[1].x,pEndLinePoints[1].y }))
		{
			return true;
		}
	}

	return false;
}

// ����ƶ���⼰��Ϣ����
// player: false ��ʾ��� 1��true ��ʾ��� 2
void MoveCheck(bool player)
{
	// ��һ֡��С��λ��
	static POINT pPrevious[2] = { {-1,-1},{-1,-1} };

	bool PinLine = false;
	bool CinLine = false;

	int rtmp = 1;

	inIce = false;
	inRoad = false;
	inSand = false;
	inWall = false;
	inEndline = false;
	DWORD* pbTch = GetImageBuffer(&Mask);
	DWORD* pCar;
	double x;
	double y;
	int w;
	int h;
	double SpeedChange = 0;
	COLORREF c;

	// �����ͼ��������Ϣ��ʱ���룩
	float fProcessingTime = 0;
	if (Processing != 0)
	{
		fProcessingTime = (clock() - Processing) / (float)CLOCKS_PER_SEC;
	}
	float fMultiple = 1 + fProcessingTime;

	if (!player)
	{
		x = Pspeed * cos(PForward) * fMultiple + Px;
		y = Pspeed * sin(PForward) * fMultiple + Py;
		DWORD* pbImg = GetImageBuffer(&Player1);
		pCar = pbImg;
		w = Player1.getwidth();
		h = Player1.getheight();
	}
	else
	{
		x = Cspeed * cos(CForward) * fMultiple + Cx;
		y = Cspeed * sin(CForward) * fMultiple + Cy;
		DWORD* pbImg = GetImageBuffer(&Player2);
		pCar = pbImg;
		w = Player2.getwidth();
		h = Player2.getheight();
	}

	int i;
	int j;

	// ��ȡ���ڵĵ�������
	for (i = 0; i < w; i++)
	{
		for (j = 0; j < h; j++)
		{
			if ((pCar[PointTsm(i, j, w, h)] & 0x00FFFFFF) == WHITE)
			{
				continue;
			}

			c = BGR(pbTch[PointTsm(i + (int)x, j + (int)y, nMapW, nMapH)] & 0x00FFFFFF);
			switch (c)
			{
			case BLACK:				inRoad = true;	break;
			case RGB(255, 255, 0):	inSand = true;	break;
			case RGB(0, 0, 255):	inIce = true;	break;
			case RGB(255, 0, 0):	inWall = true;	break;
			case RGB(0, 255, 0):	inEndline = true; inRoad = true;	break;
			}
		}
	}

	// �����������
	if (!player && pPrevious[0].x > 0)
	{
		if (CheckPass(pPrevious[0], { Px, Py }))
		{
			inEndline = true;
			inRoad = true;
		}
	}
	if (player && pPrevious[1].x > 0)
	{
		if (CheckPass(pPrevious[1], { Cx, Cy }))
		{
			inEndline = true;
			inRoad = true;
		}
	}

	//�����ڵ������ͽ��в���
	if (inSand)
	{
		SpeedChange -= FinSand;
	}
	if (inRoad)
	{
		SpeedChange -= FinRoad;
	}
	if (inIce)
	{
		SpeedChange -= FinIce;
	}
	if (inEndline)
	{
		if (!player)
		{
			PinLine = true;
			if (cos(PForward - EndLineForward) * Pspeed < 0)	// ����
			{
				inWall = true;
				Pwrong = true;
				Ptime2 = Now;
			}
			else
			{
				if (!PHadPass)	PHadPass = true;
			}
		}
		else
		{
			CinLine = true;
			if (cos(CForward - EndLineForward) * Cspeed < 0)	// ����
			{
				inWall = true;
				Cwrong = true;
				Ctime2 = Now;
			}
			else
			{
				if (!CHadPass)CHadPass = true;
			}
		}
	}
	if (!player)
	{
		if (inWall)
		{
			Pspeed = 0;
			return;
		}
		if (-SpeedChange < abs(Pspeed))
		{
			SpeedChange += abs(Pspeed);
			if (Pspeed > 0)
			{
				Pspeed = SpeedChange;
			}
			else
			{
				Pspeed = -SpeedChange;
			}
		}
		else Pspeed = 0;
		if (inWall)
		{
			/*while (c == BGR(0x0000FF))
			{
				Px += (int)round(cos(PForward));
				Py += (int)round(sin(PForward));
				for (int i = 0; i < w; i++)
				{
					for (int j = 0; j < h; j++)
					{
						c = pbTch[PointTsm((int)ceil(i + x), (int)ceil(j + y), nMapW, nMapH)] & 0x00FFFFFF;
					}
				}
			}*/
		}
		else
		{
			Px = (int)ceil(x);
			Py = (int)ceil(y);
		}
	}
	else
	{
		if (inWall)
		{
			Cspeed = 0;
			return;
		}
		if (-SpeedChange < abs(Cspeed))
		{
			SpeedChange += abs(Cspeed);
			if (Cspeed > 0)
			{
				Cspeed = SpeedChange;
			}
			else
			{
				Cspeed = -SpeedChange;
			}
		}
		else Cspeed = 0;
		if (inWall)
		{
			/*while (c == BGR(0x0000FF))
			{
				Cx += (int)round(cos(CForward));
				Cy += (int)round(sin(CForward));
				for (int i = 0; i < w; i++)
				{
					for (int j = 0; j < h; j++)
					{
						c = pbTch[PointTsm((int)ceil(i + x), (int)ceil(j + y), Racing.getwidth(), Racing.getheight())] & 0x00FFFFFF;
					}
				}
			}*/
		}
		else
		{
			Cx = (int)ceil(x);
			Cy = (int)ceil(y);
		}
	}
	if (PinLine && PHadPass)
	{
		PWaitOut = true;
	}
	if (CinLine && CHadPass)
	{
		CWaitOut = true;
	}
	if (PWaitOut && !PinLine)
	{
		Ppass++;
		PWaitOut = false;
	}
	if (CWaitOut && !CinLine)
	{
		Cpass++;
		CWaitOut = false;
	}


	// ��¼��һ�ε�����
	if (!player)
	{
		pPrevious[0] = { Px,Py };
	}
	else
	{
		pPrevious[1] = { Cx,Cy };
	}
}

int GetCommand()
{
	int c = 0;
	if (!Pover)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			c |= CMD_UP;
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000))	c |= CMD_DOWN;
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000))	c |= CMD_SHIFT;
	}

	if (TwoPlayer)
	{
		if (!Cover)
		{
			if (GetAsyncKeyState('A') & 0x8000)		c |= sCMD_LEFT;
			if (GetAsyncKeyState('D') & 0x8000)		c |= sCMD_RIGHT;
			if (GetAsyncKeyState('W') & 0x8000)		c |= sCMD_UP;
			if (GetAsyncKeyState('S') & 0x8000)		c |= sCMD_DOWN;
		}
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)	c |= CMD_QUIT;
	return c;
}

void DispatchCommand(int _cmd)
{
	if (_cmd & CMD_UP)			OnUp(false);
	if (_cmd & CMD_DOWN)		OnDown(false);
	if (_cmd & CMD_LEFT)		OnLeft(false);
	if (_cmd & CMD_RIGHT)		OnRight(false);
	if (_cmd & CMD_SHIFT)		OnShift(false);
	if (TwoPlayer)
	{
		if (_cmd & sCMD_UP)			OnUp(true);
		if (_cmd & sCMD_DOWN)		OnDown(true);
		if (_cmd & sCMD_LEFT)		OnLeft(true);
		if (_cmd & sCMD_RIGHT)		OnRight(true);
	}
	if (_cmd & CMD_QUIT)
	{
		MeumMod = true;
	}
}

void InitGame()
{
	Loading();

	BeginBatchDraw();
	if (chexit)return;

	setbkmode(TRANSPARENT);
	settextcolor(WHITE);

	Player1 = car1;
	Player2 = car2;

	SetBirth();
}

// �Զ������ٶȸ���ת���ٶ�
void AutoRotaSetting()
{
	int max = 15;
	int p = Rota_base - (int)(Pspeed / 3);
	int c = Rota_base - (int)(Cspeed / 3);
	if (p >= max)	PRota = p;
	else			PRota = max;
	if (c >= max)	CRota = c;
	else			CRota = max;
}

void gaming()
{
	//��Ϸ�˵�
	settextcolor(BLACK);
	button gc(1, WIDTH / 2 - 50, HEIGHT / 2 - 100, 100, 50, L"������Ϸ");
	button gh(2, WIDTH / 2 - 50, HEIGHT / 2, 100, 50, L"�ص����˵�");
	button ge(3, WIDTH / 2 - 50, HEIGHT / 2 + 100, 100, 50, L"�˳���Ϸ");

	page gm;

	gm.botlist.push_back(gc);
	gm.botlist.push_back(gh);
	gm.botlist.push_back(ge);

	int gid = 0;

	mciSendString(L"play mymusic from 0", NULL, 0, NULL);	// ��ͷ�������ֲ��ظ�
	mciSendString(L"play mymusic repeat", NULL, 0, NULL);

	StartWord();
	Start = clock();// ��ʼ��ʱ
	timer tCmd;		// ��ȡ�����ʱ
	timer tWinWait;	// ʤ�����ʱ
	bool bStartWaitWin = false;	// �Ƿ����ʤ����Ķ��ݵȴ�ʱ��
	while (true)
	{
		Now = clock();
		if (tCmd.WaitFor(25))						// �������0.025��
		{
			DispatchCommand(GetCommand());			// ��ȡ����
			if (Pspeed != 0)
			{
				MoveCheck(false);
			}
			if (TwoPlayer && Cspeed != 0)
			{
				MoveCheck(true);
			}
		}
		if (MeumMod)								// ��ʾ��ͣ����
		{
			MeumUsed = clock();
			gid = gm.ShownPage();
			if (gid == 1)
			{
				MeumMod = false;
			}
			if (gid == 2)
			{
				End();
				isres = true;
				return;
			}
			if (gid == 3)
			{
				isres = false;
				return;
			}
			Start += clock() - MeumUsed;
		}

		Processing = clock();

		AutoRotaSetting();
		Draw();

		// ʤ���ж�
		if ((TwoPlayer && Cover && Pover) || (!TwoPlayer && Pover))
		{
			bStartWaitWin = true;
		}
		if (bStartWaitWin && tWinWait.WaitFor(3000))
		{
			End();
			WinScene();
			isres = true;
			break;
		}
	}
}

void End()
{
	mciSendString(L"stop mymusic", NULL, 0, NULL);	// ֹͣ���ر�����
	mciSendString(L"close mymusic", NULL, 0, NULL);
	EndBatchDraw();
}

void OnLeft(bool player)
{
	if (!player)
	{
		PForward -= PI / PRota;
	}
	else
	{
		CForward -= PI / CRota;
	}
	if (!CanRota(player))
	{
		if (!player)
		{
			if (Pspeed >= 0)
			{
				Px += (int)round(cos(PForward - PI / 2));
				Py += (int)round(sin(PForward - PI / 2));
			}
			else
			{
				Px -= (int)round(cos(PForward - PI / 2));
				Py -= (int)round(sin(PForward - PI / 2));
			}
		}
		else
		{
			if (Pspeed >= 0)
			{
				Cx += (int)round(cos(CForward - PI / 2));
				Cy += (int)round(sin(CForward - PI / 2));
			}
			else
			{
				Cx -= (int)round(cos(CForward - PI / 2));
				Cy -= (int)round(sin(CForward - PI / 2));
			}
		}
	}
}

void OnRight(bool player)
{
	if (!player)
	{
		PForward += PI / PRota;
	}
	else
	{
		CForward += PI / CRota;
	}
	if (!CanRota(player))
	{
		if (!player)
		{
			if (Pspeed >= 0)
			{
				Px += (int)round(cos(PForward + PI / 2));
				Py += (int)round(sin(PForward + PI / 2));
			}
			else
			{
				Px -= (int)round(cos(PForward + PI / 2));
				Py -= (int)round(sin(PForward + PI / 2));
			}
		}
		else
		{
			if (Pspeed >= 0)
			{
				Cx += (int)round(cos(CForward + PI / 2));
				Cy += (int)round(sin(CForward + PI / 2));
			}
			else
			{
				Cx -= (int)round(cos(CForward + PI / 2));
				Cy -= (int)round(sin(CForward + PI / 2));
			}
		}
	}
}

void OnUp(bool player)
{
	if (!player)
	{
		Pspeed += SpeedAdd;
		if (Pspeed > MaxSpeed)
		{
			Pspeed = MaxSpeed;
		}
	}
	else
	{
		Cspeed += SpeedAdd;
		if (Cspeed > MaxSpeed)
		{
			Cspeed = MaxSpeed;
		}
	}
}

void OnDown(bool player)
{
	if (!player)
	{
		Pspeed -= SpeedAdd;
		if (Pspeed < -SpeedAdd)
		{
			Pspeed = -SpeedAdd;
		}
	}
	else
	{
		Cspeed -= SpeedAdd;
		if (Cspeed < -SpeedAdd)
		{
			Cspeed = -SpeedAdd;
		}
	}
}

// ����Ư�ơ���֧��˫��
void OnShift(bool player)
{
	if (!player)
	{
		// Ư�Ʋ��ܼ��ٵ� 0
		if (Pspeed - SpeedAdd / 2 > 0)
			Pspeed -= SpeedAdd / 2;
	}
}

void SetBirth()
{
	int Ax = 0;
	int Ay = 0;
	int Bx = 0;
	int By = 0;
	bool findA = false;
	DWORD* pbTch = GetImageBuffer(&Mask);
	//ͨ�������յ��߶˵��ȡ�յ��ߵ�����
	//ͨ��atan2��ȡ�յ�����������ļн�
	for (int i1 = 0; i1 < nMapW; i1++)
	{
		for (int i2 = 0; i2 < nMapH; i2++)
		{
			DWORD c = pbTch[PointTsm(i1, i2, nMapW, nMapH)] & 0x00FFFFFF;
			if (c == 0xFF00FF)
			{
				Px = i1;
				Py = i2;
			}
			if (c == 0xFFFFFF)
			{
				Cx = i1;
				Cy = i2;
			}
			if (c == 0xAAAAAA)
			{
				if (!findA)
				{
					Ax = i1;
					Ay = i2;
					findA = true;
				}
				else
				{
					Bx = i1;
					By = i2;
				}
			}
		}
	}
	EndLineForward = atan2(Ax - Bx, Ay - By);
	PForward = EndLineForward;
	CForward = EndLineForward;

	pEndLinePoints[0] = { Ax, Ay };
	pEndLinePoints[1] = { Bx, By };

	pEndLine = GetLineOfPoints(pEndLinePoints[0], pEndLinePoints[1]);
}

// ���õͷֱ���
void LowResolution()
{
	static int n = 0;

	if (n == 0)
	{
		WIDTH = 800;
		HEIGHT = 600;
	}
	else
	{
		int w = (int)(WIDTH / 1.3);
		int h = (int)(HEIGHT / 1.3);
		if (w > 300 && h > 300)
		{
			WIDTH = w;
			HEIGHT = h;
		}
	}

	dWidthZoom = WIDTH / 1280.0;
	dHeightZoom = HEIGHT / 960.0;
	closegraph();
	initgraph(WIDTH, HEIGHT);

	n++;
}

// �˵�����
void Loading()
{
	mciSendString(_T("open res\\bk.mp3 alias bk"), NULL, 0, NULL);
	mciSendString(_T("play bk repeat"), NULL, 0, NULL);

begin:

	cleardevice();
	settextstyle(16, 0, L"system");
	settextcolor(BLACK);

	//���˵���ť�����
	button bs(1, 540, 580 - 400, 200, 100, L"����ģʽ");
	button bd(2, 540, 580 - 200, 200, 100, L"˫��ģʽ");
	button bh(3, 540, 580, 200, 100, L"��Ϸ˵��");
	button be(4, 540, 580 + 200, 200, 100, L"�˳���Ϸ");

	button btnLowResolution(5, 160, 20, 160, 40, L"�ͷֱ���ģʽ");
	button btnOriginal(6, 160, 70, 160, 40, L"����ԭ���ӽǣ�2D��");
	button btnPerspective(7, 160, 120, 160, 40, L"�ر� 2.5D ͸��Ч��");

	page sm;

	sm.botlist.push_back(bs);
	sm.botlist.push_back(bh);
	sm.botlist.push_back(be);
	sm.botlist.push_back(bd);
	sm.botlist.push_back(btnLowResolution);
	sm.botlist.push_back(btnOriginal);
	sm.botlist.push_back(btnPerspective);

#define MULTIPLY(x,y) x = (int)(x * y)

	// ����ͷֱ���
	for (int i = 0; i < 7; i++)
	{
		MULTIPLY(sm.botlist[i].x, dWidthZoom);
		MULTIPLY(sm.botlist[i].y, dHeightZoom);
		MULTIPLY(sm.botlist[i].width, dWidthZoom);
		MULTIPLY(sm.botlist[i].height, dHeightZoom);
	}

	settextcolor(RGB(255, 130, 40));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 86;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfItalic = true;

	settextstyle(&f);
	RECT r = { 0, 0, (int)(1280 * dWidthZoom), (int)(200 * dHeightZoom) };
	drawtext(L"2.5D ˫������", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	settextstyle(16, 0, L"system", 0, 0, 0, false, false, false);
	settextcolor(LIGHTBLUE);
	outtextxy(50, 200, L"˫������   huidong �޸İ� Ver 2.0");
	outtextxy(50, 240, L"ԭ���ߣ���Ʒʷ��ķ");
	outtextxy(50, 300, L"���һ����������������ҿ���ʹ�� shift ����ɲ��");
	outtextxy(50, 340, L"��Ҷ���WSAD ����");

	int gid = 0;
	while (true)
	{
		if (isUseOriginal)	sm.botlist[btnOriginal.ID - 1].str = L"�ر�ԭ���ӽǣ�2D��";
		else				sm.botlist[btnOriginal.ID - 1].str = L"����ԭ���ӽǣ�2D��";

		if (isUseOriginal)				sm.botlist[btnPerspective.ID - 1].str = L"--ԭ���ӽ�����͸��--";
		else if (isPerspectiveEffect)	sm.botlist[btnPerspective.ID - 1].str = L"�ر� 2.5D ͸��Ч��";
		else							sm.botlist[btnPerspective.ID - 1].str = L"���� 2.5D ͸��Ч��";

		gid = sm.ShownPage();

		switch (gid)
		{
		case 1:	TwoPlayer = false;	goto btn_end;
		case 2:	TwoPlayer = true;	goto btn_end;
		case 3:	showhelp();			goto begin;
		case 4:	chexit = true;		return;
		case 5: LowResolution();	goto begin;
		case 6: isUseOriginal = !isUseOriginal;	break;
		case 7:	if (!isUseOriginal) isPerspectiveEffect = !isPerspectiveEffect;	break;
		}
	}

btn_end:

	int num = ChooseMap();
	if (num == -1)
	{
		goto begin;
	}

	wstring rootpath = L"map\\" + IntroList[num].filename + L"\\";
	wchar_t tmp[30];
	IMAGE imgTmp[4];

	GetPrivateProfileString(L"File", L"Car1", NULL, tmp, 30, IntroList[num].inipath.c_str());
	loadimage(&imgTmp[0], (rootpath + tmp).c_str());
	car1 = imgTmp[0];
	GetPrivateProfileString(L"File", L"Car2", NULL, tmp, 30, IntroList[num].inipath.c_str());
	loadimage(&imgTmp[1], (rootpath + tmp).c_str());
	car2 = imgTmp[1];
	GetPrivateProfileString(L"File", L"Racing", NULL, tmp, 30, IntroList[num].inipath.c_str());
	loadimage(&imgTmp[2], (rootpath + tmp).c_str());
	Racing = imgTmp[2];
	GetPrivateProfileString(L"File", L"Toucher", NULL, tmp, 30, IntroList[num].inipath.c_str());
	loadimage(&imgTmp[3], (rootpath + tmp).c_str());
	Mask = imgTmp[3];
	GetPrivateProfileString(L"File", L"Music", NULL, tmp, 30, IntroList[num].inipath.c_str());
	mciSendString((L"open " + rootpath + tmp + L" alias mymusic").c_str(), NULL, 0, NULL);	// ��MP3�ļ�
	//ͨ���ļ���ȡֵ
	MaxSpeed = (double)GetPrivateProfileInt(L"Set", L"MaxSpeed", (int)MaxSpeed, IntroList[num].inipath.c_str());
	FinSand = GetPrivateProfileInt(L"Set", L"FinSand", FinSand, IntroList[num].inipath.c_str());
	FinRoad = GetPrivateProfileInt(L"Set", L"FinRoad", FinRoad, IntroList[num].inipath.c_str());
	FinIce = GetPrivateProfileInt(L"Set", L"FinIce", FinIce, IntroList[num].inipath.c_str());
	Rota_base = GetPrivateProfileInt(L"Set", L"Rota", Rota_base, IntroList[num].inipath.c_str());
	NeedR = GetPrivateProfileInt(L"Set", L"NeedR", NeedR, IntroList[num].inipath.c_str());

	for (int i = 0; i < 3; i++)
	{
		wstring wch;
		wch += L'C' - i;
		nLevelTime[i] = GetPrivateProfileInt(L"Level", wch.c_str(), nLevelTime[i], IntroList[num].inipath.c_str());
	}

	nMapW = Racing.getwidth();
	nMapH = Racing.getheight();

	mciSendString(_T("stop bk"), NULL, 0, NULL);
	mciSendString(_T("close bk"), NULL, 0, NULL);
}

void showhelp()
{
	cleardevice();
	settextcolor(WHITE);
	settextstyle(24, 0, L"����");
	outtextxy(10, 10, L"��Ϸ˵��");

	wstring word;
	int Record = 3;

	wifstream HelpFile(L"Help.txt");

	HelpFile.imbue(std::locale(""));

	settextstyle(16, 0, L"����");
	if (HelpFile.is_open())
	{
		while (!HelpFile.eof())
		{
			getline(HelpFile, word, L'\n');//��������
			outtextxy(10, 5 + (int)(1.2 * Record * 16), word.c_str());
			Record++;
		}
	}
	else
	{
		MessageBox(NULL, L"�����ļ���ʧ", L"����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		closegraph();
		exit(1);
	}

	Record += 2;
	settextstyle(24, 0, L"����");
	outtextxy(10, (int)(1.2 * Record * 16), L"�� ESC ���������˵�");

	settextstyle(16, 0, L"����");

	clean();
	_getch();

	cleardevice();
	HelpFile.close();

	settextcolor(BLACK);
	return;
}

// ��հ���������
void clean()
{
	int k;
	while (1) {
		if (_kbhit()) {
			k = _getch();
			if (0 == k || 0xE0 == k) k = _getch();
		}
		else break;
	}
}

void restart()
{
	IntroList.clear();
	Px = 150;
	Py = 150;
	PForward = 0;		// ����
	Pspeed = 0;				// �ٶ�
	Ppass = 0;				// ͨ�������յ�
	Pwrong = false;		// �Ƿ�����
	PHadPass = false;		// �Ƿ�ͨ���յ�
	PWaitOut = false;		// �Ƿ�ȴ�ͨ���յ�
	Pover = false;			// �Ƿ����
	Ptime = 0;
	Ptime2 = 0;

	Cx = 170;
	Cy = 170;
	CForward = 0;
	Cspeed = 0;
	Cpass = 0;
	Cwrong = false;
	CHadPass = false;
	CWaitOut = false;
	Cover = false;
	Ctime = 0;
	Ctime2 = 0;

	MeumMod = false;

	Start = 0;
	Now = 0;
	MeumUsed = 0;
	Processing = 0;
}

bool CanRota(bool player)
{
	DWORD* pbTch = GetImageBuffer(&Mask);
	int x, y, w, h;
	int SpeedChange = 0;
	IMAGE tmpRotatedCar;	// �洢��ת��ĳ���
	if (!player)
	{
		x = Px;
		y = Py;
		rotateimage(&tmpRotatedCar, &car1, -PForward, WHITE, true, false);
	}
	else
	{
		x = Cx;
		y = Cy;
		rotateimage(&tmpRotatedCar, &car2, -CForward, WHITE, true, false);
	}
	DWORD* pCar = GetImageBuffer(&tmpRotatedCar);
	w = tmpRotatedCar.getwidth();
	h = tmpRotatedCar.getheight();

	// ��ȡ���ڵĵ�������
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			if ((pCar[PointTsm(i, j, w, h)] & 0x00FFFFFF) == WHITE)
			{
				continue;
			}

			if ((pbTch[PointTsm(i + x, j + y, nMapW, nMapH)] & 0x00FFFFFF) == BGR(0x0000FF))
			{
				return false;
			}
		}
	}
	return true;
}

// ����ֻ��Ҫ����һ�ε���Դ
void InitRes()
{
	loadimage(&imgWinFlag, L"res/flag.jpg");
	loadimage(&imgStar[0], L"res/star1.bmp");
	loadimage(&imgStar[1], L"res/star2.bmp");
}

int main()
{
	initgraph(WIDTH, HEIGHT);

	InitRes();

	while (isres)
	{
		InitGame();
		if (chexit)
		{
			break;
		}
		gaming();
		restart();
	}

	closegraph();
}