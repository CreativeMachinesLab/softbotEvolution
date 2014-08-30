#include "Experiments/HCUBE_CheckersCommon.h"

#include "Experiments/HCUBE_cliche.h"
#include "Experiments/HCUBE_simplech.h"

#define CHECKERS_COMMON_DEBUG (0)

namespace HCUBE
{
    CheckersMove::CheckersMove()
            :
            from(255,255),
            to(255,255),
            pieceCaptured(255),
            nextJump(NULL),
            promoted(false)
    {}

    CheckersMove::CheckersMove(Vector2<uchar> _from,Vector2<uchar> _to)
            :
            from(_from),
            to(_to),
            pieceCaptured(255),
            nextJump(NULL),
            promoted(false)
    {}

    CheckersMove::CheckersMove(const CheckersMove &other)
            :
            from(other.from),
            to(other.to),
            pieceCaptured(other.pieceCaptured),
            promoted(other.promoted)
    {
        if (other.nextJump)
        {
#if CHECKERS_USE_BOOST_POOL
            nextJump = (CheckersMove*)checkersMovePool::malloc();
            new(nextJump) CheckersMove(*other.nextJump);
#else
            nextJump = new CheckersMove(*other.nextJump);
#endif
        }
        else
        {
            nextJump = NULL;
        }
    }

    const CheckersMove &CheckersMove::operator=(const CheckersMove &other)
    {
        if (this != &other)
        {
            from = other.from;
            to = other.to;
            pieceCaptured = other.pieceCaptured;
            promoted = other.promoted;

            if (nextJump)
            {
#if CHECKERS_USE_BOOST_POOL
                nextJump->~CheckersMove();
                checkersMovePool::free( nextJump );
#else
                delete nextJump;
#endif
            }

            if (other.nextJump)
            {
#if CHECKERS_USE_BOOST_POOL
                nextJump = (CheckersMove*)checkersMovePool::malloc();
                new(nextJump) CheckersMove(*other.nextJump);
#else
                nextJump = new CheckersMove(*other.nextJump);
#endif
            }
            else
            {
                nextJump = NULL;
            }

        }

        return *this;
    }

    CheckersMove::~CheckersMove()
    {
        if (nextJump)
        {
#if CHECKERS_USE_BOOST_POOL
            nextJump->~CheckersMove();
            checkersMovePool::free( nextJump );
#else
            delete nextJump;
            nextJump = (CheckersMove*)12345678;
#endif
        }
    }

    Vector2<uchar> CheckersMove::getFinalDestination()
    {
        if (nextJump)
        {
            return nextJump->getFinalDestination();
        }
        else
        {
            return to;
        }
    }

    void CheckersMove::addJump(Vector2<uchar> from,Vector2<uchar> to)
    {
        if (nextJump)
        {
            nextJump->addJump(from,to);
        }
        else
        {
#if CHECKERS_USE_BOOST_POOL
            nextJump = (CheckersMove*)checkersMovePool::malloc();
            new(nextJump) CheckersMove(from,to);
#else
            nextJump = new CheckersMove(from,to);
#endif
        }
    }

    void CheckersMove::addJump(const CheckersMove &move)
    {
        if (nextJump)
        {
            nextJump->addJump(move);
        }
        else
        {
#if CHECKERS_USE_BOOST_POOL
            nextJump = (CheckersMove*)checkersMovePool::malloc();
            new(nextJump) CheckersMove(move);
#else
            nextJump = new CheckersMove(move);
#endif
        }
    }

