function(setdefaults)
    set(OPTIMIZATION 3 PARENT_SCOPE)
    set(DEBUG NO PARENT_SCOPE)
    set(SANITIZE NO PARENT_SCOPE)
    set(BITS 64 PARENT_SCOPE)
    set(PREFETCH NO PARENT_SCOPE)
    set(POPCNT NO PARENT_SCOPE)
    set(PEXT NO PARENT_SCOPE)
    set(MMX NO PARENT_SCOPE)
    set(SSE NO PARENT_SCOPE)
    set(SSE2 NO PARENT_SCOPE)
    set(SSSE3 NO PARENT_SCOPE)
    set(SSE41 NO PARENT_SCOPE)
    set(AVX2 NO PARENT_SCOPE)
    set(AVX512 NO PARENT_SCOPE)
    set(VNNI256 NO PARENT_SCOPE)
    set(VNNI512 NO PARENT_SCOPE)
    set(NEON NO PARENT_SCOPE)
    set(STRIP strip PARENT_SCOPE)
endfunction(setdefaults)

function(print_prams)
    message(
            SETTINGS: \n\n====================================\n
            OPTIMIZATION: ${OPTIMIZATION}\n
            DEBUG: ${DEBUG}\n
            SANITIZE: ${SANITIZE}\n
            BITS: ${BITS}\n
            PREFETCH: ${PREFETCH}\n
            POPCNT: ${POPCNT}\n
            PEXT: ${PEXT}\n
            MMX: ${MMX}\n
            SSE: ${SSE}\n
            SSE2: ${SSE2}\n
            SSSE3: ${SSSE3}\n
            SSE41: ${SSE41}\n
            AVX2: ${AVX2}\n
            AVX512: ${AVX512}\n
            VNNI256: ${VNNI256}\n
            VNNI512: ${VNNI512}\n
            NEON: ${NEON}\n
            STRIP: ${STRIP}\n
            ====================================\n
    )
endfunction(print_prams)