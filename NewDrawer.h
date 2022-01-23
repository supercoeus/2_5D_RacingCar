/////////////////////////////////////////
//
//	NewDrawer.h
//	
//	��Ϸ�л��Ƶ���غ���
//


#pragma once

#include "define.h"

/*
 *	�ο���http://tieba.baidu.com/p/5218523817?pid=109267542552&cid=0#109267542552
 *	������:zoomImage(IMAGE* pImg,int width��int height)
 *	����˵��:pImg��ԭͼָ�룬width1��height1��Ŀ��ͼƬ�ĳߴ硣
 *	��������:��ͼƬ�������ţ�����Ŀ��ͼƬ �����Զ��峤���Ҳ����ֻ�����Զ������
 *	����Ŀ��ͼƬ
*/
IMAGE zoomImage(IMAGE* pImg, int newWidth, int newHeight = 0)
{
	// ��ֹԽ��
	if (newWidth < 0 || newHeight < 0) {
		newWidth = pImg->getwidth();
		newHeight = pImg->getheight();
	}

	// ������ֻ��һ��ʱ�������Զ�����
	if (newHeight == 0) {
		// �˴���Ҫע����*��/����Ȼ��Ŀ��ͼƬС��ԭͼʱ�����
		newHeight = newWidth * pImg->getheight() / pImg->getwidth();
	}

	// ��ȡ��Ҫ�������ŵ�ͼƬ
	IMAGE newImg(newWidth, newHeight);

	// �ֱ��ԭͼ���Ŀ��ͼ���ȡָ��
	DWORD* oldDr = GetImageBuffer(pImg);
	DWORD* newDr = GetImageBuffer(&newImg);

	// ��ֵ ʹ��˫���Բ�ֵ�㷨
	for (int i = 0; i < newHeight - 1; i++) {
		for (int j = 0; j < newWidth - 1; j++) {
			int t = i * newWidth + j;
			int xt = j * pImg->getwidth() / newWidth;
			int yt = i * pImg->getheight() / newHeight;
			newDr[i * newWidth + j] = oldDr[xt + yt * pImg->getwidth()];
			// ʵ�����м���ͼƬ
			byte r = (GetRValue(oldDr[xt + yt * pImg->getwidth()]) +
				GetRValue(oldDr[xt + yt * pImg->getwidth() + 1]) +
				GetRValue(oldDr[xt + (yt + 1) * pImg->getwidth()]) +
				GetRValue(oldDr[xt + (yt + 1) * pImg->getwidth() + 1])) / 4;
			byte g = (GetGValue(oldDr[xt + yt * pImg->getwidth()]) +
				GetGValue(oldDr[xt + yt * pImg->getwidth()] + 1) +
				GetGValue(oldDr[xt + (yt + 1) * pImg->getwidth()]) +
				GetGValue(oldDr[xt + (yt + 1) * pImg->getwidth()]) + 1) / 4;
			byte b = (GetBValue(oldDr[xt + yt * pImg->getwidth()]) +
				GetBValue(oldDr[xt + yt * pImg->getwidth()] + 1) +
				GetBValue(oldDr[xt + (yt + 1) * pImg->getwidth()]) +
				GetBValue(oldDr[xt + (yt + 1) * pImg->getwidth() + 1])) / 4;
			newDr[i * newWidth + j] = RGB(r, g, b);
		}
	}

	return newImg;
}

void PutImgWithout(
	IMAGE& obj,
	int px,
	int py,
	COLORREF withouter = WHITE,
	DWORD* pbWnd = GetImageBuffer(GetWorkingImage()),
	int wX = getwidth(),
	int wY = getheight(),
	DWORD bitsub = 0x00FFFFFF
)
{
	DWORD* pbImg = GetImageBuffer(&obj);
	int iX = obj.getwidth();
	int iY = obj.getheight();
	for (int i1 = 0; i1 < iX; i1++)
	{
		for (int i2 = 0; i2 < iY; i2++)
		{
			if (PointTsm(i1 + px, i2 + py, wX, wY) == -1)continue;						// ����Ƿ�Խ��
			if ((pbImg[PointTsm(i1, i2, iX, iY)] & bitsub) == BGR(withouter))continue;	// ����Ƿ�Ҫ�ų�����ɫ


			pbWnd[PointTsm(i1 + px, i2 + py, wX, wY)] = pbImg[PointTsm(i1, i2, iX, iY)]; // �����Դ�
		}
	}
}