    void CheckersCommon::printBoard(uchar b[8][8])
    {
        /*
        const char *f = "-    wb  WB      ";

        // rotate board, so the player sees it from her point of view
        int r = 1;

        printf ("\n");
        {
            for (int y = 0; y < 8; y++)
            {
                // board setup
                for (int x = 0; x < 8; x++)
                {
                    if (r)
                        printf (" %d", b[x][8 - y - 1]);
                    else
                        printf (" %d", b[8 - x - 1][y]);
                }

                printf("\t");
                for (int x = 0; x < 8; x++)
                {
                    int tmpx = x;
                    int tmpy = y;

                    if (r)
                        tmpy = 8 - y - 1;
                    else
                        tmpx = 8 - x - 1;

                    if ( (tmpx+tmpy)%2 == 1)
                    {
                        printf("  ");
                    }
                    else
                    {
                        printf (" %c", f[b[tmpx][tmpy]]);
                    }
                }

                printf ("\n");
            }
        }
        */

        const char *f = "-    wb  WB      ";
        bool error=false;
        for (int y=0;y<8;y++)
        {
            for (int x=0;x<8;x++)
            {
                if (x==0 && y==1)
                {
                    cout << "  ";
                    continue;
                }
                if (x==0 && y==3)
                {
                    cout << "  ";
                    continue;
                }

                cout << " " << f[b[x][y]];
                if (((y+x) % 2 == 1) && f[b[x][y]]!=' ')
                {
                    error=true;
                }
            }
            cout << endl;
        }
        cout << endl;
        if (error)
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("ERROR! INVALID BOARD ^^^^!");
        }
    }

    void CheckersCommon::checkBoard(uchar b[8][8])
    {
        const char *f = "-    wb  WB      ";
        bool error=false;
        for (int y=0;y<8;y++)
        {
            for (int x=0;x<8;x++)
            {
                if (x==0 && y==1)
                    continue;
                if (x==0 && y==3)
                    continue;

                if (((y+x) % 2 == 1) && f[b[x][y]]!=' ')
                {
                    error=true;
                }
            }
        }
        if (error)
        {
            printBoard(b);
        }
    }

    void CheckersCommon::makeMove(CheckersMove &move,uchar b[8][8])
    {
#if CHECKERS_COMMON_DEBUG
        cout << "Running makeMove from " << move.from.x << ',' << move.from.y << " to " << move.to.x << ',' << move.to.y << endl;
        printBoard(b);
        CREATE_PAUSE("");
#endif
        checkBoard(b);

        bool isJump = (abs(move.to.x - move.from.x)>1);

        if (
            b[move.from.x][move.from.y] == EMPTY ||
            b[move.to.x][move.to.y] != EMPTY ||
            move.to.x >= 8 ||
            move.to.y >= 8 ||
            (move.to.x+move.to.y) %2 == 1
        )
        {
            cout << (int)move.from.x << ',' << (int)move.from.y << '\t' << (int)move.to.x << ',' << (int)move.to.y << endl;

            printBoard(b);
            throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: tried to move to a square which wasn't empty!");
        }

        b[move.to.x][move.to.y] = b[move.from.x][move.from.y];
        b[move.from.x][move.from.y] = EMPTY;

        Vector2<uchar> mdpt;

        if (isJump)
        {
            mdpt = (move.to + move.from)/2;

            move.pieceCaptured = b[mdpt.x][mdpt.y];
            b[mdpt.x][mdpt.y] = EMPTY;

            if ((move.pieceCaptured&BLACK)>0)
            {
                NUM_BLACK_PIECES(b) = NUM_BLACK_PIECES(b) - 1;
            }
            else //white was captured
            {
                NUM_WHITE_PIECES(b) = NUM_WHITE_PIECES(b) - 1;
            }
        }
        else
        {
            move.pieceCaptured = 0;
        }

        if ( b[move.to.x][move.to.y] & BLACK )
        {
            if (move.to.y == 7)
            {
                if ( b[move.to.x][move.to.y] & MAN )
                {
                    move.promoted = true;
                    b[move.to.x][move.to.y] &= (~MAN);
                    b[move.to.x][move.to.y] |= KING;
                }
                else
                {
                    move.promoted = false;
                }
            }
        }
        else if ( b[move.to.x][move.to.y] & WHITE )
        {
            if (move.to.y == 0)
            {
                if ( b[move.to.x][move.to.y] & MAN )
                {
                    move.promoted = true;
                    b[move.to.x][move.to.y] &= (~MAN);
                    b[move.to.x][move.to.y] |= KING;
                }
                else
                {
                    move.promoted = false;
                }
            }
        }

        if (move.nextJump)
        {
            makeMove(*(move.nextJump),b);
        }

        checkBoard(b);
#if CHECKERS_COMMON_DEBUG
        cout << "done!\n";
#endif
    }

    void CheckersCommon::reverseMove(CheckersMove &move,uchar b[8][8])
    {
#if CHECKERS_COMMON_DEBUG
        cout << "Running reverseMove from " << move.from.x << ',' << move.from.y << " to " << move.to.x << ',' << move.to.y << endl;
#endif
        checkBoard(b);

        if (move.nextJump)
        {
            reverseMove(*(move.nextJump),b);
        }

        bool isJump = (abs(move.to.x - move.from.x)>1);

        b[move.from.x][move.from.y] = b[move.to.x][move.to.y];
        b[move.to.x][move.to.y] = EMPTY;

        if (isJump)
        {
            Vector2<uchar> mdpt = (move.to + move.from)/2;

            b[mdpt.x][mdpt.y] = move.pieceCaptured;

            if ((move.pieceCaptured&BLACK)>0)
            {
                NUM_BLACK_PIECES(b) = NUM_BLACK_PIECES(b) + 1;
            }
            else //white was captured
            {
                NUM_WHITE_PIECES(b) = NUM_WHITE_PIECES(b) + 1;
            }
        }

        if (move.promoted)
        {
            b[move.from.x][move.from.y] &= (~KING);
            b[move.from.x][move.from.y] |= MAN;
        }

        checkBoard(b);
#if CHECKERS_COMMON_DEBUG
        cout << "done!\n";
#endif
    }

