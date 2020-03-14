
/*
               |    tWHILE opt_id '(' expr ')'
                      {
                          parser->beginLoop(poolPtrToAuto($2).get());
                      }
                           stmts tENDWHILE
                           {
                               $$ = addToPool(new Stmt::While(parser->loopID(), 
                                                                        poolToAuto($4), 
                                                                        poolToAuto($7)));
                               parser->endLoop();
                           }
               |    tFOR tID tIN '(' expr ')'
                      {
                          parser->beginLoop(poolPtrToAuto($2).get());
                      }
                           stmts tENDFOR
                           {
                                     $$ = addToPool(new Stmt::ForList(parser->loopID(), 
                                                                                 poolToAuto($5), 
                                                                                 poolToAuto($8)));
                                     parser->endLoop();
                                 }
                     |    tFOR tID tIN '[' expr tTO expr ']'
                            {
                                parser->beginLoop(poolPtrToAuto($2).get());
                            }
                                 stmts tENDFOR
                                 {
                                     $$ = addToPool(new Stmt::ForRange(parser->loopID(),
                                                                                  poolToAuto($5), 
                                                                                  poolToAuto($7), 
                                                                                  poolToAuto($10)));
                                     parser->endLoop();
                                 }
                     |    tTRY stmts excepts tENDTRY
                            {
                                $$ = addToPool(new Stmt::TryExcept(poolToAuto($2), 
                                                                              poolToAuto($3)));
                            }
                     |    tTRY stmts tFINALLY stmts tENDTRY
                            {
                                $$ = addToPool(new Stmt::TryFinally(poolToAuto($2), 
                                                                                poolToAuto($4)));
                            }
                     |    tFORK opt_id '(' expr ')'
                            {
                                parser->beginFork();

                                if($2)
                                    parser->addVar(**$2);
                            }   
                                 stmts tENDFORK
                                 {
                                     BEGIN_BLOCK();
                                     unique_ptr<string> id = poolPtrToAuto($2);
                                     Symbol forkVar = (id.get() ? 
                                                             parser->findVar(*id) : 
                                                             Symbol());
                                     $$ = addToPool(new Stmt::Fork(forkVar,
                                                                             poolToAuto($4), 
                                                                             poolToAuto($7)));
                                     parser->endFork();
                                     END_BLOCK();
                                 }
                     |    tSWITCH '(' expr ')' 
                            {
                                parser->beginSwitch();
                            }
                                 switch_cases tENDSWITCH
                                 {
                                     $$ = addToPool(new Stmt::Switch(poolToAuto($3), 
                                                                                poolToAuto($6)));
                                     parser->endSwitch();
                                 }
                     |    tRETURN ';'
                            {
                                $$ = addToPool(new Stmt::Return);
                            }
                     |    tRETURN expr ';'
                            {
                                $$ = addToPool(new Stmt::Return(poolToAuto($2)));
                            }
                     |    tCONTINUE opt_id ';'
                            {
                                BEGIN_BLOCK();
                                std::unique_ptr<std::string> id = poolPtrToAuto($2);
                                Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

                                if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
                                    if(id.get())
                                        yyerror("Invalid loop name in `continue' "
                                                  "statement: " + *id);
                                    else
                                        yyerror("No enclosing loop or switch for "
                                                  "`continue' statement");
                                }

                                $$ = addToPool(new Stmt::Continue(sym));
                                END_BLOCK();
                            }
                     |    tBREAK opt_id ';'
                            {
                                BEGIN_BLOCK();
                                std::unique_ptr<std::string> id = poolPtrToAuto($2);
                                Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

                                if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
                                    if(id.get())
                                        yyerror("Invalid loop name in `break' "
                                                  "statement: " + *id);
                                    else
                                        yyerror("No enclosing loop or switch for "
                                                  "`break' statement");
                                }

                                $$ = addToPool(new Stmt::Break(sym));
                                END_BLOCK();
                            }
                     |    expr ';' 
                            {
                                $$ = addToPool(new Stmt::ExprStmt(poolToAuto($1)));
                            }
                     |    ';'
                            {
                                $$ = 0;
                            }
     ;


opt_id:              /* nothing */
                            {
                                $$ = 0;
                            }
                     |    tID
                            {
                                $$ = $1;
                            }
     ;

