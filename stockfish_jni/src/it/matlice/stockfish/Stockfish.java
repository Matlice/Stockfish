package it.matlice.stockfish;

import java.util.Map;

public class Stockfish {

    private static boolean stockfish_is_loaded = false;
    private static Stockfish instance = null;

    static {
        try {
            System.out.println("Loading stockfish...");
            System.loadLibrary("stockfishjni");
            stockfish_is_loaded = true;
        } catch (UnsatisfiedLinkError e){
            System.err.println("Cannot load stockfish from library path, trying from " + System.getProperty("user.dir"));
            try {
                System.load(System.getProperty("user.dir") + "/libstockfishjni.so");
                stockfish_is_loaded = true;
            } catch (UnsatisfiedLinkError err){
                System.err.println("Cannot load stockfish. Some functionalities will be compromised");
            }
        }
    }

    private Stockfish(boolean is_chess_960, Map<String, String> options) {
        Stockfish.nGetInstance(is_chess_960);
        if(options != null){
            options.forEach((k, v) -> {
                if(!Stockfish.nSetOption(k, v)) System.err.println("Unknown option " + k);
            });
        }
    }

    /**
     * Returns an instance of this class if not previously loaded, initializing it with given parameters
     * @param is_chess_960 as named
     * @param opt a map containing the options to be set
     * @return the instance if the native library is loaded, null if not
     */
    public static Stockfish getInstance(boolean is_chess_960, Map<String, String> opt) {
        if(!stockfish_is_loaded) return null;
        if(instance == null) instance = new Stockfish(is_chess_960, opt);
        return instance;
    }

    /**
     * returns the instance or null as {@link Stockfish#getInstance(boolean, Map)},
     * initializing values as default (is_chess_960=false, no parameters)
     * @return {@link Stockfish#getInstance(boolean, Map)}
     */
    public static Stockfish getInstance() {
        return Stockfish.getInstance(false, null);
    }

    public boolean setOption(String name, String value){
        return Stockfish.nSetOption(name, value);
    }

    //==================================================NATIVE METHODS
    /**
     * Creates the instance
     * @param is_chess_960 as named
     */
    private static native void nGetInstance(boolean is_chess_960);
    /**
     * destroys the instance
     */
    private static native void nDestroyInstance();
    /**
     * set an option
     * @param name option name
     * @param value option value
     * @return true on success, false on error or name not found
     */
    private static native boolean nSetOption(String name, String value);
    /**
     * sets board position through fen string
     * @param fen
     */
    private static native void nSetPosition(String fen);
    /**
     * performs a move
     * @param move move description (long algebric notation)
     * @return true on success, false if the move is not possible (?)
     */
    private static native boolean nMakeMove(String move);
    /**
     * start best move search
     * @param depth depth
     * @param ponder set it to false =)
     */
    private static native void nSearchBestMove(int depth, boolean ponder);
    /**
     * waits until next move is found and returns it
     * @return integer representing the move ()
     */
    private static native int nGetFoundNextMove();
    /**
     * waits until next move is found and returns it
     * @return the move in long algebric notation
     */
    private static native String nGetFoundNextMoveStr();
    /**
     * inverts the sides
     */
    private static native void nGlip();
    /**
     * starts a new game
     */
    private static native void nNewGame();
    /**
     * starts a new game
     */
    private static native void nDbgDisplay();
    /**
     * returns the calculated score
     * @param print cout detailed scores
     * @return
     */
    private static native float nGetScore(boolean print);

    /**
     * returns current position fen
     * @return current position fen
     */
    private static native String nGetFen();
}