//
// NOTE: ���� zoomImage ��������߶�Ϊ 1px ��ͼ��ʱ�������⣬
// ���Լ���������µ�ͼ�����캯��
//
// ͼƬ����
// width, height ������ͼƬ��С
// img ԭͼ��
void ImageToSize(int width, int height, IMAGE* img)
{
	IMAGE* pOldImage = GetWorkingImage();
	SetWorkingImage(img);

	IMAGE temp_image(width, height);

	StretchBlt(
		GetImageHDC(&temp_image), 0, 0, width, height,
		GetImageHDC(img), 0, 0,
		getwidth(), getheight(),
		SRCCOPY
	);

	Resize(img, width, height);
	putimage(0, 0, &temp_image);

	SetWorkingImage(pOldImage);
}

// С��ͼ�����Ͻǣ�
void DrawSmallMap()
{
	// ����̫С���������
	if (WIDTH < 500 || HEIGHT < 500)
	{
		return;
	}

	IMAGE img = Racing;
	SetWorkingImage(&img);

	// ��ʱ Player1��2 �Ѿ���ת������������ת
	PutImgWithout(Player1, Px, Py);
	if (TwoPlayer)
		PutImgWithout(Player2, Cx, Cy);

	SetWorkingImage();

	int w = 200, h = 200;
	int x = WIDTH - w;
	//ImageToSize(w, h, &img);
	img = zoomImage(&img, w, h);
	putimage(x, 0, &img);
	rectangle(x, 0, x + w, h);
}

// 2D ������ת
POINT Rotate2D(int x, int y, double radian)
{
	if (radian == 0)	return { x,y };
	return { (int)(x * cos(radian) - y * sin(radian)),
		(int)(x * sin(radian) + y * cos(radian)) };
}

// ��ȡС������ת��ĵ�ͼ�е�λ��
// NOTE: �ɷ�������ʱ 60ms ���ң����Ƽ�ʹ�á�
POINT GetRotatedCarPosition(int width, int height, int x, int y, double radian)
{
	IMAGE p(width, height), p2;
	int nw, nh;
	SetWorkingImage(&p);
	fillrectangle(x, y, x + 2, y + 2);
	rotateimage(&p2, &p, radian, BLACK, true, false);
	nw = p2.getwidth();
	nh = p2.getheight();
	SetWorkingImage();
	DWORD* buf = GetImageBuffer(&p2);
	for (int i = 0; i < nw * nh; i++)
	{
		if (buf[i] == WHITE)
		{
			return { i - (i / nw) * nw,i / nw };
		}
	}

	// Ѱ��ʧ��
	return { -1,-1 };
}

// ��ȡС������ת��ĵ�ͼ�е�λ��
// NOTE: �·�������ʱ <= 1ms��������
POINT GetRotatedCarPosition2(int width, int height, int x, int y, double radian)
{
	int left = 0, button = 0;
	POINT points[3] = { {width,0},{0,height},{width,height} };

	// ���� IMAGE ���������ϵ y �����£�ͨ������ϵ y �����ϣ�
	// ����תʱ��Ҫ���෴�Ƕ���ת�ŵ�Ч���� IMAGE ���������ϵ��������ת
	for (int j = 0; j < 3; j++)
	{
		points[j] = Rotate2D(points[j].x, points[j].y, -radian);
	}
	POINT p = Rotate2D(x, y, -radian);

	// ��¼��������µĶ˵�
	for (int j = 0; j < 3; j++)
	{
		if (points[j].x < points[left].x)
		{
			left = j;
		}
		if (points[j].y < points[button].y)
		{
			button = j;
		}
	}

	// ��ͼ����ת��˵�����Խ�磬���䲹������λ��
	if (points[left].x < 0)		p.x += -points[left].x;
	if (points[button].y < 0)	p.y += -points[button].y;

	return p;
}

