//
//  position.cpp
//  PawnPusher9000
//
//  Created by Lavi on 2015-11-22.
//  Copyright © 2015 Lavi. All rights reserved.
//

#include "position.h"
#include <sstream>
#include <ctype.h>
#include <iostream>


ChessPosition::ChessPosition(string FEN):pieceTypes("KQRBNPkqrbnp"){
    square= new char * [8];
    for (int i=0;i<8;i++) square[i]=new char [8];
    setupPosition(FEN);
}

ChessPosition::ChessPosition(const ChessPosition & toClone):pieceTypes("KQRBNPkqrbnp"){
    square= new char * [8];
    for (int i=0;i<8;i++) square[i]=new char [8];
    setupPosition(toClone.getFEN());
}


void ChessPosition::clearBoard(){
    for (int x=0;x<8;x++){
        for (int y=0;y<8;y++){
            square[x][y]=' ';
        }
    }
    for (int i=0;i<pieceTypes.length();i++){
        pieceMem[pieceTypes[i]].clear();
    }
}

void ChessPosition::setupPosition(string FEN){
    clearBoard();
    
    stringstream ss(FEN);
    string brd;
    ss >> brd;
    char c;
    int brdpos=0;
    for (string sqrn=" 8";sqrn[1]>='1';sqrn[1]--){
        for (sqrn[0]='a';sqrn[0]<='h';sqrn[0]++){
            c= brd[brdpos];
            if ('0'<c && c<'9'){
                while (c>'0'){
                    setSquare(sqrn, ' ');
                    sqrn[0]++;
                    c--;
                }
                sqrn[0]--;
            }
            else setSquare(sqrn, c);
            brdpos++;
        }
        brdpos++;
    }
    char colour;
    ss >> colour;
    setColourToMove(colour=='b');
    
    string castleInfo;
    ss >> castleInfo;
    for (int i=0;i<4;i++) castleAllowed[i]=false;
    for (int i=0;i<castleInfo.length();i++){
        if (castleInfo[i]=='Q') castleAllowed[0]=true;
        if (castleInfo[i]=='K') castleAllowed[1]=true;
        if (castleInfo[i]=='q') castleAllowed[2]=true;
        if (castleInfo[i]=='k') castleAllowed[3]=true;
    }
    
    string enpassantInfo;
    ss>> enpassantInfo;
    if (enpassantInfo.length()==2){
        canEnPassantAt=enpassantInfo[0]-'a';
    }
    else canEnPassantAt=-1;
    
    ss >> plySinceLastAction;
    ss >> fullMoves;
}

string ChessPosition::getFEN() const{
    stringstream FEN;
    char spaceCount='0';
    for (string sqrn=" 8";sqrn[1]>='1';sqrn[1]--){
        for (sqrn[0]='a';sqrn[0]<='h';sqrn[0]++){
            if (getSquare(sqrn)==' '){
                spaceCount++;
                if (sqrn[0]=='h') {
                    FEN<<spaceCount;
                }
            }
            else{
                if (spaceCount>'0') FEN<<spaceCount;
                FEN<<getSquare(sqrn);
                spaceCount='0';
            }
        }
        if (sqrn[1]>'1') FEN<<"/";
        spaceCount='0';
    }
    FEN<<" ";
    FEN<<(colourToMove()==WHITE?"w":"b");
    FEN<<" ";
    
    string castleInfo="";
    if (castleAllowed[1])
        castleInfo+="K";
    if (castleAllowed[0])
        castleInfo+="Q";
    if (castleAllowed[3])
        castleInfo+="k";
    if (castleAllowed[2])
        castleInfo+="q";
    if (castleInfo=="") castleInfo="-";
    FEN << castleInfo << " ";
    
    if (canEnPassantAt==-1) FEN<<"-";
    else {
        FEN << canEnPassantAt+'a';
        FEN << (colourToMove()==WHITE?"6":"3");
    }
    FEN<<" ";
    FEN<<plySinceLastAction;
    FEN << " ";
    FEN << fullMoves;
    return FEN.str();
}

