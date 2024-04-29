#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>
#include <stdlib.h>

using namespace std;

/*
Checkist:
-) Main menu(done)
-) Placing ships (done)
-) Hit or miss algorithm (done)
-) AI with difficulty levels (done)
*/

/*
Main menu:
Be able to go back to main menu
Store if it is single player or multiplayer
If single player, get and store difficulty
*/

/*
Placing ship:
Render 2d array at every move, ask player for position and orientation
check:
i) If input is formatted correctly
ii) If new ship lies in the grid
iii) If new ship does not overlap or is within one block of any of the old ships
If player does not confirm their move, delete the newly shaped ship from the 2d array, and repeat the process from the previous ship
*/

/*
Hit or miss algorithm:

Input coordinates
Check if point is "     |"
If it is, do 'x' on it
If it is not, do 'o' on it
After every turn in which new x printed, check if all the x in the line have already been placed:
i) Go to top/ left of original graph
ii) Start going down/ right until last x is reached
iii) Check if last x reached is actually the end of the boat
iv) If it is, start printing special x-- or / in the required position
v) End the move
*/

/*
AI with difficulty levels:

Ships placed randomly in all difficulties

Easy mode: Hitting randomly
Normal mode: Hitting randomly, does not hit inside range of already destroyed ships
Hard mode: 1/5 chance of AI hitting your ship through do while loop. 4/5 chance of AI hitting randomly. Does not hit inside range of already destroyed ships

If a ship is hit:
Hit adjacent to the original coordinates that were hit to find if orientation is horizontal or vertical
Keep hitting in the found direction until either the whole ship is destroyed
OR if o reached, now narrow orientation to left, down, right, or up, and hit only in that direction until whole ship destroyed
Go back to random coordinates after ship is destroyed

If difficulty is normal or hard:
Make sure any new coordinates plotted are not inside range of any already destroyed ships, as there can be no ships in this range
*/

void display_menu(); // Prints the main menu
int display_howtoplay(); // Prints instruction on how to play
void initialise(string P[11][21]); // for filling empty 2d array with grid
void render(string P[11][21]); // for printing a 2d array
bool placement(string move, int length, string P[11][21]);
void placing_5_ships(string P[11][21], int mode_selection);
void placement_delete(string move, int length, string P[11][21]);
string execute(string P[11][21], string S[11][21], int mode_selection, int difficulty, int &hit, char &orientation, string &coordinates);
bool win_check(string P[11][21], string S[11][21]);

// for AI: for use in execute function to keep track of original coordinates on which the first hit occured
string old_coordinates;

int main()
{
    srand(time(0));

    string P1[11][21];
    string P2[11][21];

    initialise(P1);
    initialise(P2);

    int difficulty; // 1 is easy, 2 is normal, 3 is hard
    int mode_selection; // 1 is single, 2 is multiplayer
    do
    {
        display_menu();
        cin >> mode_selection;

        if (mode_selection == 1)
        {
            cout << "Choose your difficulty:" << endl;
            cout << "Enter 1 for easy mode" << endl;
            cout << "Enter 2 for normal mode" << endl;
            cout << "Enter 3 for hard mode" << endl;
            do
                cin >> difficulty;
            while (cin.fail() || difficulty < 1 || difficulty > 3);
        }
        if (mode_selection == 2)
        {
            break;
        }
        if (mode_selection == 3)
        {
            mode_selection = display_howtoplay();
        }
    }
    while (mode_selection != 1 && mode_selection != 2);

    // clearing input
    cin.clear();
    cin.ignore(10000, '\n');

    render(P1);

    cout << "\nPlayer 1! The war is brewing, please place your ships" << endl;
    if (mode_selection == 2)
    {
        cout << "\nPlayer 2, if you can read this please avert your eyes\n" << endl;
    }
    placing_5_ships(P1, 2);

    if (mode_selection == 2)
    {
        for (int i = 0; i < 30; i++)
        {
            cout << "peaking is bad\n";
        }
        cout << "\n";
        render(P2);
        cout << "\nPlayer 2! The other player has placed his cards, it is your turn now to place yours\n" << endl;
    }

    placing_5_ships(P2, mode_selection);

    // S1 and S2 are the grids on which moves will be executed and feedback returned to player(s)

    string S1[11][21];
    string S2[11][21];

    initialise(S1);
    initialise(S2);

    // to be used by AI to keep track of what stage it is currently in
    int hit = 0;
    char orientation = 'n';
    string coordinates;

    // never used, only to pass into function
    int hit_x = 0;
    char orientation_x = 'n';
    string coordinates_x;

    // to make mode_selection = 1 to be multiplayer, and mode_selection 2 to be single player
    if (mode_selection == 2)
        mode_selection = 1;
    else
        mode_selection = 2;

    while (true)
    {
        cout << "Player 1's turn: " << endl;
        coordinates_x = execute(P2, S2, 1, difficulty, hit_x, orientation_x, coordinates_x);
        render(S2);

        if (win_check(P2, S2) == true)
        {
            cout << "Player 1 has won the game! Bad luck player 2, better luck next time...";
            return 0;
        }

        cout << "Player 2's turn: " << endl;
        coordinates = execute(P1, S1, mode_selection, difficulty, hit, orientation, coordinates);
        render(S1);

        if (win_check(P1, S1) == true)
        {
            cout << "Player 2 has won the game! Bad luck player 1, better luck next time...";
            return 0;
        }
    }


    return 0;
}

