#pragma once
#include "Board.hpp"
#include "Piece.hpp"
#include<unordered_map>
class Manager{
    private:
        string player = WHITE;
        string player_name = WHITE_PLAYER;
        unordered_map<string,vector<Piece *>> captured;
        unordered_map<string,Piece*> name_map;
        unordered_map<string,int> promoted_count;
        Board chess_board;
        bool check_mate = false;
        bool check = false;
        vector<Piece*> checking_pieces();
        bool can_block(int x, int y);
        bool is_not_blockable_check(vector<Piece*> &checking_pieces);
        bool has_discover_check(Piece* pice, int x, int y);
        void initialize_board(int r1, int r2, string type);
        string switch_player(){
            return player = player==WHITE?BLACK : WHITE;
        }
        string get_player_name(){
            return player_name = player == WHITE?WHITE_PLAYER : BLACK_PLAYER;
        }
        short get_direction(Piece* p){
            return p->get_color() == BLACK ? -1 : 1;
        }
        void update_state(int x,int y,Piece* p);

        void capture_piece(string symbol,int x,int y);

        bool is_valid_piece(Piece* p){
            return p->get_symbol().find(player)!=string::npos;
        }
        void update_possible_moves();
        void player_move(int x, int y, string piece_name);
        bool is_valid_move(int x,int y, Piece* piece);
        void update_for_check();
        bool is_check_mate(vector<Piece*> &checking_pieces);
        bool can_promote_piece(Piece *p,int x,int y);
        Piece* piece_selection(int x,int y);
        void promote_piece(Piece* &p, int x,int y);
    public:
        Manager();
        void run_cmd();
};