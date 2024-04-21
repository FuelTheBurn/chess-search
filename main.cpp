#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#define f first
#define s second

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
int t1=100;
int t2=100;
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
void drawPieces(vector<vector<int>> board, sf::RenderWindow& window, bool board1);
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
vector<vector<int>> numbering={
        {62,41,42,43, 44, 45, 46, 61},
        {59,20,21,22,23,24,25,47},
        {40,19, 6, 7, 8, 9,26,48},
        {39,18,5,0, 1,10,27,49},
        {38,17,4,3,2,11,28,50},
        {37,16,15,14,13,12,29,51},
        {36,35,34,33,32,31,30,52},
        {63,58,57,56,55,54,53,60}
};
vector<vector<vector<int>>> possibleBoards;
vector<vector<int>> actualBoard={
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
};
vector<vector<int>> actualBoard2={
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
};
bool inRange(pair<int,int> loc){
    if(loc.f>=0 && loc.f<8 && loc.s>=0 && loc.s<8){
        return true;
    }
    return false;
}
int K=0;
int Q=8;
int R=0;
int B=0;
int N=0;
vector<pair<int,int>> Km={
        {-1,-1},{-1,0},{-1,1},
        {0,-1},{0,0},{0,1},
        {1,-1},{1,0},{1,1}
};
vector<pair<int,int>> Rm={
        {0,0},
        {-7,0},{-6,0},{-5,0},{-4,0},{-3,0},{-2,0},{-1,0},
        {7,0},{6,0},{5,0},{4,0},{3,0},{2,0},{1,0},
        {0,-7},{0,-6},{0,-5},{0,-4},{0,-3},{0,-2},{0,-1},
        {0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1}
};
vector<pair<int,int>> Bm={
        {0,0},
        {-7,-7},{-6,-6},{-5,-5},{-4,-4},{-3,-3},{-2,-2},{-1,-1},
        {7,7},{6,6},{5,5},{4,4},{3,3},{2,2},{1,1},
        {7,-7},{6,-6},{5,-5},{4,-4},{3,-3},{2,-2},{1,-1},
        {-7,7},{-6,6},{-5,5},{-4,4},{-3,3},{-2,2},{-1,1}
};
vector<pair<int,int>> Qm={
        {0,0},
        {-7,0},{-6,0},{-5,0},{-4,0},{-3,0},{-2,0},{-1,0},
        {7,0},{6,0},{5,0},{4,0},{3,0},{2,0},{1,0},
        {0,-7},{0,-6},{0,-5},{0,-4},{0,-3},{0,-2},{0,-1},
        {0,7},{0,6},{0,5},{0,4},{0,3},{0,2},{0,1},
        {-7,-7},{-6,-6},{-5,-5},{-4,-4},{-3,-3},{-2,-2},{-1,-1},
        {7,7},{6,6},{5,5},{4,4},{3,3},{2,2},{1,1},
        {7,-7},{6,-6},{5,-5},{4,-4},{3,-3},{2,-2},{1,-1},
        {-7,7},{-6,6},{-5,5},{-4,4},{-3,3},{-2,2},{-1,1}
};
vector<pair<int,int>> Nm={
        {0,0},
        {2,-1},{2,1},
        {-2,-1},{-2,1},
        {1,-2},{1,2},
        {-1,-2},{-1,2}
};
vector<vector<int>> searchBoard;
vector<vector<int>> searchBoard2;
vector<pair<char,pair<int,int>>> searchStack;
vector<char> depthMap;
int cumulativeJ=0;
vector<pair<int,int>> remapCC;
int DPlock=0;
int yay=0;
sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * 2 + BORDER, BOARD_SIZE), "Chess");
sf::Event event;
void dfs(int depth){
    if(window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    //cout<<"depth "<<depth<<endl;
    if(depth==cumulativeJ){
        yay++;
        possibleBoards.push_back(actualBoard);
        std::chrono::time_point start = std::chrono::steady_clock::now();
        //cout<<"draw1"<<endl;
        while(std::chrono::steady_clock::now() - start < std::chrono::milliseconds (t1)){

            drawBoard(window, true);
            drawBoard(window, false);
            drawPieces(possibleBoards[possibleBoards.size()-1], window, true);
            window.display();
        }


        //std::this_thread::sleep_for(std::chrono::seconds(1));
//        for(int i=0;i<8;i++){
//            for(int j=0;j<8;j++){
//                cout<<actualBoard[i][j];
//            }
//            cout<<endl;
//        }
        return;
    }
    if(depth==0||depth==Q||depth==Q+R||depth==Q+R+B||depth==Q+R+B+K){
        DPlock=0;
    }
    vector<pair<int,int>> *mptr = nullptr;
    int piece=0;
    if(depthMap[depth]=='K'){
        mptr=&Km;
        piece=6;
    }
    else if(depthMap[depth]=='Q'){
        mptr=&Qm;
        piece=5;
    }
    else if(depthMap[depth]=='R'){
        mptr=&Rm;
        piece=4;
    }
    else if(depthMap[depth]=='B'){
        mptr=&Bm;
        piece=3;
    }
    else if(depthMap[depth]=='N'){
        mptr=&Nm;
        piece=2;
    }
    vector<pair<int,int>> &m = *mptr;
    //cout<<"start placing"<<endl;
    for(int i=DPlock;i<64;i++){
        //cout<<i<<endl;
//        if(depth==0&&i==16) {
//            break;
//        }
        pair<int,int> loc=remapCC[i];
        bool working=true;
        if(searchBoard[loc.f][loc.s]!=0){//something attacks it
            //cout<<"died to attacker"<<endl;
            continue;
        }
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})&&searchBoard[loc.f+a.f][loc.s+a.s]>100){//it attacks something
//                cout<<loc.f+a.f<<" "<<loc.s+a.s<<endl;
//                cout<<searchBoard[loc.f+a.f][loc.s+a.s]<<endl;
                working=false;
                break;
            }
        }
        if(!working){
            //cout<<"arrested for murder"<<endl;
            continue;
        }

        searchBoard[loc.f][loc.s]+=200;
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})) {
                searchBoard[loc.f + a.f][loc.s + a.s] += 1;
            }
        }
        actualBoard[loc.f][loc.s]=piece;
        int oldDP=DPlock;
        DPlock=i+1;
        dfs(depth+1);
        DPlock=oldDP;
        searchBoard[loc.f][loc.s]-=200;
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})) {
                searchBoard[loc.f + a.f][loc.s + a.s] -= 1;
            }
        }
        actualBoard[loc.f][loc.s]=0;
    }
}
vector<vector<vector<int>>> possibleBoards2;
set<vector<vector<int>>> uniqueBoard;
void dfsBruteForce(int depth){
    if(window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
    //cout<<"depth "<<depth<<endl;
    if(depth==cumulativeJ){
        yay++;
        if(uniqueBoard.insert(actualBoard2).s){
            std::chrono::time_point start = std::chrono::steady_clock::now();
            //cout<<"draw2"<<endl;
            possibleBoards2.push_back(actualBoard2);
            while(std::chrono::steady_clock::now() - start < std::chrono::milliseconds (t2)){
                drawBoard(window, false);
                drawPieces(possibleBoards2[possibleBoards2.size()-1], window, false);
                window.display();
            }
        }
//        for(int i=0;i<8;i++){
//            for(int j=0;j<8;j++){
//                cout<<actualBoard[i][j];
//            }
//            cout<<endl;
//        }
        return;
    }
    vector<pair<int,int>> *mptr = nullptr;
    int piece=0;
    if(depthMap[depth]=='K'){
        mptr=&Km;
        piece=6;
    }
    else if(depthMap[depth]=='Q'){
        mptr=&Qm;
        piece=5;
    }
    else if(depthMap[depth]=='R'){
        mptr=&Rm;
        piece=4;
    }
    else if(depthMap[depth]=='B'){
        mptr=&Bm;
        piece=3;
    }
    else if(depthMap[depth]=='N'){
        mptr=&Nm;
        piece=2;
    }
    vector<pair<int,int>> &m = *mptr;
    //cout<<"start placing"<<endl;
    for(int i=0;i<64;i++){
        //cout<<i<<endl;
//        if(depth==0&&i==16) {
//            break;
//        }
        pair<int,int> loc=remapCC[i];
        bool working=true;
        if(searchBoard2[loc.f][loc.s]!=0){//something attacks it
            //cout<<"died to attacker"<<endl;
            continue;
        }
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})&&searchBoard2[loc.f+a.f][loc.s+a.s]>100){//it attacks something
//                cout<<loc.f+a.f<<" "<<loc.s+a.s<<endl;
//                cout<<searchBoard[loc.f+a.f][loc.s+a.s]<<endl;
                working=false;
                break;
            }
        }
        if(!working){
            //cout<<"arrested for murder"<<endl;
            continue;
        }

        searchBoard2[loc.f][loc.s]+=200;
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})) {
                searchBoard2[loc.f + a.f][loc.s + a.s] += 1;
            }
        }
        actualBoard2[loc.f][loc.s]=piece;
        dfsBruteForce(depth+1);
        searchBoard2[loc.f][loc.s]-=200;
        for(auto &a:m) {
            if(inRange({loc.f+a.f,loc.s+a.s})) {
                searchBoard2[loc.f + a.f][loc.s + a.s] -= 1;
            }
        }
        actualBoard2[loc.f][loc.s]=0;
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
int boardNumber=0;
void sleeper(){
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    cout<<boardNumber<<endl;
    boardNumber++;
}

