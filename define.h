/////////////////////////////////////////////////////////
// �������ƣ�2.5D ����
// ���뻷����Visual Studio 2022��EasyX_20211109
// �������ߣ���Ʒʷ��ķ (799052200@qq.com)
// �� �� ��: huidong <huidong_mail@163.com>
// �޸İ汾: Ver 2.0
// �������ڣ�2019.02.04
// ����޸ģ�2022.01.23
//

#include <graphics.h>      // ����ͼ�ο�ͷ�ļ�
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <conio.h>
// ���� MP3 ����
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

using namespace std;

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_SHIFT		512
#define	sCMD_UP			16
#define	sCMD_DOWN		32
#define	sCMD_LEFT		64
#define	sCMD_RIGHT		128
#define	CMD_QUIT		256
#define PI				3.1415926

// ֱ�߷���
struct Line
{
	double k = 0;	// ϵ����Ϊ 0 ��ʾ���ߣ���������

	// ֱ�߲�Ϊ����ʱ����ֵ��ʾ��ֱƫ����
	// ֱ��Ϊ����ʱ����ֵ��ʾˮƽƫ����
	double b = 0;

	bool isVertical = false;	// �Ƿ�Ϊ����
};

// �Ƿ���ԭ���ӽ�
bool isUseOriginal = false;

// �Ƿ��� 2.5D ��͸��Ч��
bool isPerspectiveEffect = true;

double MaxSpeed = 30;		// ����ٶȣ���λ���ң�
int FinSand = 5;			// ��ɳ�ϵ�Ħ����
int FinRoad = 1;			// ��·�ϵ�Ħ����
int FinIce = -2;			// �ڱ��ϵ�Ħ����
double SpeedAdd = PI / 2;	// ���ٶ�
int Rota_base = 64;			// ����ת�٣�ת���ٶȵ� -1 �η���
int Rota = 64;				// ʵ��ת�٣����ٿ����ת�٣�
int NeedR = 2;				// Ŀ��Ȧ��

// ���ڴ�С
int WIDTH = 1280;
int HEIGHT = 960;

// ��󴰿ڴ�С������ʼ���ڴ�С��
int nMaxW = 1280;
int nMaxH = 960;

// �������űȣ����ڵͷֱ���ģʽ��
double dWidthZoom = 1;
double dHeightZoom = 1;

double EndLineForward = 0;	// �յ�Ƕ�
POINT pEndLinePoints[2] = { {0,0},{0,0} };	// �յ�����˵�λ��
Line pEndLine;	// �յ��߷���

bool inIce;
bool inRoad;
bool inSand;
bool inWall;
bool inEndline;

IMAGE Racing;				// ������ͼ
IMAGE Mask;					// ��ײͼ
IMAGE car1;
IMAGE car2;

// ÿ���ȼ���ʱ��Ŀ�꣨�룩
int nLevelTime[3] = { 100, 70, 50 };

IMAGE imgWinFlag;	// ��ʤ����
IMAGE imgStar[2];	// ����ǣ�0 δ������1 ������

int nMapW;	// ��ͼ��
int nMapH;	// ��ͼ��

IMAGE Player1;
int Px = 150;
int Py = 150;
double PForward = 0;		// ����
double Pspeed = 0;			// �ٶȣ���λ���ң�
int PRota = 64;				// ת�٣�ת���ٶȣ�
int Ppass = 0;				// ͨ�������յ�
bool Pwrong = false;		// �Ƿ�����
bool PHadPass = false;		// �Ƿ�ͨ���յ�
bool PWaitOut = false;		// �Ƿ�ȴ�ͨ���յ�
bool Pover = false;			// �Ƿ����
clock_t Ptime = 0;
clock_t Ptime2 = 0;

IMAGE Player2;
int Cx = 170;
int Cy = 170;
double CForward = 0;
double Cspeed = 0;
int CRota = 64;
int Cpass = 0;
bool Cwrong = false;
bool CHadPass = false;
bool CWaitOut = false;
bool Cover = false;
clock_t Ctime = 0;
clock_t Ctime2 = 0;

bool TwoPlayer = false;

bool isres = true;
bool chexit = false;

bool MeumMod = false;

clock_t Start = 0;
clock_t Now = 0;
clock_t MeumUsed = 0;
clock_t Processing = 0;	// ����ÿ�ο�ʼ���ơ�������Ϣ��ʱ��

