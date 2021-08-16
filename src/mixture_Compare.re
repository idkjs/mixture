/* Mixture_Compare -- Comparable mixin

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
  let compare: (t, t) => int;
};

module type Methods = {
  type t;
  let equal: (t, t) => bool;
  let eq: (t, t) => bool;
  let neq: (t, t) => bool;
  let gt: (t, t) => bool;
  let ge: (t, t) => bool;
  let lt: (t, t) => bool;
  let le: (t, t) => bool;
  let max: (t, t) => t;
  let min: (t, t) => t;

  module Infix: {
    let (==): (t, t) => bool;
    let (!=): (t, t) => bool;
    let (>): (t, t) => bool;
    let (>=): (t, t) => bool;
    let (<): (t, t) => bool;
    let (<=): (t, t) => bool;
  };
};

module Make = (B: Basis) => {
  let equal = (a, b) => B.compare(a, b) == 0;
  let eq = (a, b) => B.compare(a, b) == 0;
  let neq = (a, b) => !eq(a, b);
  let gt = (a, b) => B.compare(a, b) > 0;
  let lt = (a, b) => B.compare(a, b) < 0;
  let ge = (a, b) => !lt(a, b);
  let le = (a, b) => !gt(a, b);
  let max = (a, b) =>
    if (ge(a, b)) {
      a;
    } else {
      b;
    };
  let min = (a, b) =>
    if (le(a, b)) {
      a;
    } else {
      b;
    };

  module Infix = {
    let (==) = eq;
    let (!=) = neq;
    let (>) = gt;
    let (>=) = ge;
    let (<) = lt;
    let (<=) = le;
  };
};

module type S = {
  type t;
  include Basis with type t := t;
  include Methods with type t := t;
};