// ��ȡ�����Ұ
IMAGE GetPlayerView(
	int pViewLength,
	int pNearViewWidth,
	int pFarViewWidth,
	int pRearViewLength,	// ����Ұ����
	IMAGE imgMap,			// ���г��ӵĵ�ͼ
	double radian,			// ���ӽǶ�
	POINT pOldCar			// ��������
)
{
	// ��Ұ���ű�
	double dViewZoomRatio = (pFarViewWidth - pNearViewWidth) / (double)pViewLength;
	IMAGE imgMapRotated;	// �Գ�Ϊ���������ת��ĵ�ͼ

	int nMapWdith = imgMap.getwidth();
	int	nMapHeight = imgMap.getheight();

	// ����ͼ��תΪ�Գ�����Ϊ������
	rotateimage(&imgMapRotated, &imgMap, radian, BLACK, true, false);

	int nRotatedMapWidth = imgMapRotated.getwidth();
	int nRotatedMapHeight = imgMapRotated.getheight();

	// ��ͼ��ת�󣬳�����ҲҪ��ת
	//POINT pCar = GetRotatedCarPosition(nMapWdith, nMapHeight, pOldCar.x, pOldCar.y, radian);
	POINT pCar = GetRotatedCarPosition2(nMapWdith, nMapHeight, pOldCar.x, pOldCar.y, radian);

	// �����Ұ
	IMAGE imgView(pFarViewWidth, pViewLength);

	// �ӳ���λ����ǰ����ȡͼ��
	// ����͸��Ч��
	if (isPerspectiveEffect)
	{
		DWORD* bufImgRotatedMap = GetImageBuffer(&imgMapRotated);
		for (int i = 0; i < pViewLength; i++)
		{
			// ��ǰ��һ����Ұ�ߵĿ��
			int nThisWidth = (int)(pNearViewWidth + dViewZoomRatio * i);

			// ��ȡ��ǰ��Ұ��ͼ�񡾷��� 1��
			//IMAGE imgThisLine;
			//SetWorkingImage(&imgMapRotated);
			//getimage(&imgThisLine, pCar.x - nThisWidth / 2, pCar.y - (i - pRearViewLength), nThisWidth, 1);

			// ��ȡ��ǰ��Ұ��ͼ�񡾷��� 2������΢��һ�㣩
			IMAGE imgThisLine(nThisWidth, 1);
			DWORD* bufThisLine = GetImageBuffer(&imgThisLine);
			for (int j = 0; j < nThisWidth; j++)
			{
				int lines_x = (pCar.y - (i - pRearViewLength)) * nRotatedMapWidth;
				int x = (pCar.x - nThisWidth / 2) + j;

				// Խ�磬���
				if (x < 0 || x > nRotatedMapWidth || lines_x + x > nRotatedMapWidth * nRotatedMapHeight || lines_x + x < 0)
				{
					bufThisLine[j] = BLACK;
				}
				else
				{
					bufThisLine[j] = bufImgRotatedMap[lines_x + x];
				}
			}

			// ����ǰ��Ұ�����������Ұ���
			IMAGE imgThisZoomLine = imgThisLine;
			ImageToSize(pFarViewWidth, 1, &imgThisZoomLine);

			// ����ǰ�߻�����Ұ
			SetWorkingImage(&imgView);
			putimage(0, pViewLength - i, &imgThisZoomLine);
		}
	}

	// �ر�͸�ӣ�������Ч���
	else
	{
		SetWorkingImage(&imgMapRotated);
		getimage(&imgView, pCar.x - pFarViewWidth / 2, pCar.y - pViewLength, pFarViewWidth, pViewLength + pRearViewLength);
	}

	SetWorkingImage();

	return imgView;
}