// ���Ƿ�λ�ھ�����
bool isInRect(int x, int y, RECT rct)
{
	if (rct.left > rct.right)	std::swap(rct.left, rct.right);
	if (rct.top > rct.bottom)	std::swap(rct.top, rct.bottom);
	return x >= rct.left && x <= rct.right && y >= rct.top && y <= rct.bottom;
}

struct button				// ���׵İ�ťʵ��
{
	int ID;

	int x;
	int y;
	int width;
	int height;
	wstring str;

	COLORREF fillcolor;
	COLORREF linecolor;
	COLORREF textcolor;
	LOGFONT textstyle;
	UINT uFormat;

	button(int gID, int gx, int gy, int gw, int gh, wstring gs)
	{
		fillcolor = getfillcolor();
		linecolor = getlinecolor();
		textcolor = gettextcolor();
		gettextstyle(&textstyle);
		uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		ID = gID;
		x = gx;
		y = gy;
		width = gw;
		height = gh;
		str = gs;
	}
};

struct page
{
	vector<button> botlist;

	bool MouseTouch(int left, int top, int right, int bottom, MOUSEMSG m)	// ��������ж�
	{
		return isInRect(m.x, m.y, { left,top,right,bottom });
	}

	int ShownPage()														// ��ʾ���ȴ���������Ӧ��������Ӧ��IDֵ
	{
		COLORREF fillcolor = getfillcolor();
		COLORREF linecolor = getlinecolor();
		COLORREF textcolor = gettextcolor();
		LOGFONT textstyle;
		gettextstyle(&textstyle);

		MOUSEMSG m;
		setbkmode(TRANSPARENT);

		for (unsigned int i = 0; i < botlist.size(); i++)
		{
			setfillcolor(botlist[i].fillcolor);
			setlinecolor(botlist[i].linecolor);
			settextcolor(botlist[i].textcolor);
			settextstyle(&botlist[i].textstyle);

			fillrectangle(botlist[i].x, botlist[i].y, botlist[i].x + botlist[i].width, botlist[i].y + botlist[i].height);

			RECT rct = { botlist[i].x, botlist[i].y, botlist[i].x + botlist[i].width, botlist[i].y + botlist[i].height };
			drawtext(botlist[i].str.c_str(), &rct, botlist[i].uFormat);
		}
		FlushBatchDraw();

		while (true)
		{
			FlushMouseMsgBuffer();
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				for (unsigned int i = 0; i < botlist.size(); i++)
				{
					if (MouseTouch(botlist[i].x, botlist[i].y, botlist[i].x + botlist[i].width, botlist[i].y + botlist[i].height, m))
					{
						return botlist[i].ID;
					}
				}
			}
		}

		setfillcolor(fillcolor);
		setlinecolor(linecolor);
		settextcolor(textcolor);
		settextstyle(&textstyle);
	}
};

struct intro	// ��ͼ�Ľ�����Ϣ
{
	wstring filename;
	wstring title;
	wstring intr;
	wstring inipath;
};
vector<intro> IntroList;

class timer		// ��ʱ��
{
private:
	bool is_start = false;
	clock_t start;
public:
	bool WaitFor(clock_t s)
	{
		if (is_start)
		{
			if ((start + s) <= clock())
			{
				is_start = false;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			start = clock();
			is_start = true;
			return false;
		}
	}
};

void init();
void gaming();
int GetCommand();
void DispatchCommand(int _cmd);
void OnLeft(bool player);		// false ��� 1��true ��� 2
void OnRight(bool player);
void OnUp(bool player);
void OnDown(bool player);
void OnShift(bool player);
void MoveCheck(bool player);	// ��ײ�ж�
int PointTsm(int x, int y, int width, int high);	// ��������ֵ��ת��
void Draw();
void End();
void PutImgWithout(IMAGE& obj, int px, int py, COLORREF withouter, DWORD* pbWnd, int wX, int wY, DWORD bitsub);	// ����ͼƬ������
void SetBirth();	// ��һ�ζ�ȡ
void StartWord();
void Loading();		// ���ص�ͼ
int ChooseMap();	// ѡ���ͼ
void LoadIntro(string File);
BOOL SearchFilesByWildcard(string wildcardPath);	// �����ļ����ο���https://blog.csdn.net/faithzzf/article/details/54290084
IMAGE zoomImage(IMAGE* pImg, int newWidth, int newHeight);	// ͼƬ����
void showhelp();	// ��ʾ�����ļ�
void clean();		// ��ջ�����
void restart();		// �������¿�ʼ��Ϸ
bool CanRota(bool player);//�Ƿ������ת
