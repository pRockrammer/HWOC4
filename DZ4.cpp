#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>
using namespace sf;
#define pi 3.14159265 

int find(std::vector<int> vect, int n) {
	for (int i = 0; i < vect.size(); i++) {
		if (vect.at(i) == n)
			return 1;
	}
	return 0;
}
int findIndex(std::vector<Vector2f> vect, Vector2f point) {
	for (int i = 0; i < vect.size(); i++) {
		if (vect.at(i).x == point.x && vect.at(i).y == point.y) {
			return i;
		}
	}
	return -1;
}
int comparePoints(Vector2f p1, Vector2f p2) {
	return (p1.x == p2.x && p1.y == p2.y) ? 1 : 0;
}
int countOf(std::vector<int> vect, int a) {
	int summ = 0;
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect.at(i) == a)
			summ++;
	}
	return summ;
}
int main(int len, char* args[])
{
	// создаём окно
	RenderWindow app(VideoMode(800, 600, 32), "HWOC2");
	int angle = 90;
	Vector2i mousepos(0, 0);
	if (len > 1)
		angle = 360 / (len - 2);
	else {
		len = 5;
	}
	std::string selectedTop = args[len - 1];
	int selectedTopIndex = 0;
	for (int i = 1; i < len - 2; i++) {
		if (selectedTop == args[i]) {
			selectedTopIndex = i - 1;
			break;
		}
	}
	bool powerFlag = false;
	std::vector<int> powerTop;
	std::vector<int> newIndex;
	std::vector<int> oldPowers;
	std::vector<Vector2f> selected;
	std::vector<std::vector<int>> communicatedMatrix;
	for (int i = 0; i < len - 2; i++) {
		std::vector<int> help;
		powerTop.push_back(0);
		oldPowers.push_back(0);
		newIndex.push_back(0);
		for (int j = 0; j < len - 2; j++) {
			help.push_back(0);
		}
		communicatedMatrix.push_back(help);
	}
	//int countOfAll = (factorial(len - 2)) / (2 * factorial(len-1 - 2));//число сочетаний из n по 2
	// основной цикл
	Font font;
	if (!font.loadFromFile("ArialBlack.ttf")) {
		printf("Error!");
		return -1;
	}
	Text text;
	text.setFont(font);
	bool rightClick = false;
	while (app.isOpen())
	{
		// проверяем события (нажатие кнопки, закрытие окна и т.д.)
		Event event;
		while (app.pollEvent(event))
		{
			// если событие "закрытие окна":
			if (event.type == Event::Closed)
				// закрываем окно 
				app.close();
		}
		if (Mouse::isButtonPressed(Mouse::Left)) {
			Mouse ms;
			mousepos = ms.getPosition(app);
		}
		if (Mouse::isButtonPressed(Mouse::Right)) {
			rightClick = true;
			powerFlag = true;
		}
		// очищаем экран и заливаем его белым цветом
		std::vector<Vector2f> lst;
		app.clear(Color(255, 255, 255));
		CircleShape circle(10);
		circle.setFillColor(Color(255, 0, 0));
		for (int i = 0; i < 360/* && !rightClick*/; i += angle) {
			circle.setPosition(150 + 100 * cos(i * pi / 180) + 100 - 10, 150 - 100 * sin(i * pi / 180) + 100 - 10);
			lst.push_back(Vector2f(150 + 100 * cos(i * pi / 180) + 100 - 10, 150 - 100 * sin(i * pi / 180) + 100 - 10));
			app.draw(circle);
		}
		for (int i = 0; i < len - 2 && !rightClick; i++) {
			if (mousepos.x >= (lst.at(i).x + 10 - 20) && mousepos.x <= ((lst.at(i).x + 10 + 20)) && mousepos.y >= (lst.at(i).y + 10 - 20) && mousepos.y <= (lst.at(i).y + 10 + 20)) {
				//если ткнули в круг то..
				if (selected.size() == 0 || selected.size() > 0 && selected.at(selected.size() - 1) != lst.at(i)) {
					selected.push_back(lst.at(i));
					printf("Cords are right!\n");

					if (selected.size() % 2 == 0) {
						int a = findIndex(lst, selected.at(selected.size() - 1));
						int b = findIndex(lst, selected.at(selected.size() - 2));
						communicatedMatrix[a][b] = 1;
						communicatedMatrix[b][a] = 1;
						powerFlag = true;
					}
				}
			}
		}
		if (rightClick) {
			for (int j = 0; j < len - 2; j++) {
				communicatedMatrix[selectedTopIndex][j] = 0;
				communicatedMatrix[j][selectedTopIndex] = 0;
			}
		}
		for (int i = 0; i < len - 2 && powerFlag; i++) {
			for (int j = 0; j < len - 2; j++) {
				if (communicatedMatrix[i][j] == 1)
					powerTop[i] += 1;
				std::cout << communicatedMatrix[i][j] << " ";
			}
			std::cout << "Top : " << i << " Power of top : " << powerTop[i] << std::endl;
			std::cout << "\n";
		}
		if (rightClick) {
			//int ind = findIndex(selected, lst[selectedTopIndex]);
			//selected.erase(selected.begin()+ind, selected.begin()+ind+2);
			for (int i = 0; i < len - 2; i++)
			{
				oldPowers.at(i) = powerTop.at(i);
			}
			std::sort(powerTop.begin(), powerTop.end());
			//приведение соответствия между старыми массивом и новым
			int* counts = (int*)malloc((powerTop.at(powerTop.size() - 1) + 1) * sizeof(int));
			for (int i = 0; i < powerTop.at(powerTop.size() - 1) + 1; i++) {
				counts[i] = countOf(powerTop, i);
			}
			for (int k = 0; k < len - 2; k++)
			{
				int count = countOf(powerTop, oldPowers[k]) - counts[oldPowers[k]];
				for (int l = 0; l < len - 2; l++) {
					if (countOf(powerTop, oldPowers[k]) > 1) {

						if (oldPowers[k] == powerTop[l]) {
							if (count > 0) {
								count--;
								continue;
							}
							else {
								newIndex[k] = l/* + countOf(powerTop, oldPowers[k]) - counts[oldPowers[k]]*/;
								break;
							}
						}
					}
					else {
						if (oldPowers[k] == powerTop[l]) {
							newIndex[k] = l;
							break;
						}
					}
				}
				counts[oldPowers[k]] -= 1;
			}
			//rightClick = false;
		}
		//countOf(powerTop, powerTop.at(powerTop.size() - 1 - i))

		for (int i = 0; i < len - 2; i++) {
			for (int j = 0; j < len - 2; j++) {
				if (communicatedMatrix[i][j] == 1) {
					Vertex line[] = {
						Vertex(Vector2f(lst.at(i).x + 10, lst.at(i).y + 10)), Vertex(Vector2f(lst.at(j).x + 10, lst.at(j).y + 10))//error
					};
					line[0].color = Color::Black;
					line[1].color = Color::Black;
					app.draw(line, 2, Lines);
				}
			}
		}
		text.setString(std::to_string(mousepos.x) + ":" + std::to_string(mousepos.y));
		text.setCharacterSize(10);
		text.setFillColor(Color::Black);
		text.setPosition(Vector2f(0, 0));
		app.draw(text);

		for (int i = 1; i < len - 1/* && len != 5 && !rightClick*/; i++) {
			text.setString(args[i]);
			text.setPosition(lst.at(i - 1));
			app.draw(text);
		}
		//for (int i = 1; i < len - 2/* && len != 5*/ && rightClick; i++) {
		//	text.setString(args[i]);
		//	text.setPosition(lst.at(lst.size() -1 -i));
		//	app.draw(text);
		//}
		if (rightClick) {
			std::cout << "top : power of top" << std::endl;
			std::vector<std::string> strsootv;
			for (int j = 0; j < newIndex.size(); j++) {
				strsootv.push_back(" ");
			}
			for (int j = 0; j < newIndex.size(); j++) {
				strsootv.at(newIndex[j]) = args[j + 1];
			}
			for (int i = newIndex.size()-1; i >= 0; i--)
			{
				std::cout << /*args[newIndex.at(i) + 1]*/strsootv.at(i) << " : " << powerTop.at(i) << std::endl;
			}

		}
		for (int i = 0; i < len - 2 /*&& !rightClick*/; i++) {
			powerTop[i] = 0;
		}
		rightClick = false;
		lst.clear();
		powerFlag = false;
		app.display();
	}

	return 0;
}
