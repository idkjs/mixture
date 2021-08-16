/* TestMonad -- Test monad structures

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

open Broken;
open Mixture;

module Stdlib_List = List;

let (|>) = (x, f) => f(x); /* Compatibility with OCaml 4.00.1 */

/* We implement several calssical monads. */

module Maybe = {
  module Basis = {
    type t('a) = option('a);

    let return = x => Some(x);

    let bind = (m, f) =>
      switch (m) {
      | Some(x) => f(x)
      | None => None
      };
  };

  module Methods = Mixture_Monad.Make(Basis);

  include Basis;
  include Methods;

  let format = (f, pp, m) =>
    Format.(
      switch (m) {
      | Some(x) => fprintf(pp, "Some(%a)", f, x)
      | None => fprintf(pp, "None")
      }
    );
};

module MaybeImplementsMonad: Mixture_Monad.S = Maybe;

module MaybeImplementsApplicative: Mixture_Applicative.S = Maybe;

module List = {
  module Basis = {
    type t('a) = list('a);

    let return = x => [x];

    let bind = (m, f) => Stdlib_List.concat(Stdlib_List.map(f, m));
  };

  let cons = (hd, tl) => [hd, ...tl];

  let format = (f, pp, m) => {
    open Format;
    let sep = ref("");
    fprintf(pp, "[");
    List.iter(
      x => {
        fprintf(pp, "%s%a", sep^, f, x);
        sep := "; ";
      },
      m,
    );
    fprintf(pp, "]");
  };

  include Stdlib_List;
  module Methods = Mixture_Monad.Make(Basis);

  include Basis;
  include Methods;
};

module ListImplementsMonad: Mixture_Monad.S = List;

module ListImplementsApplicative: Mixture_Applicative.S = List;

let assert_maybe_string = (id, ~expected_failure=?, f, a, b) =>
  assert_equal(
    id,
    ~expected_failure?,
    ~printer=Maybe.format(Format.pp_print_string),
    ~equal=(==),
    f,
    a,
    b,
  );

let assert_list_string = (id, ~expected_failure=?, f, a, b) =>
  assert_equal(
    id,
    ~expected_failure?,
    ~printer=List.format(Format.pp_print_string),
    ~equal=(==),
    f,
    a,
    b,
  );

let () =
  make_suite("monad", "Test all monad mixin features")
  |: [
    make_suite("maybe", "Test the maybe monad")
    |@ [
      assert_maybe_string(
        "map",
        Maybe.map(String.uppercase_ascii),
        Some("a"),
        Some("A"),
      ),
      assert_maybe_string(
        "map_infix",
        Maybe.Infix.(<$>)(String.uppercase_ascii),
        Some("a"),
        Some("A"),
      ),
      assert_maybe_string(
        "apply",
        Maybe.apply(Some(String.uppercase_ascii)),
        Some("a"),
        Some("A"),
      ),
      assert_maybe_string(
        "apply_infix",
        Maybe.Infix.(<*>)(Some(String.uppercase_ascii)),
        Some("a"),
        Some("A"),
      ),
      assert_maybe_string(
        "apply_left_1",
        Maybe.Infix.( <* )(None),
        Some("b"),
        None,
      ),
      assert_maybe_string(
        "apply_left_2",
        Maybe.Infix.( <* )(Some("a")),
        Some("b"),
        Some("a"),
      ),
      assert_maybe_string(
        "apply_right_1",
        Maybe.Infix.( >* )(None),
        Some("b"),
        None,
      ),
      assert_maybe_string(
        "apply_right_2",
        Maybe.Infix.( >* )(Some("a")),
        Some("b"),
        Some("b"),
      ),
    ],
    make_suite("list", "Test the list monad")
    |@ [
      assert_list_string(
        "cartesian_product",
        List.bind2(["1", "2", "3"], ["4", "5"]),
        (x, y) => [Printf.sprintf("(%s, %s)", x, y)],
        ["(1, 4)", "(1, 5)", "(2, 4)", "(2, 5)", "(3, 4)", "(3, 5)"],
      ),
    ],
  ]
  |> register;
