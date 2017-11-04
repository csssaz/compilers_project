                     GOTO                                                     main
            main:     VAR                                                        i
                      VAR                                                        j
                   ASSIGN                  0                                     i
  lab_for_expr_0:     VAR                                                       t0
                       LT                  i                  4     lab_rel_true_1
                   ASSIGN                  0                                    t0
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t0
   lab_rel_end_1:      EQ                 t0                  0      lab_for_end_0
                   APARAM                                                      100
                     CALL                                                  writeln
                   APARAM                                                        i
                     CALL                                                  writeln
                   ASSIGN                  0                                     j
  lab_for_expr_2:     VAR                                                       t1
                       LT                  j                  3     lab_rel_true_3
                   ASSIGN                  0                                    t1
                     GOTO                                            lab_rel_end_3
  lab_rel_true_3:  ASSIGN                  1                                    t1
   lab_rel_end_3:      EQ                 t1                  0      lab_for_end_2
                      VAR                                                       t2
                       EQ                  i                  j     lab_rel_true_5
                   ASSIGN                  0                                    t2
                     GOTO                                            lab_rel_end_5
  lab_rel_true_5:  ASSIGN                  1                                    t2
   lab_rel_end_5:      NE                 t2                  0   lab_true_block_4
                     GOTO                                             lab_if_end_4
lab_true_block_4:    GOTO                                           lab_for_incr_2
    lab_if_end_4:  APARAM                                                        j
                     CALL                                                  writeln
  lab_for_incr_2:     ADD                  j                  1                  j
                     GOTO                                           lab_for_expr_2
   lab_for_end_2:    GOTO                                           lab_for_incr_0
  lab_for_incr_0:     ADD                  i                  1                  i
                     GOTO                                           lab_for_expr_0
   lab_for_end_0:  RETURN                                                         
