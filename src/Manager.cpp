#include "../include/Manager.hpp"

//initialize the board with pawns
Manager::Manager(){
    chess_board = Board();
    initialize_board(0,1,WHITE);
    initialize_board(7,6,BLACK);
    chess_board.print_board();
}

void Manager::initialize_board(int r1,int r2,string val){
    Piece* p;
    vector<Piece*> pieces;
    for(int i = 0; i < 8; i++){
        p = new Pawn(val, i,r2,i);
        name_map[p->get_symbol()] = p;
        auto pos = p->get();
        chess_board.set(pos.first,pos.second,p->get_symbol());
    }
    pieces = {new Rook(val,0,r1,0) , new Rook(val,1,r1,7), new Bishop(val,0,r1,2), new Bishop(val,1,r1,5), 
            new Knight(val,0,r1,1), new Knight(val,1,r1,6), new King(val,r1,4), new Queen(val,r1,3)};

    for(auto x : pieces){
        auto pos = x->get();
        chess_board.set(pos.first,pos.second,x->get_symbol());
        name_map[x->get_symbol()] = x;
    }
}

void Manager::update_possible_moves(){
    for(auto e : name_map){
        e.second->reset_moves();
        e.second->possible_moves(get_direction(e.second),chess_board);
    }
}

void Manager::run_cmd(){
    string pawn_name;
    int x, y;
    while (true){
        if(check_mate){
            cout<<"\n\n\t*** Check Mate "<<player_name<<" has own !!! ***"<<endl;
            break;
        }
        cout<<"Turn : "<<get_player_name()<<endl;
        cout<<"move (pawn name), (row integer), (column integer) :";
        cin>>pawn_name;
        if(pawn_name == "exit") break;
        cin>>x>>y;
        cout<<pawn_name<<" "<<x<<" "<<y<<" "<<endl;
        update_possible_moves();
        player_move(x,y,pawn_name);
    }
    
}

void Manager::player_move(int x,int y, string pawn_name){
    if(!name_map.count(pawn_name)) return;
    Piece* p = name_map[pawn_name];
    if(is_valid_piece(p) and is_valid_move(x,y,p) and !has_discover_check(p,x,y)){
        // current player section
        if(chess_board.is_present(x,y)){
            capture_piece(p->get_symbol(),x, y);
        }
        if(can_promote_piece(p,x,y)){
            promote_piece(p,x,y);
        }
        else{ 
            update_state(x,y,p);
        }
        // next player section 
        update_for_check();
        chess_board.print_board();
    }
}

void Manager::update_state(int x, int y, Piece *p){    
        auto pos = p->get();
        chess_board.move(pos.first,pos.second,x,y);
        p->set(x,y);
        p->possible_moves(get_direction(p),chess_board);
        switch_player();
}

void Manager::capture_piece(string val,int x,int y){
        string symbol = chess_board.get(x,y);
        cout<<val<<" Captured (x) "<<symbol<<endl;
        captured[player].push_back(name_map[symbol]);
        name_map.erase(symbol);
        chess_board.capture_piece(player,symbol);
}

void Manager::update_for_check(){
    check = false;
    auto pieces = checking_pieces();
    if(pieces.empty()) return;
    if(is_check_mate(pieces)){
        switch_player();
        check_mate = true;
        return;
    }
    cout<<"\n*** Check !! on "<<get_player_name()<<"'s KING ***"<<endl;
    check = true;
}

bool Manager::is_check_mate(vector<Piece*> &checking_pieces){
    // can't block piece and can't escape
    auto king = name_map[KING+player];
    auto king_moves= king->get_moves();
    int num_moves = king_moves.size();
    for(auto move : king_moves){
        for(auto e : name_map){
            if(e.second != king and e.second->get_color()!=player and e.second->is_present(move)){
                    --num_moves;
                    break;
            }
        }
    }
    if(num_moves > 0) return false;
    if(checking_pieces.size() > 1) return true;
     // path block check
    return is_not_blockable_check(checking_pieces);
}


