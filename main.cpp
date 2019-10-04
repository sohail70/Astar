#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using std::vector;
using std::cout;
using std::string;
using std::ifstream;
using std::istringstream;
using std::sort;

enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

string CellString(State cell)
{
	switch (cell)
	{
	case State::kObstacle: return  "#   ";
	case State::kPath: return  "P   ";
	case State::kStart: return  "S   ";
	case State::kFinish: return  "G   ";
	default: return "0   ";
	}
}

void PrintBoard(vector<vector<State>> board)
{
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << CellString(board[i][j]);
		}
		cout << "\n";
	}
}
//for input streams: draw something from it with operator>>,
//for output streams : push something into it with operator<<.
vector<State> ParseLine(string line)
{
	vector<State> vec;
	istringstream sline(line);
	int n;
	char c;
	while (sline >> n >> c && c == ',')   //0,1 yani int-char-int --->pas vaghti stream mikuni ham n va ham c ro stream kun
	{
		if (n == 0)
			vec.push_back(State::kEmpty);
		else
			vec.push_back(State::kObstacle);

	}
	return vec;


}

vector<vector<State>> ReadBoardFile(string path)
{
	ifstream myfile(path);
	vector<vector<State>> board{};
	if (myfile) {
		string line;

		while (std::getline(myfile, line)) //khat be khat ta enteha mire -- dar vaghe har khat ro dar har loop mirize to line!
		{
			board.push_back(ParseLine(line));
		}
	}

	return board;

}

/*
A*
 */
int Heuristic(int x1, int y1, int x2, int y2)
{
	return abs(x2 - x1) + abs(y2 - y1);
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& opennodes, vector<vector<State>>& board)
{
	vector<int> node = { x,y,g,h };
	opennodes.push_back(node);
	board[x][y] = State::kClosed;
}

bool Compare(vector<int> node1, vector<int> node2)
{
	int f1 = node1[2] + node1[3];
	int f2 = node2[2] + node2[3];
	if (f1 > f2)
		return true;
	else
		return false;
}
vector<int> CellSort(vector<vector<int>> opennodes)
{
	sort(opennodes.begin(), opennodes.end(), Compare);  //check https://www.geeksforgeeks.org/sort-c-stl/
	vector<int> currentNode = opennodes.back(); //last element has lower fValue
	return currentNode;
}
bool CheckValidCell(int x, int y, vector<vector<State>>& board)
{
	if (x < 0 || y < 0 || x >= board.size() || y >= board.front().size())
		return false;
	else
	{
		switch (board[x][y])
		{
		case State::kObstacle: return false;
		case State::kClosed: return false;
		case State::kPath: return false;
		default: return true;
		}
	}
}

void ExpandNeighbors(vector<int>& currNode, vector<vector<int>>& opennodes, vector<vector<State>>& board, vector<int> goal)
{
	const int numDir = 4; //4 or 8
	const int delta[numDir][2]{ {-1,0},{0,-1},{1,0},{0,1} }; //bala chap paeen rast
	for (int i = 0; i < numDir; i++)
	{
		int xNeigh = currNode[0] + delta[i][0];
		int yNeigh = currNode[1] + delta[i][1];
		if (CheckValidCell(xNeigh, yNeigh, board))
		{
			int gNeigh = currNode[2]++;
			int hNeigh = Heuristic(xNeigh, yNeigh, goal[0], goal[1]);
			AddToOpen(xNeigh, yNeigh, gNeigh, hNeigh, opennodes, board);
		}
	}
}


vector<vector<State>> search(vector<vector<State>> board, vector<int> start, vector<int> goal)
{
	vector <vector<int>> openList;
	int g = 0;
	int h = Heuristic(start[0], start[1], goal[0], goal[1]);
	AddToOpen(start[0], start[1], g, h, openList, board);
	while (!openList.empty()) {
		vector<int> currNode = CellSort(openList);
		openList.pop_back();
		int x = currNode[0];
		int y = currNode[1];
		board[x][y] = State::kPath;
		
		if (x == goal[0] && y == goal[1])
		{
			board[start[0]][start[1]] = State::kStart;
			board[goal[0]][goal[1]] = State::kFinish;
			return board;
		}
		ExpandNeighbors(currNode, openList, board, goal);
	}
	cout << "NO PATH";

}


int main()
{
	auto board = ReadBoardFile("D:/far/cpp nano degree/tamrinha/board.txt");//Unless you're targeting something really ancient like Windows 98, always use forward slashes. All modern operating systems including Windows support forward slashes in paths. Only Windows support backlashes, so using backslashes reduces the portability of your code.
	vector<int> start, goal;
	start = { 0,0 };
	goal = { 4,5 };
	vector<vector<State>> solution = search(board, start, goal);
	PrintBoard(solution);
	std::cin.get();
}