ChessPosition::~ChessPosition(){
    for (int i=0;i<8;i++) delete [] square[i];
    delete [] square;
}

//checks all but putting king in check
bool ChessPosition::isPsuedoLegalMove(int begPos, int endPos){
    if (!isValidSquare(begPos)) return false;
    if (!isValidSquare(endPos)) return false;
    char piece=getSquare(begPos);
    if (colourToMove()==WHITE && (piece <'A' || piece >'Z')) return false;
    if (colourToMove()==BLACK && (piece <'a' || piece > 'z')) return false;
    if (begPos==endPos) return false;
    //can't capture own pieces
    if (getSquare(endPos)>='A' && getSquare(endPos)<='Z' && colourToMove()==WHITE) return false;
    if (getSquare(endPos)>='a' && getSquare(endPos)<='z' && colourToMove()==BLACK) return false;
    if (colourToMove()==BLACK) piece += 'A'-'a';
    vector<int> rayMoves;
    int cx= endPos%8-begPos%8;
    int cy= endPos/8-begPos/8;
    switch (piece) {
        case 'Q':
        case 'B':
        case 'R':
            for (int i=0;i<9;i++){
                if (i!=4) {
                    if (i%2==0 && piece=='R') continue;
                    if (i%2==1 && piece=='B') continue;
                    rayMoves.push_back(i);
                }
            }
            break;
        case 'N':
            if (cx*cx + cy*cy!=5) return false;
            break;
        case 'K':
            //castling
            if (cx*cx==4 && cy==0){
                if (isCheck()) return false;
                if ((colourToMove()==WHITE &&  begPos!=4) || (colourToMove()==BLACK && begPos!=60)) return false;
                int rookSpot=8*(begPos/8)+(cx==2?7:0);
                if (getSquare(rookSpot)!=(colourToMove()==WHITE?'R':'r')) return false;
                if (!canCastle(colourToMove(), cx==2)) return false;
                //check for clear space between king and rook
                for (int file=4+(cx/2);(file < 7 && cx==2) || (file > 1 && cx==-2); file+=(cx/2) ){
                    int intermediate=(begPos/8)*8+file;
                    if (getSquare(intermediate)!=' ') return false;
                }
                //Lastly, make sure the square the king moves over isn't under check
                int interKing=begPos+cx/2;
                if (!isLegalMove(begPos, interKing)) return false;
            }
            else if (cx*cx + cy*cy >2) return false;
            break;
        case 'P':
            if (!((cy>0 && colourToMove()==WHITE) || (cy<0 && colourToMove()==BLACK))) return false;
            if (cy*cy>4) return false;
            if (cy*cy==4){
                if (cx!=0) return false;
                if (begPos/8!=1 && begPos/8!=6) return false;
                for (int dy=1;dy<=2;dy++){
                    int sqrn=begPos+8*(dy*(colourToMove()==WHITE?1:-1));
                    if (getSquare(sqrn)!=' ') return false;
                }
            }
            else if (cx==0){
                if (getSquare(endPos)!=' ') return false;
            }
            else if (cx*cx==1){
                if (getSquare(endPos)==' '){ //almost always bad, except for enpassant.
                    if (endPos%8!=canEnPassantAt) return false;
                    if (colourToMove()==WHITE && endPos/8!=5) return false;
                    if (colourToMove()==BLACK && endPos/8!=2) return false;
                }
            }
            else return false;
            break;
        default:
            return false;
    }
    bool validRay=false;
    for (int i=0;i<rayMoves.size();i++){
        int dx=rayMoves[i]%3-1;
        int dy=rayMoves[i]/3-1;
        if (dx*cy==dy*cx && cx*dx>=0 && cy*dy>=0){
            int clearWay=true;
            for (int vx=dx, vy=dy;(vx!=cx || vy!=cy) && clearWay;vx+=dx,vy+=dy){
                int sqrn=begPos+8*vy+vx;
                if (getSquare(sqrn)!=' ') clearWay=false;
            }
            if (clearWay){
                validRay=true;
                break;
            }
        }
    }
    if (!validRay && rayMoves.size()!=0) return false;
    return true;
}