excepts:             tEXCEPT opt_id '(' except_codes ')' stmts
                            {
                                BEGIN_BLOCK();
                                $$ = addToPool(new ExceptList);
                                Symbol var = ($2 ? 
                                                  parser->addVar(*poolPtrToAuto($2)) : 
                                                  Symbol());
                                safePushBack(*$$, new Except(var,
                                                                      poolToAuto($4),
                                                                      poolToAuto($6)));
                                END_BLOCK();
                            }
                     |    excepts tEXCEPT opt_id '(' except_codes ')' stmts
                            {
                                BEGIN_BLOCK();
                                if($1->back()->codes().empty())
                                    yyerror("Unreachable EXCEPT clause");
                                
                                $$ = $1;
                                Symbol var = ($3 ? 
                                                  parser->addVar(*poolPtrToAuto($3)) :
                                                  Symbol());
                                safePushBack(*$$, new Except(var,
                                                                      poolToAuto($5),
                                                                      poolToAuto($7)));
                                END_BLOCK();
                            }
     ;

except_codes:        tANY
                            {
                                $$ = addToPool(new Expr::ArgList);
                            }
                     |    ne_arglist
                            {
                                $$ = $1;
                            }
     ;

switch_cases:        /* nothing */
                            {
                                $$ = addToPool(new Stmt::Switch::CaseList);
                            }
                     |    switch_cases tCASE '(' expr ')' stmts
                            {
                                $$ = $1;
                                safePushBack(*$$, 
                                                 new Stmt::Switch::Case(poolToAuto($4), 
                                                                                poolToAuto($6)));
                            }
                     |    switch_cases tDEFAULT 
                            {
                                if(find_if($1->begin(), $1->end(),
                                              mem_fun(&Stmt::Switch::Case::isDefault)) != $1->end())
                                    yyerror("Multiple default clauses in switch statement");
                            }
                                 stmts
                                 {
                                     $$ = $1;
                                     safePushBack(*$$, 
                                                      new Stmt::Switch::Default(poolToAuto($4)));
                                 }
     ;

arglist:             /* nothing */
                            {
                                $$ = addToPool(new Expr::ArgList);
                            }
                     |    ne_arglist
                            {
                                $$ = $1;
                            }
     ;

ne_arglist:          argument
                            {
                                $$ = addToPool(new Expr::ArgList);
                                safePushBack(*$$, poolToAuto($1));
                            }
                     |    ne_arglist ',' argument
                            {
                                $$ = $1;
                                safePushBack(*$$, poolToAuto($3));
                            }
     ;

