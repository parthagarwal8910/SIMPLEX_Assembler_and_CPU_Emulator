; -------------------------------------------------------------------------
; SIMPLEX Assembler - Bubble Sort
; -------------------------------------------------------------------------

; Initialize algorithm and check for empty/1-item array
init:
        ldc N
        ldnl 0
        adc -1
        brz done       ; If N=1, already sorted
        brlz done      ; If N<1, nothing to sort

sort_start:
        ; var_swapped = 0
        ldc 0
        ldc var_swapped
        stnl 0

        ; var_ptr = address of array (Point to the start)
        ldc array
        ldc var_ptr
        stnl 0

        ; var_count = N - 1 (Number of comparisons for this pass)
        ldc N
        ldnl 0
        adc -1
        ldc var_count
        stnl 0

inner_loop:
        ; Load val1 = *ptr
        ldc var_ptr
        ldnl 0
        ldnl 0
        ldc val1
        stnl 0

        ; Load val2 = *(ptr + 1)
        ldc var_ptr
        ldnl 0
        ldnl 1
        ldc val2
        stnl 0

        ; Compare val1 and val2 (Math: A = val1 - val2)
        ldc val1
        ldnl 0         ; A = val1
        ldc val2
        ldnl 0         ; B = val1, A = val2
        sub            ; A = val1 - val2
        brlz no_swap   ; if val1 < val2 (A < 0), do not swap
        brz no_swap    ; if val1 == val2 (A == 0), do not swap

        ; --- SWAP VALUES ---
        
        ; *ptr = val2
        ldc val2
        ldnl 0         ; A = val2
        ldc var_ptr
        ldnl 0         ; B = val2, A = ptr
        stnl 0         ; memory[ptr] = val2

        ; *(ptr + 1) = val1
        ldc val1
        ldnl 0         ; A = val1
        ldc var_ptr
        ldnl 0         ; B = val1, A = ptr
        stnl 1         ; memory[ptr + 1] = val1

        ; var_swapped = 1 (Flag that a swap occurred)
        ldc 1
        ldc var_swapped
        stnl 0

no_swap:
        ; var_ptr++ (Move to next element)
        ldc var_ptr
        ldnl 0
        adc 1
        ldc var_ptr
        stnl 0

        ; var_count-- (One less comparison to do)
        ldc var_count
        ldnl 0
        adc -1
        ldc var_count
        stnl 0

        ; loop condition check (If count <= 0, break inner loop)
        ldc var_count
        ldnl 0
        brz end_inner
        brlz end_inner
        br inner_loop

end_inner:
        ; Check if we swapped anything this pass
        ldc var_swapped
        ldnl 0
        brz done       ; if swapped == 0, array is fully sorted!
        br sort_start  ; else, reset variables and do another pass

done:
        HALT

; -------------------------------------------------------------------------
; Variables and Data Segment
; -------------------------------------------------------------------------
; These 'data' pseudo-instructions reserve memory space for our variables
var_swapped: data 0
var_ptr:     data 0
var_count:   data 0
val1:        data 0
val2:        data 0

; The array to be sorted
N:           data 6
array:       data 42
             data 15
             data 8
             data 99
             data 23
             data 4