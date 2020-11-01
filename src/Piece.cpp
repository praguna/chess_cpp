#include "../include/Piece.hpp"

void Piece::print_possible_moves(){
    cout<<"Piece :"<<symbol<<" possible moves"<<endl;
            if(moves.empty()) cout<<"Not Present"<<endl;
            for(pair<int,int> x : moves){
                cout<<x.first<<" "<<x.second<<endl;
            }
     cout<<endl;
}

bool Piece::is_present(pair<int,int> pos){
    return find(moves.begin(),moves.end(),pos)!=moves.end(); 
}

void Piece::populate_possible_moves_default(Board board,int lmt, bool allow_diag, bool allow_ver){
    string curr_type = board.get_piece_type(pos.first,pos.second);
    vector<bool> directional_block(8);
    // scan horizontal positions from current location
    // scan vertical positions from current location
    for(int i = 1; i <= lmt and allow_ver; i++){
        if(pos.second - i >= 0 and  !directional_block[0] and !(directional_block[0] = block_piece_handle_default(board,curr_type,pos.first, pos.second - i)))
            moves.push_back(make_pair(pos.first, pos.second - i));
        if(pos.second + i < 8 and !directional_block[1] and !(directional_block[1] = block_piece_handle_default(board,curr_type,pos.first, pos.second + i)))
            moves.push_back(make_pair(pos.first, pos.second + i));
         if(pos.first - i >= 0 and !directional_block[2] and !(directional_block[2] = block_piece_handle_default(board,curr_type,pos.first-i, pos.second)))
            moves.push_back(make_pair(pos.first-i, pos.second));
        if(pos.first + i < 8 and !directional_block[3] and !(directional_block[3] = block_piece_handle_default(board,curr_type,pos.first+i, pos.second)))
            moves.push_back(make_pair(pos.first+i, pos.second));
    }

    // scan diagonal positions from current location
    for(int i = 1; i <= lmt && allow_diag; i++){
        if(pos.second - i >=0 and pos.first+i < 8  and !directional_block[4] 
            and !(directional_block[4] = block_piece_handle_default(board,curr_type,pos.first + i, pos.second - i)))
            moves.push_back(make_pair(pos.first + i, pos.second - i));
        if(pos.second + i <8 and pos.first+i < 8 and !directional_block[5] 
            and !(directional_block[5] = block_piece_handle_default(board,curr_type,pos.first + i, pos.second + i)))
            moves.push_back(make_pair(pos.first + i, pos.second + i));
        if(pos.second + i < 8 and pos.first - i >=0 and !directional_block[6] 
            and !(directional_block[6] = block_piece_handle_default(board,curr_type,pos.first - i, pos.second + i)))
            moves.push_back(make_pair(pos.first - i, pos.second + i));
        if(pos.second - i >=0 and pos.first - i >=0 and !directional_block[7] 
            and !(directional_block[7] = block_piece_handle_default(board,curr_type,pos.first - i, pos.second - i)))
            moves.push_back(make_pair(pos.first - i, pos.second - i));
    }  
}

bool Piece::block_piece_handle_default(Board board,string curr_type,int x, int y){
     if(board.is_present(x, y)){
         if(board.not_matches_type(curr_type, x, y))
             moves.push_back(make_pair(x,y));
         return true;
     }
     return false;
}

vector<pair<int,int> >  King::possible_moves(short allow_castle,Board board){
        populate_possible_moves_default(board,1);
        if(!board.has_moved(symbol) and allow_castle){
            int r = board.default_king_row(board.get_piece_type(pos.first,pos.second));
            if(!board.has_moved(r,0) and !board.num_pieces_between_hor(pos.second,0,r)) moves.push_back({pos.first,pos.second-2});
            if(!board.has_moved(r,7) and !board.num_pieces_between_hor(pos.second,7,r)) moves.push_back({pos.first,pos.second+2});
        }
        return moves;
    }

vector<pair<int,int> >  Pawn::possible_moves(short pawn_dir,Board board){
        string curr_type = board.get_piece_type(pos.first,pos.second);
        int new_x = pos.first + 1 * pawn_dir;
        if(!board.is_present(new_x,pos.second))moves.push_back({new_x,pos.second});
        if(pos.second + 1 < 8 and board.not_matches_type(curr_type,new_x,pos.second + 1)) moves.push_back({new_x, pos.second+1});
        if(pos.second - 1 >=0 and board.not_matches_type(curr_type,new_x, pos.second-1)) moves.push_back({new_x, pos.second-1});
        new_x= new_x+ 1 * pawn_dir;
        if(pos.first == 6 or pos.first == 1 and !board.is_present(new_x,pos.second)) moves.push_back({new_x,pos.second});
        return moves;
}



vector<pair<int,int> > Knight::possible_moves(short ignore,Board board){
       string curr_type = board.get_piece_type(pos.first,pos.second); 
       if(pos.first + 2 < 8 and pos.second + 1 < 8 and board.not_matches_type_if_present(curr_type, pos.first + 2, pos.second + 1)) 
         moves.push_back(make_pair(pos.first+2,pos.second+1));
       if(pos.first + 2 < 8 and pos.second - 1 >=0 and board.not_matches_type_if_present(curr_type, pos.first + 2, pos.second - 1))
          moves.push_back(make_pair(pos.first+2,pos.second-1));
       if(pos.first - 2 >= 0 and pos.second + 1 < 8 and board.not_matches_type_if_present(curr_type, pos.first - 2, pos.second + 1)) 
         moves.push_back(make_pair(pos.first-2,pos.second+1));
       if(pos.first - 2 >= 0 and pos.second - 1 >=0 and board.not_matches_type_if_present(curr_type, pos.first - 2, pos.second - 1))
          moves.push_back(make_pair(pos.first-2,pos.second-1));
       return moves; 
}

vector<pair<int,int> > Bishop::possible_moves(short ignore,Board board){
       populate_possible_moves_default(board,7,true,false);
       return moves;
}

vector<pair<int,int> > Queen::possible_moves(short ignore,Board board){
       populate_possible_moves_default(board,7,true,true);
       return moves;
}

vector<pair<int,int> > Rook::possible_moves(short ignore,Board board){
       populate_possible_moves_default(board,7,false,true);
       return moves;
}