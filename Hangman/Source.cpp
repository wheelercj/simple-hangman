#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

void ShowConsoleCursor(bool);
vector<string> loadDictionary();
void display(int);
void printMan(int, HANDLE);

int main()
{
	const int maxGuesses = 6;
	ShowConsoleCursor(false);
	vector<string> dictionary = loadDictionary();
	srand((unsigned)time(0));
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return 0;

	while (true)
	{
		string pastGuesses(' ', 50),
			word = dictionary[rand() % (dictionary.size() - 2)];
		int letterCount = word.length(),
			guessCount = 0;
		short wrongGuessCount = 0;
		bool gameOver = false;

		// set up the interface
		display(letterCount);
		
		// begin the game
		while (!gameOver)
		{
			char playerChoice;

			do  // read the player's guess
			{
				playerChoice = tolower(_getch());
			} while (playerChoice < 'a' || playerChoice > 'z' || pastGuesses.find(playerChoice) != string::npos);

			// check if the guessed letter is in the word
			if (word.find(playerChoice) != string::npos) // the guessed letter is in the word
			{
				pastGuesses[guessCount] = playerChoice;
				guessCount++;

				// display the correctly guessed letter
				for (short i = 0; i < word.length(); i++)
				{
					if (word[i] == playerChoice)
					{
						SetConsoleCursorPosition(hStdOut, { (i + 1) * 2, 8 });
						cout << playerChoice;
						letterCount--;
					}
				}
			}
			else // the guessed letter is not in the word
			{
				pastGuesses[guessCount] = playerChoice;
				guessCount++;
				wrongGuessCount++;

				// display playerChoice in the wrong guesses area
				SetConsoleCursorPosition(hStdOut, { wrongGuessCount + 16, 11 });
				cout << playerChoice;

				// display part of the man
				printMan(wrongGuessCount, hStdOut);
			}

			if (wrongGuessCount == maxGuesses)
			{
				gameOver = true;
				SetConsoleCursorPosition(hStdOut, { 20, 3 });
				cout << "Hangman!";

				// display the entire word
				for (short i = 1; i <= word.length(); i++)
				{
					SetConsoleCursorPosition(hStdOut, { i * 2, 9 });
					cout << word[i - 1];
				}

			}
			else if (!letterCount)
			{
				gameOver = true;
				SetConsoleCursorPosition(hStdOut, { 20, 3 });
				cout << "You win!";
			}
		}

		SetConsoleCursorPosition(hStdOut, { 20, 5 });
		cout << "Play again? (y/n)";
		char playAgain = tolower(_getch());
		if (playAgain == 'n')
			return 0;
	}
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);

	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

vector<string> loadDictionary()
{
	cout << " Loading dictionary file . . .";
	ifstream file("../dictionary.txt");

	if (!file)
	{
		cout << "\n Error opening dictionary file. \n ";
		system("pause");
		exit(0);
	}

	string word;
	vector<string> dictionary;
	while (file >> word)
		dictionary.push_back(word);
	file.close();

	return dictionary;
}

void display(int letterCount)
{
	// variables for displaying ascii chars that cannot be typed here
	char vert = 179, // vertical bar
		T = 193,     // upside-down T
		dash = 196;

	system("cls"); // clear screen

	// hanging platform
	cout << "     _____ \n     |   ";
	for (int i = 0; i < 4; i++)
		cout << vert << " \n         ";
	cout << vert << " \n  ";
	for (int i = 0; i < 7; i++)
		cout << dash;
	cout << T;
	for (int i = 0; i < 4; i++)
		cout << dash;

	// print underdashes showing how many letters the word has
	cout << "\n\n ";
	for (int i = 0; i < letterCount; i++)
		cout << " _";
	cout << "\n\n\n Wrong Guesses: ";
}

void printMan(int wrongGuessCount, HANDLE hStdOut)
{
	switch (wrongGuessCount)
	{
	case 1:
		SetConsoleCursorPosition(hStdOut, { 5, 2 });
		cout << (char)234;  // head
		break;
	case 2:
		SetConsoleCursorPosition(hStdOut, { 5, 3 });
		cout << (char)219;  // torso
		break;
	case 3:
		SetConsoleCursorPosition(hStdOut, { 4, 3 });
		cout << '/';  // left arm
		break;
	case 4:
		SetConsoleCursorPosition(hStdOut, { 6, 3 });
		cout << '\\';  // right arm
		break;
	case 5:
		SetConsoleCursorPosition(hStdOut, { 4, 4 });
		cout << '/';  // left leg
		break;
	case 6:
		SetConsoleCursorPosition(hStdOut, { 6, 4 });
		cout << '\\';  // right leg
	}
}