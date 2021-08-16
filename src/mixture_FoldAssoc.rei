/* Mixture_FoldAssoc -- Foldable associative mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Foldable associative mixin.

    A foldable associative mixin is an associative container structure
    that can be walked through, accumulating observation of bindings in a
    state variable.  This operation is known as folding or reducing. */;

/** Input signature of the functor [Mixture_FoldAssoc.Make]. */

module type Basis = {
  /** The type of foldable mixins containing values of type ['a]. */

  type t(+'a);

  /** The type of keys indexing elements of the mixin. */

  type key;

  /** [fold f cont acc] apply [f] on each binding of [cont],
      accumulating results in [acc]. */

  let fold: ((key, 'a, 'b) => 'b, t('a), 'b) => 'b;
};

/** Output signature of the functor [Mixture_FoldAssoc.Make]. */

module type Methods = {
  /** The type of assoc foldable mixins containing
      values of type ['a]. */

  type t(+'a);

  /** The type of keys indexing elements of the mixin. */

  type key;

  /** Folding without accumulation. */

  let iter: ((key, 'a) => unit, t('a)) => unit;

  /** Return the elements of the container. */

  let elements: t('a) => list('a);

  /** Return the bindings of the container. */

  let bindings: t('a) => list((key, 'a));

  /** Return the number of elements in the container. */

  let cardinal: t('a) => int;

  /** [exists p cont] is true iff [cont] contains a binding satisfying [p]. */

  let exists: ((key, 'a) => bool, t('a)) => bool;

  /** [for_all p cont] is true iff each binding of [cont] satisfies [p]. */

  let for_all: ((key, 'a) => bool, t('a)) => bool;
};

/** Functor implementing foldable mixins based on a folding function. */

module Make:
  (B: Basis) => Methods with type t('a) := B.t('a) and type key := B.key;

/** Signature of foldable assoc mixins. */

module type S = {
  type t(+'a);
  type key;
  include Basis with type t('a) := t('a) and type key := key;
  include Methods with type t('a) := t('a) and type key := key;
};
