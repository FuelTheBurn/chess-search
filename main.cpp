#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

int BOARD_SIZE = 512;
int SQUARE_SIZE = BOARD_SIZE / 8;
int BORDER = 16;

const int None = 0;
const int Pawn = 1;
const int Knight = 2;
const int Bishop = 3;
const int Rook = 4;
const int Queen = 5;
const int King = 6;
map<char, int> pieceChars{{'p', Pawn}, {'b', Bishop}, {'n', Knight}, {'r', Rook}, {'q', Queen}, {'k', King}};

map<int, sf::Texture> textures;
sf::Texture wPTexture;
sf::Texture wNTexture;
sf::Texture wBTexture;
sf::Texture wRTexture;
sf::Texture wQTexture;
sf::Texture wKTexture;


void loadTextures(){
    wPTexture.loadFromFile("pieces-basic-png/White-Pawn.png");
    wNTexture.loadFromFile("pieces-basic-png/White-Knight.png");
    wBTexture.loadFromFile("pieces-basic-png/White-Bishop.png");
    wRTexture.loadFromFile("pieces-basic-png/White-Rook.png");
    wQTexture.loadFromFile("pieces-basic-png/White-Queen.png");
    wKTexture.loadFromFile("pieces-basic-png/White-King.png");

    textures[Pawn] = wPTexture;
    textures[Knight] = wNTexture;
    textures[Bishop] = wBTexture;
    textures[Rook] = wRTexture;
    textures[Queen] = wQTexture;
    textures[King] = wKTexture;
}

vector<vector<int>> loadFEN(const string& fen) {
    vector<vector<int>> loadBoard(8);
    for (int i = 0; i < 8; i++){
        loadBoard[i].resize(8);
    }
    string fenBoard = fen.substr(0, fen.find(' '));
    int rank = 0;
    int file = 0;
    for (char symbol : fenBoard){
        if (symbol == '/'){
            file = 0;
            rank++;
        }
        else if (isdigit(symbol)){
            file += symbol - '0';
        }
        else{
            int pieceType = pieceChars[symbol];

            loadBoard[rank][file] = pieceType;
            file++;
        }
    }
    return loadBoard;
}

void drawBoard(sf::RenderWindow& window, bool board1){
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(board1){
                tile.setPosition(sf::Vector2f(SQUARE_SIZE * j, SQUARE_SIZE * i));
            }
            else{
                tile.setPosition(sf::Vector2f(SQUARE_SIZE * j + BOARD_SIZE + BORDER, SQUARE_SIZE * i));
            }
            if ((i + j) % 2 == 0) {
                tile.setFillColor(sf::Color(240,217,181,255));
            }
            else {
                tile.setFillColor(sf::Color(181,136,99,255));
            }
            window.draw(tile);
        }
    }
}

void drawPieces(vector<vector<int>> board, sf::RenderWindow& window, bool board1){
    sf::Sprite sprite;
    sprite.setScale(0.5f, 0.5f);
    if(board1){

    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (textures.count(board[i][j])){
                sprite.setTexture(textures[board[i][j]]);
                if(board1){
                    sprite.setPosition(SQUARE_SIZE * j, SQUARE_SIZE * i);
                }
                else{
                    sprite.setPosition(SQUARE_SIZE * j + BOARD_SIZE + BORDER, SQUARE_SIZE * i);
                }
                window.draw(sprite);
            }
        }
    }
}

bool validateInput(int input){

}

int main() {
    int numQ, numR, numB, numN, numK;
    cout << "Enter number of Queens: " << endl;
    if (!(cin >> numQ) || numQ < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Rooks: " << endl;
    if (!(cin >> numR) || numR < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Bishops: " << endl;
    if (!(cin >> numB) || numB < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Knights: " << endl;
    if (!(cin >> numN) || numN < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Kings: " << endl;
    if (!(cin >> numK) || numK < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * 2 + BORDER, BOARD_SIZE), "Chess");
    loadTextures();

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        vector<vector<int>> board1 = loadFEN("rnbqkbnr/pppppppp/8/8/8/8/8/8");
        vector<vector<int>> board2 = loadFEN("8/8/8/8/8/8/pppppppp/rnbqkbnr");

        drawBoard(window, true);
        drawPieces(board1, window, true);

        drawBoard(window, false);
        drawPieces(board2, window, false);

        window.display();
    }

    return 0;
}
