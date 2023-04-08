#include <cstdio>
#include <cstring>
//#include "conio2.h"
#include "interface.h"
//#include "checkerslib.h"
#include "clcheckers.h"
#include "appconsts.h"
#include <windows.h>

#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)

/*
 * THIS FILE IS FOR TESTING OF THE GAME ENGINE UNTIL I RELEASE THE MAIN INTERFACE
 *
 */


void renderBoard(Board* board, Color playerSide, short drawX, short drawY, bool pasteCoordinates) {
    drawFrame(10, 10, drawX, drawY);
    if (pasteCoordinates) {
        gotoxy(drawX + 1, drawY - 1); printf("abcdefgh");
        gotoxy(drawX + 1, drawY + 10); printf("abcdefgh");
        for (int i = 0; i < 8; i++) {
            gotoxy(drawX - 1, drawY + 1 + i);
            printf("%d", i + 1);
            gotoxy(drawX + 10, drawY + 1 + i);
            printf("%d", i + 1);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i];
            if (playerSide == Black) {
                gotoxy(drawX + 8 - i, drawY + 1 + j);
            }
            else if (playerSide == White) {
                gotoxy(drawX + 1 + i, drawY + 8 - j);
            }
            switch(bStat) {
                case EMPTY_BLACK: {
                    textbackground(0);
                    printf(" ");
                    break;
                }
                case EMPTY_WHITE: {
                    textbackground(15);
                    printf(" ");
                    break;
                }
                case REG_BLACK: {
                    textcolor(12);
                    printf("O");
                    break;
                }
                case REG_WHITE: {
                    textcolor(15);
                    printf("O");
                    break;
                }
                case KING_BLACK: {
                    textcolor(12);
                    printf("W");
                    break;
                }
                case KING_WHITE: {
                    textcolor(15);
                    printf("W");
                    break;
                }
                /*case AVALIABLE_MOVEMENT_CELL: {
                    textbackground(2);
                    printf(" ");
                    break;
                }*/
            }
            textbackground(0);
        }
    }
    textbackground(0);
    textcolor(15);
}

