                     GOTO                                                     main
            main:     VAR                                                        b
                   ASSIGN                1.0                                     b
                      VAR                                                       t0
                     MULT                  b             3.1415                 t0
                       NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_0:  RETURN                                                         
