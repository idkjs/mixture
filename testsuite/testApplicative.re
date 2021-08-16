/* TestApplicative -- Test applicative structures

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */
open Broken;

let (|>) = (x, f) => f(x); /* Compatibility with OCaml 4.00.1 */

/* We implement Reader as an exmple of Applicative */
module Reader = {
  type environment = list((string, bool));

  let fetch = (x, env) => List.assoc(x, env);

  module Basis = {
    type t('a) = environment => 'a;

    let return = (x, _) => x;

    let apply = (f, x, env) => (f(env))(x(env));
  };

  module Methods = Mixture.Mixture_Applicative.Make(Basis);

  include Basis;
  include Methods;
};

module ReaderImplementsApplicative: Mixture.Mixture_Applicative.S = Reader;

module BooleanExpression = {
  type expr =
    | Variable(string)
    | Immediate(bool)
    | Not(expr)
    | And(expr, expr)
    | Or(expr, expr);

  let rec eval =
    Reader.Infix.(
      fun
      | Variable(name) => Reader.fetch(name)
      | Immediate(b) => (_ => b)
      | Not(expr) => (!) <$> eval(expr)
      | [@implicit_arity] Or(a, b) => (||) <$> eval(a) <*> eval(b)
      | [@implicit_arity] And(a, b) => (&&) <$> eval(a) <*> eval(b)
    );
};

let () = {
  let expr =
    BooleanExpression.(
      [@implicit_arity]
      And(
        [@implicit_arity] Or(Variable("a"), Not(Variable("b"))),
        Immediate(true),
      )
    );

  make_suite("applicative", "Test all applicative mixin features")
  |@ [
    assert_true(
      "1",
      BooleanExpression.eval(expr),
      [("a", true), ("b", true)],
    ),
  ]
  |> register;
};
