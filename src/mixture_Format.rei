/* Mixture_Format -- Formatted mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

/** Formatted mixin. */;

/** Input signature of the functor [Mixture_Format.Make]. */

module type Basis = {
  /** The type of formatted elements. */

  type t;

  /** [pp_print fft a] pretty print [a] on [fft]. */

  let pp_print: (Format.formatter, t) => unit;
};

/** Output signature of the functor [Mixture_Format.Make]. */

module type Methods = {
  type t;

  /** Convert to string. */

  let to_string: t => string;

  /** Output on the given output channel. */

  let output: (out_channel, t) => unit;

  /** Output on the standard output channel. */

  let print: t => unit;

  /** Output on the standard error channel. */

  let prerr: t => unit;
};

/** Functor implementing output mixins based on a format definition. */

module Make: (B: Basis) => Methods with type t := B.t;

/** Signature of formatted mixins. */

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