#define IS_IN_BOUNDS(X,Y) ((X)>=0&&(Y)>=0&&(X)<8&&(Y)<8)

//Check if we haven't found a jump yet, erase the moves and add the jump
#define CHECK_FIRST_JUMP if (!foundJump) { totalMoveList.erase(totalMoveList.begin()+moveBeginIndex,totalMoveList.end()); numMoves=0; foundJump=true; }

#define MAX_MOVES (128)

    bool CheckersCommon::hasJump(
        uchar b[8][8],
        int color,
        int x,
        int y,
        int deltax,
        int deltay
    )
    {
#if CHECKERS_COMMON_DEBUG
        cout << "Running hasJump\n";
#endif
        if (IS_IN_BOUNDS(x+deltax,y+deltay))
        {
            if (b[x+deltax][y+deltay]&(MAN|KING))
            {
                //A piece exists in the square we could move to, can we jump it?

                if (
                    IS_IN_BOUNDS(x+deltax*2,y+deltay*2) && //must be in bounds
                    (!(b[x+deltax][y+deltay]&color)) && //can't jump your own color
                    (!(b[x+deltax*2][y+deltay*2]&(MAN|KING))) //must have an empty square behind
                )
                {
                    return true;
                }
            }
        }
#if CHECKERS_COMMON_DEBUG
        cout << "Done Running hasJump\n";
#endif

        return false;
    }

    /* tryMoveJumps:
        returns true if more jumps exist and false otherwise.
        If returns true, that means that you don't need to worry about adding the current jump
        */
    bool CheckersCommon::tryMoreJumps(
        vector<CheckersMove> &totalMoveList,
        int &numMoves,
        CheckersMove &moveInProgress,
        int color,
        uchar b[8][8]
    )
    {
        int x = moveInProgress.getFinalDestination().x;
        int y = moveInProgress.getFinalDestination().y;
#if CHECKERS_COMMON_DEBUG
        cout << "Running tryMoreJumps on location " << x << ',' << y << endl;
#endif

        bool hasMoreJumps=false;

        if ( (b[x][y]&color) )
        {
            if ( (b[x][y]&(BLACK|KING)) )
            {
                //Black moves in the +y direction

                for (int deltax=-1;deltax<=1;deltax+=2)
                {
                    if (
                        hasJump(b,color,x,y,deltax,1)
                        &&
                        (
                            moveInProgress.from.x != x+deltax*2 ||
                            moveInProgress.from.y != y+2
                            //This conditional logic is there to prevent making jumps which
                            //contain the starting position.  This is because it tends to bomb on
                            //the current checkers implementation
                        )
                    )
                    {
                        hasMoreJumps=true;

                        //A new jump exists.  Try the jump
                        CheckersMove jump = CheckersMove(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax*2,y+2));

                        CheckersMove fullMove = moveInProgress;
                        fullMove.addJump(jump);

                        makeMove(jump,b);
                        bool moreJumps=false;
                        if (!jump.promoted)
                        {
                            //"A piece that has just kinged, cannot continue jumping pieces, until the next move."
                            moreJumps = tryMoreJumps(totalMoveList,numMoves,fullMove,color,b);
                        }
                        reverseMove(jump,b);

                        if (!moreJumps)
                        {
                            //Add the current move if there's no more jumps
                            //(otherwise you shuold have added the
                            //jumps and this move is not legal)
                            //CheckersMove tmpMove = moveInProgress;
                            //tmpMove.addJump(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax*2,y+2));
                            totalMoveList.push_back(fullMove);
                            numMoves++;
                        }
                    }
                }
            }

            if ( (b[x][y]&(WHITE|KING)) )
            {
                //White moves in the -y direction

                for (int deltax=-1;deltax<=1;deltax+=2)
                {
                    if (
                        hasJump(b,color,x,y,deltax,-1)
                        &&
                        (
                            moveInProgress.from.x != x+deltax*2 ||
                            moveInProgress.from.y != y-2
                            //This conditional logic is there to prevent making jumps which
                            //contain the starting position.  This is because it tends to bomb on
                            //the current checkers implementation
                        )
                    )
                    {
                        hasMoreJumps=true;

                        //A new jump exists.  Try the jump
                        CheckersMove jump = CheckersMove(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax*2,y-2));

                        CheckersMove fullMove = moveInProgress;
                        fullMove.addJump(jump);

                        makeMove(jump,b);
                        bool moreJumps=false;
                        if (!jump.promoted)
                        {
                            //"A piece that has just kinged, cannot continue jumping pieces, until the next move."
                            moreJumps = tryMoreJumps(totalMoveList,numMoves,fullMove,color,b);
                        }
                        reverseMove(jump,b);

                        if (!moreJumps)
                        {
                            //Add the current move if there's no more jumps
                            //(otherwise you shuold have added the
                            //jumps and this move is not legal)
                            //CheckersMove tmpMove = moveInProgress;
                            //tmpMove.addJump(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax*2,y-2));

                            totalMoveList.push_back(fullMove);
                            numMoves++;
                        }
                    }
                }
            }
        }
        else
        {
            CREATE_PAUSE("ERROR!");
            throw CREATE_LOCATEDEXCEPTION_INFO("Tried to add more jumps but there wasn't a piece!");
        }

