/* Mixture_NumericalFunction -- Numerical function mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Numerical function mixin.

    A numerical function mixin represents a numerical function of a real
    value, defined on a compact interval. */;

/** Input signature of the functor [Mixture_NumericalFunction.Make]. */

module type Basis = {
  /** The type of numerical functions. */

  type t;

  /** The range [(range_min,range_max)] of the numerical function. */

  let range: t => (float, float);

  /** [eval f x] evaluate [f] at [x].
      @raise Out_of_range if [x] is not in the range of the function. */

  let eval: (t, float) => float;
};

/** Output signature of the functor [Mixture_NumericalFunction.Make]. */

module type Methods = {
  /** The type of numerical functions. */

  type t;

  /** {6 Range of the numerical function} */;

  /** The smallest value of [x] for which the numerical
      function is defined. */

  let range_min: t => float;

  /** The larges value of [x] for which the numerical
      function is defined. */

  let range_max: t => float;

  /** Predicate recognising points lying in the range of
      a numerical function. */

  let is_within_range: (t, float) => bool;

  /** {6 Walking through the values of the function} */;

  /** [fold_step f step loop acc] compute [loop x (f x) acc] for all [x] in
      the range of [f] with the given [step], accumulating state in [acc].

      If [step < 0.0] the range is explored backwards.

      @raise Invalid_argument if [step = 0.0]. */

  let fold_step: (t, float, (float, float, 'a) => 'a, 'a) => 'a;

  /** [iter_step f step loop] compute [loop x (f x)] for all [x] in
      the range of [f] with the given [step].

      @raise Invalid_argument if [step <= 0.0]. */

  let iter_step: (t, float, (float, float) => unit) => unit;

  /** [fold_ticks f ticks loop acc] compute [loop x (f x) acc] for [n]
      evenly spaced values of [x] in the range of [f],
      accumulating state in [acc].

      @raise Invalid_argument if [n] is smaller than [0]. */

  let fold_ticks: (t, int, (float, float, 'a) => 'a, 'a) => 'a;

  /** [iter_ticks f ticks loop] compute [loop x (f x)] for [n]
      evenly spaced values of [x] in the range of [f].

      if [n < 0], the range is explored backwards.

      @raise Invalid_argument if [n = 0]. */

  let iter_ticks: (t, int, (float, float) => unit) => unit;
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
