#pragma once
#include<vector>
#include<iostream>
#include<unordered_map>
#include "Constants.hpp"
using namespace std;
class Board{
    private:
        vector<vector<string> > board;
        unordered_map<string,vector<string> > capture; 
        unordered_map<string,bool > moved;
        void print_captured_piece(string player);
    public:
        Board();
        void move(int x1,int y1,int x2,int y2);
        void print_board();
        void set(int i,int j, string val){
            board[i][j] = val;
        }
        string get(int i,int j){
            return board[i][j];
        }
        bool is_present(int i, int j){
            return  i<8 and i>=0 and j<8 and j>=0 and board[i][j]!=BLACK and board[i][j]!=WHITE;
        }
        string color_from_pos(int i, int j){
            if(i&1) return (j&1)?BLACK:WHITE;
            return (j&1)?WHITE:BLACK;
        }
        string get_piece_type(int i, int j){
            return get(i,j).find(WHITE)!=string::npos?WHITE : BLACK;
        }
        bool not_matches_type(string type,int i,int j){
            if(is_present(i,j)) return type != get_piece_type(i,j);
            return false;
        }
        bool not_matches_type_if_present(string type, int i, int j){
             return not_matches_type(type,i,j) or !is_present(i,j);
        }
        void capture_piece(string player, string name){
            capture[player].push_back(name);
        }
        int num_pieces_between_hor(int y1, int y2,int x);
        bool has_moved(string symbol){
            return moved[symbol];
        }
        bool has_moved(int x,int y){
            return is_present(x,y) and moved[get(x,y)];
        }
        void set_moved(string symbol){
            moved[symbol] = true;
        }
        int default_king_row(string player){
            return player==WHITE?0 : 7;
        }
 }; 