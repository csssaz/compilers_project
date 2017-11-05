                     GOTO                                                     main
            main:     VAR                                                        x
                      VAR                                                       t0
                       NE                  1                  0      lab_or_true_0
                       NE                1.0                  0      lab_or_true_0
                   ASSIGN                  0                                    t0
                     GOTO                                             lab_or_end_0
   lab_or_true_0:  ASSIGN                  1                                    t0
    lab_or_end_0:  ASSIGN                 t0                                     x
                   RETURN                                                         
