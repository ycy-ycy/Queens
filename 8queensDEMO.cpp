#include <graphics.h>
#include <vector>

int rows = 8;
int cols = 8;

int squareSize = 50;
int queenSize = 40;
int attackSize = 40;

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
	int col_row[8];
	COLORREF bkColor = BLACK;
	int row = 0;
	int pos;

	void restart() {
		memset(col_row, 0, 8 * sizeof(int));
		bkColor = BLACK;
		row = 0;
	}

	bool next(int ROW, bool forcase = false) {

		if (row < 0) { return false; }
		if (ROW != row) { return next(ROW + 1, forcase); }

		for (; col_row[row] <= 8; col_row[row]++) {
			if (col_row[row] == 8) { // run out of columns
				bkColor = RED;
				row--; // start with previous row
				return true;
			}
			pos = row * cols + col_row[row];
			if (queens[pos]) { // remove existing queen and continue
				queens[pos] = false;
				calcAttack();
				if (!forcase) { // for nextPiece()
					col_row[row]++;
					bkColor = BLACK;
					return true;
				}
			}
			else {
				if (!attack[pos]) { // safe to add
					queens[pos] = true;
					calcAttack();
					if (row == 7) { // passed
						bkColor = GREEN;
						return true;
					}
					bkColor = BLACK;
					row++;
					col_row[row] = 0;
					if (!forcase) { // for nextPiece()
						return true;
					}
					bool result = next(row, forcase); // continue with next row
					if (result) { return true; }
				}
			}
		}
	}

	void nextPiece() {
		next(0);
	}

	void nextCase() {
		next(0, true);
	}
}

void initialize() {
	SIMULATION::restart();
	memset(queens, false, rows * cols);
	memset(attack, false, rows * cols);
}

int main() {

	queens = new bool[rows * cols];
	attack = new bool[rows * cols];
	initialize();

	initgraph(squareSize * cols, squareSize * rows, EX_NOCLOSE);

	BeginBatchDraw;

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
					SIMULATION::nextCase();
				}
			}
			if (msg.message == WM_LBUTTONDOWN) {
				SIMULATION::nextPiece();
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

	return 0;
}