void display_menu()
{
    cout << " _______________________________________" << endl;
    cout << "|                                       |" << endl;
    cout << "|          B A T T L E S H I P          |" << endl;
    cout << "|_______________________________________|" << endl;

    cout << "Enter 1 for single player\n";
    cout << "Enter 2 for pass and play\n";
    cout << "Enter 3 for instructions on how to play\n";
}

int display_howtoplay()
{
    cout << "\nH O W   T O   P L A Y\n\n";
    cout << "i) Each player will place five ships of varying lengths on the grid\n";
    cout << "ii) For placement, enter the starting coordinates and orientation, example: E8 v / a2 H\n";
    cout << "iii) Your ships can not overlap or be within one block of each other\n";
    cout << "iv) After each player has placed their ships, each player takes turns to strike the other player's grid\n";
    cout << "v) The first player to destroy all of their opponent's ships wins!\n";
    cout << "\nEnter 3 for main menu\n";
    int selection;
    while (cin.fail() || selection != 3)
    {
        if (cin.fail() == true)
        {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin >> selection;
    }
    return selection;
}

void initialise(string P[11][21])
{
    P[0][0] = "   |    1|    2|    3|    4|    5|    6|    7|    8|    9|   10|\n___|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|";

    for (int j = 1; j <= 20; j += 2)
    {
        for (int i = 1; i <= 10; i++)
        {
            P[i][j] = "     |";
        }
    }

    for (int j = 2; j <= 20; j += 2)
    {
        for (int i = 1; i <= 10; i++)
        {
            P[i][j] = "_____|";
        }
    }

    // odd numbers are for printing ships
    // 1 = A
    // 3 = B
    P[0][1] = "\n A |";
    P[0][3] = "\n B |";
    P[0][5] = "\n C |";
    P[0][7] = "\n D |";
    P[0][9] = "\n E |";
    P[0][11] = "\n F |";
    P[0][13] = "\n G |";
    P[0][15] = "\n H |";
    P[0][17] = "\n I |";
    P[0][19] = "\n J |";

    for (int j = 2; j <= 20; j += 2)
    {
        P[0][j] = "\n___|";
    }
}

void render(string P[11][21])
{
    for (int j = 0; j <= 20; j++)
    {
        for (int i = 0; i <= 10; i++)
        {
            cout << P[i][j];
        }
    }
    cout << "\n";
}

bool placement(string move, int length, string P[11][21], int mode_selection)
{
    int startx = 0;
    int starty = 0;

    // return 0 if input is not a valid move

    // converting lowercase into uppercase so lowercase moves are also valid
    if (move[0] >= 'a' && move[0] <= 'j')
    {
        move[0] = move[0] - 32;
    }

    if (move[0] < 'A' || move[0] > 'J')
    {
        if (mode_selection == 2)
            cout << "Please place the ship within the grid. Sample input: E8 h\n";

        return 0;
    }

    // for exception case 10
    if (move.substr(1, 2) == "10")
    {
        startx = 10;
        move = move.substr(0, 2) + move.substr(3);
    }

    if (move[1] < '1' || move[1] > '9')
    {
        if (mode_selection == 2)
            cout << "Incorrect formatting of coordinates. Sample input: E8 h\n";

        return 0;
    }

    if (move[2] != ' ')
    {
        if (mode_selection == 2)
            cout << "Incorrect formatting of coordinates. Sample input: E8 h\n";

        return 0;
    }

    // converting uppercase into lowercase so uppercase moves are also valid
    if (move[3] == 'H')
    {
        move[3] = 'h';
    }
    else if (move[3] == 'V')
    {
        move[3] = 'v';
    }

    if (move[3] != 'h' && move[3] != 'v')
    {
        if (mode_selection == 2)
            cout << "Incorrect formatting of coordinates. Sample input: E8 h\n";

        return 0;
    }

    // Converting coordinates into numbers we can use
    starty = (int(move[0]) - 65)*2 + 1;
    if (startx != 10)
        startx = stoi(move.substr(1, 1));

    // for case in which orientation is vertical
    if (move.substr(3, 1) == "v")
    {
        // checking if ship is within the grid
        if (starty + (length-2)*2 + 2 > 19)
        {
            if (mode_selection == 2)
                cout << "Please place the ship within the grid\n";

            return 0;
        }

        // Checking if any previous ships around the new ship's placement
        for (int i = startx-1; i < 11 && i <= startx + 1; i++)
        {
            for (int j = starty-2; j < 21 && j <= (starty+length*2); j += 2)
            {
                if (P[i][j] == " /^/ |" || P[i][j] == " / / |" || P[i][j] == " /_/ |" || P[i][j] == " <====" || P[i][j] == "======" || P[i][j] == "===> |")
                {
                    if (mode_selection == 2)
                        cout << "Please leave one block distance between all of your ships\n";

                    return 0;
                }
            }
        }

        // updating original 2d array with the newly placed ship
        P[startx][starty] = " /^/ |";
        for (int i = 0; i < (length-2)*2 + 1; i++)
        {
            P[startx][starty + i + 1] = " / / |";
        }
        P[startx][starty + (length-2)*2 + 2] = " /_/ |";
    }
    else if (move.substr(3) == "h") // for case in which coordinates are horizontal
    {
        // checking if ship is within the grid
        if (startx + length - 1 > 10)
        {
            if (mode_selection == 2)
                cout << "Please place the ship within the grid\n";

            return 0;
        }

        // Checking if any previous ships around the new ship's placement
        for (int i = startx - 1; i < 11 && i <= startx + length; i++)
        {
            for (int j = starty - 2; j < 21 && j <= starty + 2; j += 2)
            {
                if (P[i][j] == " /^/ |" || P[i][j] == " / / |" || P[i][j] == " /_/ |" || P[i][j] == " <====" || P[i][j] == "======" || P[i][j] == "===> |")
                {
                    if (mode_selection == 2)
                        cout << "Please leave one block distance between all of your ships\n";

                    return 0;
                }
            }
        }

        // updating original array with newly placed ship
        P[startx][starty] = " <====";
        for (int i = 0; i < length - 2; i++)
        {
            P[startx + i + 1][starty] = "======";
        }
        P[startx + length - 1][starty] = "===> |";
    }
    else
    {
        if (mode_selection == 2)
            cout << "Unknown error. Sample format: E3 h";

        return 0;
    }

    return 1;
}

void placement_delete(string move, int length, string P[11][21])
{
    // converting lowercase into uppercase so lowercase moves are also valid
    if (move[0] >= 'a' && move[0] <= 'j')
    {
        move[0] = move[0] - 32;
    }

    // converting uppercase into lowercase so uppercase moves are also valid
    if (move[3] == 'H')
    {
        move[3] = 'h';
    }
    else if (move[3] == 'V')
    {
        move[3] = 'v';
    }

    int startx;
    int starty;
    starty = (int(move[0]) - 65)*2 + 1;
    startx = stoi(move.substr(1, 1));

    // for exception case 10
    if (move.substr(1, 2) == "10")
    {
        startx = 10;
        move = move.substr(0, 2) + move.substr(3);
    }

    // doing opposite of placement function
    if (move.substr(3) == "v")
    {
        P[startx][starty] = "     |";
        for (int i = 0; i < (length-2)*2 + 1; i++)
        {
            if ((starty + i + 1) % 2 == 0)
            {
                P[startx][starty + i + 1] = "_____|";
            }
            else
            {
                P[startx][starty + i + 1] = "     |";
            }
        }
        P[startx][starty + (length-2)*2 + 2] = "     |";
    }
    else if (move.substr(3) == "h")
    {

        P[startx][starty] = "     |";
        for (int i = 0; i < length - 2; i++)
        {
            P[startx + i + 1][starty] = "     |";
        }
        P[startx + length - 1][starty] = "     |";
    }

}

void placing_5_ships(string P[11][21], int mode_selection)
{
    string confirmation;
    string input;
    int length;
    int x;
    int y;
    bool allignment;

    for (int i = 0; i < 5; i++)
    {
        // repeating loop for each ship to be placed
        do
        {
            if (i == 0)
            {
                if (mode_selection == 2)
                    cout << "Enter coordinates for destroyer (length 2)";

                length = 2;
            }
            if (i == 1)
            {
                if (mode_selection == 2)
                    cout << "Enter coordinates for submarine (length 3)";

                length = 3;
            }
            if (i == 2)
            {
                if (mode_selection == 2)
                    cout << "Enter coordinates for cruiser (length 3)";

                length = 3;
            }
            if (i == 3)
            {
                if (mode_selection == 2)
                    cout << "Enter coordinates for battleship (length 4)";

                length = 4;
            }
            if (i == 4)
            {
                if (mode_selection == 2)
                    cout << "Enter coordinates for carrier (length 5)";

                length = 5;
            }
            if (mode_selection == 2)
                cout << ": ";
            
            // getting coordinates from human or AI depending on the mode
            if (mode_selection == 2)
            {
                getline(cin, input);
            }
            else if (mode_selection == 1)
            {
                // AI to randomly choose a move
                x = rand() % (10 - 1 + 1) + 1;
                y = rand() % (10 - 1 + 1) + 1;

                if (x == 10)
                    input = "#####";
                else
                    input = "####";

                allignment = rand() % (1 - 0 + 1) + 0;
                
                input[0] = char(64 + y);

                if (x == 10)
                {
                    input[1] = '1';
                    input[2] = '0';

                    input[3] = ' ';

                    if(allignment == true)
                        input[4] = 'h';
                    if (allignment == false)
                        input[4] = 'v';
                }
                else
                {
                    input[1] = char(48 + x);

                    input[2] = ' ';

                    if(allignment == true)
                        input[3] = 'h';
                    if (allignment == false)
                        input[3] = 'v';
                }
            }
        }
        while (placement(input, length, P, mode_selection) == 0); // repeat while placement is returning false (error) // error: dont cout when AI is doing the moves

        if (mode_selection == 2)
        {
            render(P);
            do
            {
                cout << "Confirm selection Y/N\n";
                cin >> confirmation;
            }
            while (confirmation != "N" && confirmation != "Y" && confirmation != "n" && confirmation != "y");

            if (confirmation == "N" || confirmation == "n")
            {
                placement_delete(input, length, P);
                i--; // so loop can run from initial i again
            }
            cin.clear();
            cin.ignore(10000, '\n');

        }

    }
}

string execute(string P[11][21], string S[11][21], int mode_selection, int difficulty, int &hit, char &orientation, string &coordinates)
{
    do // keep repeating while loop until valid coordinates are given by either AI or human
    {
    int player = 1;

    int x;
    int y;
    char buffer;

    /*
    AI pseudocode:
    -) if x is hit, hit = 1
    -) at every turn, check if hit is 1. If it is, then hit in the coordinates adjacent the OG coordinates until the second x is hit. Orientation is set here to h or v
    -) Value of hit incremented to 2 after second x is hit
    -) When hit is 2, keep hitting in given orientation until either o appears, or x-- appears
    -) When o appears, set orientation either a, b, c, d, depending on where we need to move now
    -) After x-- reached, hit is 0 again and orientation is 'n' again
    */

    if (mode_selection == 1)
    {
        cin >> coordinates;
        if (coordinates[0] >= 'a' && coordinates[0] <= 'j') // convert any lowercases into uppercases so such moves are also valid
        {
            coordinates[0] = coordinates[0] - 32;
        }
    }
    else if (hit == 1) // for AI
    {
        // 1/4 chance of moving in either direction in xy plane
        if (rand() % 2 == 0)
        {
            if (rand() % 2 == 0)
            {
                // moving right
                if (coordinates.length() == 3)
                {
                    continue;
                }
                else
                {
                    if (coordinates[1] == '9')
                    {
                        buffer = coordinates[0];
                        coordinates = "###";
                        coordinates[0] = buffer;
                        coordinates[1] = '1';
                        coordinates[2] = '0';
                    }
                    else
                    {
                        coordinates[1] = coordinates[1] + 1;
                    }
                }
            }
            else
            {
                // moving left
                if (coordinates.length() == 3)
                {
                    buffer = coordinates[0];
                    coordinates = "##";
                    coordinates[0] = buffer;
                    coordinates[1] = '9';
                }
                else if (coordinates[1] == '1')
                {
                    continue;
                }
                else
                {
                    coordinates[1] = coordinates[1] - 1;
                }
            }
        }
        else
        {
            if (rand() % 2 == 0)
            {
                // moving down
                if (coordinates[0] == 'J')
                {
                    continue;
                }
                else
                {
                    coordinates[0] = coordinates[0] + 1;
                }
            }
            else
            {
                // moving up
                if (coordinates[0] == 'A')
                {
                    continue;
                }
                else
                {
                    coordinates[0] = coordinates[0] - 1;
                }
            }
        }

    }
    else if (hit == 2) // for AI
    {
        if (orientation == 'v')
        {
            if (rand() % 2 == 0)
            {
                // go up
                if (coordinates[0] == 'A')
                {
                    continue;
                }
                else
                {
                    coordinates[0] = coordinates[0] - 1;
                }
            }
            else
            {
                // go down
                if (coordinates[0] == 'J')
                {
                    continue;
                }
                else
                {
                    coordinates[0] = coordinates[0] + 1;
                }
            }
        }
        else if (orientation == 'h')
        {
            if (rand() % 2 == 0)
            {
                // go right
                if (coordinates.length() == 3)
                {
                    continue;
                }
                else
                {
                    if (coordinates[1] == '9')
                    {
                        buffer = coordinates[0];
                        coordinates = "###";
                        coordinates[0] = buffer;
                        coordinates[1] = '1';
                        coordinates[2] = '0';
                    }
                    else
                    {
                        coordinates[1] = coordinates[1] + 1;
                    }
                }
            }
            else
            {
                // go left
                if (coordinates.length() == 3)
                {
                    buffer = coordinates[0];
                    coordinates = "##";
                    coordinates[0] = buffer;
                    coordinates[1] = '9';
                }
                else if (coordinates[1] == '1')
                {
                    continue;
                }
                else
                {
                    coordinates[1] = coordinates[1] - 1;
                }
            }

        }
        else if (orientation == 'a')
        {
            // go right
            if (coordinates.length() == 3)
            {
                continue;
            }
            else
            {
                if (coordinates[1] == '9')
                {
                    buffer = coordinates[0];
                    coordinates = "###";
                    coordinates[0] = buffer;
                    coordinates[1] = '1';
                    coordinates[2] = '0';
                }
                else
                {
                    coordinates[1] = coordinates[1] + 1;
                }
            }
        }
        else if (orientation == 'b')
        {
            // go left
            if (coordinates.length() == 3)
            {
                buffer = coordinates[0];
                coordinates = "##";
                coordinates[0] = buffer;
                coordinates[1] = '9';
            }
            else if (coordinates[1] == '1')
            {
                continue;
            }
            else
            {
                coordinates[1] = coordinates[1] - 1;
            }
        }
        else if (orientation == 'c')
        {
            // go up
            if (coordinates[0] == 'A')
            {
                continue;
            }
            else
            {
                coordinates[0] = coordinates[0] - 1;
            }
        }
        else if (orientation == 'd')
        {
            // go down
            if (coordinates[0] == 'J')
            {
                continue;
            }
            else
            {
                coordinates[0] = coordinates[0] + 1;
            }
        }
    }
    else // for AI: randomly chosing a move
    {
        if (difficulty == 1)
        {
            x = rand() % (10 - 1 + 1) + 1;
            y = rand() % (10 - 1 + 1) + 1;
        }

        if (difficulty == 2)
        {
            x = rand() % (10 - 1 + 1) + 1;
            y = rand() % (10 - 1 + 1) + 1;
        }

        if (difficulty == 3)
        {
            if (rand() % 5 != 0)
            {
                x = rand() % (10 - 1 + 1) + 1;
                y = rand() % (10 - 1 + 1) + 1;
            }
            else
            {
                // impossible not to hit a ship in this move. Repeat move if it does not
                do
                {
                    x = rand() % (10 - 1 + 1) + 1;
                    y = rand() % (10 - 1 + 1) + 1;
                }
                while(P[x][(y-1)*2+1] != " /^/ |" && P[x][(y-1)*2+1] != " / / |" && P[x][(y-1)*2+1] != " /_/ |" && P[x][(y-1)*2+1] != " <====" && P[x][(y-1)*2+1] != "======" && P[x][(y-1)*2+1] != "===> |");
            }
        }

        // to prevent array overflow error
        coordinates = "##";

        if (x == 10)
        {
            coordinates = "###";
            coordinates[1] = '1';
            coordinates[2] = '0';
        }
        else
        {
            coordinates[1] = char(48 + x);
        }
        coordinates[0] = char(64 + y);
    }

    // repeat loop if invalid parameters of coordinates
    if (coordinates[0] < 'A' || coordinates[0] > 'J' || coordinates[1] < '1' || coordinates[1] > '9')
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter valid coordinates\n";
        continue;
    }

    // extreme case 10
    if (coordinates.length() != 2)
    {
        if (coordinates.substr(1) == "10")
        {
            // do nothing
        }
        else
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter valid coordinates\n";
            continue;
        }
    }

    // converting string coordinates into integer coordinates we can use
    int checky = (int(coordinates[0]) - int('A'))*2 + 1;
    int checkx = stoi(coordinates.substr(1));

    // for AI
    // if hit is 1, store OG coordinates
    if (hit == 1 && mode_selection == 2)
    {
        coordinates = old_coordinates;
    }

    // for AI
    // checking this in case circle has already been placed adjacent to x
    if (hit == 2 && S[checkx][checky] == "  o  |" && mode_selection == 2) //possible error
    {
        // if x is on right, move right
        // if x is on left, move left
        if (orientation == 'h')
        {
            if (checkx != 1 && S[checkx - 1][checky] == "  x  |")
            {
                orientation = 'b';
            }
            else
            {
                orientation = 'a';
            }
        }
        // if x is up, move up
        // if x is down, move down
        if (orientation == 'v')
        {
            if (checky != 1 && S[checkx][checky - 2] == "  x  |")
            {
                orientation = 'c';
            }
            else
            {
                orientation = 'd';
            }
        }

    }

    // repeat loop if invalid coordinates
    if (S[checkx][checky] != "     |")
    {
        if (mode_selection == 1)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter coordinates that are free\n";
        }
        continue;
    }

    //for AI
    // don't hit at any point in grid within one block of a ship that has already been hit
    bool donthit;
    donthit = false;
    if (hit == 0 && difficulty > 1 && mode_selection == 2)
    {
        for (int i = checkx - 1; i < 11 && i <= checkx + 1; i++)
        {
            for (int j = checky - 2; j < 21 && j <= checky + 2; j += 2)
            {
                if (S[i][j] == "  x  |" || S[i][j] == "--x  |" || S[i][j] == "--x---" || S[i][j] == "  x---")
                {
                    donthit = true;
                }
            }
        }
        if (donthit == true)
            continue;
    }

    donthit = false;
    if (hit == 1 && difficulty > 1 && mode_selection == 2)
    {
        for (int i = checkx - 1; i < 11 && i <= checkx + 1; i++)
        {
            for (int j = checky - 2; j < 21 && j <= checky + 2; j += 1)
            {
                if (S[i][j] == "  /  |" || S[i][j] == "--x  |" || S[i][j] == "--x---" || S[i][j] == "  x---")
                {
                    donthit = true;
                }
            }
        }
        if (donthit == true)
            continue;
    }

    if (P[checkx][checky] == "     |")
    {
        S[checkx][checky] = "  o  |";

        // for AI
        // in case in which orientation is already known, and now first o in given orientation has been hit, decide optimal direction to move
        if (hit == 2 && mode_selection == 2)
        {
            // if x is on right, move right
            // if x is on left, move left
            if (orientation == 'h')
            {
                if (checkx != 1 && S[checkx - 1][checky] == "  x  |")
                {
                    orientation = 'b';
                }
                else
                {
                    orientation = 'a';
                }
            }
            // if x is up, move up
            // if x is down, move down
            if (orientation == 'v')
            {
                if (checky != 1 && S[checkx][checky - 2] == "  x  |")
                {
                    orientation = 'c';
                }
                else
                {
                    orientation = 'd';
                }
            }

        }
    }
    else
    {
        S[checkx][checky] = "  x  |";

        // for AI
        if (hit == 0 && mode_selection == 2)
        {
            hit = 1;
            // storing coordinates on which first hit is recorded
            old_coordinates = coordinates;
        }
        else if (hit == 1 && mode_selection == 2)
        {
            hit = 2;

            // checking for horizontal orientation, else it is vertical
            if (P[checkx][checky] == " <====" || P[checkx][checky] == "===> |" || P[checkx][checky] == "======")
            {
                orientation = 'h';
            }
            else
            {
                orientation = 'v';
            }
        }

        // go to top left of original graph for ship
        while (checkx > 1 && P[checkx-1][checky] != "     |" )
        {
            checkx--;
        }

        while (checky > 2 && P[checkx][checky - 2] != "     |")
        {
            checky -= 2;
        }

        // start going down/ right, until x stops
        while (S[checkx][checky] == "  x  |")
        {
            if (checkx != 10 && S[checkx + 1][checky] == "  x  |")
            {
                checkx++;
            }
            else if (checky != 19 && S[checkx][checky + 2] == "  x  |")
            {
                checky += 2;
            }
            else
            {
                break;
            }
        }

        // checking if last x seen is actually end of boat
        if ((checkx == 10 || P[checkx + 1][checky] == "     |") && (checky == 19 || P[checkx][checky + 2] == "     |"))
        {
            // for AI
            // initialising AI so it moves randomly now after whole ship has been hit
            if (mode_selection == 2)
            {
                hit = 0;
                orientation = 'n';
            }

            // x---x needs to be printed
            // starting from bottom right

            // for vertical
            while (checky > 0 && S[checkx][checky-2] == "  x  |")
            {
                S[checkx][checky-1] = "  /  |";
                checky = checky - 2;
            }

            // for horizontal
            while (S[checkx][checky] == "  x  |")
            {
                if (P[checkx][checky] == "===> |")
                    S[checkx][checky] = "--x  |";

                if (P[checkx][checky] == "======")
                    S[checkx][checky] = "--x---";

                if (P[checkx][checky] == " <====")
                    S[checkx][checky] = "  x---";
            
                checkx--;
            }


        }
        

    }

    // time lag to let player register the opponent's move
    if (mode_selection == 2)
    {
        Sleep(2000);
    }


    return coordinates;
    }
    while (true);


}