//expects begPos and endPos to be valid squares
bool ChessPosition::isLegalMove(int begPos, int endPos){
    
    if (!isPsuedoLegalMove(begPos, endPos)) return false;
    //and finally, we need to check if the move puts our king in check.
    MoveFootprint &fp=makeMove(begPos, endPos);
    bool success=!kingCanBeCaptured();
    undoMove(fp);
    return success;
}

char ChessPosition::getSquare(int sqr) const{
    return square[sqr%8][sqr/8];
}

char ChessPosition::getSquare(string sqrName) const{
    return getSquare(ChessSquareConverts(sqrName));
}

void ChessPosition::setSquare(int sqr,char piece){
    char prevPiece=getSquare(sqr);
    if (prevPiece!=' '){
        vector<int> & prevArray=pieceMem.at(prevPiece);
        for (int i=0;i<prevArray.size();i++){
            if (prevArray[i]==sqr) {
                prevArray.erase(prevArray.begin()+i);
                break;
            }
        }
    }
    if (piece!=' '){
        pieceMem.at(piece).insert(lower_bound(pieceMem.at(piece).begin(), pieceMem.at(piece).end(), sqr), sqr);
        //pieceMem.at(piece).push_back(sqr);
        
    }
    square[sqr%8][sqr/8]=piece;
}
void ChessPosition::setSquare(string sqrName, char piece){
    setSquare(ChessSquareConverts(sqrName), piece);
}

//Assumes the move is legal, that begPos and endPos are real squares, and that promotion is set to {B,N,R,Q} if the move consists of a promotion
//returns a footprint which can be used to undo the move
MoveFootprint & ChessPosition::makeMove(int begPos, int endPos, char promotion){
    if (promotion>='A' && promotion<='Z' && colourToMove()==BLACK) promotion+='a'-'A';
    MoveFootprint & fp=*new MoveFootprint();
    fp.begpos=begPos;
    fp.endpos=endPos;
    for (int i=0;i<4;i++) fp.castleAllowed[i]=castleAllowed[i];
    fp.canEnPassantAt=canEnPassantAt;
    fp.pieceOnEndPos=getSquare(endPos);
    fp.isPromotion=false;
    fp.plySinceLastAction=plySinceLastAction;
    char piece=getSquare(begPos);
    bool isLimitedAction=(fp.pieceOnEndPos!=' ');
    int nextCanEnPassantAt=-1;
    if (piece=='K'){
        castleAllowed[0]=false;
        castleAllowed[1]=false;
        if (begPos==4 && endPos==6){
            setSquare(7, ' ');
            setSquare(5, 'R');
        }
        if (begPos==4 && endPos==2){
            setSquare(0, ' ');
            setSquare(3, 'R');
        }
    }
    if (piece=='k'){
        castleAllowed[2]=false;
        castleAllowed[3]=false;
        if (begPos==60 && endPos==62){
            setSquare(63, ' ');
            setSquare(61, 'r');
        }
        if (begPos==60 && endPos==58){
            setSquare(56, ' ');
            setSquare(59, 'r');
        }
    }
    if (piece=='R'){
        if (begPos/8==0){
            if (begPos%8==0) castleAllowed[0]=false;
            if (begPos%8==7) castleAllowed[1]=false;
        }
    }
    if (piece=='r'){
        if (begPos/8==7){
            if (begPos%8==0) castleAllowed[2]=false;
            if (begPos%8==7) castleAllowed[3]=false;
        }
    }
    if (piece=='P'){
        isLimitedAction=true;
        if (begPos/8==1 && endPos/8==3) nextCanEnPassantAt=begPos%8;
        if (endPos%8==canEnPassantAt && endPos/8==5){
            setSquare(endPos-8, ' ');
        }
        if (endPos/8==7) fp.isPromotion=true;
    }
    if (piece=='p'){
        isLimitedAction=true;
        if (begPos/8==6 && endPos/8==4) nextCanEnPassantAt=begPos%8;
        if (endPos%8==canEnPassantAt && endPos/8==2){
            setSquare(endPos+8, ' ');
        }
        if (endPos/8==0) fp.isPromotion=true;
    }
    setSquare(begPos, ' ');
    setSquare(endPos, fp.isPromotion?promotion:piece);
    if (isLimitedAction) plySinceLastAction=0;
    else plySinceLastAction++;
    if (colourToMove()==BLACK) fullMoves++;
    canEnPassantAt=nextCanEnPassantAt;
    toggleColour();
    return fp;
}//returns a dynamically allocated chessPosition representing the result of making the move


