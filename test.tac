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
                   ASSIGN                5.0                                     x
                      SUB                  x                1.0                  x
                      VAR                                                       t3
                       EQ                  0                  1      lab_or_true_3
                      VAR                                                       t4
                       LT                  i                  j     lab_rel_true_4
                   ASSIGN                  0                                    t4
                     GOTO                                            lab_rel_end_4
  lab_rel_true_4:  ASSIGN                  1                                    t4
   lab_rel_end_4:      EQ                 t4                  1      lab_or_true_3
                   ASSIGN                  0                                    t3
                     GOTO                                             lab_or_end_3
   lab_or_true_3:  ASSIGN                  1                                    t3
    lab_or_end_3:  ASSIGN                 t3                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t5
                      VAR                                                       t6
                       GT                  i                  j     lab_rel_true_6
                   ASSIGN                  0                                    t6
                     GOTO                                            lab_rel_end_6
  lab_rel_true_6:  ASSIGN                  1                                    t6
   lab_rel_end_6:      EQ                 t6                  1      lab_or_true_5
                      VAR                                                       t7
                      VAR                                                       t8
                       LT                  i                  j     lab_rel_true_8
                   ASSIGN                  0                                    t8
                     GOTO                                            lab_rel_end_8
  lab_rel_true_8:  ASSIGN                  1                                    t8
   lab_rel_end_8:      NE                 t8                  0     lab_not_true_7
                   ASSIGN                  1                                    t7
                     GOTO                                            lab_not_end_7
  lab_not_true_7:  ASSIGN                  0                                    t7
   lab_not_end_7:      EQ                 t7                  1      lab_or_true_5
                   ASSIGN                  0                                    t5
                     GOTO                                             lab_or_end_5
   lab_or_true_5:  ASSIGN                  1                                    t5
    lab_or_end_5:  ASSIGN                 t5                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t9
                       LT                  i                  j     lab_rel_true_9
                   ASSIGN                  0                                    t9
                     GOTO                                            lab_rel_end_9
  lab_rel_true_9:  ASSIGN                  1                                    t9
   lab_rel_end_9:     VAR                                                      t10
                       LT                  i                  j    lab_rel_true_11
                   ASSIGN                  0                                   t10
                     GOTO                                           lab_rel_end_11
 lab_rel_true_11:  ASSIGN                  1                                   t10
  lab_rel_end_11:      EQ                t10                  1  lab_true_block_10
                     GOTO                                            lab_if_end_10
lab_true_block_10:  APARAM                                                     1212
                     CALL                                                  writeln
   lab_if_end_10:     VAR                                                      t11
                       GT                  i                  j    lab_rel_true_12
                   ASSIGN                  0                                   t11
                     GOTO                                           lab_rel_end_12
 lab_rel_true_12:  ASSIGN                  1                                   t11
  lab_rel_end_12:     VAR                                                      t12
                       GT                  i                  j    lab_rel_true_14
                   ASSIGN                  0                                   t12
                     GOTO                                           lab_rel_end_14
 lab_rel_true_14:  ASSIGN                  1                                   t12
  lab_rel_end_14:      EQ                t12                  1  lab_true_block_13
                   APARAM                                                     1111
                     CALL                                                  writeln
                     GOTO                                            lab_if_end_13
lab_true_block_13:     VAR                                                      t13
                   UMINUS                  1                                   t13
                   APARAM                                                      t13
                     CALL                                                  writeln
   lab_if_end_13:  APARAM                                                      666
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
                      SUB                  x                1.0                  x
                   APARAM                                                        x
                     CALL                                                  writeln
                      SUB                  x                1.0                  x
                   APARAM                                                        x
                     CALL                                                  writeln
                   RETURN                                                         
