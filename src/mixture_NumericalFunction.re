/* Mixture_NumericalFunction -- Numerical function mixin

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
  let range: t => (float, float);
  let eval: (t, float) => float;
};

module type Methods = {
  type t;
  let range_min: t => float;
  let range_max: t => float;
  let is_within_range: (t, float) => bool;
  let fold_step: (t, float, (float, float, 'a) => 'a, 'a) => 'a;
  let iter_step: (t, float, (float, float) => unit) => unit;
  let fold_ticks: (t, int, (float, float, 'a) => 'a, 'a) => 'a;
  let iter_ticks: (t, int, (float, float) => unit) => unit;
};

module Make = (B: Basis) => {
  open B;

  let range_min = f => fst(range(f));

  let range_max = f => snd(range(f));

  let is_within_range = (f, x) => {
    let (xmin, xmax) = range(f);
    xmin <= x && x <= xmax;
  };

  let fold_step = (f, step, comp, acc) => {
    let (xmin, xmax) = range(f);
    let rec loop = (x, test, acc) =>
      if (test(x)) {
        loop(x +. step, test, comp(x, eval(f, x), acc));
      } else {
        acc;
      };

    if (step > 0.0) {
      loop(xmin, x => x <= xmax, acc);
    } else if (step < 0.0) {
      loop(xmax, x => x >= xmin, acc);
    } else {
      invalid_arg("fold_step");
    };
  };

  let fold_ticks = (f, n, comp, acc) => {
    let (xmin, xmax) = range(f);
    let step =
      if (n != 0) {
        (xmax -. xmin) /. float_of_int(n);
      } else {
        invalid_arg("fold_ticks");
      };

    fold_step(f, step, comp, acc);
  };

  let iter_step = (f, step, comp) => {
    let comp' = (x, y, ()) => comp(x, y);
    if (step != 0.0) {
      fold_step(f, step, comp', ());
    } else {
      invalid_arg("iter_step");
    };
  };

  let iter_ticks = (f, n, comp) => {
    let comp' = (x, y, ()) => comp(x, y);
    if (n != 0) {
      fold_ticks(f, n, comp', ());
    } else {
      invalid_arg("iter_ticks");
    };
  };
};

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