bool ChessPosition::kingCanBeCaptured(){
    int kpos=getSquareOfPiece(colourToMove()==WHITE?'k':'K', 0);
    //test for K,P,N
    for (int vx=-2;vx<=2;vx++){
        for (int vy=-2;vy<=2;vy++){
            int nearpos=kpos+vx+8*vy;
            if (kpos%8+vx<0 || kpos%8+vx>7) continue;
            if (!isValidSquare(nearpos)) continue;
            char piece=getSquare(nearpos);
            if ((piece < 'A' || piece > 'Z') && colourToMove()==WHITE) continue;
            if ((piece < 'a' || piece > 'z') && colourToMove()==BLACK) continue;
            if (piece < 'A' || piece > 'Z') piece += 'A'-'a';
            if (vx*vx+vy*vy==5 && piece =='N') return true;
            if (vx*vx+vy*vy==2 && piece =='P'){
                if (colourToMove()==WHITE && vy==-1) return true;
                if (colourToMove()==BLACK && vy==1) return true;
            }
            if (vx*vx+vy*vy<=2 && piece =='K') return true;
        }
    }
    //test for Q,B,R
    for (int dx=-1;dx<=1;dx++){
        for (int dy=-1;dy<=1;dy++){
            if (dx==0 && dy==0) continue;
            for (int factor=1;(kpos%8)+dx*factor<8 && (kpos%8)+dx*factor>=0;factor++){
                int raypos=kpos+dx*factor+dy*factor*8;
                if (kpos%8+dx*factor<0 || kpos%8+dx*factor>7) break;
                if (!isValidSquare(raypos)) break;
                char piece=getSquare(raypos);
                if (piece!=' '){
                    if ((colourToMove()==WHITE && piece >='A' && piece <='Z') || (colourToMove()==BLACK && piece >='a' && piece <='z')){
                        if (piece < 'A' || piece > 'Z') piece += 'A'-'a';
                        if (piece=='Q') return true;
                        if (dx*dy==0 && piece=='R') return true;
                        if (dx*dy!=0 && piece=='B') return true;
                    }
                    
                    break;
                }
            }
        }
    }
    return false;
}


bool ChessPosition::isCheck(){
    toggleColour();
    bool res=kingCanBeCaptured();
    toggleColour();
    return res;
}

void ChessPosition::undoMove(MoveFootprint & footprint){
    toggleColour();
    plySinceLastAction=footprint.plySinceLastAction;
    for (int i=0;i<4;i++) castleAllowed[i]=footprint.castleAllowed[i];
    setSquare(footprint.begpos, (footprint.isPromotion?(colourToMove()==WHITE?'P':'p'):getSquare(footprint.endpos)));
    setSquare(footprint.endpos, footprint.pieceOnEndPos);
    canEnPassantAt=footprint.canEnPassantAt;
    if (colourToMove()==BLACK) fullMoves--;
    
    if (getSquare(footprint.begpos)=='K'){
        if (footprint.begpos==4 && footprint.endpos==6){
            setSquare(7, 'R');
            setSquare(5, ' ');
        }
        if (footprint.begpos==4 && footprint.endpos==2){
            setSquare(0, 'R');
            setSquare(3, ' ');
        }
    }
    if (getSquare(footprint.begpos)=='k'){
        if (footprint.begpos==60 && footprint.endpos==62){
            setSquare(63, 'r');
            setSquare(61, ' ');
        }
        if (footprint.begpos==60 && footprint.endpos==58){
            setSquare(56, 'r');
            setSquare(59, ' ');
        }
    }
    if (canEnPassantAt!=-1){
        int sqrn=canEnPassantAt+8*(colourToMove()==WHITE?4:3);
        setSquare(sqrn, colourToMove()==WHITE?'p':'P');
    }
}

