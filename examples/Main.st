PROGRAM Main
VAR
    counter : INT := 0;
    running : BOOL := FALSE;
    timer : TIME;
END_VAR

(* 主程序逻辑 *)
IF running THEN
    counter := counter + 1;
    
    IF counter >= 100 THEN
        counter := 0;
    END_IF;
END_IF;

END_PROGRAM
