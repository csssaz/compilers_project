                      VAR                                                        i
                      VAR                                                        j
                      VAR                                                       oe
                      VAR                                                        x
                     GOTO                                                     main
            main:     VAR                                                        k
                   ASSIGN                  2                                     i
                   ASSIGN                  3                                     j
                      VAR                                                       t0
                     MULT                  i                  j                 t0
                   ASSIGN                 t0                                     k
                   APARAM                                                        k
                     CALL                                                  writeln
                      VAR                                                       t1
                       GT                  i                  j     lab_rel_true_0
                   ASSIGN                  0                                    t1
                     GOTO                                            lab_rel_end_0
  lab_rel_true_0:  ASSIGN                  1                                    t1
   lab_rel_end_0:     VAR                                                       t2
                       GT                  i                  j     lab_rel_true_2
                   ASSIGN                  0                                    t2
                     GOTO                                            lab_rel_end_2
  lab_rel_true_2:  ASSIGN                  1                                    t2
   lab_rel_end_2:      EQ                 t2                  1   lab_true_block_1
                      VAR                                                       t3
                       LT                  i                  j     lab_rel_true_3
                   ASSIGN                  0                                    t3
                     GOTO                                            lab_rel_end_3
  lab_rel_true_3:  ASSIGN                  1                                    t3
   lab_rel_end_3:     VAR                                                       t4
                       LT                  i                  j     lab_rel_true_5
                   ASSIGN                  0                                    t4
                     GOTO                                            lab_rel_end_5
  lab_rel_true_5:  ASSIGN                  1                                    t4
   lab_rel_end_5:      EQ                 t4                  1   lab_true_block_4
                     GOTO                                             lab_if_end_4
lab_true_block_4:  APARAM                                                     1212
                     CALL                                                  writeln
    lab_if_end_4:     VAR                                                       t5
                       GT                  i                  j     lab_rel_true_6
                   ASSIGN                  0                                    t5
                     GOTO                                            lab_rel_end_6
  lab_rel_true_6:  ASSIGN                  1                                    t5
   lab_rel_end_6:     VAR                                                       t6
                       GT                  i                  j     lab_rel_true_8
                   ASSIGN                  0                                    t6
                     GOTO                                            lab_rel_end_8
  lab_rel_true_8:  ASSIGN                  1                                    t6
   lab_rel_end_8:      EQ                 t6                  1   lab_true_block_7
                   APARAM                                                     1111
                     CALL                                                  writeln
                     GOTO                                             lab_if_end_7
lab_true_block_7:     VAR                                                       t7
                   UMINUS                  1                                    t7
                   APARAM                                                       t7
                     CALL                                                  writeln
    lab_if_end_7:  APARAM                                                      666
                     CALL                                                  writeln
                     GOTO                                             lab_if_end_1
lab_true_block_1:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_1:     SUB                  j                  1                  j
                   APARAM                                                        j
                     CALL                                                  writeln
                      SUB                  j                  1                  j
                   APARAM                                                        j
                     CALL                                                  writeln
                   RETURN                                                         