// α 3D ��������ӽ�
void Draw3D_PlayerView()
{
	int nViewLength = 400;		// ��Ұ����
	int nNearViewWidth = 100;	// ����Ұ���
	int nFarViewWidth = 250;	// Զ��Ұ���

	// ����Ұ���ȣ����ڿ������������ݳ��ӳ��ȶ�̬����

	// ˫��ģʽ��Ϊ��С���ģ���խ��Ұ
	if (TwoPlayer)
	{
		nViewLength /= 2;
		nNearViewWidth /= 2;
		nFarViewWidth /= 2;
	}

	IMAGE imgMap = Racing;	// ��ͼ���г���

	// ���������ͼ
	SetWorkingImage(&imgMap);
	rotateimage(&Player1, &car1, -PForward, WHITE, true, false);
	PutImgWithout(Player1, Px - car1.getwidth() / 2, Py - car1.getheight() / 2);
	if (TwoPlayer)
	{
		rotateimage(&Player2, &car2, -CForward, WHITE, true, false);
		PutImgWithout(Player2, Cx - car2.getwidth() / 2, Cy - car2.getheight() / 2);
	}

	// ��ȡ�����Ұ
	IMAGE imgRealView[2];
	imgRealView[0] = GetPlayerView(nViewLength, nNearViewWidth, nFarViewWidth, car1.getheight() * 2, imgMap, PForward + PI / 2, { Px,Py });
	if (TwoPlayer)
		imgRealView[1] = GetPlayerView(nViewLength, nNearViewWidth, nFarViewWidth, car2.getheight() * 2, imgMap, CForward + PI / 2, { Cx,Cy });

	// ʵ������Ļ����ʾ��ͼ����Ҫ����
	if (!TwoPlayer)
	{
		ImageToSize(WIDTH, HEIGHT, &imgRealView[0]);

		// �˷���̫��
		//imgRealView[0] = zoomImage(&imgRealView[0],WIDTH, HEIGHT);

		putimage(0, 0, &imgRealView[0]);
	}
	else
	{
		ImageToSize(WIDTH / 2, HEIGHT, &imgRealView[0]);
		ImageToSize(WIDTH / 2, HEIGHT, &imgRealView[1]);
		putimage(WIDTH / 2, 0, &imgRealView[0]);	// ��� 1 ��������������Է��ұ�
		putimage(0, 0, &imgRealView[1]);		// ��� 2 ���� WSAD�������

		setlinestyle(PS_SOLID, 2);
		line(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
		setlinestyle(PS_SOLID, 1);
	}
}

// ��ȡ��Ϸ�е��ٶȶ�Ӧ�� km/h ��λ���ٶ�
double GetKMSpeed(double speed)
{
	return speed *= 6;
}

wstring GetKMSpeedText(double speed)
{
	wstring str = to_wstring(GetKMSpeed(speed));
	str[str.length() - 5] = L'\0';
	return str.c_str() + (wstring)L" km/h";
}

// ��ȡ�ٶȶ�Ӧ����ɫ
COLORREF GetSpeedColor(double speed, double max)
{
	double stage = max / 3;

	if (speed >= stage * 2)
		return RED;
	else if (speed >= stage * 1)
		return RGB(250, 110, 0);	// ��ɫ
	else
		return WHITE;
}

// �����ٶ�
void DrawSpeed()
{
	settextstyle(50, 0, L"system");

	// �ң����һ
	settextcolor(GetSpeedColor(Pspeed, MaxSpeed));
	wstring str = GetKMSpeedText(Pspeed);
	int w = textwidth(str.c_str());
	int h = textheight(str.c_str());
	outtextxy(WIDTH - w, HEIGHT - h, str.c_str());

	// ����Ҷ�
	if (TwoPlayer)
	{
		settextcolor(GetSpeedColor(Cspeed, MaxSpeed));
		wstring str2 = GetKMSpeedText(Cspeed);
		int h2 = textheight(str2.c_str());
		outtextxy(0, HEIGHT - h2, str2.c_str());
	}

	settextstyle(16, 0, L"����");
	settextcolor(WHITE);
}

// ������ͨ�� 2D �����ӽ�
void Draw2DView()
{
	IMAGE img(Racing.getwidth(), Racing.getheight());
	SetWorkingImage(&img);

	putimage(0, 0, &Racing);
	rotateimage(&Player1, &car1, -PForward, WHITE, true, false);
	PutImgWithout(Player1, Px, Py);

	if (TwoPlayer)
	{
		rotateimage(&Player2, &car2, -CForward, WHITE, true, false);
		PutImgWithout(Player2, Cx, Cy);
	}

	SetWorkingImage();

	if (WIDTH != Racing.getwidth() || HEIGHT != Racing.getheight())
	{
		img = zoomImage(&img, WIDTH, HEIGHT);
		//ImageToSize(WIDTH, HEIGHT, &img);
	}

	putimage(0, 0, &img);
}

// ��ʤ����
void WinScene()
{
	IMAGE imgEmpty(nMaxW, 100);
	IMAGE imgFlag = imgWinFlag;
	IMAGE imgScores[2] = { imgEmpty ,imgEmpty };	// ����
	IMAGE imgPlayers[2] = { imgEmpty ,imgEmpty };	// �����Ϣ
	bool isSwap = false;	// �Ƿ񽻻������ 1��2 ��˳��

	if (dWidthZoom != 1)
	{
		ImageToSize(WIDTH, WIDTH * imgFlag.getheight() / imgFlag.getwidth(), &imgFlag);
	}

	// ����
	SetWorkingImage(&imgScores[0]);
	settextstyle(42, 0, L"����");
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, L"����");
	int nStarY = 10;	// ������ǵ���� Y ����
	for (int i = 3; i > 0; i--)
	{
		putimage(nMaxW - i * imgStar[0].getwidth(), nStarY, &imgStar[0]);
	}
	if (TwoPlayer)
	{
		imgScores[1] = imgScores[0];
	}

	// �����Ϣ
	for (int i = 0; i < TwoPlayer + 1; i++)
	{
		SetWorkingImage(&imgPlayers[i]);
		settextstyle(42, 0, L"����");
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);

		IMAGE img;
		int time;	// ��ʱ����λ����
		LPCTSTR strPlayer;

		if (i == 0)
		{
			img = Player1;
			time = Ptime;
			strPlayer = L"��� 1";
		}
		else
		{
			img = Player2;
			time = Ctime;
			strPlayer = L"��� 2";
		}

		int dstH = 50;
		int dstW = dstH * img.getwidth() / img.getheight();
		//img = zoomImage(&img, dstW, dstH);
		ImageToSize(dstW, dstH, &img);
		PutImgWithout(img, 50, 30);

		// ��ʱ
		int m = time / 1000 / 60;
		int s = (time - m * 1000 * 60) / 1000;
		wstring strM = to_wstring(m);
		wstring strS = to_wstring(s);
		if (strM.size() == 1)	strM = L"0" + strM;
		if (strS.size() == 1)	strS = L"0" + strS;
		wstring str = strM + L"��";
		str += strS + L"��";
		LPCTSTR text = str.c_str();

		outtextxy(200, 20, strPlayer);
		outtextxy(nMaxW - 350, 20, text);
	}

	// ����
	for (int i = 0; i < 2 && dWidthZoom != 1; i++)
	{
		imgScores[i] = zoomImage(&imgScores[i], WIDTH);
		imgPlayers[i] = zoomImage(&imgPlayers[i], WIDTH);

		//ImageToSize(WIDTH, WIDTH * imgScores[i].getheight() / imgScores[i].getwidth(), &imgScores[i]);
		//ImageToSize(WIDTH, WIDTH * imgPlayers[i].getheight() / imgPlayers[i].getwidth(), &imgPlayers[i]);
	}
	imgEmpty = zoomImage(&imgEmpty, WIDTH);


	//////// ��չʾ����ͼ��

	// ʤ��������
	mciSendString(_T("open res\\win.mp3 alias win"), NULL, 0, NULL);
	mciSendString(_T("play win repeat"), NULL, 0, NULL);

	SetWorkingImage();

	const int nStageNum = 6;

	// ͼ�����
	IMAGE* pStages[nStageNum] = { &imgFlag,&imgScores[0],&imgPlayers[0],&imgScores[1],&imgPlayers[1],&imgEmpty };
	int nOutY[nStageNum] = { (int)(100 * dHeightZoom) };	// ÿ��ͼ������ Y ����
	if (TwoPlayer && Ctime < Ptime)
	{
		isSwap = true;
		IMAGE* ps[nStageNum] = { &imgFlag,&imgScores[1],&imgPlayers[1],&imgScores[0],&imgPlayers[0],&imgEmpty };
		for (int i = 0; i < nStageNum; i++)	pStages[i] = ps[i];
	}
	for (int i = 1; i < nStageNum; i++)
	{
		nOutY[i] = nOutY[i - 1] + pStages[i - 1]->getheight();
	}

	clock_t tStart = 0;
	int nX[nStageNum] = { 0 };	// ÿ��ͼ���λ��
	for (int i = 0; i < nStageNum; i++)	nX[i] = -WIDTH;
	int nNum = 0;	// ���ڿ�ʼ�˶���ͼ������
	while (true)
	{
		// ÿ��һ��ʱ�俪ʼ�˶�һ��ͼ��
		if ((clock() - tStart) / (double)CLOCKS_PER_SEC >= 0.06)
		{
			if (nNum < nStageNum)
			{
				nNum++;
			}

			// ��û����Ҷ�����ôʣ�µ�ͼƬֱ�Ӹ������һ�����һ��ͼƬ������
			/*if (nNum == 3 && !TwoPlayer)
			{
				nNum = nStageNum;
			}*/

			tStart = clock();
		}

		// �ƶ���ǰ�����˶���ͼ��
		for (int i = 0; i < nNum; i++)
		{
			// ���Լ����˶�
			nX[i] += (int)(100 * dWidthZoom - nX[i] * 0.3);
			putimage((int)((nX[i] > 0 ? 0 : nX[i]) * dWidthZoom), nOutY[i], pStages[i]);
			FlushBatchDraw();
			if (nX[i] > 0 && i == nStageNum - 1)	// ȫ��ͼ���˶����
			{
				goto move_end;
			}
		}

		Sleep(50);
	}

