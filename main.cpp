//
//  main.cpp
//  Lumen
//
// 
//

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

const int rows = 8;
const int columns = 22;
const int cells = rows * columns;
const int start = 1*  columns + 1;
const int symbols = 1* columns +19;
const int note = 6 * columns + 2;
const int drawer = 6 * columns +19;
const int Exit = 1* columns +2;
const int infinity = 1000000;

class Map{
private:
    // * is wall
    // . is walkable floor
    char tiles[rows][columns + 1] = {
        "**********************",
        "*....................*",
        "*........*......*....*",
        "*....*..........*....*",
        "*....*......*........*",
        "*................*...*",
        "*....................*",
        "**********************"
    };
    std::vector<std::vector<int>>connections;
    
    int playerrow = 1;
    int playercolumn = 1;
    
    static const int rows = 8;
    static const int columns= 22;
    
    int getId(int row, int column) const {
        return row * columns + column;
    }
    
    void makinggraph(){
        connections.resize(cells);
        int rowchange[4] = {-1, 1, 0, 0};
        int columnchange[4] = {0, 0, -1, 1};
        
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                if (!iswalkable(row, column)){
                    continue;
                }
                int current = getId(row, column);
                
                for (int direction = 0; direction < 4; ++direction) {
                    int nextrow = row + rowchange[direction];
                    int nextcolumn = column + columnchange[direction];
                    
                    if (iswalkable(nextrow, nextcolumn)){
                        int next = getId(nextrow, nextcolumn);
                        connections[current].push_back(next);
                    }
                }
            }
        }
    }
public:
    Map(){
        
        makinggraph();
    }
    bool iswalkable(int row, int column) const {
        if(row<0 || row>= rows){
            return false;
        }
        if (column <0 || column>= columns){
            return false;
        }
        return tiles[row][column] == '.';
    }
    
    int playerposition() const{
        return getId(playerrow, playercolumn);
    }
    
    bool moveplayer(const std::string& direction){
        int nextrow = playerrow;
        int nextcolumn = playercolumn;
        
        if (direction == "up"){
            --nextrow;
        }
        
        else if (direction == "down") {
            ++nextrow;
            
        }
        
        else if (direction == "left") {
            --nextcolumn;
        }
        
        else if (direction == "right") {
            ++nextcolumn;
            
        }
        
        else {
            return false;
        }
        
        if (!iswalkable(nextrow, nextcolumn)){
            return false;
        }
        
        int current = playerposition();
        int destination = getId(nextrow, nextcolumn);
        
        for (int next : connections[current]){
            if (next == destination){
                playerrow = nextrow;
                playercolumn = nextcolumn;
                return true;
            }
        }
        return false;
    }
    std::vector<int> BFS (int destination) const {
        std::vector<int>(route);
        
        if (destination < 0 || destination>= cells){
            return route;
        }
        
        int destinationrow = destination/columns;
        int destinationcolumn = destination % columns;
        
        if (!iswalkable(destinationrow, destinationcolumn)){
            return route;
        }
        std::vector<int> distance(cells,infinity);
        std::vector<int> previous(cells, -1);
        
        std::queue<int>pending;
        
        int beginning = playerposition();
        
        distance[beginning] = 0;
        pending.push(beginning);
        
        while (!pending.empty()) {
            int current = pending.front();
            pending.pop();
            
            for (int next : connections [current]) {
                if (distance[next] == infinity) {
                    distance[next] = distance[current] +1;
                    previous[next] = current;
                    pending.push(next);
                }
            }
        }
        
        if (distance[destination] == infinity) {
            return route;
        }
        
        int current = destination;
        while (current != beginning) {
            route.push_back(current);
            current = previous[current];
        }
        
        route.push_back(beginning);
        
        std::vector<int> correctorder;
        
        for (int i = static_cast<int>(route.size()) - 1;
             i >= 0;
             --i){
            correctorder.push_back(route[i]);
        }
        
        return  correctorder;
    }
    
    void print(bool lampOn) const {
        std::cout << "\n";
        
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column){
                int current = getId(row, column);
                
                if (current == playerposition()) {
                    std:: cout << '@';
                }
                
                else if (!lampOn){
                    std::cout << '?';
                }
                else if (current == symbols) {
                    std::cout << 'S';
                }
                else if (current == note){
                    std::cout << 'N';
                }
                else if (current == drawer){
                    std::cout << 'D';
                }
                else if (current == Exit) {
                    std::cout << 'E';
                }
                else {
                    std::cout << tiles[row][column];
                }
            }
            std::cout << '\n';
        }
        if (lampOn){
            std::cout << "\n@ player | S symbols | N note";
            std::cout << " | D drawer | E exit\n";
        }
    }
    
    bool saveimage(const std::string& message, bool lampOn) const {
        std::ofstream image("lumenmap.svg");
        
        if (!image) {
            return false;
        }
        
        const int tilesize = 60;
        
        int width = columns * tilesize;
        int mapheight = rows * tilesize;
        int height = mapheight + 150;
        
        image << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << width << "\" "
        << "height=\"" << height << "\" "
        << "viewBox=\"0 0 " << width << " " << height << "\">\n";
        
        image << "<rect width=\"100%\" height=\"100%\" "
        << "fill=\"#101923\"/>\n";
        
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                std::string color;
                
                if (!lampOn) {
                    color = "#172331";
                }
                else if (iswalkable(row, column)) {
                    color = "#314f69";
                }
                else {
                    color = "#8497ad";
                }
                
                image << "<rect x=\"" << column * tilesize
                << "\" y=\"" << row * tilesize
                << "\" width=\"" << tilesize - 1
                << "\" height=\"" << tilesize - 1
                << "\" fill=\"" << color << "\"/>\n";
                
                if (lampOn) {
                    int current = getId(row, column);
                    std::string label;
                    
                    if (current == symbols) {
                        label = "S";
                    }
                    else if (current == note) {
                        label = "N";
                    }
                    else if (current == drawer) {
                        label = "D";
                    }
                    else if (current == Exit) {
                        label = "E";
                    }
                    
                    if (!label.empty()) {
                        image << "<text x=\"" << column * tilesize + 20
                        << "\" y=\"" << row * tilesize + 40
                        << "\" fill=\"white\" "
                        << "font-family=\"sans-serif\" "
                        << "font-size=\"24\">"
                        << label << "</text>\n";
                    }
                }
            }
        }
        
        int playerx = playercolumn * tilesize + tilesize / 2;
        int playery = playerrow * tilesize + tilesize / 2;
        
        image << "<circle cx=\"" << playerx
        << "\" cy=\"" << playery
        << "\" r=\"15\" fill=\"#f7f7f7\"/>\n";
        
        image << "<rect x=\"0\" y=\"" << mapheight
        << "\" width=\"" << width
        << "\" height=\"150\" fill=\"#172331\"/>\n";
        
        image << "<text x=\"25\" y=\"" << mapheight + 35
        << "\" fill=\"#c4b91b\" "
        << "font-family=\"sans-serif\" font-size=\"20\">"
        << "LUMEN</text>\n";
        
        image << "<text x=\"25\" y=\"" << mapheight + 80
        << "\" fill=\"white\" "
        << "font-family=\"sans-serif\" font-size=\"16\">"
        << message << "</text>\n";
        
        image << "</svg>\n";
        image.close();
        
        return !image.fail();
    }
};

