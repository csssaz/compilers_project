                     GOTO                                                     main
             odd:  FPARAM                                                      num
                      VAR                                                       t0
                      VAR                                                       t1
                      MOD                num                  2                 t1
                       NE                 t1                  0     lab_not_true_0
                   ASSIGN                  1                                    t0
                     GOTO                                            lab_not_end_0
  lab_not_true_0:  ASSIGN                  0                                    t0
   lab_not_end_0:  ASSIGN                 t0                                   odd
                   RETURN                                                         
            dumb:     VAR                                                        i
                   ASSIGN                  0                                     i
  lab_for_expr_1:     VAR                                                       t2
                     MULT                3.0                5.0                 t2
                       EQ                 t2                  0      lab_for_end_1
                   APARAM                                                        i
                     CALL                                                  writeln
  lab_for_incr_1:     ADD                  i                  1                  i
                     GOTO                                           lab_for_expr_1
   lab_for_end_1:  RETURN                                                         
            main:     VAR                                                        i
                   APARAM                                                        1
                     CALL                                                  writeln
                   RETURN                                                         