move_end:

	Sleep(200);

	//////// �����Ǽ�

	mciSendString(_T("open res\\prize.mp3 alias prize"), NULL, 0, NULL);

	// ���ź������
	IMAGE imgStarZoom = dWidthZoom == 1 ? imgStar[1] : zoomImage(&imgStar[1], (int)(imgStar[1].getwidth() * dWidthZoom));
	for (int i = 0; i < TwoPlayer + 1; i++)
	{
		int p = isSwap ? !i : i;	// ��ǰ��ұ��

		int layer;	// ��¼����ͼ���ǵڼ���
		if (p == 0) if (!isSwap) layer = 1; else layer = 3;
		else		if (!isSwap) layer = 3; else layer = 1;

		int time;	// ��Һ�ʱ���룩
		if (p == 0)	time = Ptime;
		else		time = Ctime;
		time /= CLOCKS_PER_SEC;

		// �佱
		for (int j = 0; j < 3; j++)
		{
			if (time <= nLevelTime[j])
			{
				putimage(WIDTH - imgStarZoom.getwidth() * (j + 1), nOutY[layer] + (int)(nStarY * dHeightZoom), &imgStarZoom);
				FlushBatchDraw();
				Sleep(50);
				mciSendString(_T("play prize from 0"), NULL, 0, NULL);
				Sleep(300);
			}
		}

		mciSendString(_T("stop prize"), NULL, 0, NULL);
	}

	mciSendString(_T("close prize"), NULL, 0, NULL);

	settextstyle(30, 0, L"����");
	settextcolor(BLACK);
	button btnOK(0, (int)(WIDTH - 250 * dWidthZoom), nOutY[nStageNum - 1] + 10, (int)(200 * dWidthZoom), (int)(80 * dHeightZoom), L"Back");
	page mypage;
	mypage.botlist.push_back(btnOK);
	mypage.ShownPage();

	mciSendString(_T("stop win"), NULL, 0, NULL);
	mciSendString(_T("close win"), NULL, 0, NULL);
}

