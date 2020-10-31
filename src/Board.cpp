#include "../include/Board.hpp"
Board::Board(){
     board.resize(8,vector<string>(8));
            for (size_t i = 0; i < 8; i++){
                for(size_t j = 0; j < 8; j++){
                    board[i][j] = color_from_pos(i,j);
                }
            }
}

void Board::move(int x1,int y1,int x2,int y2){
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = color_from_pos(x1,y1); 
}

void Board::print_captured_piece(string player){
    for(auto x : capture[player]) cout<<x<<" ";
    cout<<"\n\n";
}

void Board::print_board(){
    cout<<"\n";
    print_captured_piece(BLACK);
    cout<<"Black Side \n"<<endl;
    for(int i = 7; i >=0; i--){
        for(int j = -1; j < 8; j++){
            if(j == -1) cout<<i<<"\t";
            else cout<<board[i][j]<<"\t";
        }
        cout<<"\n\n";
    }
    cout<<"\t";
    for(int i = 0; i < 8; i++) cout<<i<<"\t";
    cout<<"\n\nWhite Side \n"<<endl;
    print_captured_piece(WHITE);
}

int Board::num_pieces_between_hor(int y1, int y2,int x){
    int i = y1, j = y2, res = 0;
    if(y1 > y2){
        i = y2, j = y1;
    }
    while(++i < j){
        if(is_present(x,i)) ++res;
    }
    return res;
}