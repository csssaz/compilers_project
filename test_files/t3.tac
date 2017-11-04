                     GOTO                                                     main
            main:     VAR                                                       t0
                       LT                  1                  2     lab_rel_true_1
                   ASSIGN                  0                                    t0
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t0
   lab_rel_end_1:      NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:     VAR                                                       t1
                       LT                  2                  3     lab_rel_true_3
                   ASSIGN                  0                                    t1
                     GOTO                                            lab_rel_end_3
  lab_rel_true_3:  ASSIGN                  1                                    t1
   lab_rel_end_3:      NE                 t1                  0   lab_true_block_2
                     GOTO                                             lab_if_end_2
lab_true_block_2:  APARAM                                                        1
                     CALL                                                  writeln
                   RETURN                                                         
    lab_if_end_2:    GOTO                                             lab_if_end_0
    lab_if_end_0:  APARAM                                                        2
                     CALL                                                  writeln
                   RETURN                                                         