// ������Ϸ�г���
void Draw()
{
	cleardevice();
	settextcolor(WHITE);

	// ��������
	if (isUseOriginal)
	{
		Draw2DView();
		Sleep(10);
	}
	else
	{
		Draw3D_PlayerView();
		DrawSmallMap();
		DrawSpeed();
	}

	if (!isUseOriginal)
	{
		settextstyle(72, 0, L"����");
	}

	setbkmode(OPAQUE);

	if (Pwrong)
	{
		if (isUseOriginal)
		{
			outtextxy((int)((Px - 20) * dWidthZoom), (int)((Py - 20) * dHeightZoom), _T("��������"));
		}
		else
		{
			if (TwoPlayer)
			{
				outtextxy((int)(700 * dWidthZoom), 300, _T("��������"));
			}
			else
			{
				outtextxy(300, 300, _T("��������"));
			}
		}

		if ((Now - Ptime2) > 100)
		{
			Pwrong = false;
		}
	}
	if (TwoPlayer && Cwrong)
	{
		if (isUseOriginal)
		{
			outtextxy((int)((Cx - 20) * dWidthZoom), (int)((Cy - 20) * dHeightZoom), _T("��������"));
		}
		else
		{
			outtextxy((int)(300 * dWidthZoom), 300, _T("��������"));
		}

		if ((Now - Ctime2) > 100)
		{
			Cwrong = false;
		}
	}

	settextstyle(20, 0, L"system");
	setbkmode(TRANSPARENT);

	//���Ʒ���
	if (!Pover)
	{
		outtextxy(10, 10, (L"���1  " + to_wstring(Ppass) + L" / " + to_wstring(NeedR)).c_str());
		outtextxy(10, 30, (L"���1��ʱ " + to_wstring((Now - Start) / 1000) + L"." + to_wstring((Now - Start) % 1000) + L"s").c_str());
		if (Ppass == NeedR)
		{
			Pover = true;
			Ptime = Now - Start;
		}
	}
	else
	{
		outtextxy(10, 10, L"���1�����!");
		outtextxy(10, 30, (L"���1��ʱ " + to_wstring(Ptime / 1000) + L"." + to_wstring(Ptime % 1000) + L"s").c_str());
	}
	if (TwoPlayer) if (!Cover)
	{
		outtextxy(10, 50, (L"���2  " + to_wstring(Cpass) + L" / " + to_wstring(NeedR)).c_str());
		outtextxy(10, 70, (L"���2��ʱ " + to_wstring((Now - Start) / 1000) + L"." + to_wstring((Now - Start) % 1000) + L"s").c_str());
		if (Cpass == NeedR)
		{
			Cover = true;
			Ctime = Now - Start;
		}
	}
	else
	{
		outtextxy(10, 50, L"���2�����!");
		outtextxy(10, 70, (L"���2��ʱ " + to_wstring(Ctime / 1000) + L"." + to_wstring(Ctime % 1000) + L"s").c_str());
	}
	if (TwoPlayer && Cover && Pover)
	{
		if (Ctime > Ptime)outtextxy(10, 90, _T("���1��ʤ!"));
		else outtextxy(10, 90, _T("���2��ʤ!"));
		outtextxy(10, 110, _T("��Ϸ��������ESC�ص����˵�"));
	}
	else if (Pover)
	{
		outtextxy(10, 50, _T("��Ϸ��������ESC�ص����˵�"));
	}
	FlushBatchDraw();
}