argument:            expr
                            {
                                $$ = $1;
                            }
                     |    '@' expr
                            {
                                $$ = addToPool(new Expr::Splice(poolToAuto($2)));
                            }
     ;


     */

                |   '!' expr
                    {
                        $$ = std::make_unique<Expr::Not>(std::move($2);
                    }
                |    '-' expr   %prec tNEGATE
                       {
                           $$ = addToPool(new Expr::Negate(poolToAuto($2)));
                       }
                |    tINCREMENT expr
                       {
                           if(!$2->assignable())
                               yyerror("Invalid operand to ++ operator");

                           $$ = addToPool(new Expr::PreInc(poolToAuto($2)));
                       }
                |    tDECREMENT expr
                       {
                           if(!$2->assignable())
                               yyerror("Invalid operand to -- operator");

                           $$ = addToPool(new Expr::PreDec(poolToAuto($2)));
                       }
                |    expr tINCREMENT
                       {
                           if(!$1->assignable())
                               yyerror("Invalid operand to ++ operator");

                           $$ = addToPool(new Expr::PostInc(poolToAuto($1)));
                       }
                |    expr tDECREMENT
                       
                       {
                           if(!$1->assignable())
                               yyerror("Invalid operand to -- operator");

                           $$ = addToPool(new Expr::PostDec(poolToAuto($1)));

                       }
                |    expr '=' expr
                       {
                           BEGIN_BLOCK();
                           std::unique_ptr<Expr::Expr> dest = poolToAuto($1);

                           if(!dest->assignable()) {
                               bool converted = false;
                               Expr::List* list;

                               if((list = dynamic_cast<Expr::List*>(dest.get()))) {
                                   std::unique_ptr<ScatterTargetList> targets(new ScatterTargetList);
                                   converted = listToScatter(*targets, 
                                                                     list->elements());
                                   dest.reset(new Expr::Scatter(std::move(targets)));
                               } 
                               
                               if(!converted)
                                   yyerror("Invalid expression on left side of assignment");
                           }

                           $$ = addToPool(new Expr::Assign(std::move(dest), poolToAuto($3)));
                           END_BLOCK();
                       }
                |    '{' scatter '}' '=' expr
                       {
                           BEGIN_BLOCK();
                           unique_ptr<Expr::Expr> scatter(new Expr::Scatter(poolToAuto($2)));
                           $$ = addToPool(new Expr::Assign(std::move(scatter),
                                                                     poolToAuto($5)));
                           END_BLOCK();
                       }
                     |    expr tOR expr
                            {
                                $$ = MAKE_BINARY_EXPR(Or, $1, $3);
                            }
                     |    expr tAND expr
                            {
                                $$ = MAKE_BINARY_EXPR(And, $1, $3);
                            }
                     |    expr tEQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(Equal, $1, $3);
                            }
                     |    expr tNE expr
                            {
                                $$ = MAKE_BINARY_EXPR(NotEqual, $1, $3);
                            }
                     |    expr '<' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Less, $1, $3);
                            }
                     |    expr tLE expr
                            {
                                $$ = MAKE_BINARY_EXPR(LessEqual, $1, $3);
                            }
                     |    expr '>' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Greater, $1, $3);
                            }
                     |    expr tGE expr
                            {
                                $$ = MAKE_BINARY_EXPR(GreaterEqual, $1, $3);
                            }
                     |    expr tIN expr
                            {
                                $$ = MAKE_BINARY_EXPR(In, $1, $3);
                            }
                     |    expr '+' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Add, $1, $3);
                            }
                     |    expr '-' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Sub, $1, $3);
                            }
                     |    expr '*' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Mul, $1, $3);
                            }
                     |    expr '/' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Div, $1, $3);
                            }
                     |    expr '%' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Mod, $1, $3);
                            }
                     |    expr '^' expr
                            {
                                $$ = MAKE_BINARY_EXPR(Exp, $1, $3);
                            }
                     |    expr tADD_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(AddEqual, $1, $3);
                            }
                     |    expr tSUB_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(SubEqual, $1, $3);
                            }
                     |    expr tMUL_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(MulEqual, $1, $3);
                            }
                     |    expr tDIV_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(DivEqual, $1, $3);
                            }
                     |    expr tMOD_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(ModEqual, $1, $3);
                            }
                     |    expr tEXP_EQ expr
                            {
                                $$ = MAKE_BINARY_EXPR(ExpEqual, $1, $3);
                            }
                     |    expr '?' expr '|' expr
                            {
                                $$ = addToPool(new Expr::Conditional(poolToAuto($1), 
                                                                                 poolToAuto($3), 
                                                                                 poolToAuto($5)));
                            }
                     |    expr '.' tID
                            {
                                BEGIN_BLOCK();
                                unique_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto($3)));
                                $$ = addToPool(new Expr::Prop(poolToAuto($1),
                                                                        std::move(name)));
                                END_BLOCK();
                            }
                     |    expr '.' '(' expr ')'
                            {
                                $$ = addToPool(new Expr::Prop(poolToAuto($1), 
                                                                        poolToAuto($4)));
                            }
                     |    expr ':' tID '(' arglist ')'
                            {
                                BEGIN_BLOCK();
                                unique_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto($3)));
                                $$ = addToPool(new Expr::VerbCall(poolToAuto($1), 
                                                                             std::move(name), 
                                                                             poolToAuto($5)));
                                END_BLOCK();
                            }
                     |    expr ':' '(' expr ')' '(' arglist ')'
                            {
                                $$ = addToPool(new Expr::VerbCall(poolToAuto($1),
                                                                             poolToAuto($4),
                                                                             poolToAuto($7)));
                            }
                     |    expr '[' dollars_up expr ']'
                            {
                                parser->decDollarDepth();
                                $$ = addToPool(new Expr::Index(poolToAuto($1),
                                                                         poolToAuto($4)));
                            }
                     |    expr '[' dollars_up expr tTO expr ']'
                            {
                                parser->decDollarDepth();
                                $$ = addToPool(new Expr::Range(poolToAuto($1), 
                                                                         poolToAuto($4), 
                                                                         poolToAuto($6)));
                            }
                     |    '$'
                            {
                                if(!parser->dollarDepth())
                                    yyerror("Illegal context for `$' expression");

                                $$ = addToPool(new Expr::Length);
                            }
                     |    '$' tID
                            {
                                $$ = addToPool(new Expr::SystemProp(poolPtrToAuto($2)));
                            }
                     |    '$' tID '(' arglist ')'
                            {
                                $$ = addToPool(new Expr::SystemCall(poolPtrToAuto($2),
                                                                                poolToAuto($4)));
                            }
                     |    tID '(' arglist ')'
                            {
                                $$ = addToPool(new Expr::Builtin(poolPtrToAuto($1), 
                                                                            poolToAuto($3)));
                            }
                     |    '`' expr '!' except_codes catch_result '\''
                            {
                                $$ = addToPool(new Expr::Catch(poolToAuto($2), 
                                                                         poolToAuto($4),
                                                                         poolToAuto($5)));
                            }
                     |    '(' expr ')'
                            {
                                 $$ = $2;
                            }
     ;


