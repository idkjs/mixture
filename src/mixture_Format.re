/* Mixture_Format -- Formatted mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

module type Basis = {
  type t;
  let pp_print: (Format.formatter, t) => unit;
};

module type Methods = {
  type t;
  let to_string: t => string;
  let output: (out_channel, t) => unit;
  let print: t => unit;
  let prerr: t => unit;
};

module Make = (B: Basis) => {
  open Format;
  open B;

  let to_string = a => {
    pp_print(str_formatter, a);
    flush_str_formatter();
  };

  let output = (c, a) => output_string(c, to_string(a));

  let print = a => output(stdout, a);

  let prerr = a => output(stderr, a);
};

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