#if CHECKERS_COMMON_DEBUG
        cout << "Done Running tryMoreJumps\n";
#endif
        return hasMoreJumps;
    }

    void CheckersCommon::tryMove(
        vector<CheckersMove> &totalMoveList,
        int moveBeginIndex,
        uchar b[8][8],
        int &numMoves,
        bool &foundJump,
        int color,
        int x,
        int y,
        int deltax,
        int deltay
    )
    {
#if CHECKERS_COMMON_DEBUG
        cout << "Running tryMove\n";
#endif
        if (IS_IN_BOUNDS(x+deltax,y+deltay))
        {
            if (b[x+deltax][y+deltay]&(MAN|KING))
            {
                //A piece exists in the square we could move to, can we jump it?

                if (
                    IS_IN_BOUNDS(x+deltax*2,y+deltay*2) && //must be in bounds
                    (!(b[x+deltax][y+deltay]&color)) && //can't jump your own color
                    (!(b[x+deltax*2][y+deltay*2]&(MAN|KING))) //must have an empty square behind
                )
                {
                    CHECK_FIRST_JUMP
                    CheckersMove jump = CheckersMove(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax*2,y+deltay*2));

                    bool foundMoreJumps=false;
                    //At this point, we have found a jump, try to find more jumps
                    makeMove(jump,b);
                    if (!jump.promoted)
                    {
                        //"A piece that has just kinged, cannot continue jumping pieces, until the next move."
                        foundMoreJumps = tryMoreJumps(totalMoveList,numMoves,jump,color,b);
                    }
                    reverseMove(jump,b);

                    if (!foundMoreJumps)
                    {
                        //No multi-jumps found, just add the one jump
                        totalMoveList.push_back(jump);
                        numMoves++;
                    }
                }
            }
            else
            {
                //No piece exists, a move is possible
                if (!foundJump)
                {
                    //Only try to move if you haven't yet found a possible capture
                    totalMoveList.push_back(CheckersMove(Vector2<uchar>(x,y),Vector2<uchar>(x+deltax,y+deltay)) );
                    numMoves++;
                }
            }
        }

        if (numMoves==MAX_MOVES)
        {
            throw CREATE_LOCATEDEXCEPTION_INFO("Too many possible moves for a given board state! Oh shiz!");
        }
#if CHECKERS_COMMON_DEBUG
        cout << "Done Running tryMove\n";
