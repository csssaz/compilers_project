                     GOTO                                                     main
     side_effect:  FPARAM                                                        n
                      VAR                                                       t0
                     MULT                100                  n                 t0
                   APARAM                                                       t0
                     CALL                                                  writeln
                   ASSIGN                  n                           side_effect
                   RETURN                                                         
            main:     VAR                                                        a
                      VAR                                                        b
                      VAR                                                        c
                      VAR                                                       t1
                       NE                  2                  0      lab_or_true_0
                   APARAM                                                        1
                     CALL                                              side_effect
                       NE        side_effect                  0      lab_or_true_0
                   ASSIGN                  0                                    t1
                     GOTO                                             lab_or_end_0
   lab_or_true_0:  ASSIGN                  1                                    t1
    lab_or_end_0:  ASSIGN                 t1                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t2
                       EQ                  0                  0    lab_and_false_1
                   APARAM                                                        2
                     CALL                                              side_effect
                       EQ        side_effect                  0    lab_and_false_1
                   ASSIGN                  1                                    t2
                     GOTO                                            lab_and_end_1
 lab_and_false_1:  ASSIGN                  0                                    t2
   lab_and_end_1:  ASSIGN                 t2                                     b
                   APARAM                                                        b
                     CALL                                                  writeln
                      VAR                                                       t3
                       NE                  3                  0     lab_not_true_2
                   ASSIGN                  1                                    t3
                     GOTO                                            lab_not_end_2
  lab_not_true_2:  ASSIGN                  0                                    t3
   lab_not_end_2:  ASSIGN                 t3                                     c
                   APARAM                                                        c
                     CALL                                                  writeln
                      VAR                                                       t4
                       NE                  0                  0      lab_or_true_3
                   APARAM                                                        3
                     CALL                                              side_effect
                       NE        side_effect                  0      lab_or_true_3
                   ASSIGN                  0                                    t4
                     GOTO                                             lab_or_end_3
   lab_or_true_3:  ASSIGN                  1                                    t4
    lab_or_end_3:  ASSIGN                 t4                                     a
                   APARAM                                                        a
                     CALL                                                  writeln
                      VAR                                                       t5
                       EQ                  1                  0    lab_and_false_4
                   APARAM                                                        4
                     CALL                                              side_effect
                       EQ        side_effect                  0    lab_and_false_4
                   ASSIGN                  1                                    t5
                     GOTO                                            lab_and_end_4
 lab_and_false_4:  ASSIGN                  0                                    t5
   lab_and_end_4:  ASSIGN                 t5                                     b
                   APARAM                                                        b
                     CALL                                                  writeln
                   RETURN                                                         