class puzzlegraph {
private:
    std::vector<std::string>puzzles;
    std::vector<std::vector<int>>connections;
    
    bool isValidpuzzel (int id) const{
        return id >= 0 &&
        static_cast<std::size_t>(id) < puzzles.size();
        
    }
    
public:
    int addpuzzle (const std::string& name) {
        puzzles.push_back(name);
        connections.emplace_back();
        return static_cast<int>(puzzles.size()) - 1;
    }
    void addconnections(int from,int to) {
        if (!isValidpuzzel(from) || !isValidpuzzel(to)) {
            std::cout << "Error: Invalid puzzle ID.\n";
            return;
        }
        
        connections[from].push_back(to);
        
    }
    void print() const {
        for (std::size_t i = 0; i< puzzles.size(); ++i) {
            std::cout << puzzles[i] << " leads to: ";
            
            if(connections[i].empty()) {
                std::cout << "no next puzzle";
            }
            for (int next : connections[i]) {
                std::cout<< puzzles[next] << " ";
            }
            std::cout<< '\n';
        }
    }
    
    void BFS (int start) const {
        if(!isValidpuzzel(start)) {
            std::cout << "invalid starting puzzle.\n";
            return;
        }
        
        std::vector<bool> visited (puzzles.size(), false);
        std::queue<int> pending;
        
        visited[start] = true;
        pending.push(start);
        
        while(!pending.empty()) {
            int current = pending.front();
            pending.pop();
            
            std::cout<< puzzles[current]<< '\n';
            
            for (int next : connections[current]){
                if(!visited.at(next)){
                    visited[next] = true;
                    pending.push(next);
                }
            }
        }
    }
};