bool Manager::is_not_blockable_check(vector<Piece*> &checking_pieces){
    auto king = name_map[KING+player];
    pair<int,int> dst = king->get(); 
    short x_diff = 0, y_diff = 0, x_inc = 0, y_inc = 0; 
    Piece* piece = checking_pieces.front();
    pair<int,int> src = piece->get();
    x_diff =  dst.first - src.first, y_diff =  dst.second - src.second;
    if(x_diff) x_inc = x_diff < 0?-1 : 1;
    if(y_diff) y_inc = y_diff < 0?-1 : 1;  
    if(abs(x_diff) == abs(y_diff) or !x_diff or !y_diff){
    while(x_diff!=0 or y_diff!=0){
            if(can_block(x_diff+src.first,y_diff+src.second)) return false;
            if(x_diff) x_diff-=x_inc;
            if(y_diff) y_diff-=y_inc;
        }
    }
    return !can_block(src.first, src.second);
}

bool Manager::can_block(int x,int y){
    // discover check
    auto king = name_map[KING+player];
    for(auto e : name_map){
        if(e.second != king and e.second->get_color()==player and e.second->is_present({x,y}) and !has_discover_check(e.second,x,y)){
                return true;
        }
    }
    return false;
}

bool Manager::has_discover_check(Piece* p, int x, int y){
    pair<int,int> pos = p->get();
    Piece *d = NULL;
    if(chess_board.is_present(x,y)) {
        d = name_map[chess_board.get(x,y)];
        name_map.erase(chess_board.get(x,y));
    }
    p->set(x,y);
    chess_board.move(pos.first,pos.second,x,y);
    update_possible_moves();
    bool res = false;
    if(!checking_pieces().empty()) res = true;
    p->set(pos.first,pos.second);
    chess_board.move(x,y,pos.first,pos.second);
    if(d){
        name_map[d->get_symbol()] = d;
        chess_board.set(x,y,d->get_symbol());
    }
    update_possible_moves();
    return res;
}

vector<Piece*> Manager::checking_pieces(){
    vector<Piece *> pieces;
    auto king = name_map[KING+player];
    for(auto e : name_map){
        if(e.second != king and e.second->get_color()!=player and e.second->is_present(king->get())){
            pieces.push_back(e.second);
        }
    }
    return pieces;
}

bool Manager::can_promote_piece(Piece *p,int x,int y){
    pair<int,int> pos = p->get();
    return  p->get_symbol().find(PAWN)!=string::npos and (x == 0 and p->get_color() == BLACK) or (x == 7 and p->get_color() == WHITE);
}

bool Manager::is_valid_move(int x, int y, Piece* piece){
    for(auto move : piece->get_moves()){
        if(move.first == x and move.second == y) return true;
     }
    piece->print_possible_moves();
    return false;
}

Piece* Manager::piece_selection(int x, int y){
    char name;
    int new_index = 10 + promoted_count[player]++;
    while(true){
        cout<<"promote pawn to (Q, B, K, P, R) :";
        cin>>name;
        cout<<name<<endl;
        switch (name){
            case 'Q': return new Queen(player,new_index,x,y);
            case 'B' : return new Bishop(player,new_index,x,y);
            case 'K' : return new Knight(player,new_index,x,y);
            case 'R' : return new Rook(player,new_index,x,y);
            case 'P' : return new Pawn(player,new_index,x,y);
        }
    }
    return NULL;
}

void Manager::promote_piece(Piece* &p, int x,int y){
        auto pos = p->get();
        chess_board.move(pos.first,pos.second,x,y);
        name_map.erase(p->get_symbol());
        p = piece_selection(x,y);
        name_map[p->get_symbol()] = p;
        chess_board.set(x,y,p->get_symbol());
        p->possible_moves(get_direction(p),chess_board);
        switch_player();
}