//TODO: add stalemate/checkmate
PositionFlag ChessPosition::flag(){
    bool check=isCheck();
    bool anyLegals=anyLegalMoves();
    if (check && anyLegals) return CHECK;
    else if (check && !anyLegals) return CHECKMATE;
    else if (!check && !anyLegals) return STALEMATE;
    else if (plySinceLastAction==100) return FIFTYMOVE;
    else return NONE;
}

bool ChessPosition::canCastle(Colour colour, bool kingSide) const{
    return castleAllowed[colour*2+kingSide];
}

bool ChessPosition::anyLegalMoves(){
    for (int i=0;i<6;i++){
        char piece=pieceTypes[colourToMove()==WHITE?i:i+6];
        for (int j=0;j<numPiecesOfType(piece);j++){
            vector<int> legals=getLegalMovesFrom(getSquareOfPiece(piece, j));
            bool success=(legals.size()>0);
            if (success) return true;
        }
    }
    return false;
}


//gets all possible squares endpos so that begpos->endpos is legal.
vector<int> ChessPosition::getLegalMovesFrom(int begPos, bool pseudo){
    vector<int> legalEnds;
    if (isCheck() && !pseudo){
        if (getSquare(begPos)=='K' || getSquare(begPos)=='k'){
            for (int dx=-1;dx<=1;dx++){
                for (int dy=-1;dy<=1;dy++){
                    int destPos=begPos+dy*8+dx;
                    if (isValidSquare(destPos) &&  (destPos/8)-(begPos/8)==dy){
                        if (isLegalMove(begPos, destPos)){
                            legalEnds.push_back(destPos);
                        }
                    }
                }
            }
        }
        else {
            int kingSqr=getSquareOfPiece(colourToMove()==WHITE?'K':'k', 0);
            toggleColour();
            vector<int> attackers=getLegalMovesTo(kingSqr, true);
            toggleColour();
            if (attackers.size()==0){
                for (int i=7;i>=0;i--){
                    for (int j=0;j<8;j++){
                        cout << getSquare(i*8+j);
                    }
                    cout << endl;
                }
                isCheck();
            }
            //only king can move on double check
            if (attackers.size()>1) return legalEnds;
            //try capturing the piece
            if (isLegalMove(begPos, attackers[0])) legalEnds.push_back(attackers[0]);
            char attackPiece=toupper(getSquare(attackers[0]));
            //try blocking the line
            if (attackPiece=='B' || attackPiece=='R' || attackPiece=='Q'){
                int cx=attackers[0]%8-kingSqr%8;
                int cy=attackers[0]/8-kingSqr/8;
                int dx=(cx>0?1:(cx<0?-1:0));
                int dy=(cy>0?1:(cy<0?-1:0));
                int sqrn=kingSqr;
                while (sqrn%8+dx<8 && sqrn%8+dx>=0){
                    sqrn+=dx+8*dy;
                    if (sqrn==attackers[0]) break;
                    if (isLegalMove(begPos, sqrn)) legalEnds.push_back(sqrn);
                }
            }
        }
        return legalEnds;
    }
    vector<int> pseudoLegalEnds;
    char piece=toupper((getSquare(begPos)));
    switch (piece) {
        case 'K':{
            for (int dx=-1;dx<=1;dx++){
                for (int dy=-1;dy<=1;dy++){
                    int endPos=begPos+dx+dy*8;
                    if (isValidSquare(endPos) && begPos%8+dx<8 && begPos%8+dx>=0){
                        if (getSquare(endPos)==' ' || colourOf(getSquare(endPos))!= colourToMove())
                        pseudoLegalEnds.push_back(endPos);
                    }
                }
            }
            int castleDests[4]={2,6,58,62};
            for (int i=0;i<4;i++)
                if (isPsuedoLegalMove(begPos, castleDests[i])) pseudoLegalEnds.push_back(castleDests[i]);
            
            break;
        }
        case 'N':
            for (int xp=0;xp<=1;xp++){
                for (int yp=0;yp<=1;yp++){
                    for (int xt=0;xt<=1;xt++){
                        int dx=1;
                        int dy=1;
                        if (xp==0) dx*=-1;
                        if (yp==0) dy*=-1;
                        if (xt==0) dy*=2;
                        else dx*=2;
                        int endPos=begPos+dx+dy*8;
                        if (isValidSquare(endPos)  && begPos%8+dx<8 && begPos%8+dx>=0){
                            if (getSquare(endPos)==' ' || colourOf(getSquare(endPos))!= colourToMove())
                            pseudoLegalEnds.push_back(endPos);
                        }
                    }
                }
            }
            break;
        case 'P':
        {
            int endPos=begPos;
            endPos+=8*2*(colourToMove()==WHITE?1:-1);
            if (isPsuedoLegalMove(begPos, endPos)) pseudoLegalEnds.push_back(endPos);
            endPos-=8*(colourToMove()==WHITE?1:-1);
            if (isPsuedoLegalMove(begPos, endPos)) pseudoLegalEnds.push_back(endPos);
            if (endPos%8>0){
                endPos--;
                if (isPsuedoLegalMove(begPos, endPos)) pseudoLegalEnds.push_back(endPos);
                endPos++;
            }
            if (endPos%8<7){
                endPos++;
                if (isPsuedoLegalMove(begPos, endPos)) pseudoLegalEnds.push_back(endPos);
            }
            break;
        }
        case 'Q':
        case 'R':
        case 'B':
            for (int i=0;i<9;i++){
                if (i==4) continue;
                if (i%2==0 && piece=='R') continue;
                if (i%2==1 && piece=='B') continue;
                int dx=i%3-1;
                int dy=i/3-1;
                
                int sqrn=begPos;
                while (sqrn%8+dx<8 && sqrn%8+dx>=0){
                    sqrn+=dx+dy*8;
                    if (!isValidSquare(sqrn)) break;
                    if (getSquare(sqrn)!=' '){
                        if (colourToMove()!=colourOf(getSquare(sqrn))){
                            pseudoLegalEnds.push_back(sqrn);
                        }
                        break;
                    }
                    else{
                        pseudoLegalEnds.push_back(sqrn);
                    }
                }
            }
            break;
        default:
            break;
    }
    for (int i=0;i<pseudoLegalEnds.size();i++){
        if (pseudo) legalEnds.push_back(pseudoLegalEnds[i]);
        else {
            MoveFootprint &fp=makeMove(begPos, pseudoLegalEnds[i]);
            if (!kingCanBeCaptured()){
                legalEnds.push_back(pseudoLegalEnds[i]);
            }
            undoMove(fp);
            delete &fp;
        }
    }
    return legalEnds;
}

