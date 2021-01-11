package it.matlice;


import it.matlice.stockfish.Stockfish;

import java.util.Scanner;

public class EntryPoint {
    public static void main(String[] args) {
        Stockfish engine = Stockfish.getInstance();
        //engine.nSetOption("Use NNUE", "false");

//        var s = new Scanner(System.in);
//        engine.nDbgDisplay();
//        while(true){
//            if(engine.nMakeMove(s.next())) {
//                engine.flip();
//                engine.nSearchBestMove(15, false);
//                engine.nMakeMove(engine.getFoundNextMoveStr());
//                engine.flip();
//                engine.nDbgDisplay();
//                engine.nGetScore(true);
//            }
//        }
    }
}
