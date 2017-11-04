                     GOTO                                                     main
            main:     VAR                                                        i
                   ASSIGN                  0                                     i
  lab_for_expr_0:     VAR                                                       t0
                       LT                  i                  4     lab_rel_true_1
                   ASSIGN                  0                                    t0
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t0
   lab_rel_end_1:      EQ                 t0                  0      lab_for_end_0
                   APARAM                                                        i
                     CALL                                                  writeln
  lab_for_incr_0:     ADD                  i                  1                  i
                     GOTO                                           lab_for_expr_0
   lab_for_end_0:  RETURN                                                         