bool ChessPosition::isLegalPosition(){ // should have a few sanity checks such as 1 of each king, etc..
    if (numPiecesOfType('k')!=1) return false;
    if (numPiecesOfType('K')!=1) return false;
    if (numPiecesOfType('P')>8) return false;
    if (numPiecesOfType('p')>8) return false;
    for (string sqrn="a1";sqrn[0]<='h';sqrn[0]++){
        if (getSquare(sqrn)=='p' || getSquare(sqrn)=='P') return false;
        if (sqrn=="h1"){
            sqrn="a8";
            sqrn[0]--;
        }
    }
    if (kingCanBeCaptured()) return false;
    
    
    return true;
}

int ChessPosition::numPiecesOfType(char piece){
    return pieceMem.at(piece).size();
}

int ChessPosition::getSquareOfPiece(char piece, int instanceNum){
    int isqr= pieceMem[piece].at(instanceNum);
    return isqr;
}

vector<int> ChessPosition::getAllLegalMoves(){
    vector<int> allLegalMoves;
    for (int i=0;i<12;i++){
        if (plySinceLastAction==8 && i==5){
            
        }
        if (colourOf(pieceTypes[i])==colourToMove()){
            for (int j=0;j<numPiecesOfType(pieceTypes[i]);j++){
                
                int begPos=getSquareOfPiece(pieceTypes[i], j);
                vector<int> legalFromHere=getLegalMovesFrom(begPos);
                for (int k=0;k<legalFromHere.size();k++){
                    allLegalMoves.push_back(begPos*64+legalFromHere[k]);
                }
            }
        }
    }
    
     return allLegalMoves;
}

