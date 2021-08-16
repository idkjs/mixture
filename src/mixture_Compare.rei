/* Mixture_Compare -- Comparable mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Comparable mixin. */;

/** Input signature of the functor [Mixture_Compare.Make]. */

module type Basis = {
  /** The type of comparable elements. */

  type t;

  /** [compare x y] returns [0] if [x] is equal to [y] , a negative
      integer if [x] is less than [y] , and a positive integer if [x] is
      greater than [y]. */

  let compare: (t, t) => int;
};

/** Output signature of the functor [Mixture_Compare.Make]. */

module type Methods = {
  /** The type of comparable elements. */

  type t;

  /** {i Equal}. */

  let equal: (t, t) => bool;

  /** {6 Comparison operators}

      All tautological relations between these comparison operators and the
      [compare] function are satisfied. */;

  /** {i Equal}. */

  let eq: (t, t) => bool;

  /** {i Equal}. */

  let neq: (t, t) => bool;

  /** {i Greater than}. */

  let gt: (t, t) => bool;

  /** {i Greater or equal to}. */

  let ge: (t, t) => bool;

  /** {i Lesser than}. */

  let lt: (t, t) => bool;

  /** {i Lesser or equal to}. */

  let le: (t, t) => bool;

  /** The maximum of two comparable elements. */

  let max: (t, t) => t;

  /** The minimum of two comparable elements. */

  let min: (t, t) => t;

  /** Operators for expression evaluations. */

  module Infix: {
    /** {i Equal}. */

    let (==): (t, t) => bool;

    /** {i Equal}. */

    let (!=): (t, t) => bool;

    /** {i Greater than}. */

    let (>): (t, t) => bool;

    /** {i Greater or equal to}. */

    let (>=): (t, t) => bool;

    /** {i Lesser than}. */

    let (<): (t, t) => bool;

    /** {i Lesser or equal to}. */

    let (<=): (t, t) => bool;
  };
};

/** Functor implementing comparison mixins based on a comparison
    definition. */

module Make: (B: Basis) => Methods with type t := B.t;

/** Signature of comparable mixins. */

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
