#pragma once
#include <ctime>

struct Object
{
	float x;
	float y;
	float dx;
	float dy;
	int life;
	Object(): x(0),y(0),dx(0),dy(0),life(0) {}
	Object(float x, float y, float life) : x(x), y(y), life(life), dx(0), dy(0) {} // дл€ блоков
	Object(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy), life(0) {} // дл€ м€чика \ платформы
};

Object* arr;
int size = 0;
int Width = 50;
int Height = 25;
int Radius = 25;
int TopShift = 0;
int LeftShift = 80;
int PlatformWidth = Width * 3;
int destroy = 0;
float UserSpeed = 0;
int timer = 5;
int life = 3;

void StartGame(int level,HWND hWnd, Object& Ball) {
	life = 3;
	destroy = 0;
	Object* temp;
	UserSpeed = 0;
	Ball.x = 6;
	Ball.y = 12;
	Ball.dx = 0.1f;
	Ball.dy = 0.6f;
	switch (level)
	{
	case 1: {
		temp = new Object[100];
		size = 100;
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
			}
		}
		arr = temp;
		break;
	}
	case 2: {
		temp = new Object[100];
		size = 0;
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 0; j < 5; j++)
		{
			for (int i = 4; i < 6; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 0; j < 5; j++)
		{
			for (int i = 7; i < 10; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 5; j < 10; j++)
		{
			for (int i = 0; i < 10 - j; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 5; j < 10; j++)
		{
			for (int i = j; i < 10; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		arr = temp;
		break;
	}
	case 3: {
		temp = new Object[100];
		size = 0;
		for (int j = 0; j < 1; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 1; j < 9; j++)
		{
			for (int i = 0; i < 9; i+=2)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		for (int j = 9; j < 10; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				temp[10 * j + i] = Object(i, j, level);
				size++;
			}
		}
		arr = temp;
		break;
	}
	}
	SetTimer(hWnd, timer, 40, NULL);
}

void BallMove(HWND hWnd, Object& Ball, Object& Platform) {
	bool FirstDestroy = true;
	Ball.x += Ball.dx;
	Ball.y += Ball.dy;
	RECT rt;
	GetClientRect(hWnd, &rt);
	if (Ball.x <= 0 || Ball.x * Width + Radius >= rt.right) // стенки
	{
		Ball.dx *= -1;
	}
	if (Ball.y <= 0)// потолок
	{
		Ball.dy *= -1;
		Ball.y += 1;
	}
	if (Ball.y * Height + Radius >= rt.bottom) // пол
	{
		Ball.x = 6;
		Ball.y = 12;
		Ball.dy = 0.6f;
		life--;
	}
	if (Ball.y * Height + Radius >= Platform.y * Height && Platform.y * Height >= Ball.y * Height) // колизици€ с платформой
	{
		if (Platform.x * Width - Radius <= Ball.x * Width && Platform.x * Width + PlatformWidth >= Ball.x * Width)
		{
			Ball.dy *= -1;
			Ball.y -= 1;
		}
	}
	for (int i = 0; i < 100; i++) // колизи€ м€чик и блок 
	{
		if (arr[i].life > 0)
		{
			// поиск соприкосновени€ по y
			if ((arr[i].y * Height + TopShift + Height > Ball.y * Height &&
				arr[i].y * Height + TopShift + Height < Ball.y * Height + Radius) // м€чик снизу
				||
				(arr[i].y * Height + TopShift > Ball.y * Height &&
				arr[i].y * Height + TopShift < Ball.y * Height + Radius)) // м€чик сверху
			{
				// поиск соприкосновени€ по х
				if (arr[i].x * Width + LeftShift >= Ball.x * Width &&
					arr[i].x * Width + LeftShift <= Ball.x * Width + Radius // м€чик слева
					||
					(arr[i].x * Width + LeftShift + Width >= Ball.x * Width &&
					arr[i].x * Width + LeftShift + Width <= Ball.x * Width + Radius) // м€чик справа
					|| 
					arr[i].x * Width + LeftShift < Ball.x * Width && 
					Ball.x * Width + Radius < arr[i].x * Width + LeftShift + Width) // м€чик внутри
				{
					arr[i].life--;
					destroy++;
					
					if (FirstDestroy)
					{
						Ball.dy *= -1;
						Ball.dx *= -1;
						srand(time(NULL));
						float RandomizeX = ((rand() % 15)) / 100.0 + 0.25f;
						float RandomizeY = ((rand() % 15)) / 100.0 + 0.4f;
						Ball.dx = Ball.dx >= 0 ? RandomizeX : RandomizeX * -1;
						Ball.dy = Ball.dy >= 0 ? RandomizeY : RandomizeY * -1;
						Ball.dy += Ball.dy >= 0 ? UserSpeed : UserSpeed * -1;
						FirstDestroy = false;
					}
					if (destroy == size)
					{
						KillTimer(hWnd, timer);
						MessageBox(hWnd, L"¬се блоки уничтожены", L"ѕобеда!", MB_OK);
						return;
					}
				}
			}
		}
		
	}
	
}