// ������ʼ�����ĳ���
void StartWord()
{
	int tmp;
	COLORREF fontcol = 0x0;
	LOGFONT font;
	LOGFONT cha;
	gettextstyle(&font);
	cha = font;
	cha.lfHeight = 500;
	cha.lfWeight = FW_BOLD;
	settextstyle(&cha);
	settextcolor(fontcol);
	//ʵ�������𽥱��
	for (int i = 3; i > 0; i--)
	{
		Start = Now = clock();
		while ((Now - Start) < 1000)
		{
			cleardevice();

			if (isUseOriginal)
			{
				Draw2DView();
			}
			else
			{
				Draw3D_PlayerView();
				DrawSmallMap();
			}

			RECT r = { 0, 0, WIDTH, HEIGHT };
			drawtext(to_wstring(i).c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			Now = clock();
			tmp = 255 * (Now - Start) / 1000;
			fontcol = RGB(tmp, tmp, tmp);
			settextcolor(fontcol);
			FlushBatchDraw();
		}
	}
	Start = Now = clock();
	while ((Now - Start) < 500)
	{
		cleardevice();

		if (isUseOriginal)
		{
			Draw2DView();
		}
		else
		{
			Draw3D_PlayerView();
			DrawSmallMap();
		}

		Now = clock();
		RECT r = { 0, 0, WIDTH, HEIGHT };
		drawtext(_T("START��"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FlushBatchDraw();
	}
	settextstyle(&font);
	settextcolor(WHITE);
}
