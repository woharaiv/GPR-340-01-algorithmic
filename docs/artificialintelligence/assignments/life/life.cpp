#include <iostream>
#include <vector>

using namespace std;

struct Board {
  vector<bool> board;
  int width, height;
  bool set(int x, int y, bool val) {
    return (board[y * width + x] = val);
  }
};

void InitializeBoard(bool**, int, int);
void PrintBoard(bool**, int, int);
void SwapBuffer(bool**& board, bool**& buffer);
void StepBoard(bool**, bool**, int, int);
int CountNeighbors(bool** board, int checkRow, int checkCol, int totalRows, int totalCols);


int main()
{
  int cols;
  int rows;
  int turns;

  cin >> cols >> rows >> turns;
  bool **board = new bool*[rows];
  bool **buffer = new bool*[rows];
  for (int i = 0; i < rows; i++)
  {
    board[i] = new bool[cols];
    buffer[i] = new bool[cols];
  }
  InitializeBoard(board, rows, cols);
  for(int i = 0; i < turns; i++) {
    SwapBuffer(board, buffer);
    StepBoard(board, buffer, rows, cols);
  }
  cout << endl;
  PrintBoard(board, rows, cols);

  delete board;
  delete buffer;
  return 0;
}

void InitializeBoard(bool** board, int rows, int cols)
{
  string rowInput;
  for(int i = 0; i < rows; i++)
  {
    cin >> rowInput;
    //cout << endl;
    //cout << std::to_string(i) << " ";
    for(int j = 0; j < cols; j++)
    {
      switch(rowInput[j])
      {
        case '#':
          board[i][j] = true;
          //cout << "#";
          break;
        case '.':
        default:
          board[i][j] = false;
          //cout << ".";
          break;
      }
    }
  }
}

void SwapBuffer(bool**& board, bool**& buffer) {
  bool** temp = board;
  board = buffer;
  buffer = temp;
}

void StepBoard(bool** board, bool** buffer, int rows, int cols) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      //Switch(CountNeighbors(buffer, row, col, totalRows, totalCols))
      //case 0, 1, 4: board[cell] = false;
      //case 2: board[cell] = cell;
      //case 3: board[cell] = true;
    }
  }
}

int CountNeighbors(bool** board, int checkRow, int checkCol, int totalRows, int totalCols) {
  int count = 0;

  return count;
}

void PrintBoard(bool** board, int rows, int cols) {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      if(board[i][j])
        cout << '#';
      else
        cout << '.';
    }
    cout << endl;
  }
}