vector<int> ChessPosition::getLegalMovesTo(int endPos, bool pseudo){
    vector<int> LegalBegs;
    vector<int> pseudoLegalBegs;
    for (int vx=-2;vx<=2;vx++){
        for (int vy=-2;vy<=2;vy++){
            int nearpos=endPos+vx+8*vy;
            if (endPos%8+vx>7 || endPos%8+vx<0) continue;
            if (!isValidSquare(nearpos)) continue;
            char piece=getSquare(nearpos);
            if ((piece < 'A' || piece > 'Z') && colourToMove()==WHITE) continue;
            if ((piece < 'a' || piece > 'z') && colourToMove()==BLACK) continue;
            if (piece < 'A' || piece > 'Z') piece += 'A'-'a';
            if (vx*vx+vy*vy==5 && piece =='N') pseudoLegalBegs.push_back(nearpos);
            if (vx*vx+vy*vy==2 && piece =='P'){
                if (colourToMove()==WHITE && vy==-1) pseudoLegalBegs.push_back(nearpos);
                if (colourToMove()==BLACK && vy==1) pseudoLegalBegs.push_back(nearpos);
            }
            if (vx*vx+vy*vy<=2 && piece =='K') pseudoLegalBegs.push_back(nearpos);
        }
    }
    //test for Q,B,R
    for (int dx=-1;dx<=1;dx++){
        for (int dy=-1;dy<=1;dy++){
            if (dx==0 && dy==0) continue;
            for (int factor=1;;factor++){
                int raypos=endPos+(dx+dy*8)*factor;
                if (endPos%8+dx*factor>7 || endPos%8+dx*factor<0) break;
                if (!isValidSquare(raypos)) break;
                char piece=getSquare(raypos);
                if (piece!=' '){
                    if ((colourToMove()==WHITE && piece >='A' && piece <='Z') || (colourToMove()==BLACK && piece >='a' && piece <='z')){
                        if (piece < 'A' || piece > 'Z') piece += 'A'-'a';
                        if (piece=='Q') pseudoLegalBegs.push_back(raypos);
                        if (dx*dy==0 && piece=='R') pseudoLegalBegs.push_back(raypos);
                        if (dx*dy!=0 && piece=='B') pseudoLegalBegs.push_back(raypos);
                    }
                    
                    break;
                }
            }
        }
    }
    
    for (int i=0;i<pseudoLegalBegs.size();i++){
        if (pseudo) LegalBegs.push_back(pseudoLegalBegs[i]);
        else {
            MoveFootprint &fp=makeMove(pseudoLegalBegs[i], endPos);
            if (!kingCanBeCaptured()){
                LegalBegs.push_back(pseudoLegalBegs[i]);
            }
            undoMove(fp);
            delete &fp;
        }
    }
    
    
    return LegalBegs;

}