bool win_check(string P[11][21], string S[11][21])
{
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 19; j += 2)
        {
            if (P[i][j] != "     |" && S[i][j] == "     |")
            {
                // return fail if any position in which P has boat placed does not have anything placed on S
                return 0;
            }
        }
    }
    // return true if it does not fail
    return 1;
}

/*
   |    1|    2|    3|    4|    5|    6|    7|    8|    9|   10|
___|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 A |  x-----x  |     |  x  |     |     |  x-----x-----x  |     |
___|_____|_____|_____|  /  |_____|_____|_____|_____|_____|_____|
 B |     |     |     |  x  |     |     |     |     |     |     |
___|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
*/


/*

    /*
    Hit or miss algorithm:

    Input coordinates
    Check if point is "     |"
    If it is, do 'x' on it
    If it is not, do 'o' on it
    After every turn in which new x printed, check if all the x in the line have already been placed
    */

    // odd numbers starting from 1, incremented by 2 are y axis
    // x is x axis starting from 1, incremented by 1

    // checking if x should be lined
    // go to top left of original graph for ship
    // start going down/ right, until either x's stop, or ship stops
    // if x stops, no x--x printed
    // if ship stops, x--x printed

    /*
    P[3][11] = " <====";
    P[4][11] = "===> |";
    P[3][12] = "______";
    */

    /*
    P[3][11] = " /^/ |";
    P[3][12] = " / / |";
    P[3][13] = " / / |";
    P[3][14]=  " / / |";
    P[3][15]=  " /_/ |";
    */

    // checking if x should be lined
    // go to top left of original graph for ship
    // start going down/ right, until either x's stop, or ship stops
    // if x stops, no x--x printed
    // if ship stops, x--x printed
