package it.matlice.stockfish;

import java.util.Map;

public class Stockfish {

    static {
        try {
            System.out.println("Loading stockfish...");
            System.loadLibrary("stockfishjni");
        } catch (UnsatisfiedLinkError e){
            System.out.println("Loading stockfish from cwd (" + System.getProperty("user.dir") + ")");
            System.load(System.getProperty("user.dir") + "/../cmake-build-debug/libstockfishjni.so");
        }
    }

    private static Stockfish instance = null;

    private Stockfish(boolean is_chess_960, Map<String, String> options) {
        this.nGetInstance(is_chess_960);
        if(options != null){
            options.forEach((k, v) -> {
                if(!this.nSetOption(k, v)) System.err.println("Unknown option " + k);
            });
        }
    }

    public static Stockfish getInstance(boolean is_chess_960, Map<String, String> opt) {
        if(instance == null) instance = new Stockfish(is_chess_960, opt);
        return instance;
    }

    public static Stockfish getInstance() {
        return Stockfish.getInstance(false, null);
    }

    /**
     * Creates the instance
     * @param is_chess_960 as named
     */
    public native void nGetInstance(boolean is_chess_960);

    /**
     * destroys the instance
     */
    public native void nDestroyInstance();

    /**
     * set an option
     * @param name option name
     * @param value option value
     * @return true on success, false on error or name not found
     */
    public native boolean nSetOption(String name, String value);

    /**
     * sets board position through fen string
     * @param fen
     */
    public native void nSetPosition(String fen);

    /**
     * performs a move
     * @param move move description (long algebric notation)
     * @return true on success, false if the move is not possible (?)
     */
    public native boolean nMakeMove(String move);

    /**
     * start best move search
     * @param depth depth
     * @param ponder set it to false =)
     */
    public native void nSearchBestMove(int depth, boolean ponder);

    /**
     * waits until next move is found and returns it
     * @return integer representing the move ()
     */
    public native int getFoundNextMove();

    /**
     * waits until next move is found and returns it
     * @return the move in long algebric notation
     */
    public native String getFoundNextMoveStr();

    /**
     * inverts the sides
     */
    public native void flip();

    /**
     * starts a new game
     */
    public native void nNewGame();

    /**
     * starts a new game
     */
    public native void nDbgDisplay();

    /**
     * returns the calculated score
     * @param print cout detailed scores
     * @return
     */
    public native float nGetScore(boolean print);
}
