                      VAR                                                        b
                     GOTO                                                     main
       factorial:  FPARAM                                                        n
                      VAR                                                       t0
                       LE                  n                  1     lab_rel_true_1
                   ASSIGN                  0                                    t0
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t0
   lab_rel_end_1:      NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:  ASSIGN                  1                             factorial
                   RETURN                                                         
    lab_if_end_0:     VAR                                                       t1
                      SUB                  n                  1                 t1
                   APARAM                                                       t1
                     CALL                                                factorial
                      VAR                                                       t2
                     MULT                  n          factorial                 t2
                   ASSIGN                 t2                             factorial
                   RETURN                                                         
         testing:     VAR                                                        n
                   APARAM                                                        4
                     CALL                                                factorial
                   ASSIGN          factorial                                     n
                   APARAM                                                        n
                     CALL                                                  writeln
                   RETURN                                                         
            main:     VAR                                                        a
                      VAR                                                       t3
                     MULT                  1                  3                 t3
                      VAR                                                       t4
                      ADD                 t3                  5                 t4
                   ASSIGN                 t4                                     a
                     CALL                                                  testing
                   RETURN                                                         
