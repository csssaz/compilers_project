                      VAR                                                      mod
                     GOTO                                                     main
    fast_mod_exp:  FPARAM                                                        b
                   FPARAM                                                        e
                      VAR                                                       t0
                       EQ                  e                  0     lab_rel_true_1
                   ASSIGN                  0                                    t0
                     GOTO                                            lab_rel_end_1
  lab_rel_true_1:  ASSIGN                  1                                    t0
   lab_rel_end_1:      NE                 t0                  0   lab_true_block_0
                     GOTO                                             lab_if_end_0
lab_true_block_0:  ASSIGN                  1                          fast_mod_exp
                   RETURN                                                         
    lab_if_end_0:     VAR                                                       t1
                       EQ                  e                  1     lab_rel_true_3
                   ASSIGN                  0                                    t1
                     GOTO                                            lab_rel_end_3
  lab_rel_true_3:  ASSIGN                  1                                    t1
   lab_rel_end_3:      NE                 t1                  0   lab_true_block_2
                     GOTO                                             lab_if_end_2
lab_true_block_2:  ASSIGN                  b                          fast_mod_exp
                   RETURN                                                         
    lab_if_end_2:     VAR                                                       t2
                      MOD                  e                  2                 t2
                      VAR                                                       t3
                       EQ                 t2                  0     lab_rel_true_5
                   ASSIGN                  0                                    t3
                     GOTO                                            lab_rel_end_5
  lab_rel_true_5:  ASSIGN                  1                                    t3
   lab_rel_end_5:      NE                 t3                  0   lab_true_block_4
                     GOTO                                             lab_if_end_4
lab_true_block_4:     VAR                                                       t4
                     MULT                  b                  b                 t4
                      VAR                                                       t5
                      MOD                 t4                mod                 t5
                      VAR                                                       t6
                   DIVIDE                  e                  2                 t6
                   APARAM                                                       t5
                   APARAM                                                       t6
                     CALL                                             fast_mod_exp
                      VAR                                                       t7
                      MOD       fast_mod_exp                mod                 t7
                   ASSIGN                 t7                          fast_mod_exp
                   RETURN                                                         
    lab_if_end_4:     VAR                                                       t8
                     MULT                  b                  b                 t8
                      VAR                                                       t9
                      MOD                 t8                mod                 t9
                      VAR                                                      t10
                      SUB                  e                  1                t10
                      VAR                                                      t11
                   DIVIDE                t10                  2                t11
                   APARAM                                                       t9
                   APARAM                                                      t11
                     CALL                                             fast_mod_exp
                      VAR                                                      t12
                      MOD       fast_mod_exp                mod                t12
                      VAR                                                      t13
                     MULT                  b                t12                t13
                      VAR                                                      t14
                      MOD                t13                mod                t14
                   ASSIGN                t14                          fast_mod_exp
                   RETURN                                                         
        is_prime:  FPARAM                                                        n
                      VAR                                                      div
                      VAR                                                        i
                   ASSIGN                  0                                     i
                      VAR                                                      t15
                      VAR                                                      t16
                       EQ                  n                  2     lab_rel_true_8
                   ASSIGN                  0                                   t16
                     GOTO                                            lab_rel_end_8
  lab_rel_true_8:  ASSIGN                  1                                   t16
   lab_rel_end_8:      NE                t16                  0      lab_or_true_7
                      VAR                                                      t17
                       EQ                  n                  3     lab_rel_true_9
                   ASSIGN                  0                                   t17
                     GOTO                                            lab_rel_end_9
  lab_rel_true_9:  ASSIGN                  1                                   t17
   lab_rel_end_9:      NE                t17                  0      lab_or_true_7
                   ASSIGN                  0                                   t15
                     GOTO                                             lab_or_end_7
   lab_or_true_7:  ASSIGN                  1                                   t15
    lab_or_end_7:      NE                t15                  0   lab_true_block_6
                     GOTO                                             lab_if_end_6
lab_true_block_6:  ASSIGN                  1                              is_prime
                   RETURN                                                         
    lab_if_end_6:     VAR                                                      t18
                      MOD                  n                  2                t18
                      VAR                                                      t19
                       EQ                t18                  0    lab_rel_true_11
                   ASSIGN                  0                                   t19
                     GOTO                                           lab_rel_end_11
 lab_rel_true_11:  ASSIGN                  1                                   t19
  lab_rel_end_11:      NE                t19                  0  lab_true_block_10
                     GOTO                                            lab_if_end_10
lab_true_block_10:  ASSIGN                  0                              is_prime
                   RETURN                                                         
   lab_if_end_10:  ASSIGN                  3                                   div
 lab_for_expr_12:     VAR                                                      t20
                     MULT                div                div                t20
                      VAR                                                      t21
                       LE                t20                  n    lab_rel_true_13
                   ASSIGN                  0                                   t21
                     GOTO                                           lab_rel_end_13
 lab_rel_true_13:  ASSIGN                  1                                   t21
  lab_rel_end_13:      EQ                t21                  0     lab_for_end_12
                      VAR                                                      t22
                      MOD                  n                div                t22
                      VAR                                                      t23
                       EQ                t22                  0    lab_rel_true_15
                   ASSIGN                  0                                   t23
                     GOTO                                           lab_rel_end_15
 lab_rel_true_15:  ASSIGN                  1                                   t23
  lab_rel_end_15:      NE                t23                  0  lab_true_block_14
                     GOTO                                            lab_if_end_14
lab_true_block_14:  ASSIGN                  0                              is_prime
                   RETURN                                                         
   lab_if_end_14:     VAR                                                      t24
                      ADD                div                  2                t24
                   ASSIGN                t24                                   div
 lab_for_incr_12:     ADD                  i                  1                  i
                     GOTO                                          lab_for_expr_12
  lab_for_end_12:  ASSIGN                  1                              is_prime
                   RETURN                                                         
            main:     VAR                                                        i
                   ASSIGN         1000000007                                   mod
                   APARAM                                                      mod
                     CALL                                                 is_prime
                   APARAM                                                 is_prime
                     CALL                                                  writeln
                   ASSIGN               1000                                     i
 lab_for_expr_16:     VAR                                                      t25
                       LT                  i           20000000    lab_rel_true_17
                   ASSIGN                  0                                   t25
                     GOTO                                           lab_rel_end_17
 lab_rel_true_17:  ASSIGN                  1                                   t25
  lab_rel_end_17:      EQ                t25                  0     lab_for_end_16
                   APARAM                                                        i
                     CALL                                                 is_prime
                       NE           is_prime                  0  lab_true_block_18
                     GOTO                                            lab_if_end_18
lab_true_block_18:  ASSIGN                  i                                   mod
                     GOTO                                           lab_for_end_16
   lab_if_end_18:    GOTO                                          lab_for_incr_16
 lab_for_incr_16:     ADD                  i                  1                  i
                     GOTO                                          lab_for_expr_16
  lab_for_end_16:  APARAM                                                      mod
                     CALL                                                  writeln
                   APARAM                                                      mod
                     CALL                                                 is_prime
                   APARAM                                                 is_prime
                     CALL                                                  writeln
                   APARAM                                                        2
                   APARAM                                                        3
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                        5
                   APARAM                                                        5
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                        3
                   APARAM                                                       17
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                    17423
                   APARAM                                                   103123
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                     1112
                   APARAM                                                  7777777
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                     9912
                   APARAM                                                 76457812
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   APARAM                                                      666
                   APARAM                                                 66666666
                     CALL                                             fast_mod_exp
                   APARAM                                             fast_mod_exp
                     CALL                                                  writeln
                   RETURN                                                         
