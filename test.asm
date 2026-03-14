start: ldc 5
       adc 3
loop:  sub
       brz end
       br loop
end:   halt