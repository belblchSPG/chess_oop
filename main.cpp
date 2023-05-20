#include <iostream>
#include <iomanip>

enum class Color : int {

	DEFAULT = -1,
	BLACK = 0,
	WHITE = 1
};

class Figure {
private:

	char type_;

public:

	Figure() { type_ = 'd'; }

	void ChangeName(char new_type);

	char GetName() const;
};

void Figure::ChangeName(char new_type) { type_ = new_type; }
char Figure::GetName() const { return type_; }

class Point {

private:

	int row_;
	int col_;

public:
	Point();
	Point(int row, int col) : row_(row), col_(col) {}
	~Point() {}

	int GetRow() { return row_; }
	int GetCol() { return col_; }
};

Point::Point() { 
	row_ = -1;
	col_ = -1;
}

class Cell {

private:
	Point coordinate_;
	Color color_;
	Figure figure_;

public:
	Cell();
	Cell(Point coordinate): coordinate_(coordinate) { color_ = Color::DEFAULT; }
	Cell(Point coordinate, int color);
	bool CheckEmpty() const;

	Color GetColor() const;

	void SetColor(int new_color);

	void PlaceFigure(char type) { figure_.ChangeName(type); }
	friend std::ostream& operator << (std::ostream& out, const Cell& cell);
};

Cell::Cell() {
	
	color_ = Color::DEFAULT;
	coordinate_ =  Point(-1, -1);
}

Cell::Cell(Point coordinate, int color) { 
	coordinate_ = coordinate;
	switch (color)
	{
	case -1: {color_ = Color::DEFAULT; break; }
	case 0: {color_ = Color::BLACK; break; }
	case 1: {color_ = Color::WHITE; break; }
	default:
		break;
	}
}

bool Cell::CheckEmpty() const { return figure_.GetName() == 'd'; }

void Cell::SetColor(int new_color) {

	switch (new_color)
	{
	case -1:{
		color_ = Color::DEFAULT;
		break;
	}
	case 0:
	{
		color_ = Color::BLACK;
		break; 
	}
	case 1:{
		color_ = Color::WHITE;
		break;
	}
	default:
		break;
	}
}

Color Cell::GetColor() const { return color_; }

std::ostream& operator << (std::ostream& out, const Cell& cell) {
	
	if (cell.CheckEmpty()) {
		
		if (cell.GetColor() == Color::BLACK) { out << '*' << ' '; }
		else { out<< '-' << ' '; }
	}
	else { out << cell.figure_.GetName(); }
	return out;
}

class Board {

private:

	int size_;
	Cell** board_;

public:

	Board(int new_size);
	~Board() {}
	int GetBoardSize() const { return size_; }
	void PlaceFigure( Point& coord);
	void ReplaceFigure(Point& coord);
	friend std::ostream& operator << (std::ostream& out, const Board& board);
	Cell* operator [](int index) { return board_[index]; }
	const Cell* operator [](int index) const { return board_[index]; }
	bool CheckPositionVertically(Point& current) const;
	bool CheckPositionHorizontally(Point& current) const;
	bool CheckPosotionDiagonally(Point& current) const;
	bool CheckPosition(Point& current) const;
};

Board::Board(int new_size) {

	size_ = new_size;
	board_ = new Cell * [size_];
	int color = 1;
	for (int i = 0; i < size_; ++i) {

		board_[i] = new Cell[size_];
		for (int j = 0; j < size_; ++j) {

			if (color == 1) { board_[i][j] = Cell(Point(i,j), color ); color = 0; }
			else { board_[i][j] = Cell(Point(i, j), color); color = 1; }
		}
		if (color == 1) { color = 0; }
		else { color = 1; }
	}

}

void Board::PlaceFigure(Point& coord) { board_[size_ - coord.GetCol()][coord.GetRow() - 1].PlaceFigure('q'); }
void Board::ReplaceFigure(Point& coord) { board_[size_ - coord.GetCol()][coord.GetRow() - 1].PlaceFigure('d'); }

std::ostream& operator << (std::ostream& out, const Board& board)
{
	out << "\n";
	for (int i = 0; i < board.size_; i++)
	{
		out << std::setw(3) << board.size_ - i << " ";
		for (int j = 0; j < board.size_; j++)
		{
			out << ' ' << board.board_[i][j];
		}
		out << "\n";
	}
	out << "  ";
	const char* letters[8] = { "A","B","C","D","E","F","G","H" };
	out << "  ";
	for (int i = 0; i < board.size_; i++)
	{
		out << " " << letters[i % 8] << " ";
	}
	out << "\n\n";
	return out;
}

bool Board::CheckPositionHorizontally(Point& current) const {


	for (int i = 0; i < size_; ++i) {

		if (!board_[size_ - current.GetCol()][i].CheckEmpty()) { return true; }
	}
	return false;
}
bool Board::CheckPositionVertically(Point& current_position) const {


	for (int i = 0; i < size_; i++)
	{
		if (!board_[i][current_position.GetRow() - 1].CheckEmpty()) { return true; }

	}
	return false;
}
bool Board::CheckPosotionDiagonally(Point& current_position) const
{


	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			if (!(board_[i][j].CheckEmpty() && abs(i - size_ + current_position.GetCol()) == abs(j - current_position.GetRow() + 1))) { return true; }
		}
	}
	return false;
} 
bool Board::CheckPosition(Point& current) const {

	if (!(CheckPositionHorizontally(current) && CheckPositionVertically(current) && CheckPosotionDiagonally(current))) { return false; }
	return true;
};

class Queen : public Figure {

public:

	Queen() { ChangeName('q'); }
};

enum class Rules : int {

	Default = -1,
	QueensProblem = 0,
	ClassicalChess = 1,
};


class QueensProblem{

private:

	Board board;

public:

	QueensProblem();

	bool SolveQueensProblem(int col);
};

bool QueensProblem::SolveQueensProblem(int col) {

	if (col == board.GetBoardSize())
	{
		return true;
	}
	for (int j = 0; j < board.GetBoardSize(); j++)
	{
		Point position(col + 1, j + 1);
		if(board.CheckPosition(position))
		{
			board.PlaceFigure(position);
			if (SolveQueensProblem(col + 1))
			{
				return true;
			}
		}
		board.ReplaceFigure(position);
	}
	return false;
}

class Game {

private:

	Rules game_rules;

public:

	Game(int rules) {
		switch (rules)
		{
		case -1: game_rules = Rules::Default; break;
		case 0:game_rules = Rules::QueensProblem; break;
		case 1:game_rules = Rules::ClassicalChess; break;
		default:
			break;
		}
	}

	void start_game();
};

void Game::start_game() {

	if (game_rules == Rules::QueensProblem) {

		QueensProblem problem;
		problem.SolveQueensProblem(0);
	}
}

int main(){

	Game game(0);

	return 1;
}