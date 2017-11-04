                      VAR                                                        i
                      VAR                                                        j
                      VAR                                                        a
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
                       GT                  i                  j     lab_rel_true_1
                   ASSIGN                  0                                    t1
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t1
   lab_rel_end_1:      NE                 t1                  0   lab_true_block_0
                   ASSIGN                5.0                                     x
                      SUB                  x                1.0                  x
                      VAR                                                       t2
                       NE                  0                  0      lab_or_true_2
                      VAR                                                       t3
                       LT                  i                  j     lab_rel_true_3
                   ASSIGN                  0                                    t3
                     GOTO                                            lab_rel_end_3
  lab_rel_true_3:  ASSIGN                  1                                    t3
   lab_rel_end_3:      NE                 t3                  0      lab_or_true_2
                   ASSIGN                  0                                    t2
                     GOTO                                             lab_or_end_2
   lab_or_true_2:  ASSIGN                  1                                    t2
    lab_or_end_2:  ASSIGN                 t2                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t4
                      VAR                                                       t5
                       GT                  i                  j     lab_rel_true_5
                   ASSIGN                  0                                    t5
                     GOTO                                            lab_rel_end_5
  lab_rel_true_5:  ASSIGN                  1                                    t5
   lab_rel_end_5:      NE                 t5                  0      lab_or_true_4
                      VAR                                                       t6
                      VAR                                                       t7
                       LT                  i                  j     lab_rel_true_7
                   ASSIGN                  0                                    t7
                     GOTO                                            lab_rel_end_7
  lab_rel_true_7:  ASSIGN                  1                                    t7
   lab_rel_end_7:      NE                 t7                  0     lab_not_true_6
                   ASSIGN                  1                                    t6
                     GOTO                                            lab_not_end_6
  lab_not_true_6:  ASSIGN                  0                                    t6
   lab_not_end_6:      NE                 t6                  0      lab_or_true_4
                   ASSIGN                  0                                    t4
                     GOTO                                             lab_or_end_4
   lab_or_true_4:  ASSIGN                  1                                    t4
    lab_or_end_4:  ASSIGN                 t4                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t8
                       LT                  i                  j     lab_rel_true_9
                   ASSIGN                  0                                    t8
                     GOTO                                            lab_rel_end_9
  lab_rel_true_9:  ASSIGN                  1                                    t8
   lab_rel_end_9:      NE                 t8                  0   lab_true_block_8
                     GOTO                                             lab_if_end_8
lab_true_block_8:  APARAM                                                     1212
                     CALL                                                  writeln
    lab_if_end_8:     VAR                                                       t9
                       GT                  i                  j    lab_rel_true_11
                   ASSIGN                  0                                    t9
                     GOTO                                           lab_rel_end_11
 lab_rel_true_11:  ASSIGN                  1                                    t9
  lab_rel_end_11:      NE                 t9                  0  lab_true_block_10
                   APARAM                                                     1111
                     CALL                                                  writeln
                     GOTO                                            lab_if_end_10
lab_true_block_10:     VAR                                                      t10
                   UMINUS                  1                                   t10
                   APARAM                                                      t10
                     CALL                                                  writeln
   lab_if_end_10:  APARAM                                                      666
                     CALL                                                  writeln
                     GOTO                                             lab_if_end_0
lab_true_block_0:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_0:     SUB                  j                  1                  j
                   APARAM                                                        j
                     CALL                                                  writeln
                      SUB                  j                  1                  j
                   APARAM                                                        j
                     CALL                                                  writeln
                      SUB                  x                1.0                  x
                   APARAM                                                        x
                     CALL                                                  writeln
                      SUB                  x                1.0                  x
                   APARAM                                                        x
                     CALL                                                  writeln
                   RETURN                                                         
