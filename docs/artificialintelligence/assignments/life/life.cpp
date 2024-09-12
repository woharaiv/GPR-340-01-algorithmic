#include <iostream>
#include <vector>

using namespace std;

struct Board {
  vector<bool> board;
  int width = 0;
  int height = 0;
  Board(int width_, int height_) {
    width = width_;
    height = height_;
    board.resize(width * height, false);
  }
public:
  int GetWidth() { return width; }
  int GetHeight() { return height; }

  bool set(int x, int y, bool val) {
    if(y >= height || x < 0 || y < 0 || x >= width)
      throw std::invalid_argument("Position is out of bounds");
    return (board[y * width + x] = val);
  }
  bool get(int x, int y) {
    if(y >= height || x < 0 || y < 0 || x >= width)
      throw std::invalid_argument("Position is out of bounds");
    return board[y * width + x];
  }

  void SetInitialState() {
    string rowInput;
    for(int y = 0; y < width; y++)
    {
      cin >> rowInput;
      //cout << endl;
      //cout << std::to_string(i) << " ";
      for(int x = 0; x < height; x++)
      {
        switch(rowInput[x])
        {
          case '#':
            set(x, y, true);
            //cout << "#";
          break;

          case '.':
          default:
            set(x, y, false);
            //cout << ".";
          break;
        }
      }
    }
  }

  int CountNeighbors(int x, int y) {
    int count = 0;
    if(y >= height || x < 0 || y < 0 || x >= width)
      throw std::invalid_argument("Position is out of bounds");
    int spaceNorth = (y > 0 ? y - 1 : height - 1);
    int spaceEast = (x < width - 1 ? x + 1 : 0);
    int spaceSouth = (y < height - 1 ? y + 1 : 0);
    int spaceWest = (x > 0 ? x - 1 : width - 1);

    //North
    if(get(x, spaceNorth))
      count++;
    //Northeast
    if(get(spaceEast, spaceNorth))
        count++;
    //East
    if(get(spaceEast, y))
      count++;
    //Southeast
    if(get(spaceEast, spaceSouth))
      count++;
    //South
    if(get(x, spaceSouth))
      count++;
    //Southwest
    if(get(spaceWest, spaceSouth))
      count++;
    //West
    if(get(spaceWest, y))
      count++;
    //Northwest
    if(get(spaceWest, spaceNorth))
      count++;

    return count;
  }

  void Print() {
    for(int y = 0; y < height; y++) {
      for(int x = 0; x < width; x++) {
        if(get(x, y))
          cout << '#';
        else
          cout << '.';
      }
      cout << endl;
    }
  }

  void ConwayStep(Board* buffer, bool printDebug = false) {
    for(int y = 0; y < height; y++) {
      for(int x = 0; x < width; x++) {
        bool initialState = get(x, y);
        if(printDebug)
          cout << "Tile (" << to_string(x) << ',' << to_string(y) << ")";
        switch(CountNeighbors(x, y)) {
          //Dies of loneliness
          case 0:
          case 1:
            if(printDebug) {
              if(initialState)
                cout << " dies of loneliness" << endl;
              else
                cout << " remains dead" << endl;
            }
            buffer->set(x, y, false);
          break;

          //Continues to live
          case 2:
            if(printDebug) {
              if(initialState)
                cout << " continues to live" << endl;
              else
                cout << " remains dead" << endl;
            }
            buffer->set(x, y, this->get(x,y));
          break;

          //Neighbors reproduce
          case 3:
            if(printDebug) {
              if(initialState)
                cout << " continues to live" << endl;
              else
                cout << "'s neighbors reproduce'" << endl;
            }
            buffer->set(x, y, true);
          break;

          //Too many neighbors
          default:
            if(printDebug) {
              if(initialState)
                cout << " dies of overpopulation" << endl;
              else
                cout << "remains dead" << endl;
            }
            buffer->set(x, y, false);
          break;
        }
      }
    }
  }

};

void SwapBuffer(Board*& board, Board*& buffer);


int main()
{
  int cols;
  int rows;
  int turns;

  cin >> cols >> rows >> turns;
  Board *board = new Board(cols, rows);
  Board *buffer = new Board(cols, rows);
  board->SetInitialState();
  for(int i = 0; i < turns; i++) {
    board->ConwayStep(buffer);
    SwapBuffer(board, buffer);
  }

  cout << endl;
  board->Print();

  delete board;
  delete buffer;
  return 0;
}

void SwapBuffer(Board*& board, Board*& buffer) {
  Board* temp = board;
  board = buffer;
  buffer = temp;
}