scatter:             opt_target
                            {
                                $$ = addToPool(new ScatterTargetList);
                                safePushBack(*$$, poolToAuto($1));
                            }
                     |    scatter ',' opt_target
                            {
                                $$ = $1;
                                safePushBack(*$$, poolToAuto($3));
                            }
                     |    scatter ',' tID
                            {
                                BEGIN_BLOCK();
                                $$ = $1;
                                Symbol var = parser->addVar(*poolPtrToAuto($3));
                                safePushBack(*$$, 
                                                 new ScatterTarget(ScatterTarget::REQUIRED, var));
                                END_BLOCK();
                            }
                     |    scatter ',' '@' tID
                            {
                                BEGIN_BLOCK();
                                $$ = $1;
                                Symbol var = parser->addVar(*poolPtrToAuto($4));
                                safePushBack(*$$, 
                                                 new ScatterTarget(ScatterTarget::REST, var));
                                END_BLOCK();
                            }
                     |    ne_arglist ',' opt_target
                            {
                                $$ = addToPool(new ScatterTargetList);
                                listToScatter(*$$, *poolToAuto($1));
                                safePushBack(*$$, poolToAuto($3));
                            }
     ;

opt_target:          '?' tID
                            {
                                BEGIN_BLOCK();
                                Symbol var = parser->addVar(*poolPtrToAuto($2));
                                $$ = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
                                                                            var));
                                END_BLOCK();
                            }
                     |    '?' tID '=' expr
                            {
                                BEGIN_BLOCK();
                                Symbol var = parser->addVar(*poolPtrToAuto($2));
                                $$ = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
                                                                            var,
                                                                            poolToAuto($4)));
                                END_BLOCK();
                            }
     ;

dollars_up:          /* nothing */
                            {
                                parser->incDollarDepth();
                            }
     ;

catch_result:        /* nothing */
                            {
                                $$ = 0;
                            }
                     |    tARROW expr
                            {
                                $$ = $2;
                            }
     ;

     */
         
