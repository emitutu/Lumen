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

class puzzlegraph {
private:
    std::vector<std::string> puzzles;
    std::vector<std::vector<int>> connections;
    
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
            std::cout << puzzles[i] << "leads to: ";
            
            if(connections[i].empty()) {
                std::cout << "no text puzzle";
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
    
    return 0;
}
