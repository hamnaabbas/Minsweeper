#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

class Minesweeper {
private:
    int rows, cols, totalMines;
    char** board;
    char** displayBoard;
    bool** revealed;
    bool gameOver;
    time_t startTime, endTime;

    void initializeBoard() {
        board = new char*[rows];
        displayBoard = new char*[rows];
        revealed = new bool*[rows];
        for (int i = 0; i < rows; i++) {
            board[i] = new char[cols];
            displayBoard[i] = new char[cols];
            revealed[i] = new bool[cols];
            for (int j = 0; j < cols; j++) {
                board[i][j] = '0';
                displayBoard[i][j] = '-';
                revealed[i][j] = false;
            }
        }
        placeMines();
        calculateNumbers();
    }

    void placeMines() {
        srand(time(0));
        int minesPlaced = 0;
        while (minesPlaced < totalMines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (board[r][c] != 'M') {
                board[r][c] = 'M';
                minesPlaced++;
            }
        }
    }

    void calculateNumbers() {
        int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] == 'M') continue;
                int minesCount = 0;
                for (int i = 0; i < 8; i++) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == 'M') {
                        minesCount++;
                    }
                }
                board[r][c] = minesCount + '0';
            }
        }
    }

    void reveal(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c]) return;
        revealed[r][c] = true;
        displayBoard[r][c] = board[r][c];
        if (board[r][c] == '0') {
            int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
            for (int i = 0; i < 8; i++) {
                reveal(r + dr[i], c + dc[i]);
            }
        }
    }

    bool checkWin() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] != 'M' && !revealed[r][c]) return false;
            }
        }
        return true;
    }

    void printBoard() {
        cout << "  ";
        for (int c = 0; c < cols; c++) cout << setw(2) << c;
        cout << endl;
        for (int r = 0; r < rows; r++) {
            cout << setw(2) << r;
            for (int c = 0; c < cols; c++) {
                cout << setw(2) << displayBoard[r][c];
            }
            cout << endl;
        }
    }

public:
    Minesweeper(int r = 7, int c = 7, int mines = 15) : rows(r), cols(c), totalMines(mines), gameOver(false) {
        initializeBoard();
        startTime = time(0);
    }

    ~Minesweeper() {
        for (int i = 0; i < rows; i++) {
            delete[] board[i];
            delete[] displayBoard[i];
            delete[] revealed[i];
        }
        delete[] board;
        delete[] displayBoard;
        delete[] revealed;
    }

    void play() {
        while (!gameOver) {
            printBoard();
            int r, c;
            char action;
            cout << "Enter row and column (e.g., 3 4) and action (r for reveal, f for flag, ? for unsure): ";
            cin >> r >> c >> action;
            if (action == 'r') {
                if (board[r][c] == 'M') {
                    gameOver = true;
                    cout << "Game Over! You hit a mine!" << endl;
                } else {
                    reveal(r, c);
                    if (checkWin()) {
                        gameOver = true;
                        endTime = time(0);
                        cout << "Congratulations! You have cleared all safe squares!" << endl;
                        cout << "Time taken: " << difftime(endTime, startTime) << " seconds." << endl;
                    }
                }
            } else if (action == 'f') {
                displayBoard[r][c] = 'F';
            } else if (action == '?') {
                displayBoard[r][c] = '?';
            }
        }
    }
};

int main() {
    int rows, cols, mines;
    cout << "Welcome to Minesweeper!" << endl;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    cout << "Enter the number of mines: ";
    cin >> mines;
    Minesweeper game(rows, cols, mines);
    game.play();
    return 0;
}
