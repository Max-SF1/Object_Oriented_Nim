#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class NimBoard {
    public:
        vector<int> board_placements;

        NimBoard(vector<int> generated_placements) {
            board_placements = generated_placements;  
        }

        bool IsEmpty(){
            return board_placements.empty();
        }

        void DrawBoard() {           
            if (IsEmpty()) {
                cout << "\n=== Board is empty ===" << endl << endl;
                return;
            }
            int star_gap = 0;
            int max_stars = *max_element(board_placements.begin(), board_placements.end());
            cout << "\n=== Current Board ===" << endl;
            for (int i = 0; i < board_placements.size(); i++) {  
                star_gap = (max_stars - board_placements[i])/2;
                cout << i+1 << ") ";  
                cout << string(star_gap, ' ');
                cout << string(board_placements[i], '*') << '\n';
            }
            cout << endl;
        }

        bool MakeMove(int row, int amount){
            if (isValidMove(row, amount) && !(IsEmpty()))
            {
                apply_move(row, amount);
                return true;
            }
            else {
                cout << "\n>>> Invalid move! try again! <<<\n" << endl;
                return false;
            }
        }

    private: 
        bool isValidMove(int row, int amount)
        {
            row = row-1;
            if (amount <= 0 || board_placements[row]==0) {
                return false; 
            }
            if (row>= 0 && row < static_cast<int>(board_placements.size())) {
                if (board_placements[row]>=amount)
                {
                    return true; 
                }
                else
                {
                    return false; 
                }
            }
            else return false; 
        }
        void apply_move(int row, int amount)
            {
                row = row-1; 
                board_placements[row] -= amount;
                clean_empty_rows();
            }
        void clean_empty_rows()
            {
                vector <int> copy; 
                for (int placement: board_placements){
                    if (placement != 0) {
                        copy.push_back(placement);
                        }           
                    }
                board_placements = copy; 
            } 
    };

class Board_Fetcher { 
    public:
        static NimBoard Construct_Board(){
            cout << "\n=== Game Setup ===" << endl;
            cout << "Custom board? [y/n]" << endl;
            string answer; 
            cin >>  answer;
            if (answer != "n") 
            {   
                std::vector<int> user_board;
                int rows_num; 
                cout << "\nEnter a number of rows: " << endl;
                cin >> rows_num;
                for (int i=0; i<rows_num; i++) {
                    int objects_in_row;
                    cout << "Enter a number for the amount of objects in row " << i+1 << ": " << endl;
                    cin >> objects_in_row;
                    user_board.push_back(objects_in_row);
                }
                cout << endl;
                return NimBoard(user_board);
                }  
            cout << endl;
            return NimBoard({3,5,7});


        }
};

class Player { 
    public: 
        virtual void EnterMove(NimBoard& board)=0;
        virtual string PlayerData()=0;
};

class HumanPlayer: public Player {
    string name;
    public:
        HumanPlayer() {
            cout << "\nEnter your name: " << endl;
            cin >> name;
            cout << endl;
        }
        string PlayerData() override{
            return name; 
        }
        void EnterMove(NimBoard& board) override{
            int amount;
            int row; 
            do {
                cout << "Enter row: " << endl;
                cin >> row;
                cout << "Enter amount: " << endl;
                cin >> amount; 
                cout << endl;
            } while (!board.MakeMove(row,amount));
        }
};

class RandomBotPlayer : public Player {
    public: 
        static mt19937 gen;
        string PlayerData() override {
            return "RANDOM_MOVE_BOT";
        }
        void EnterMove(NimBoard& board) override {
            uniform_int_distribution<int> dist_rows(1, board.board_placements.size());
            int row = dist_rows(gen);
            uniform_int_distribution<int> dist_amount(1, board.board_placements[row-1]);
            int amount = dist_amount(gen);
            cout << endl;
            board.MakeMove(row,amount);
        }
};

class StrategicallyOptimalPlayer : public Player {
    public: 
        static mt19937 gen;
        string PlayerData() override {
            return "STRATEGICALLY_OPTIMAL_MOVE_BOT";
        }
        void EnterMove(NimBoard& board) override {
            int nimsum=0; 
            for (int row : board.board_placements)
            {
                nimsum = nimsum ^ row; 
            }
            if (nimsum == 0) //no optimal move, yoink a random row and hope the opponent makes an error. 
            {
                uniform_int_distribution<int> dist_rows(1, board.board_placements.size());
                int row = dist_rows(gen);
                uniform_int_distribution<int> dist_amount(1, board.board_placements[row-1]);//convert to 0 based indexing
                int amount = dist_amount(gen);
                board.MakeMove(row,amount); 
            }
            else { //strategy! 
                int amount; // amount to remove from the chosen pile
                for (int row = 0; row < board.board_placements.size(); row++)
                {   
                    int pile = board.board_placements[row];
                    int reduced = pile ^ nimsum; //the amount that balances the nimsum
                    if (reduced < pile)
                    {
                       amount = pile - reduced; // remove this many to make pile == reduced
                       board.MakeMove(row + 1, amount); // MakeMove expects 1-based row
                       return; 
                    }
                }
            }
        }
};

mt19937 RandomBotPlayer::gen(random_device{}());
mt19937 StrategicallyOptimalPlayer::gen(random_device{}());

class GameOrganizer {
    vector <shared_ptr<Player>> players;
    public:
        GameOrganizer(){
            cout << "1) play 1v1 \n2) against optimal bot \n3) against random bot" <<endl;
            string input;
            cin >> input; 
            if (input == "1") {
                players.push_back(make_shared<HumanPlayer>());
                players.push_back(make_shared<StrategicallyOptimalPlayer>());
            }
            else if (input == "2") {
                cout << "go first? [y/n]" <<endl;
                cin >> input; 
                if (input=="y")
                {
                    players.push_back(make_shared<HumanPlayer>());
                    players.push_back(make_shared<StrategicallyOptimalPlayer>());

                }
                else
                {
                    players.push_back(make_shared<StrategicallyOptimalPlayer>());
                    players.push_back(make_shared<HumanPlayer>());
                }
            }
            else {
                cout << "go first? [y/n]" <<endl;
                cin >> input; 
                if (input=="y")
                {
                    players.push_back(make_shared<HumanPlayer>());
                    players.push_back(make_shared<RandomBotPlayer>());

                }
                else
                {
                    players.push_back(make_shared<RandomBotPlayer>());
                    players.push_back(make_shared<HumanPlayer>());
                }
            }
        }
        void StartGame(NimBoard& board){
            shared_ptr <Player> current_player_pointer;
            for (int i = 0; ; ++i) {
                board.DrawBoard(); 
                current_player_pointer = players[i%players.size()]; 
                cout << "\n" << current_player_pointer->PlayerData() << "'s turn:" << endl << endl;
                current_player_pointer->EnterMove(board);
                if (board.IsEmpty()){
                    cout << "\n=== " << current_player_pointer->PlayerData() << " won on move " << i << " ! ===" << endl;
                    break; 
                }
            }
            
        }

};

int main() {
    NimBoard board = Board_Fetcher::Construct_Board();
    GameOrganizer manager;
    manager.StartGame(board);
}


