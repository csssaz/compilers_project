                      VAR                                                      hey
                      VAR                                                      why
                     GOTO                                                     main
              hi:     VAR                                                       t0
                     MULT                3.0                5.0                 t0
                       NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_0:  RETURN                                                         
             wut:     VAR                                                        j
                      VAR                                                        x
                   ASSIGN               1010                                     j
                   ASSIGN           123123.0                                     x
                   ASSIGN                  0                                     j
  lab_for_expr_1:     VAR                                                       t1
                      ADD                  x                3.0                 t1
                       EQ                 t1                  0      lab_for_end_1
  lab_for_incr_1:     ADD                  j                  1                  j
                     GOTO                                           lab_for_expr_1
   lab_for_end_1:  ASSIGN                  j                                   wut
                   RETURN                                                         
            main:     VAR                                                        i
                      VAR                                                        a
                      VAR                                                        b
                   ASSIGN                1.0                                     a
                       NE                  a                  0   lab_true_block_2
                     GOTO                                             lab_if_end_2
lab_true_block_2:  APARAM                                                        1
                     CALL                                                  writeln
    lab_if_end_2:  ASSIGN                1.0                                     b
                   ASSIGN                  0                                     i
  lab_for_expr_3:      EQ                  b                  0      lab_for_end_3
                   APARAM                                                       32
                     CALL                                                  writeln
  lab_for_incr_3:     ADD                  i                  1                  i
                     GOTO                                           lab_for_expr_3
   lab_for_end_3:  RETURN                                                         
