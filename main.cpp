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

struct move {
	int pos;
	bool add;

	move(int POS, bool ADD) {
		pos = POS;
		add = ADD;
	}
};

std::vector<move> moves;

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

void draw() {
	cleardevice();
	drawBoard();
	drawQueens();
	calcAttack();
	drawAttacks();
}

void undo(move MOVE) {
	int pos = MOVE.pos;
	queens[pos] = !MOVE.add;
}

void initialize() {
	moves.clear();
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
	cols = std::stoi(COLS);
	squareSize = std::stoi(SQUARESIZE);
	queenSize = std::stoi(QUEENSIZE);
	attackSize = std::stoi(ATTACKSIZE);

	inputFile.close();

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
				if (msg.vkcode == 0x5A) { // z
					if (moves.size() > 0) {
						undo(moves.back());
						moves.pop_back();
					}
				}
				if (msg.vkcode == 0x52) { // r
					initialize();
					draw();
				}
			}

			if (msg.message == WM_LBUTTONDOWN) {
				int col = msg.x/squareSize;
				int row = msg.y/squareSize;
				int pos = row * cols + col;
				if (queens[pos]) { // queen exist
					queens[pos] = false;
					moves.push_back(move(pos, false));
				}
				else {
					queens[pos] = true;
					moves.push_back(move(pos, true));
				}
			}
		}

		draw();
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

	return 0;
}