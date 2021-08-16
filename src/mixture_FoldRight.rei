/* Mixture_FoldRight --  Right foldable mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Right foldable mixin.

    A right foldable mixin is a container structure that can be walked through,
    accumulating observation of elements in a state variable.  This
    operation is known as folding or reducing. */;

/** Input signature of the functor [Mixture_FoldRight.Make]. */

module type Basis = {
  /** The type of foldable mixins containing values of type ['a]. */

  type t(+'a);

  /** [fold f cont acc] apply [f] on each element of [cont],
      accumulating results in [acc]. */

  let fold: (('a, 'b) => 'b, t('a), 'b) => 'b;
};

/** Output signature of the functor [Mixture_FoldRight.Make]. */

module type Methods = {
  /** The type of right foldable mixins containing
      values of type ['a]. */

  type t(+'a);

  /** Folding without accumulation. */

  let iter: ('a => unit, t('a)) => unit;

  /** Return the elements of the container. */

  let elements: t('a) => list('a);

  /** Return the number of elements in the container. */

  let cardinal: t('a) => int;

  /** [exists p cont] is true iff [cont] contains an element satisfying [p]. */

  let exists: ('a => bool, t('a)) => bool;

  /** [for_all p cont] is true iff each element of [cont] satisfies [p]. */

  let for_all: ('a => bool, t('a)) => bool;
};

/** Functor implementing foldable mixins based on a folding function. */

module Make: (B: Basis) => Methods with type t('a) := B.t('a);

/** Signature of right foldable mixins. */

module type S = {
  type t(+'a);
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};