int main() {

    clrscr();
    hideCursor();
    enableCP1251;
    //Board test_board = initiateGameBoard(false);


    setWindowSize(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH);
    textcolor(15);
    //printf("AAA");
    //waitForKey(13);
    drawFrame(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH, 1, 1);
    int choice;
    Game test = createANewGame(White, White, RvsC);
    //test.situation = makeNullGameSituation(White);
    test.situation.board = createANewBoard();
    TextLineWidget tlw;
    tlw.length = 29;
    tlw.locX = 4; tlw.locY = 2;
    setInitialText(&tlw, "pidarok");
    //drawTextLineWidget(tlw);
    drawMenu(1, mainMenuPoints, 5, 2, 2, 12);
    do {
        choice = drawMenu(APPLICATION_TITLE, 1, mainMenuPoints, 5, mainMenuPointsDescriptions, 5, 16, 4, 15, 3, 12);
        switch (choice) {
            case 1: {

                //ascendChecker(&test.situation.board.checkers[White][10]);
                //ascendChecker(&test.situation.board.checkers[Black][9]);
                for (int i = 0; i < 6; i++) {
                    removeChecker(&test.situation.board, 0, Black);

                }
                for (int i = 0; i < 11; i++) removeChecker(&test.situation.board, 0, White);
                //removeChecker(&test.situation.board, 1, White);
                //ascendChecker(&test.situation.board.checkers[White][0]);
                updateBoardRender(&test.situation.board);
                Color forWho = Black;
                while (true) {
                    drawFrame(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH, 1, 1);
                    renderBoard(&test.situation.board, White, 4, 4, true);
                    //findAllRegularMoves(&test.situation, White);
                    //findAllRegularKingMoves(&test.situation, White);
                    findAllMoves(&test.situation, forWho);

                    for (int i = 0; i < test.situation.rmsCount; i++) {
                        gotoxy(16, 2 + i);
                        RegMoveSequence extracted = test.situation.regMoveSequences[i];
                        if (extracted.rmsCount == 2) {
                            printf("%d) [%d : %d] -> [%d : %d] + [%d : %d] -> [%d : %d]",
                                   i,
                                   test.situation.regMoveSequences[i].regularMoves[0].source.x,
                                   test.situation.regMoveSequences[i].regularMoves[0].source.y,
                                   test.situation.regMoveSequences[i].regularMoves[0].destination.x,
                                   test.situation.regMoveSequences[i].regularMoves[0].destination.y,
                                   test.situation.regMoveSequences[i].regularMoves[1].source.x,
                                   test.situation.regMoveSequences[i].regularMoves[1].source.y,
                                   test.situation.regMoveSequences[i].regularMoves[1].destination.x,
                                   test.situation.regMoveSequences[i].regularMoves[1].destination.y
                                   );

                        }
                        else if (extracted.rmsCount == 1) {
                            printf("%d) [%d : %d] -> [%d : %d]",
                                   i,
                                   test.situation.regMoveSequences[i].regularMoves[0].source.x,
                                   test.situation.regMoveSequences[i].regularMoves[0].source.y,
                                   test.situation.regMoveSequences[i].regularMoves[0].destination.x,
                                   test.situation.regMoveSequences[i].regularMoves[0].destination.y
                            );
                        }
                    }

                    /*for (int i = 0; i < test.situation.rmCount; i++) {
                        gotoxy(16, 2 + i);
                        printf("%d) [%d : %d] -> [%d : %d]",
                               i,
                               test.situation.regularMoves[i].source.x,
                               test.situation.regularMoves[i].source.y,
                               test.situation.regularMoves[i].destination.x,
                               test.situation.regularMoves[i].destination.y);
                    }*/
                    for (int i = 0; i < test.situation.tmsCount; i++) {

                        gotoxy(16, 2 + i);
                        printf("%d)", i);
                        for (int j = 0; j < test.situation.takingSequences[i].tmsCount; j++) {
                            printf("[%d:%d][%d:%d][%d:%d]|",
                                   test.situation.takingSequences[i].takingMoves[j].source.x,
                                   test.situation.takingSequences[i].takingMoves[j].source.y,
                                   test.situation.takingSequences[i].takingMoves[j].victim.x,
                                   test.situation.takingSequences[i].takingMoves[j].victim.y,
                                   test.situation.takingSequences[i].takingMoves[j].destination.x,
                                   test.situation.takingSequences[i].takingMoves[j].destination.y);
                        }

                    }
                    gotoxy(15, 16);
                    //waitForKey(13);
                    int move;
                    scanf("%d", &move);
                    int stat;
                    clearLastTakingSequence(&test.situation);
                    if (test.situation.tmsCount > 0) {

                        //do {
                            //TakingMove t = test.situation.takingMoves[move];
                            //addToLastTakingSequence(&test.situation, t);
                            //stat = makeATakingMove(&test.situation, t);
                            makeATakingMoveSequence(&test.situation, test.situation.takingSequences[move]);
                            /*renderBoard(&test.situation.board, White, 4, 4, true);
                            if (stat == -1) {

                                for (int i = 0; i < test.situation.tmCount; i++) {
                                    gotoxy(39, 2 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.takingMoves[i].source.x,
                                           test.situation.takingMoves[i].source.y,
                                           test.situation.takingMoves[i].victim.x,
                                           test.situation.takingMoves[i].victim.y,
                                           test.situation.takingMoves[i].destination.x,
                                           test.situation.takingMoves[i].destination.y);
                                }
                                for (int i = 0; i < test.situation.lastTakingSequence.tmsCount; i++) {
                                    gotoxy(39, 10 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.lastTakingSequence.takingMoves[i].source.x,
                                           test.situation.lastTakingSequence.takingMoves[i].source.y,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.x,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.y,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.x,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.y);
                                }
                                gotoxy(15, 13);
                                printf("%d", test.situation.tmCount);
                            }

                            if (stat == -1) scanf("%d", &move);
                            clrscr();*/



                            //cancelATakingMove(&test.situation, t);

                        //} while (stat == -1);
                        removeMarkedForDeath(&test.situation, negateColor(forWho));
                        //renderBoard(&test.situation.board, White, 4, 4, true);
                        //waitForKey(13);
                        //if (test.situation.lastTakingSequence.tmsCount > 1) cancelLastTakingSequence(&test.situation);
                    }
                    else {
                        makeARegMoveSequence(&test.situation, test.situation.regMoveSequences[move]);
                        //do {
                            /*
                            if (test.situation.tmCount == 0) {
                                Move t = test.situation.regularMoves[move];
                                stat = makeAMove(&test.situation, t);
                            }
                            else {
                                TakingMove t = test.situation.takingMoves[move];
                                stat = makeATakingMove(&test.situation, t);
                                addToLastTakingSequence(&test.situation, t);
                                //stat = 1;
                            }
                            renderBoard(&test.situation.board, White, 4, 4, true);
                            if (stat == 1 || stat == -1) {
                                //findAllKingTakingMovesForOne(&test.situation, forWho)
                                //renderBoard(&test.situation.board, White, 4, 4, true);

                                for (int i = 0; i < test.situation.tmCount; i++) {
                                    gotoxy(39, 2 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.takingMoves[i].source.x,
                                           test.situation.takingMoves[i].source.y,
                                           test.situation.takingMoves[i].victim.x,
                                           test.situation.takingMoves[i].victim.y,
                                           test.situation.takingMoves[i].destination.x,
                                           test.situation.takingMoves[i].destination.y);
                                }
                                for (int i = 0; i < test.situation.lastTakingSequence.tmsCount; i++) {
                                    gotoxy(39, 10 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.lastTakingSequence.takingMoves[i].source.x,
                                           test.situation.lastTakingSequence.takingMoves[i].source.y,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.x,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.y,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.x,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.y);
                                }
                                gotoxy(15, 13);
                                printf("%d", test.situation.tmCount);

                            } else if (stat == 2) {
                                for (int i = 0; i < test.situation.rmCount; i++) {
                                    gotoxy(16, 2 + i);
                                    printf("%d) [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.regularMoves[i].source.x,
                                           test.situation.regularMoves[i].source.y,
                                           test.situation.regularMoves[i].destination.x,
                                           test.situation.regularMoves[i].destination.y);
                                }
                            }
                            //renderBoard(&test.situation.board, White, 4, 4, true);
                            //waitForKey(13);
                            //cancelAMove(&test.situation, t);
                            //makeAMove(&test.situation, test.situation.regularMoves[move]);
                            if (stat != 0) scanf("%d", &move);*/
                        //} while (stat != 0);
                        //removeMarkedForDeath(&test.situation, negateColor(forWho));
                    }
                    //updateBoardRender(&test.situation.board);
                    forWho = negateColor(forWho);
                    clrscr();
                    //renderBoard(&test.situation.board, White, 4, 4, true);
                    /*int key;
                    do {
                        key = getch();
                        if (key == 224) {
                            key = getch();
                            switch (key) {
                                case 80: {

                                }
                                case 72: {

                                }
                            }
                        }
                    } while (key != 13);*/
                    clearAllSequencesLists(&test.situation);
                    //clearMoveLists(&test.situation);
                    //waitForKey(13);
                }
                break;
            }
            case 2: {
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                break;
            }
            case -1: {
                printf("mpdcount != pointscount");
                break;
            }
        }
        //Board gameBoard = initiateGameBoard(true);
        //renderBoard(&gameBoard, 4, 4);
    } while (choice != 5);
    return 0;
}
