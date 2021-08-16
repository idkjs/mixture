/* Mixture_Parse -- Parsable mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Parsable mixin. */;

/** Input signature of the functor [Mixture_Parse.Make]. */

module type Basis = {
  /** The type of parsed items. */

  type t;

  /** The current module path, as a string. */

  let path: string;

  /** Parse a value of the given type.
      @raise Failure "lexing: empty token" on a lexing failure. */

  let parse: Lexing.lexbuf => t;
};

/** Output signature of the functor [Mixture_Parse.Make]. */

module type Methods = {
  /** The type of parsed items. */

  type t;

  /** Parse the given string.

      @raise Failure [Parameter.path ^ ".of_string"] when the string is bad. */

  let of_string: string => t;
};

/** Functor implementing parse mixins based on a parser definition. */

module Make: (B: Basis) => Methods with type t := B.t;

/** Signature of parsable mixins. */

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