class Game {
private:
    
    Map map;

    bool lampFound = false;
    bool lampOn = false;
    
    std::string message = "You wake up on a cold floor. Everything is dark. Find a lamp.";
    
    void updatemap() const {
        std::cout<< "\n" << message << "\n";
        
        if (!map.saveimage(message, lampOn)) {
            std::cerr << "Cannot save lumenmap.svg.\n";
        }
    }
    
    void showMenu() const {
        std::cout << "\nWhat will you do??\n";
        std::cout << "1. Look around\n";
        std::cout << "2. Search nearby\n";
        std::cout << "3. Switch on the light\n";
        std::cout << "4. Move\n";
        std::cout << "5. Quit\n";
        std::cout << "> " ;
    }
    void lookAround() {
        if (lampOn) {
            message = 
            "The lamp reveals wall and open path.";
        } else {
            message =
            "You cannot see anything. Somewhere closer, water drips onto floor.";
        }
    }
    void searchNearby() {
        if (!lampFound) {
            lampFound = true;
            
           message = "You carefully feel along the floor. Your hand touches a lamp. You pick it up and find its switch.";
        } else {
            message = "You search within reach, but find nothing else.";
        }
    }
    void switchoLight(){
        if (!lampFound){
            message = "You need to find a lamp first.";
        } else if (!lampOn) {
            lampOn = true;
            
            message = "The lamp is on. You can explore";
        } else {
            message = "The lamp is already on.";
        }
    }
    void walkForward() {
        if (!lampOn) {
            message = "It is too dark to walk. You need to find and switch on a light.";
            return;
        }
            std::cout << "\nWhich direction do you want to go? up, down, left, right\n" << "> ";
        
        std::string direction;
        if (!std::getline(std::cin, direction)) {
            message = "No direction entered.";
            return;
        }
        if (direction !="up" && direction != "down" && direction != "left" && direction != "right") {
            message = "Unknown direction. Enter up, down, left or right." ;
            return;
        }
        if (map.moveplayer(direction)) {
            message = "You move " + direction + ", carrying the lamp.";
        } else {
            message = "A wall blocks your way.";
        }
    };
    
public:
    void run() {
        std::cout << "L U M E N\n\n"
        << "You wake up on a cold floor.\n"
        << "Everything around you is dark.\n"
        << "You need to find a light.\n";
        
        updatemap();
        std::string choice;
        
        while (true) {
            showMenu();
            if (!std::getline(std::cin, choice)) {
                return;
            }
            if (choice == "1") {
                lookAround();
            }
            else if (choice == "2") {
                searchNearby();
            }
            else if (choice == "3") {
                switchoLight();
            }
            else if (choice == "4") {
                walkForward();
            }
            else if (choice == "5") {
                message = "Closing Lumen.";
                updatemap();
                return;
            } else {
                message = "Invalid choice. Enter a number from 1 to 5.";
            }
            updatemap();
        }
        
    }
};

int main(){
    puzzlegraph room;
    int lamp = room.addpuzzle("turn on lamp");
    int symbol = room.addpuzzle("discover wall symbol");
    int drawer = room.addpuzzle("open drawer");
    int key = room.addpuzzle("collect keys");
    int exit = room.addpuzzle("unlock exit");
    
    room.addconnections(lamp,symbol);
    room.addconnections(symbol, drawer);
    room.addconnections(drawer, key);
    room.addconnections(key, exit);
    
    std::cout<< "Lumen puzzle graph:\n";
    room.print();
    
    std::cout << "\nBFS traversal from the lamp:\n";
    room.BFS(lamp);
    
    std::ofstream preview("lumenpreview.html");
    
    if(!preview) {
        std::cerr << "Cannot create lumenpreview.html.\n";
        return 1;
    }
    
    preview << R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content = "width = device-width, initial-scale=1">
    <title> Lumen </title>
    
    <style>
body{ margin: 0;
padding: 20px;
background: #101923;
color: white;
font-family: sans-serif;
}
img{
display: block;
width: 100%;
max-width: 1320px;
height:auto;
margin: auto;
}
p {
text-align: center;
}

</style>
</head>
<body>
<img src="lumenmap.svg" alt="Lumen map and story">
<p> Choose an action in the console, then refresh to see the result.</p>
</body>
</html>
)HTML" ;
            
    preview.close();
    if (!preview) {
        std::cerr << "Cannot finish saving lumenpreview.html.\n";
        return 1;
    }
    std::cout << "\nOpen lumenpreview.html for updates.\n";
    Game game;
    game.run();
    
    return 0;
};
