/* Mixture_Applicative -- Applicative mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Applicative mixin.

    Applicative is a functional programming structure popularised by
    Conor McBride and Ross Paterson.  This structure is a bit weaker
    than monads while being very close to them.

    {b See Also}
    {{:https://hackage.haskell.org/package/base-4.8.1.0/docs/Control-Applicative.html} Applicative structure in Haskell}. */;

/** Input signature of the functor [Mixture_Applicative.Make]. */

module type Basis = {
  /** The type of applicative structures. */

  type t(+'a);

  /** Lift a value. */

  let return: 'a => t('a);

  /** [apply f] sequence computations and combine their results with [f]. */

  let apply: (t('a => 'b), t('a)) => t('b);
};

/** Output signature of the functor [Mixture_Applicative.Make]. */

module type Methods = {
  /** The type of applicative structures. */

  type t(+'a);

  /** [map f] is the function on computations deduced from [f]. */

  let map: ('a => 'b, t('a)) => t('b);

  /** A version of [map] for binary functions. */

  let map2: (('a, 'b) => 'c, t('a), t('b)) => t('c);

  /** A version of [map] for ternary functions. */

  let map3: (('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);

  /** A version of [map] for quaternary functions. */

  let map4: (('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);

  /** The applicative distributor for list, that is, the natural
      transformation of a list of computations in the computation of a
      list. */

  let dist: list(t('a)) => t(list('a));

  /** Infix operators. */

  module Infix: {
    /** A shorthand for [apply], the sequential application. */

    let (<*>): (t('a => 'b), t('a)) => t('b);

    /** A shorthand for [map]. */

    let (<$>): ('a => 'b, t('a)) => t('b);

    /** Sequence actions, discarding the value of the first
        argument. */

    let ( <* ): (t('a), t('b)) => t('a);

    /** Sequence actions, discarding the value of the second
        argument. */

    let ( >* ): (t('a), t('b)) => t('b);
  };
};

/** Functor implementing applicative methods based on an applicative
    definition. */

module Make: (B: Basis) => Methods with type t('a) := B.t('a);

/** Signature of applicative structures. */

module type S = {
  type t(+'a);
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};
