#pragma once
#include<vector>
#include<iostream>
#include "Constants.hpp"
#include "Board.hpp"
using namespace std;
class Piece{
    protected:
        pair<int,int> pos;
        vector<pair<int,int> > moves;
        string symbol; 
        void populate_possible_moves_default(Board board,int lmt = 7, bool allow_diag = true, bool allow_ver = true);
        bool block_piece_handle_default(Board board,string curr_type,int x, int y);
    public:
        Piece(string symbol,int x,int y):symbol(symbol),pos(make_pair(x,y)){}

        virtual vector<pair<int,int> > possible_moves(short pawn_dir,Board board){
            return moves;
        };

        void set(int x,int y){
                pos.first = x, pos.second = y;
                reset_moves();
        }

        void reset_moves(){ moves.clear();}

        pair<int,int> get(){
            return pos;
        }

        vector<pair<int,int>> get_moves(){
            return moves;
        }

        bool is_present(pair<int,int> pos);

        void print_possible_moves();

        string get_symbol(){return symbol;}

        string get_color(){
            return symbol.find(WHITE)!=string::npos?WHITE : BLACK;
        }
};

class King:public Piece{
    public:
        King(string color,int x,int y):Piece(KING+color,x,y){}
        vector<pair<int,int> > possible_moves(short pawn_dir,Board board);
};

class Pawn:public Piece{
    private:
        int index;
    public:
        Pawn(string color, int index,int x,int y):Piece(PAWN+to_string(index)+color,x,y),index(index){}
        vector<pair<int,int> > possible_moves(short pawn_dir,Board board);
};

class Knight:public Piece{
    private:
        int index;
    public:
        Knight(string color,int index,int x,int y):Piece(KNIGHT+to_string(index)+color,x,y),index(index){}
        vector<pair<int,int> > possible_moves(short pawn_dir,Board board);
};

class Bishop:public Piece{
    private:
        int index;
    public:
        Bishop(string color,int index,int x,int y):Piece(BISHOP+to_string(index)+color,x,y),index(index){}
        vector<pair<int,int> > possible_moves(short pawn_dir, Board board);
};

class Queen:public Piece{
    public:
        Queen(string color,int x,int y):Piece(QUEEN+color,x,y){}
        vector<pair<int,int> > possible_moves(short pawn_dir,Board board);
};

class Rook:public Piece{
      private:
        int index;
      public:
        Rook(string color,int index,int x,int y):Piece(ROOK+to_string(index)+color,x,y),index(index){}
        vector<pair<int,int> > possible_moves(short pawn_dir, Board board);
};