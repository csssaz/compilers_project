                     GOTO                                                     main
            main:     VAR                                                        j
                   ASSIGN                  1                                     j
                      VAR                                                       t0
                       EQ                  j                  0    lab_and_false_1
                       EQ                3.0                  0    lab_and_false_1
                   ASSIGN                  1                                    t0
                     GOTO                                            lab_and_end_1
 lab_and_false_1:  ASSIGN                  0                                    t0
   lab_and_end_1:      NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_0:  RETURN                                                         
