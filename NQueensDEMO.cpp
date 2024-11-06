#include <graphics.h>
#include <vector>
#include <string>
#include <fstream>

std::ifstream inputFile("config.txt");

std::string ROWS;
std::string COLS;
std::string SQUARESIZE;
std::string QUEENSIZE;
std::string ATTACKSIZE;

int rows;
int cols;

int squareSize;
int queenSize;
int attackSize;

bool* queens;
bool* attack;

void drawAttack(int row, int col) {
	line(col * squareSize + squareSize / 2 - attackSize / 2,
		row * squareSize + squareSize / 2 - attackSize / 2,
		col * squareSize + squareSize / 2 + attackSize / 2,
		row * squareSize + squareSize / 2 + attackSize / 2);
	line(col * squareSize + squareSize / 2 - attackSize / 2,
		row * squareSize + squareSize / 2 + attackSize / 2,
		col * squareSize + squareSize / 2 + attackSize / 2,
		row * squareSize + squareSize / 2 - attackSize / 2);
}

void calcAttack() {
	memset(attack, false, rows * cols);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int pos = i * cols + j;
			if (queens[pos]) {
				int k, l;
				for (k = j + 1; k < cols; k++) {
					int attackPos = i * cols + k;
					attack[attackPos] = true;
				}
				for (k = i + 1; k < rows; k++) {
					int attackPos = k * cols + j;
					attack[attackPos] = true;
				}
				for (k = j - 1; k >= 0; k--) {
					int attackPos = i * cols + k;
					attack[attackPos] = true;
				}
				for (k = i - 1; k >= 0; k--) {
					int attackPos = k * cols + j;
					attack[attackPos] = true;
				}
				k = j + 1;
				l = i + 1;
				while (k < cols && l < rows) {
					int attackPos = l * cols + k;
					attack[attackPos] = true;
					k++;
					l++;
				}
				k = j - 1;
				l = i + 1;
				while (k >= 0 && l < rows) {
					int attackPos = l * cols + k;
					attack[attackPos] = true;
					k--;
					l++;
				}
				k = j + 1;
				l = i - 1;
				while (k < cols && l >= 0) {
					int attackPos = l * cols + k;
					attack[attackPos] = true;
					k++;
					l--;
				}
				k = j - 1;
				l = i - 1;
				while (k >= 0 && l >= 0) {
					int attackPos = l * cols + k;
					attack[attackPos] = true;
					k--;
					l--;
				}
			}
		}
	}
}

void drawAttacks() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (attack[i * cols + j]) {
				drawAttack(i, j);
			}
		}
	}
}

void drawQueen(int row, int col) {
	circle(col * squareSize + squareSize / 2, row * squareSize + squareSize / 2, queenSize / 2);
}

void drawQueens() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (queens[i * cols + j]) {
				drawQueen(i, j);
			}
		}
	}
}

void drawBoard() {
	for (int i = 1; i < rows; i++) {
		line(0, i * squareSize, squareSize * cols, i * squareSize);
	}
	for (int i = 1; i < cols; i++) {
		line(i * squareSize, 0, i * squareSize, squareSize * rows);
	}
}

void draw(COLORREF bkColor) {
	cleardevice();
	setbkcolor(bkColor);
	drawBoard();
	drawQueens();
	calcAttack();
	drawAttacks();
}

namespace SIMULATION {
	int n;
	int* col_row;
	COLORREF bkColor = BLACK;
	int row = 0;
	int pos;

	void restart() {
		memset(col_row, 0, n * sizeof(int));
		bkColor = BLACK;
		row = 0;
	}

	void next(int ROW, int CASE = 0) {

		if (row < 0) { return; }
		if (ROW != row) { return next(ROW + 1, CASE); }

		for (; col_row[row] <= n; col_row[row]++) {
			if (col_row[row] == n) { // run out of columns
				bkColor = RED;
				row--; // start with previous row
				if (CASE != 2) {
					if (row < 0) {
						restart();
					}
					return;
				}
				else {
					if (row < 0) {
						restart();
						return;
					}
				}
			}
			pos = row * cols + col_row[row];
			if (queens[pos]) { // remove existing queen and continue
				queens[pos] = false;
				calcAttack();
				if (CASE == 0) { // for nextPiece()
					col_row[row]++;
					bkColor = BLACK;
					return;
				}
			}
			else {
				if (!attack[pos]) { // safe to add
					queens[pos] = true;
					calcAttack();
					if (row + 1 == n) { // passed
						bkColor = GREEN;
						return;
					}
					bkColor = BLACK;
					row++;
					col_row[row] = 0;
					if (CASE == 0) { // for nextPiece()
						return;
					}
					return next(row, CASE); // continue with next row
				}
			}
		}
	}
}

void initialize() {
	SIMULATION::restart();
	memset(queens, false, rows * cols);
	memset(attack, false, rows * cols);
}

int main() {

	std::getline(inputFile, ROWS);
	std::getline(inputFile, COLS);
	std::getline(inputFile, SQUARESIZE);
	std::getline(inputFile, QUEENSIZE);
	std::getline(inputFile, ATTACKSIZE);

	rows = std::stoi(ROWS);
	cols = rows;
	squareSize = std::stoi(SQUARESIZE);
	queenSize = std::stoi(QUEENSIZE);
	attackSize = std::stoi(ATTACKSIZE);

	inputFile.close();

	SIMULATION::n = rows;
	SIMULATION::col_row = new int[rows];

	queens = new bool[rows * cols];
	attack = new bool[rows * cols];
	initialize();

	initgraph(squareSize * cols, squareSize * rows, EX_NOCLOSE);

	BeginBatchDraw();

	bool running = true;

	while (running) {

		int start = GetTickCount();

		ExMessage msg;
		while (peekmessage(&msg)) {
			if (msg.message == WM_KEYDOWN) {
				if (msg.vkcode == 0x1b) { // esc
					running = false;
					break;
				}
				if (msg.vkcode == 0x52) { // r
					initialize();
				}
				if (msg.vkcode == 0x20) { // space
					SIMULATION::next(0, 1);
				}
				if (msg.vkcode == 0x46) { // f
					SIMULATION::next(0, 2);
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				SIMULATION::next(0, 0);
			}
		}

		draw(SIMULATION::bkColor);
		FlushBatchDraw();

		int delta_t = GetTickCount() - start;
		if (delta_t < 16) {
			Sleep(16 - delta_t);
		}
	}

	EndBatchDraw();
	closegraph();

	delete[] queens;
	delete[] attack;
	delete[] SIMULATION::col_row;

	return 0;
}