int main() {
    cout << "Enter number of Queens: " << endl;
    if (!(cin >> Q) || Q < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Rooks: " << endl;
    if (!(cin >> R) || R < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Bishops: " << endl;
    if (!(cin >> B) || B < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Knights: " << endl;
    if (!(cin >> N) || N < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    cout << "Enter number of Kings: " << endl;
    if (!(cin >> K) || K < 0) {
        cout << "Invalid input. Please enter a non-negative integer." << endl;
        return 0;
    }
    searchBoard.resize(8,vector<int>(8,0));
    searchBoard2.resize(8,vector<int>(8,0));
    remapCC.resize(64);
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            remapCC[numbering[row][col]]={row,col};
        }
    }

    for(int j=0;j<Q;j++){
        depthMap.push_back('Q');
        cumulativeJ++;
    }
    for(int j=0;j<R;j++){
        depthMap.push_back('R');
        cumulativeJ++;
    }
    for(int j=0;j<B;j++){
        depthMap.push_back('B');
        cumulativeJ++;
    }
    for(int j=0;j<K;j++){
        depthMap.push_back('K');
        cumulativeJ++;
    }
    for(int j=0;j<N;j++){
        depthMap.push_back('N');
        cumulativeJ++;
    }

    loadTextures();
    bool runOnce=false;
    while (window.isOpen()){

        if(!runOnce){
            drawBoard(window, false);
            drawBoard(window, true);
            window.display();
            cout<<"Running Algorithm 1 (Dynamic Programming Depth First Search)"<<endl;
            dfs(0);
            cout<<"Finished Algorithm 1"<<endl;
            cout<<"Found "<<possibleBoards.size()<<" Boards"<<endl;
            cout<<"Running Algorithm 2 (Brute Force Depth First Search)"<<endl;
            dfsBruteForce(0);
            cout<<"Finished Algorithm 2"<<endl;
            cout<<"Found "<<possibleBoards2.size()<<" Boards"<<endl;
            runOnce=true;
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.display();
    }

    return 0;
}
