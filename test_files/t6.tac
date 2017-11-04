                     GOTO                                                     main
            main:     VAR                                                        a
                      VAR                                                        b
                      VAR                                                        l
                   ASSIGN                  2                                     a
                      VAR                                                       t0
                     MULT                  2                  3                 t0
                      VAR                                                       t1
                      ADD                  a                 t0                 t1
                   ASSIGN                 t1                                     b
                   APARAM                                                        b
                     CALL                                                  writeln
                      VAR                                                       t2
                      SUB                  5                  3                 t2
                      VAR                                                       t3
                      SUB                 t2                  2                 t3
                   ASSIGN                 t3                                     b
                   APARAM                                                        b
                     CALL                                                  writeln
                      VAR                                                       t4
                      MOD                  5                  2                 t4
                      VAR                                                       t5
                   DIVIDE                 t4                  2                 t5
                   ASSIGN                 t5                                     b
                   APARAM                                                        b
                     CALL                                                  writeln
                   RETURN                                                         
