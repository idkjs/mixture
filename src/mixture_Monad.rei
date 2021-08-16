/* Mixture_Monad -- Monadic mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Monad mixin. */;

/** Input signature of the functor [Mixture_Monad.Make]. */

module type Basis = {
  /** The type of monads. */

  type t(+'a);

  /** [bind m f] bind [f] to the monad [m]. */

  let bind: (t('a), 'a => t('b)) => t('b);

  /** [return a] embed the value [a] in the monad. */

  let return: 'a => t('a);
};

/** Output signature of the functor [Mixture_Monad.Make]. */

module type Methods = {
  /** The type of monads. */

  type t(+'a);

  /** [apply f] sequence computations and combine their results with [f]. */

  let apply: (t('a => 'b), t('a)) => t('b);

  /** [join mm] bind [mm] to the identity, reducing the monad. */

  let join: t(t('a)) => t('a);

  /** [map f] is the natural tranformation between monads,
      induced by [f]. */

  let map: ('a => 'b, t('a)) => t('b);

  /** Similar to [bind], but works on two arguments. */

  let bind2: (t('a), t('b), ('a, 'b) => t('c)) => t('c);

  /** Similar to [bind], but works on three arguments. */

  let bind3: (t('a), t('b), t('c), ('a, 'b, 'c) => t('d)) => t('d);

  /** Similar to [bind], but works on four arguments. */

  let bind4:
    (t('a), t('b), t('c), t('d), ('a, 'b, 'c, 'd) => t('e)) => t('e);

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

  /** Monadic ignore. */

  let ignore: t('a) => t(unit);

  /** Filter a list of computations with the given monadic predicate. */

  let filter: ('a => t(bool), list(t('a))) => t(list('a));

  /** [only_if flag m] returns [m] only if [flag] is [true]. */

  let only_if: (bool, t(unit)) => t(unit);

  /** [unless flag m] returns [m] only if [flag] is [false]. */

  let unless: (bool, t(unit)) => t(unit);

  /** [catch f h] is a computation yielding the same result as [f ()]
      if this computation does not throw an exception. If this computation
      raises an exception, then it is passed to [h] to determine the
      result of the overall computation. */

  let catch: (unit => t('a), exn => t('a)) => t('a);

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

    /** [ m >>= f] is equivalent to [bind m f]. */

    let (>>=): (t('a), 'a => t('b)) => t('b);

    /** A composable shorthand for [map]. */

    let (>|=): (t('a), 'a => 'b) => t('b);

    /** [m >> f] binds [m] to [f], a context function. */

    let (>>): (t('a), unit => t('b)) => t('b);

    /** [g >=> f] is the (contravariant) monadic composition of [g]
        followed by [f]. */

    let (>=>): ('a => t('b), 'b => t('c), 'a) => t('c);

    /** [f <=< g] is the monadic composition of [g] followed by [f]. */

    let (<=<): ('b => t('c), 'a => t('b), 'a) => t('c);
  };
};

/** Functor implementing monadic methods based on a monadic definition. */

module Make: (B: Basis) => Methods with type t('a) := B.t('a);

/** Signature of monads. */

module type S = {
  type t(+'a);
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};

/** Monad Transformers. */

module Transformer: {
  module type MonadBasis = Basis;
  module type MonadS = S;
  module type S = {
    include MonadS;
    type u(+'a);
    let lift: u('a) => t('a);
  };

  /** Functor implementing monadic transformers. */

  module Make:
    (B: Basis, M: Basis, G: Basis with type t('a) = M.t(B.t('a))) =>
     S with type t('a) = M.t(B.t('a)) and type u('a) = M.t('a);
};