#endif
    }

    int CheckersCommon::generateMoveList(
        vector<CheckersMove> &totalMoveList,
        int moveBeginIndex,
        uchar b[8][8],
        int color,
        bool &foundJump
        )
    {
#if CHECKERS_COMMON_DEBUG
        cout << "Running generateMoveList\n";
#endif
        int numMoves=0;

        foundJump=false;

        for (int x=0;x<8;x++)
        {
            for (int y=(x%2);y<8;y+=2)
            {
                if ( (b[x][y]&color) )
                {
                    if ( (b[x][y]&(BLACK|KING)) )
                    {
                        //Black moves in the +y direction
                        tryMove(totalMoveList,moveBeginIndex,b,numMoves,foundJump,color,x,y,1,1);
                        tryMove(totalMoveList,moveBeginIndex,b,numMoves,foundJump,color,x,y,-1,1);
                    }

                    if ( (b[x][y]&(WHITE|KING)) )
                    {
                        //White moves in the -y direction
                        tryMove(totalMoveList,moveBeginIndex,b,numMoves,foundJump,color,x,y,1,-1);
                        tryMove(totalMoveList,moveBeginIndex,b,numMoves,foundJump,color,x,y,-1,-1);
                    }
                }
            }
        }

#if CHECKERS_COMMON_DEBUG
        cout << "Done Running generateMoveList\n";
#endif
        return numMoves;
    }

    bool CheckersCommon::hasMove(
        uchar b[8][8],
        int color,
        int x,
        int y,
        int deltax,
        int deltay
    )
    {
        if (IS_IN_BOUNDS(x+deltax,y+deltay))
        {
            if (b[x+deltax][y+deltay]&(MAN|KING))
            {
                //A piece exists in the square we could move to, can we jump it?

                if (
                    IS_IN_BOUNDS(x+deltax*2,y+deltay*2) && //must be in bounds
                    (!(b[x+deltax][y+deltay]&color)) && //can't jump your own color
                    (!(b[x+deltax*2][y+deltay*2]&(MAN|KING))) //must have an empty square behind
                )
                {
                    return true;
                }
            }
            else
            {
                //No piece exists, a move is possible
                return true;
            }
        }

        return false;
    }

    bool CheckersCommon::hasAnyMove(uchar b[8][8],int color)
    {
        for (int x=0;x<8;x++)
        {
            for (int y=(x%2);y<8;y+=2)
            {
                if ( (b[x][y]&color) )
                {
                    if ( (b[x][y]&(BLACK|KING)) )
                    {
                        //Black regular pieces move in the +y direction
                        if (hasMove(b,color,x,y,1,1)) return true;
                        if (hasMove(b,color,x,y,-1,1)) return true;
                    }

                    if ( (b[x][y]&(WHITE|KING)) )
                    {
                        //White regular pieces move in the -y direction
                        if (hasMove(b,color,x,y,1,-1)) return true;
                        if (hasMove(b,color,x,y,-1,-1)) return true;
                    }
                }
            }
        }
        return false;
    }

    int CheckersCommon::getWinner(uchar b[8][8])
    {
        if (!(NUM_WHITE_PIECES(b)))
        {
            return BLACK;
        }
        else if (!(NUM_BLACK_PIECES(b)))
        {
            return WHITE;
        }

        return -1;
    }

    int CheckersCommon::getWinner(uchar b[8][8],int colorToMove)
    {
        int pieceWin = getWinner(b);

        if (pieceWin!=-1)
            return pieceWin;

        if (colorToMove == BLACK)
        {
            if (!hasAnyMove(b,BLACK))
            {
                return WHITE;
            }
        }
        else
        {
            if (!hasAnyMove(b,WHITE))
            {
                return BLACK;
            }
        }

        return -1;
    }

    void CheckersCommon::countPieces(uchar b[8][8],int &whiteMen,int &blackMen,int &whiteKings,int &blackKings)
    {
        whiteMen = blackMen = whiteKings = blackKings = 0;

        for (int x=0;x<8;x++)
        {
            for (int y=(x%2);y<8;y+=2)
            {
                if ( b[x][y]&BLACK )
                {
                    if (b[x][y]&KING)
                    {
                        blackKings++;
                    }
                    else
                    {
                        blackMen++;
                    }
                }

                if ( b[x][y]&WHITE )
                {
                    if (b[x][y]&KING)
                    {
                        whiteKings++;
                    }
                    else
                    {
                        whiteMen++;
                    }
                }
            }
        }
    }

    void CheckersCommon::resetBoard(uchar b[8][8])
    {
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                if ((x + y) % 2 == 0)
                    b[x][y] = EMPTY;
                else
                    b[x][y] = FREE;

        for (int x=0;x<8;x+=2)
        {
            for (int y=0;y<3;y++)
            {
                b[x + (y%2)][y] = BLACK|MAN;
            }
        }

        for (int x=0;x<8;x+=2)
        {
            for (int y=7;y>4;y--)
            {
                b[x + (y%2)][y] = WHITE|MAN;
            }
        }

        NUM_BLACK_PIECES(b) = 12;
        NUM_WHITE_PIECES(b) = 12;
    }
}
